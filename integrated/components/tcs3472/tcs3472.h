#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"

typedef struct {
	i2c_port_t i2c_port_num;
	uint8_t i2c_address;
} tcs3472_dev_t;

esp_err_t tcs3472_init(i2c_port_t port, uint8_t addr, tca9534_dev_t *dev);
esp_err_t tcs3472_get_input(tca9534_dev_t *dev, int *r, int *g, int *b, int *c);
