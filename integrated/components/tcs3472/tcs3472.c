#include "tca9534.h"
#include "esp_err.h"
#include "esp_log.h"

#define TCS_CMD_CMD 0x80
#define TCS_CMD_REP 0x00
#define TCS_CMD_INC 0x20
#define TCS_CMD_SFN 0x60


#define TCS_ENABLE 0
#define TCS_ATIME 1
#define TCS_WTIME 3
#define TCS_AILTL 4
#define TCS_AILTH 5
#define TCS_AIHTL 6
#define TCS_AIHTH 7
#define TCS_PERS 0xc
#define TCS_CONFIG 0xd
#define TCS_CONTROL 0xf
#define TCS_ID 0x12
#define TCS_STATUS 0x13
#define TCS_CDATAL 0x14
#define TCS_CDATAH 0x15
#define TCS_RDATAL 0x16
#define TCS_RDATAH 0x17
#define TCS_GDATAL 0x18
#define TCS_GDATAH 0x19
#define TCS_BDATAL 0x1a
#define TCS_BDATAH 0x1b

#define ACK_CHECK_EN true

static esp_err_t tcs_get_regs(tca9534_dev_t *dev, int reg, uint8_t *data, int len) {
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
	i2c_master_read(cmd, data, len, I2C_MASTER_LAST_NACK);
	i2c_master_stop(cmd);
	err = i2c_master_cmd_begin(dev->i2c_port_num, cmd, 1000/portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
	return err;
}

static esp_err_t tcs_set_reg(tca9534_dev_t *dev, int reg, uint8_t data) {
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


esp_err_t tcs3472_init(i2c_port_t port, uint8_t addr, tca9534_dev_t *dev) {
	dev->i2c_port_num=port;
	dev->i2c_address=addr;
	uint8_t id=0;
	esp_err_t ret=tcs_set_reg(dev, TCS_CMD_CMD|TCS_ENABLE, 0xb);
	if (ret!=ESP_OK) return ret;
	vTaskDelay(10/portTICK_RATE_MS);
	ret=tcs_get_regs(dev, TCS_CMD_CMD|TCS_ID, &id, 1);
	if (ret!=ESP_OK) return ret;
	printf("TCS3472 found,  ID %x\n", id);
	ret=tcs_set_reg(dev, TCS_CMD_CMD|TCS_ATIME, 0xC0);
	if (ret!=ESP_OK) return ret;
	ret=tcs_set_reg(dev, TCS_CMD_CMD|TCS_WTIME, 0xFF);
	if (ret!=ESP_OK) return ret;
	ret=tcs_set_reg(dev, TCS_CMD_CMD|TCS_CONTROL, 1); //4x gain
	return ret;
}

esp_err_t tcs3472_get_input(tca9534_dev_t *dev, int *r, int *g, int *b, int *c) {
	uint8_t buf[8];
	esp_err_t ret=tcs_get_regs(dev, TCS_CMD_CMD|TCS_CMD_INC|TCS_CDATAL, buf, 8);
	if (c) *c=buf[0]|(buf[1]<<8);
	if (r) *r=buf[2]|(buf[3]<<8);
	if (g) *g=buf[4]|(buf[5]<<8);
	if (b) *b=buf[6]|(buf[7]<<8);
	return ret;
}
