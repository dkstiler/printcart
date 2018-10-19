#include "driver/spi_master.h"

#pragma once

spi_device_handle_t st7735_init(int bus, int cs, int my_dc_pin);
void st7735_send(spi_device_handle_t spi, uint8_t *buf, int xs, int ys, int xe, int ye);
