/*
Driver for a ST7735-based 160x80 LCD (LH096T)
*/

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"
#include "st7735.h"

#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MH  0x04

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

#define ST_CMD_DELAY   0x80	   // special signifier for command lists

#define ST77XX_NOP	   0x00
#define ST77XX_SWRESET 0x01
#define ST77XX_RDDID   0x04
#define ST77XX_RDDST   0x09

#define ST77XX_SLPIN   0x10
#define ST77XX_SLPOUT  0x11
#define ST77XX_PTLON   0x12
#define ST77XX_NORON   0x13

#define ST77XX_INVOFF  0x20
#define ST77XX_INVON   0x21
#define ST77XX_DISPOFF 0x28
#define ST77XX_DISPON  0x29
#define ST77XX_CASET   0x2A
#define ST77XX_RASET   0x2B
#define ST77XX_RAMWR   0x2C
#define ST77XX_RAMRD   0x2E

#define ST77XX_PTLAR   0x30
#define ST77XX_COLMOD  0x3A
#define ST77XX_MADCTL  0x36

#define ST77XX_MADCTL_MY  0x80
#define ST77XX_MADCTL_MX  0x40
#define ST77XX_MADCTL_MV  0x20
#define ST77XX_MADCTL_ML  0x10
#define ST77XX_MADCTL_RGB 0x00

#define ST77XX_RDID1   0xDA
#define ST77XX_RDID2   0xDB
#define ST77XX_RDID3   0xDC
#define ST77XX_RDID4   0xDD


static const uint8_t
  Bcmd[] = {				  // Initialization commands for 7735B screens
	18,						  // 18 commands in list:
	ST77XX_SWRESET,	  ST_CMD_DELAY,	 //	 1: Software reset, no args, w/delay
	  50,					  //	 50 ms delay
	ST77XX_SLPOUT ,	  ST_CMD_DELAY,	 //	 2: Out of sleep mode, no args, w/delay
	  255,					  //	 255 = 500 ms delay
	ST77XX_COLMOD , 1+ST_CMD_DELAY,	 //	 3: Set color mode, 1 arg + delay:
	  0x05,					  //	 16-bit color
	  10,					  //	 10 ms delay
	ST7735_FRMCTR1, 3+ST_CMD_DELAY,	 //	 4: Frame rate control, 3 args + delay:
	  0x00,					  //	 fastest refresh
	  0x06,					  //	 6 lines front porch
	  0x03,					  //	 3 lines back porch
	  10,					  //	 10 ms delay
	ST77XX_MADCTL , 1	   ,  //  5: Memory access ctrl (directions), 1 arg:
	  0x08,					  //	 Row addr/col addr, bottom to top refresh
	ST7735_DISSET5, 2	   ,  //  6: Display settings #5, 2 args, no delay:
	  0x15,					  //	 1 clk cycle nonoverlap, 2 cycle gate
							  //	 rise, 3 cycle osc equalize
	  0x02,					  //	 Fix on VTL
	ST7735_INVCTR , 1	   ,  //  7: Display inversion control, 1 arg:
	  0x0,					  //	 Line inversion
	ST7735_PWCTR1 , 2+ST_CMD_DELAY,	 //	 8: Power control, 2 args + delay:
	  0x02,					  //	 GVDD = 4.7V
	  0x70,					  //	 1.0uA
	  10,					  //	 10 ms delay
	ST7735_PWCTR2 , 1	   ,  //  9: Power control, 1 arg, no delay:
	  0x05,					  //	 VGH = 14.7V, VGL = -7.35V
	ST7735_PWCTR3 , 2	   ,  // 10: Power control, 2 args, no delay:
	  0x01,					  //	 Opamp current small
	  0x02,					  //	 Boost frequency
	ST7735_VMCTR1 , 2+ST_CMD_DELAY,	 // 11: Power control, 2 args + delay:
	  0x3C,					  //	 VCOMH = 4V
	  0x38,					  //	 VCOML = -1.1V
	  10,					  //	 10 ms delay
	ST7735_PWCTR6 , 2	   ,  // 12: Power control, 2 args, no delay:
	  0x11, 0x15,
	ST7735_GMCTRP1,16	   ,  // 13: Magical unicorn dust, 16 args, no delay:
	  0x09, 0x16, 0x09, 0x20, //	 (seriously though, not sure what
	  0x21, 0x1B, 0x13, 0x19, //	  these config values represent)
	  0x17, 0x15, 0x1E, 0x2B,
	  0x04, 0x05, 0x02, 0x0E,
	ST7735_GMCTRN1,16+ST_CMD_DELAY,	 // 14: Sparkles and rainbows, 16 args + delay:
	  0x0B, 0x14, 0x08, 0x1E, //	 (ditto)
	  0x22, 0x1D, 0x18, 0x1E,
	  0x1B, 0x1A, 0x24, 0x2B,
	  0x06, 0x06, 0x02, 0x0F,
	  10,					  //	 10 ms delay
	ST77XX_CASET  , 4	   ,  // 15: Column addr set, 4 args, no delay:
	  0x00, 0x02,			  //	 XSTART = 2
	  0x00, 0x81,			  //	 XEND = 129
	ST77XX_RASET  , 4	   ,  // 16: Row addr set, 4 args, no delay:
	  0x00, 0x02,			  //	 XSTART = 1
	  0x00, 0x81,			  //	 XEND = 160
	ST77XX_NORON  ,	  ST_CMD_DELAY,	 // 17: Normal display on, no args, w/delay
	  10,					  //	 10 ms delay
	ST77XX_DISPON ,	  ST_CMD_DELAY,	 // 18: Main screen turn on, no args, w/delay
	  255 },				  //	 255 = 500 ms delay

  Rcmd1[] = {				  // Init for 7735R, part 1 (red or green tab)
	15,						  // 15 commands in list:
	ST77XX_SWRESET,	  ST_CMD_DELAY,	 //	 1: Software reset, 0 args, w/delay
	  150,					  //	 150 ms delay
	ST77XX_SLPOUT ,	  ST_CMD_DELAY,	 //	 2: Out of sleep mode, 0 args, w/delay
	  255,					  //	 500 ms delay
	ST7735_FRMCTR1, 3	   ,  //  3: Frame rate ctrl - normal mode, 3 args:
	  0x01, 0x2C, 0x2D,		  //	 Rate = fosc/(1x2+40) * (LINE+2C+2D)
	ST7735_FRMCTR2, 3	   ,  //  4: Frame rate control - idle mode, 3 args:
	  0x01, 0x2C, 0x2D,		  //	 Rate = fosc/(1x2+40) * (LINE+2C+2D)
	ST7735_FRMCTR3, 6	   ,  //  5: Frame rate ctrl - partial mode, 6 args:
	  0x01, 0x2C, 0x2D,		  //	 Dot inversion mode
	  0x01, 0x2C, 0x2D,		  //	 Line inversion mode
	ST7735_INVCTR , 1	   ,  //  6: Display inversion ctrl, 1 arg, no delay:
	  0x07,					  //	 No inversion
	ST7735_PWCTR1 , 3	   ,  //  7: Power control, 3 args, no delay:
	  0xA2,
	  0x02,					  //	 -4.6V
	  0x84,					  //	 AUTO mode
	ST7735_PWCTR2 , 1	   ,  //  8: Power control, 1 arg, no delay:
	  0xC5,					  //	 VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
	ST7735_PWCTR3 , 2	   ,  //  9: Power control, 2 args, no delay:
	  0x0A,					  //	 Opamp current small
	  0x00,					  //	 Boost frequency
	ST7735_PWCTR4 , 2	   ,  // 10: Power control, 2 args, no delay:
	  0x8A,					  //	 BCLK/2, Opamp current small & Medium low
	  0x2A,	 
	ST7735_PWCTR5 , 2	   ,  // 11: Power control, 2 args, no delay:
	  0x8A, 0xEE,
	ST7735_VMCTR1 , 1	   ,  // 12: Power control, 1 arg, no delay:
	  0x0E,
	ST77XX_INVOFF , 0	   ,  // 13: Don't invert display, no args, no delay
	ST77XX_MADCTL , 1	   ,  // 14: Memory access control (directions), 1 arg:
	  0xC8,					  //	 row addr/col addr, bottom to top refresh
	ST77XX_COLMOD , 1	   ,  // 15: set color mode, 1 arg, no delay:
	  0x05 },				  //	 16-bit color
  Rcmd2green160x80[] = {			  // Init for 7735R, part 2 (mini 160x80)
	2,						  //  2 commands in list:
	ST77XX_CASET  , 4	   ,  //  1: Column addr set, 4 args, no delay:
	  0x00, 0x00,			  //	 XSTART = 0
	  0x00, 0x7F,			  //	 XEND = 79
	ST77XX_RASET  , 4	   ,  //  2: Row addr set, 4 args, no delay:
	  0x00, 0x00,			  //	 XSTART = 0
	  0x00, 0x9F },			  //	 XEND = 159


  Rcmd3[] = {				  // Init for 7735R, part 3 (red or green tab)
	4,						  //  4 commands in list:
	ST7735_GMCTRP1, 16		, //  1: Magical unicorn dust, 16 args, no delay:
	  0x02, 0x1c, 0x07, 0x12,
	  0x37, 0x32, 0x29, 0x2d,
	  0x29, 0x25, 0x2B, 0x39,
	  0x00, 0x01, 0x03, 0x10,
	ST7735_GMCTRN1, 16		, //  2: Sparkles and rainbows, 16 args, no delay:
	  0x03, 0x1d, 0x07, 0x06,
	  0x2E, 0x2C, 0x29, 0x2D,
	  0x2E, 0x2E, 0x37, 0x3F,
	  0x00, 0x00, 0x02, 0x10,
	ST77XX_NORON  ,	   ST_CMD_DELAY, //	 3: Normal display on, no args, w/delay
	  10,					  //	 10 ms delay
	ST77XX_DISPON ,	   ST_CMD_DELAY, //	 4: Main screen turn on, no args w/delay
	  100 };				  //	 100 ms delay


static int dc_pin=-1;


/* Send a command to the LCD. Uses spi_device_polling_transmit, which waits
 * until the transfer is complete.
 *
 * Since command transactions are usually small, they are handled in polling
 * mode for higher speed. The overhead of interrupt transactions is more than
 * just waiting for the transaction to complete.
 */
static void lcd_cmd(spi_device_handle_t spi, const uint8_t cmd) {
	esp_err_t ret;
	spi_transaction_t t;
	memset(&t, 0, sizeof(t));		//Zero out the transaction
	t.length=8;						//Command is 8 bits
	t.tx_buffer=&cmd;				//The data is the cmd itself
	t.user=(void*)0;				//D/C needs to be set to 0
	ret=spi_device_polling_transmit(spi, &t);  //Transmit!
	assert(ret==ESP_OK);			//Should have had no issues.
}

/* Send data to the LCD. Uses spi_device_polling_transmit, which waits until the
 * transfer is complete.
 *
 * Since data transactions are usually small, they are handled in polling
 * mode for higher speed. The overhead of interrupt transactions is more than
 * just waiting for the transaction to complete.
 */
static void lcd_data(spi_device_handle_t spi, const uint8_t *data, int len) {
	esp_err_t ret;
	spi_transaction_t t;
	if (len==0) return;				//no need to send anything
	memset(&t, 0, sizeof(t));		//Zero out the transaction
	t.length=len*8;					//Len is in bytes, transaction length is in bits.
	t.tx_buffer=data;				//Data
	t.user=(void*)1;				//D/C needs to be set to 1
	ret=spi_device_polling_transmit(spi, &t);  //Transmit!
	assert(ret==ESP_OK);			//Should have had no issues.
}

//This function is called (in irq context!) just before a transmission starts. It will
//set the D/C line to the value indicated in the user field.
static void lcd_spi_pre_transfer_callback(spi_transaction_t *t) {
	int dc=(int)t->user;
	gpio_set_level(dc_pin, dc);
}


static void send_cmd_list(spi_device_handle_t spi, const uint8_t *cmd) {
	int no_cmd=*cmd++;
	uint8_t buf[32];
	for (int cmdno=0; cmdno<no_cmd; cmdno++) {
		int mycmd=*cmd++;
		lcd_cmd(spi, mycmd);
		int flag=*cmd++;
		printf("Cmd %x delay param %x\n", mycmd, flag);
		if (flag&31) {
			memcpy(buf, cmd, flag&31);
			cmd+=(flag&31);
			lcd_data(spi,buf, (flag&31));
		}
		if (flag&ST_CMD_DELAY) {
			vTaskDelay((*cmd++)/portTICK_RATE_MS);
		}
	}
}


//Initialize the display
static void lcd_init(spi_device_handle_t spi) {
	//Initialize non-SPI GPIOs
	gpio_set_direction(dc_pin, GPIO_MODE_OUTPUT);

	send_cmd_list(spi, Bcmd);
	send_cmd_list(spi, Rcmd1);
	send_cmd_list(spi, Rcmd2green160x80);
	send_cmd_list(spi, Rcmd3);
}

void st7735_send(spi_device_handle_t spi, uint8_t *buf, int xs, int ys, int xe, int ye) {
	const int ox=26;
	const int oy=1;
	xs+=ox; xe+=ox;
	ys+=oy; ye+=oy;
	uint8_t cabuf[]={xs>>8, xs&0xff, xe>>8, xe&0xff};
	uint8_t rabuf[]={ys>>8, ys&0xff, ye>>8, ye&0xff};
	lcd_cmd(spi, ST77XX_CASET);
	lcd_data(spi, cabuf, 4);
	lcd_cmd(spi, ST77XX_RASET);
	lcd_data(spi, rabuf, 4);

	lcd_cmd(spi, ST77XX_RAMWR);
	esp_err_t ret;
	spi_transaction_t t;
	int len=((xe-xs+1)*(ye-ys+1))*2;
	memset(&t, 0, sizeof(t));		//Zero out the transaction
	t.length=len*8;					//Len is in bytes, transaction length is in bits.
	t.tx_buffer=buf;				//Data
	t.user=(void*)1;				//D/C needs to be set to 1
	ret=spi_device_transmit(spi, &t);  //Transmit!
	assert(ret==ESP_OK);			//Should have had no issues.
}


spi_device_handle_t st7735_init(int bus, int cs, int my_dc_pin) {
	dc_pin=my_dc_pin;
	spi_device_interface_config_t devcfg={
		.clock_speed_hz=26*1000*1000,			//Clock out at 26 MHz
		.mode=0,								//SPI mode 0
		.spics_io_num=cs,						//CS pin
		.queue_size=7,							//We want to be able to queue 7 transactions at a time
		.pre_cb=lcd_spi_pre_transfer_callback,	//Specify pre-transfer callback to handle D/C line
	};
	spi_device_handle_t spi;
	//Attach the LCD to the SPI bus
	esp_err_t ret=spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
	ESP_ERROR_CHECK(ret);
	//Initialize the LCD
	lcd_init(spi);
	return spi;
}
