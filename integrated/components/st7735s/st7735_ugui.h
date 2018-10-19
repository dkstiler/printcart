#pragma once
#include "st7735.h"
#include "ugui.h"

void st7735_pset(UG_S16 x, UG_S16 y, UG_COLOR c);
void st7735_ugui_cls();
void st7735_ugui_flush();
void st7735_ugui_init(spi_device_handle_t dev);
