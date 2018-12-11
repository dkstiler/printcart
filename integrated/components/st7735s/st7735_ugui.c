/*
 Adapter functions between the ugui graphics library and the st7735 LCD driver
*/

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */


#include "st7735.h"
#include "ugui.h"
#include "st7735_ugui.h"
#include <string.h>

static uint16_t *fb;
static spi_device_handle_t lcddev;
static UG_GUI *ugui;


void st7735_pset(UG_S16 x, UG_S16 y, UG_COLOR c) {
	//hackhack.. don't want to modify the init sequence to rotate and invert the display.
	c=~c;
	x=159-x;
	if (fb==NULL) return;
	if (x<0 || x>=160) return;
	if (y<0 || y>=80) return;
	fb[(y)+(x*80)]=(c>>8)|(c<<8);
}

void st7735_ugui_cls() {
	memset(fb, 0xff, 160*80*2);
}

void st7735_ugui_flush() {
	st7735_send(lcddev, (uint8_t*)fb, 0, 0, 79, 159);
}

void st7735_ugui_init(spi_device_handle_t dev) {
	fb=heap_caps_malloc(160*80*2, MALLOC_CAP_DMA);
	lcddev=dev;
	ugui=malloc(sizeof(UG_GUI));
	UG_Init(ugui, st7735_pset, 160, 80);
	st7735_ugui_cls();
	st7735_ugui_flush();
}

