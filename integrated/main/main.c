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

static VL53L0X_Dev_t vl_dev[2];

static void i2c_master_init()
{
	i2c_port_t i2c_master_port = I2C_NUM_1;
	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = GPIO_NUM_23;
	conf.sda_pullup_en = GPIO_PULLUP_DISABLE;
	conf.scl_io_num = GPIO_NUM_22;
	conf.scl_pullup_en = GPIO_PULLUP_DISABLE;
	conf.master.clk_speed = 400000;

	ESP_ERROR_CHECK(i2c_param_config(i2c_master_port, &conf));
	ESP_ERROR_CHECK(i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0));
}


#define PIN_NUM_MOSI 33
#define PIN_NUM_CLK	 25
#define PIN_NUM_CS	 26
#define PIN_NUM_DC	 0


#define TCA_XSHUT3 (1<<0)
#define TCA_XSHUT1 (1<<1)
#define TCA_BOOSTEN (1<<2)
#define TCA_WHLED (1<<3)
#define TCA_LCDRST (1<<4)
#define TCA_BKLT (1<<5)
#define TCA_XSHUT2 (1<<7)

spi_device_handle_t lcddev;
tca9534_dev_t *tca;
tcs3472_dev_t *tcs;
mpu9250_dev_t *mpu;

uint16_t *fb[160*80];

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


int app_main(void)
{
	i2c_master_init();
	printf("Tca9534 init\n");
	tca=malloc(sizeof(tca9534_dev_t));
	tcs=malloc(sizeof(tcs3472_dev_t));
	mpu=malloc(sizeof(mpu9250_dev_t));
	ESP_ERROR_CHECK(tca9534_init(I2C_NUM_1, 0x20, tca));
	tca9534_set_dir(tca, 0);
	tca9534_set_output(tca, TCA_BOOSTEN);
	lcd_init();

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
	//Do this now; all conflicting VL* chips are moved away...
	ESP_ERROR_CHECK(tcs3472_init(I2C_NUM_1, 0x29, tcs));

	while(1) {
		VL53L0X_RangingMeasurementData_t measurement_data[3];
		for (int i=0; i<3; i++) {
			VL53L0X_Error status = take_reading(&vl_dev[i], &measurement_data[i]);
			assert(status == VL53L0X_ERROR_NONE);
		}
		printf("%d / %d / %d mm\n", measurement_data[0].RangeMilliMeter, measurement_data[1].RangeMilliMeter, measurement_data[2].RangeMilliMeter);
//		vTaskDelay(10);
		int cc, cr, cg, cb;
		ESP_ERROR_CHECK(tcs3472_get_input(tcs, &cr, &cg, &cb, &cc));
		printf("RGBC %d %d %d %d\n", cr, cg, cb, cc);
		mpu9250_accel_tp mpumeas[128];
		int mpuct=mpu9250_read_fifo(mpu, mpumeas, 128);
		printf("Accel %d %d %d\n", mpumeas[0].accelx, mpumeas[0].accely, mpumeas[0].accelz);
	}
}

