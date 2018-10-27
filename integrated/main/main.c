#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "driver/rmt.h"
#include "esp_now.h"
#include "esp_log.h"
#include <string.h>
#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_helper.h"
#include "tca9534.h"
#include "st7735.h"
#include "st7735_ugui.h"
#include "ugui.h"
#include "tcs3472.h"
#include "mpu9250.h"
#include "vl_task.h"
#include "printcart_genwaveform.h"
#include "printcart_i2s.h"
#include "pixelpusher.h"

#define PIN_NUM_MOSI 33
#define PIN_NUM_CLK	 25
#define PIN_NUM_CS	 26
#define PIN_NUM_DC	 0

#define PIN_NUM_CART_NOZD_M 12
#define PIN_NUM_CART_NOZD_C 27
#define PIN_NUM_CART_NOZD_Y 13
#define PIN_NUM_CART_OPTD 14
#define PIN_NUM_CART_OPT2 32
#define PIN_NUM_CART_OPT4 2
#define PIN_NUM_CART_OPT1 4
#define PIN_NUM_CART_OPT5 5
#define PIN_NUM_CART_NOZCLK 18
#define PIN_NUM_CART_OPT3 19
#define PIN_NUM_CART_PWRA 15
#define PIN_NUM_CART_PWRB 21

#define PIN_NUM_BTNA 34
#define PIN_NUM_BTNB 35

#define PIN_NUM_SDA 23
#define PIN_NUM_SCL 22

#define TCA_XSHUT3 (1<<0)
#define TCA_XSHUT1 (1<<1)
#define TCA_BOOSTEN (1<<2)
#define TCA_WHLED (1<<3)
#define TCA_LCDRST (1<<4)
#define TCA_BKLT (1<<5)
#define TCA_XSHUT2 (1<<7)

static VL53L0X_Dev_t vl_dev[3];
spi_device_handle_t lcddev;
tca9534_dev_t *tca;
tcs3472_dev_t *tcs;
mpu9250_dev_t *mpu;

uint16_t *fb[160*80];


static void i2c_master_init()
{
	i2c_port_t i2c_master_port = I2C_NUM_1;
	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = PIN_NUM_SDA;
	conf.sda_pullup_en = GPIO_PULLUP_DISABLE;
	conf.scl_io_num = PIN_NUM_SCL;
	conf.scl_pullup_en = GPIO_PULLUP_DISABLE;
	conf.master.clk_speed = 400000;

	ESP_ERROR_CHECK(i2c_param_config(i2c_master_port, &conf));
	ESP_ERROR_CHECK(i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0));
}


void lcd_init() {
	esp_err_t ret;
	spi_bus_config_t buscfg={
		.miso_io_num=-1,
		.mosi_io_num=PIN_NUM_MOSI,
		.sclk_io_num=PIN_NUM_CLK,
		.quadwp_io_num=-1,
		.quadhd_io_num=-1,
		.max_transfer_sz=160*80*2+10,
	};
	//Initialize the SPI bus
	ret=spi_bus_initialize(HSPI_HOST, &buscfg, 1);
	ESP_ERROR_CHECK(ret);

	//Reset and turn on backlight
	tca9534_change_output(tca, TCA_LCDRST, 0);
	vTaskDelay(10/portTICK_RATE_MS);
	tca9534_change_output(tca, 0, TCA_LCDRST|TCA_BKLT);
	vTaskDelay(10/portTICK_RATE_MS);

	lcddev=st7735_init(HSPI_HOST, PIN_NUM_CS, PIN_NUM_DC);
	st7735_ugui_init(lcddev);
	UG_FontSelect(&FONT_6X8);
	UG_SetForecolor(C_WHITE);
	UG_PutString(0, 0, "HELLO WORLD!");
	st7735_ugui_flush();
}



#define WVLEN 900
void printcart_init() {
	i2s_parallel_buffer_desc_t bufdesc[2][2];
	i2s_parallel_config_t i2scfg={
		.gpio_bus={
			PIN_NUM_CART_NOZD_C, //0
			PIN_NUM_CART_NOZD_M, //1
			PIN_NUM_CART_NOZD_Y, //2
			PIN_NUM_CART_OPTD, //3
			PIN_NUM_CART_OPT2, //4
			PIN_NUM_CART_OPT4, //5
			PIN_NUM_CART_OPT1, //6
			PIN_NUM_CART_OPT5, //7
			PIN_NUM_CART_NOZCLK, //8
			PIN_NUM_CART_OPT3, //9
			PIN_NUM_CART_PWRA, //10
			PIN_NUM_CART_PWRB, //11
			-1, -1, -1, -1 //12-15
		},
		.gpio_clk=-1,
		.bits=I2S_PARALLEL_BITS_16,
		.clkspeed_hz=16*1000*1000, //note: ignored for now by driver?
		.bufa=bufdesc[0],
		.bufb=bufdesc[1],
	};

	uint16_t *mema=heap_caps_malloc(WVLEN*2, MALLOC_CAP_DMA);
	uint16_t *memb=heap_caps_malloc(WVLEN*2, MALLOC_CAP_DMA);
	memset(mema, 0, WVLEN*2);
	memset(memb, 0, WVLEN*2);
	bufdesc[0][0].memory=mema;
	bufdesc[0][0].size=WVLEN*2;
	bufdesc[1][0].memory=memb;
	bufdesc[1][0].size=WVLEN*2;
	bufdesc[0][1].memory=NULL;
	bufdesc[1][1].memory=NULL;

	i2s_parallel_setup(&I2S1, &i2scfg);
	vTaskDelay(5);
	i2s_parallel_start(&I2S1);

	printf("Printcart driver inited\n");
	vTaskDelete(NULL);
}

void peripherals_init() {
	i2c_master_init();
	vTaskDelay(10);
	tca=malloc(sizeof(tca9534_dev_t));
	tcs=malloc(sizeof(tcs3472_dev_t));
	mpu=malloc(sizeof(mpu9250_dev_t));

	//Init TCA first, as the vl53l0x reset lines and the LCD reset lines are controlled by it
	ESP_ERROR_CHECK(tca9534_init(I2C_NUM_1, 0x20, tca));
	tca9534_set_dir(tca, 0);
	tca9534_set_output(tca, TCA_BOOSTEN);

	//Init VL53L0x and move them to new I2C addresses, as they occupy the same address as the TCS3472.
	const uint8_t vl_gpios[3]={TCA_XSHUT1, TCA_XSHUT2, TCA_XSHUT3};
	for (int i=0; i<3; i++) {
		tca9534_change_output(tca, 0, vl_gpios[i]);
		vTaskDelay(1);
		VL53L0X_Error status;
		vl_dev[i].i2c_port_num = I2C_NUM_1;
		vl_dev[i].i2c_address = 0x29;
		int new_addr=0x29+1+i;
		printf("Moving dev %d to new addr %x...\n", i, new_addr);
		status=VL53L0X_SetDeviceAddress(&vl_dev[i], (0x29+1+i)*2);
		if (status != VL53L0X_ERROR_NONE) printf("Moving failed. Dev already at new addr?\n");
		vl_dev[i].i2c_address = new_addr;
		printf("Init dev %d\n", i);
		status = init_vl53l0x(&vl_dev[i]);
		assert(status == VL53L0X_ERROR_NONE);
		printf("Inited VL53L0X no %d\n", i);
	}
	
	ESP_ERROR_CHECK(mpu9250_init(I2C_NUM_1, 0x69, mpu));
	ESP_ERROR_CHECK(tcs3472_init(I2C_NUM_1, 0x29, tcs));

	lcd_init();

	vl_task_init(vl_dev, 3);

	gpio_config_t io_conf={
		.mode=GPIO_MODE_INPUT,
		.pin_bit_mask=((1ULL<<PIN_NUM_BTNA)|(1ULL<<PIN_NUM_BTNB)),
		.pull_up_en=1,
	};
	gpio_config(&io_conf);

	//init print cart on core 0
	xTaskCreatePinnedToCore(printcart_init, "printcart_init", 1024*16, NULL, 8, NULL, 1);
}

int app_main(void) {
	peripherals_init();

	pixel_pusher_init();
	UG_FontSelect(&FONT_6X8);
	UG_SetForecolor(C_WHITE);
	VL53L0X_RangingMeasurementData_t measurement_data[3]={0};
	while(1) {
		pixel_pusher_ena(!gpio_get_level(PIN_NUM_BTNA));
		char buff[256];
		st7735_ugui_cls();

		vl_task_get_results(&measurement_data[0]);
		sprintf(buff, "%d / %d / %d mm\n", measurement_data[0].RangeMilliMeter, measurement_data[1].RangeMilliMeter, measurement_data[2].RangeMilliMeter);
		UG_PutString(0, 0, buff);

		int cc=0, cr=0, cg=0, cb=0;
		ESP_ERROR_CHECK(tcs3472_get_input(tcs, &cr, &cg, &cb, &cc));
		sprintf(buff, "RGBC %d %d %d %d\n", cr, cg, cb, cc);
		UG_PutString(0, 8, buff);

		mpu9250_accel_tp mpumeas[128];
		int mpuct=mpu9250_read_fifo(mpu, mpumeas, 128);
		sprintf(buff, "Accel ct %d\n%d %d %d\n", mpuct, mpumeas[0].accelx, mpumeas[0].accely, mpumeas[0].accelz);
		UG_PutString(0, 16, buff);

		st7735_ugui_flush();
	}
}

