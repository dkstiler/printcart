#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"

typedef struct {
	i2c_port_t i2c_port_num;
	uint8_t i2c_address;
	uint8_t last_out;
} tca9534_dev_t;

esp_err_t tca9534_init(i2c_port_t port, uint8_t addr, tca9534_dev_t *dev);
esp_err_t tca9534_set_output(tca9534_dev_t *dev, uint8_t val);
esp_err_t tca9534_change_output(tca9534_dev_t *dev, uint8_t clrmask, uint8_t setmask);
esp_err_t tca9534_set_dir(tca9534_dev_t *dev, uint8_t val);
uint8_t tca9534_get_input(tca9534_dev_t *dev);
