#pragma once
#include <stdint.h>
#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"

typedef struct {
	int16_t accelx;
	int16_t accely;
	int16_t accelz;
	int16_t gyrox;
	int16_t gyroy;
	int16_t gyroz;
} mpu9250_accel_tp;

typedef struct {
	i2c_port_t i2c_port_num;
	uint8_t i2c_address;
	uint8_t last_out;
} mpu9250_dev_t;


int mpu9250_read_fifo(mpu9250_dev_t *dev, mpu9250_accel_tp *meas, int maxct);

esp_err_t mpu9250_poll(mpu9250_dev_t *dev, int *x, int *y, int *z);
int mpu9250_init(i2c_port_t port, uint8_t addr, mpu9250_dev_t *dev);
void mpu9250_sleep();