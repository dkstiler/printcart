//Simple MPU9250 driver. Actually only handles the gyro and accelerometer aspects of it; doesn't do anything
//with the digital compass. Kinda hackish, probably not that useful as a general purpose driver.

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include "driver/i2c.h"
#include "mpu9250.h"

static esp_err_t setreg(mpu9250_dev_t *dev, int reg, int val) {
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, ( (dev->i2c_address<<1) | I2C_MASTER_WRITE), true);
	i2c_master_write_byte(cmd, reg, true);
	i2c_master_write_byte(cmd, val, true);
	i2c_master_stop(cmd);
	esp_err_t ret = i2c_master_cmd_begin(dev->i2c_port_num, cmd, 30 / portTICK_RATE_MS);
	if (ret!=ESP_OK) printf("MPU9250: NACK setting reg 0x%X to val 0x%X\n", reg, val);
	i2c_cmd_link_delete(cmd);
	return ret;
}

static int getreg(mpu9250_dev_t *dev, int reg) {
	unsigned char byte;
	esp_err_t ret;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, ( (dev->i2c_address<<1) | I2C_MASTER_WRITE), true);
	i2c_master_write_byte(cmd, reg, true);
	ret=i2c_master_cmd_begin(dev->i2c_port_num, cmd, 100 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);

	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, ( (dev->i2c_address<<1) | I2C_MASTER_READ), true);
	i2c_master_read_byte(cmd, &byte, true);
	i2c_master_stop(cmd);
	ret=i2c_master_cmd_begin(dev->i2c_port_num, cmd, 100 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
	if (ret!=ESP_OK) printf("MPU9250: NACK reading reg 0x%X\n", reg);
	if (ret!=ESP_OK) {
		return -1;
	}
	return byte;
}


esp_err_t mpu9250_start(mpu9250_dev_t *dev, int samp_div) {
	printf("Initializing MPU9250...\n");
	setreg(dev, 107, 0x80);		//Reset
	vTaskDelay(10);
	int i=getreg(dev, 0x75);
	if (i!=0x71) {
		printf("No MPU9250 detected at i2c addr %x! (%d)\n", dev->i2c_address, i);
		return ESP_ERR_NOT_FOUND;
	}
	setreg(dev, 107, 0x80);		//reset
	setreg(dev, 107, 1);		//Take out of sleep, auto-select clk
	setreg(dev, 108, 0);		//Un-standby everything
	setreg(dev, 106, 0x0C);		//reset fifo, i2c master, signal paths
	vTaskDelay(10);
	setreg(dev, 25, samp_div);	//Sample divider
	setreg(dev, 26, 0x41);		//fifo replaces old data, dlpf=1, no fsync
	setreg(dev, 27, 0);			//gyro def, gyro bw = 250Hz
	setreg(dev, 28, 0);			//accel 2G
	setreg(dev, 35, 0x78);		//fifo: emit accel data plus xyz gyros
	setreg(dev, 36, 0x00);		//no slave
	setreg(dev, 106, 0x40);		//fifo: enable
	printf("MPU9250 found and initialized.\n");
	
	return ESP_OK;
}


int mpu9250_read_fifo(mpu9250_dev_t *dev, mpu9250_accel_tp *meas, int maxct) {
	int i;
	int no;
	uint8_t buf[6*2];
	i2c_cmd_handle_t cmd;
	esp_err_t ret;
	no=(getreg(dev, 114)<<8);
	no|=getreg(dev, 115);
	if (no>0x8000) {
		printf("Huh? Fifo has %x bytes.\n", no);
		return 0; //huh?
	}
//	printf("FIFO: %d\n", no);
	no=no/12; //bytes -> samples
	if (no>maxct) no=maxct;

	for (i=0; i<no; i++) {
		cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, ( (dev->i2c_address<<1) | I2C_MASTER_WRITE), true);
		i2c_master_write_byte(cmd, 116, true);
		i2c_master_stop(cmd);
		ret=i2c_master_cmd_begin(dev->i2c_port_num, cmd, 100 / portTICK_RATE_MS);
		i2c_cmd_link_delete(cmd);

		cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, ( (dev->i2c_address<<1) | I2C_MASTER_READ), true);
		i2c_master_read(cmd, buf, 11, 0);
		i2c_master_read(cmd, buf+11, 1, 1);
		i2c_master_stop(cmd);
		ret=i2c_master_cmd_begin(dev->i2c_port_num, cmd, 100 / portTICK_RATE_MS);
		if (ret!=ESP_OK) {
			printf("Error reading packet %d/%d\n", i, no);
			return 0;
		}
		meas[i].accelx=(buf[0]<<8)|buf[1];
		meas[i].accely=(buf[2]<<8)|buf[3];
		meas[i].accelz=(buf[4]<<8)|buf[5];
		meas[i].gyrox=(buf[6]<<8)|buf[7];
		meas[i].gyroy=(buf[8]<<8)|buf[9];
		meas[i].gyroz=(buf[10]<<8)|buf[11];
		i2c_cmd_link_delete(cmd);
	}
	return no;
}

void mpu9250_sleep(mpu9250_dev_t *dev) {
	setreg(dev, 106, 0x02);		//kill fifo
//	setreg(dev, 55, 0xD0);		//enable int, active lvl hi, int active until any read
	setreg(dev, 107, 0x08);		//8MHz clock, temp sens disabled
	setreg(dev, 108, 0x07);		//disable gyro, wake up at 1.25Hz
	setreg(dev, 107, 0x28);		//8MHz clock, cycle mode, temp sens disabled
	setreg(dev, 28, 2);			//accel 8G

#if 0
	while(1) {
		vTaskDelay(10);
		int16_t x=(getreg(dev, 59)<<8)+getreg(dev, 60);
		int16_t y=(getreg(dev, 61)<<8)+getreg(dev, 62);
		int16_t z=(getreg(dev, 63)<<8)+getreg(dev, 64);
		printf("%d %d %d\n", x, y, z);
	}
#endif
}
esp_err_t mpu9250_init(i2c_port_t port, uint8_t addr, mpu9250_dev_t *dev) {
	dev->i2c_port_num=port;
	dev->i2c_address=addr;

	//MPU has sample rate of 8KHz. We have an 1K fifo where we put 6byte samples --> about 128 samples.
	//We want to grab the data at about 10Hz -> sample rate of about 1KHz.
	esp_err_t r=mpu9250_start(dev, 8);
	return r;
}

