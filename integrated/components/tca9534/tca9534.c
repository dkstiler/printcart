/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */

#include "tca9534.h"
#include "esp_err.h"
#include "esp_log.h"

#define TCA_REG_INPUT 0
#define TCA_REG_OUTPUT 1
#define TCA_REG_POLINV 2
#define TCA_REG_CONF 3

#define ACK_CHECK_EN true

static esp_err_t tca_get_reg(tca9534_dev_t *dev, int reg, uint8_t *data) {
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (dev->i2c_address<<1)|I2C_MASTER_WRITE, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
	i2c_master_stop(cmd);
	esp_err_t err = i2c_master_cmd_begin(dev->i2c_port_num, cmd, 1000/portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
	if (err!=ESP_OK) return err;
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (dev->i2c_address<<1)|I2C_MASTER_READ, ACK_CHECK_EN);
	i2c_master_read_byte(cmd, data, I2C_MASTER_NACK);
	i2c_master_stop(cmd);
	err = i2c_master_cmd_begin(dev->i2c_port_num, cmd, 1000/portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
	return err;
}

static esp_err_t tca_set_reg(tca9534_dev_t *dev, int reg, uint8_t data) {
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (dev->i2c_address<<1)|I2C_MASTER_WRITE, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, data, ACK_CHECK_EN);
	i2c_master_stop(cmd);
	esp_err_t err = i2c_master_cmd_begin(dev->i2c_port_num, cmd, 1000/portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
	return err;
}


esp_err_t tca9534_init(i2c_port_t port, uint8_t addr, tca9534_dev_t *dev) {
	dev->i2c_port_num=port;
	dev->i2c_address=addr;
	uint8_t dummy;
	esp_err_t ret=tca_get_reg(dev, TCA_REG_OUTPUT, &dummy);
	if (ret!=ESP_OK) return ret;
	ret=tca_set_reg(dev, TCA_REG_POLINV, 0);
	return ret;
}

esp_err_t tca9534_set_output(tca9534_dev_t *dev, uint8_t val) {
	dev->last_out=val;
	return tca_set_reg(dev, TCA_REG_OUTPUT, val);
}

esp_err_t tca9534_change_output(tca9534_dev_t *dev, uint8_t clrmask, uint8_t setmask) {
	dev->last_out&=~clrmask;
	dev->last_out|=setmask;
	return tca_set_reg(dev, TCA_REG_OUTPUT, dev->last_out);
}


//Note: 1 = input!
esp_err_t tca9534_set_dir(tca9534_dev_t *dev, uint8_t val) {
	return tca_set_reg(dev, TCA_REG_CONF, val);
}

uint8_t tca9534_get_input(tca9534_dev_t *dev) {
	uint8_t val=0;
	tca_get_reg(dev, TCA_REG_INPUT, &val);
	return val;
}
