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
#include "hsv.h"

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

#define FIXED_SPEED (186<<FP_SHIFT)

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


void lcd_init_hw() {
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
}

void lcd_init() {
	lcddev=st7735_init(HSPI_HOST, PIN_NUM_CS, PIN_NUM_DC);
	st7735_ugui_init(lcddev);
	UG_FontSelect(&FONT_6X8);
	UG_SetForecolor(C_WHITE);
}



//#define WVLEN 900
#define WVLEN 1500
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
	vTaskDelay(10/portTICK_RATE_MS);
	i2s_parallel_start(&I2S1);

	printf("Printcart driver inited\n");
}

void printcart_init_tsk(void *arg) {
	TaskHandle_t *tsk=(TaskHandle_t*)arg;
	printcart_init();
	xTaskNotifyGive(*tsk);
	vTaskDelete(NULL);
}

void peripherals_init_first() {
	i2c_master_init();
	vTaskDelay(10/portTICK_RATE_MS);
	tca=malloc(sizeof(tca9534_dev_t));

	//Init TCA first, as the vl53l0x reset lines and the LCD reset lines are controlled by it
	ESP_ERROR_CHECK(tca9534_init(I2C_NUM_1, 0x20, tca));
	tca9534_set_dir(tca, 0);
	tca9534_set_output(tca, TCA_BOOSTEN);
}

void peripherals_init() {
	vTaskDelay(10/portTICK_RATE_MS);
	tcs=malloc(sizeof(tcs3472_dev_t));
	mpu=malloc(sizeof(mpu9250_dev_t));

	//Init VL53L0x and move them to new I2C addresses, as they occupy the same address as the TCS3472.
	const uint8_t vl_gpios[3]={TCA_XSHUT1, TCA_XSHUT2, TCA_XSHUT3};
	for (int i=0; i<3; i++) {
		tca9534_change_output(tca, 0, vl_gpios[i]);
		vTaskDelay(10/portTICK_RATE_MS);
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

	vl_task_init(vl_dev, 3);

	gpio_config_t io_conf={
		.mode=GPIO_MODE_INPUT,
		.pin_bit_mask=((1ULL<<PIN_NUM_BTNA)|(1ULL<<PIN_NUM_BTNB)),
		.pull_up_en=1,
	};
	gpio_config(&io_conf);

	//init print cart on specified core, wait until that thread is done
	TaskHandle_t tsk=xTaskGetCurrentTaskHandle();
	xTaskCreatePinnedToCore(printcart_init_tsk, "printcart_init_tsk", 1024*16, &tsk, 8, NULL, 1);
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

int get_cur_btn() {
	int v=0;
	if (!gpio_get_level(PIN_NUM_BTNB)) v|=1;
	if (!gpio_get_level(PIN_NUM_BTNA)) v|=2;
	return v;
}

void wait_btn_rel() {
	int v=0;
	do {
		vTaskDelay(50/portTICK_RATE_MS);
		v=get_cur_btn();
	} while(v);
}

int get_btn() {
	int v=0;
	wait_btn_rel();
	while(!v) {
		vTaskDelay(50/portTICK_RATE_MS);
		v=get_cur_btn();
	}
	return v;
}

void do_mona() {
	st7735_ugui_cls();
	UG_SetForecolor(C_YELLOW);
	UG_PutString(0,0, "MONA");
	st7735_ugui_flush();
	pixel_pusher_set_type(PP_MONA);
	int b=0;
	wait_btn_rel();
	int stripe=0;
	while(1) {
		b=get_cur_btn();
		if (b==1) {
			wait_btn_rel();
			return;
		} else if (b==2) {
//			st7735_ugui_cls();
//			UG_SetForecolor(C_WHITE);
//			st7735_ugui_flush();

			VL53L0X_RangingMeasurementData_t measurement_data[3]={0};
			//flush queue
			vl_task_get_results(&measurement_data[0]);
			vl_task_get_results(&measurement_data[0]);
			vl_task_get_results(&measurement_data[0]);
			do {
				vl_task_get_results(&measurement_data[0]);
				printf("dist: ");
				for (int i=0; i<3; i++) {
					printf("% 5d ", measurement_data[i].RangeMilliMeter);
				}
				printf("\n");
			} while (measurement_data[1].RangeMilliMeter<100);


			pixel_pusher_mona_set_stripe(stripe);
			pixel_pusher_set_speed_pos(0,0,FIXED_SPEED,FIXED_SPEED,1);
			wait_btn_rel();
			pixel_pusher_set_speed_pos(0,0,FIXED_SPEED,FIXED_SPEED,0);
			stripe++;
		}
		vTaskDelay(50/portTICK_RATE_MS);
	}
}

float hyp_vx, hyp_vy;
float hyp_x, hyp_y;
float startx, starty;

const float const_int_rate=0.5; //1 for more noise, 0 for smoother

void do_imu_magic(int ena) {
	static int prev_ena=0;
	float int_rate=const_int_rate;
	static int start_cd=0;

	VL53L0X_RangingMeasurementData_t measurement_data[3]={0};
	vl_task_get_results(&measurement_data[0]);
	int meas_x=measurement_data[1].RangeMilliMeter;
	int meas_y=-measurement_data[0].RangeMilliMeter;

	if (!prev_ena && ena) {
		hyp_vx=0;
		hyp_vy=0;
		int_rate=0.7;
		start_cd=20;
		startx=meas_x;
		starty=meas_y;
	}
	if (start_cd) {
		start_cd--;
		startx=meas_x;
		starty=meas_y;
	}

	prev_ena=ena;

	meas_x-=startx;
	meas_y-=starty;

	int meas_vx=meas_x-hyp_x;
	int meas_vy=meas_y-hyp_y;

	hyp_vx=(meas_vx*int_rate)+((hyp_vx*(1-int_rate)));
	hyp_vy=(meas_vy*int_rate)+((hyp_vy*(1-int_rate)));

	//update pos guess
	hyp_x+=hyp_vx;
	hyp_y+=hyp_vy;

//	printf("x=% 6.2f y=% 6.2f vx=% 6.2f vy=% 6.2f mx=%d my=%d\n", hyp_x, hyp_y, hyp_vx, hyp_vy, meas_x, meas_y);
	
//	ena=0;
	pixel_pusher_set_speed_pos((hyp_x-20)*(1<<FP_SHIFT),(hyp_y-20)*(1<<FP_SHIFT),hyp_vx*(1<<FP_SHIFT),hyp_vy*(1<<FP_SHIFT),ena);
}

void do_mona_imu() {
	st7735_ugui_cls();
	UG_SetForecolor(C_YELLOW);
	UG_PutString(0,0, "MONA_IMU");
	st7735_ugui_flush();
	pixel_pusher_set_type(PP_MONA_IMU);
	int b=0;
	wait_btn_rel();
	while(1) {
		b=get_cur_btn();
		if (b==1) {
			wait_btn_rel();
			return;
		} else if (b==2) {
			//flush queue
			for (int i=0; i<20; i++) do_imu_magic(1);
			while(get_cur_btn()) {
				do_imu_magic(1);
			}
			do_imu_magic(0);
		}
		vTaskDelay(50/portTICK_RATE_MS);
	}
}


void do_mine() {
	st7735_ugui_cls();
	UG_SetForecolor(C_YELLOW);
	UG_PutString(0,0, "MINE!");
	st7735_ugui_flush();
	pixel_pusher_set_type(PP_MINE);
	int b=0;
	wait_btn_rel();
	while(1) {
		b=get_cur_btn();
		if (b==1) {
			wait_btn_rel();
			return;
		} else if (b==2) {
			st7735_ugui_cls();
			UG_SetForecolor(C_PINK);
			UG_PutString(0,0, "MINE MINE MINE");
			st7735_ugui_flush();
			pixel_pusher_set_speed_pos(0,0,FIXED_SPEED,FIXED_SPEED,1);
			wait_btn_rel();
			pixel_pusher_set_speed_pos(0,0,FIXED_SPEED,FIXED_SPEED,0);
			st7735_ugui_cls();
			UG_SetForecolor(C_YELLOW);
			UG_PutString(0,0, "MINE!");
			st7735_ugui_flush();
		}
		vTaskDelay(50/portTICK_RATE_MS);
	}
}


void do_nyancat() {
	st7735_ugui_cls();
	UG_SetForecolor(C_YELLOW);
	UG_PutString(0,0, "NYAN NYAN NYAN NYAN");
	st7735_ugui_flush();
	pixel_pusher_set_type(PP_NYANCAT);
	int b=0;
	wait_btn_rel();
	while(1) {
		b=get_cur_btn();
		if (b==1) {
			wait_btn_rel();
			return;
		} else if (b==2) {
			st7735_ugui_cls();
			UG_SetForecolor(C_PINK);
			UG_PutString(0,0, "NYAN NYAN NYAN NYAN");
			st7735_ugui_flush();
			pixel_pusher_set_speed_pos(0,0,FIXED_SPEED,FIXED_SPEED,1);
			wait_btn_rel();
			pixel_pusher_set_speed_pos(0,0,FIXED_SPEED,FIXED_SPEED,0);
			st7735_ugui_cls();
			UG_SetForecolor(C_YELLOW);
			UG_PutString(0,0, "NYAN NYAN NYAN NYAN");
			st7735_ugui_flush();
		}
		vTaskDelay(50/portTICK_RATE_MS);
	}
}


void do_whee() {
	st7735_ugui_cls();
	UG_SetForecolor(C_YELLOW);
	UG_PutString(0,0, "WHEEEEEEEEEE");
	st7735_ugui_flush();
	pixel_pusher_set_type(PP_WHEE);
	int b=0;
	wait_btn_rel();
	while(1) {
		b=get_cur_btn();
		if (b==1) {
			wait_btn_rel();
			return;
		} else if (b==2) {
			st7735_ugui_cls();
			UG_SetForecolor(C_PINK);
			UG_PutString(0,0, "WHEEEEEEEEEE");
			st7735_ugui_flush();
			pixel_pusher_set_speed_pos(0,0,FIXED_SPEED,FIXED_SPEED,1);
			wait_btn_rel();
			pixel_pusher_set_speed_pos(0,0,FIXED_SPEED,FIXED_SPEED,0);
			st7735_ugui_cls();
			UG_SetForecolor(C_YELLOW);
			UG_PutString(0,0, "WHEEEEEEEEEE");
			st7735_ugui_flush();
		}
		vTaskDelay(50/portTICK_RATE_MS);
	}
}


int rgbtocol(int r, int g, int b) {
	if (r>255) r=255;
	if (g>255) g=255;
	if (b>255) b=255;
	if (r<0) r=0;
	if (g<0) g=0;
	if (b<0) b=0;
	return ((r>>3)<<11)|((g>>2)<<5)|(b>>3);
}

#define MAX_R 1165
#define MAX_G 1880
#define MAX_B 2600

void do_colorcopy() {
	st7735_ugui_flush();
	int state=0;
	wait_btn_rel();
	pixel_pusher_set_type(PP_COLOR);
	while(1) {
		if (state==0 || state==1) {
			tca9534_change_output(tca, 0, TCA_WHLED);
		} else {
			tca9534_change_output(tca, TCA_WHLED, 0);
		}
		int cc=0, cr=0, cg=0, cb=0;
		ESP_ERROR_CHECK(tcs3472_get_input(tcs, &cr, &cg, &cb, &cc));
		int rgb[2][3];

		RgbColor c;
		cr=(cr*255)/MAX_R;
		cg=(cg*255)/MAX_G;
		cb=(cb*255)/MAX_B;
		if (cr>255) cr=255;
		if (cg>255) cg=255;
		if (cb>255) cb=255;
		c.r=cr; c.g=cg; c.b=cb;
		HsvColor hsv=RgbToHsv(c);
		int sat=hsv.s;
		sat+=50;
		if (sat>255) sat=255;
		hsv.s=sat;
		int value=hsv.v;
		if (value<128) {
			value+=64;
			if (value>128) value=128;
		}
		hsv.v=value;
		c=HsvToRgb(hsv);
		if (state==0) {
			rgb[0][0]=c.r;
			rgb[0][1]=c.g;
			rgb[0][2]=c.b;
		} else if (state==1) {
			rgb[1][0]=c.r;
			rgb[1][1]=c.g;
			rgb[1][2]=c.b;
		}

		st7735_ugui_cls();
		UG_SetForecolor(C_YELLOW);
		UG_PutString(0,0, "COPY-A-COLOR");
		UG_SetForecolor(C_WHITE);
		char buff[128];
		sprintf(buff, "%06d %06d %06d %06d\n", cr, cg, cb, cc);
		UG_PutString(10,10, buff);
		UG_SetForecolor(rgbtocol(rgb[0][0], rgb[0][1], rgb[0][2]));
		UG_PutString(16, 26, "COLOR 1");
		UG_SetForecolor(rgbtocol(rgb[1][0], rgb[1][1], rgb[1][2]));
		UG_PutString(16, 38, "COLOR 2");
		st7735_ugui_flush();
		int b=get_cur_btn();
		if (b==1) {
			state++;
			if (state==3) state=0;
			wait_btn_rel();
		}
		if (b==2) {
			if (state==0 || state==1) break;
			pixel_pusher_color_set(rgb[0][0], rgb[0][1], rgb[0][2], 0);
			pixel_pusher_color_set(rgb[1][0], rgb[1][1], rgb[1][2], 1);

			pixel_pusher_set_speed_pos(0, 0, FIXED_SPEED, FIXED_SPEED, 1);
			wait_btn_rel();
			pixel_pusher_set_speed_pos(42<<FP_SHIFT, 0, FIXED_SPEED, FIXED_SPEED, 0);
		}
	}
	tca9534_change_output(tca, TCA_WHLED, 0);
}

#define UI_EV_STARTED (1<<0)


void ui_proc(void *arg) {
	lcd_init();

	UG_FontSelect(&FONT_6X8);
	UG_SetForecolor(C_WHITE);
	UG_PutString(10, 10, "INITIALIZING");
	st7735_ugui_flush();
	xTaskNotifyWait(0, UI_EV_STARTED, NULL, portMAX_DELAY);

	const char *menu[]={
		"NYANCAT",
		"COLOR COPY",
		"MONA",
		"MINE!",
		"MONA_IMU",
		"WHEE (B)",
		NULL
	};

	int sel=0;
	while(1) {
		st7735_ugui_cls();
		int i=0;
		UG_SetForecolor(C_YELLOW);
		UG_PutString(0,0, "MAGIC PAINTBRUSH MENU");
		UG_SetForecolor(C_WHITE);
		while (menu[i]) {
			UG_PutString(8, i*8+16, menu[i]);
			i++;
		}
		UG_SetForecolor(C_YELLOW);
		UG_PutString(0, sel*8+16, "*");
		st7735_ugui_flush();
		int b=get_btn();
		if (b==1) {
			sel++;
			if (menu[sel]==NULL) sel=0;
		}
		if (b==2) {
			if (sel==0) do_nyancat();
			if (sel==1) do_colorcopy();
			if (sel==2) do_mona();
			if (sel==3) do_mine();
			if (sel==4) do_mona_imu();
			if (sel==5) do_whee();
		}
	}

	vTaskDelete(NULL);
}


#define OFF_MUL (64<<FP_SHIFT)
#define SPEED 110

int app_main(void) {
	peripherals_init_first();
	lcd_init_hw();
	TaskHandle_t ui_tid;
	xTaskCreatePinnedToCore(ui_proc, "ui", 1024*16, NULL, 7, &ui_tid, 0);
	peripherals_init();
	pixel_pusher_init();
	xTaskNotify(ui_tid, UI_EV_STARTED, eSetBits);

#if 0

	VL53L0X_RangingMeasurementData_t measurement_data[3]={0};
	int prev_meas[3], diff[3];
	int prev_ena=0;
	int start[2];
	while(1) {
		int ena=(!gpio_get_level(PIN_NUM_BTNA));
		char buff[256];
//		st7735_ugui_cls();
		vl_task_get_results(&measurement_data[0]);

		printf("%llu: %d %d %d\n", esp_timer_get_time(), 
				measurement_data[0].RangeMilliMeter,
				measurement_data[1].RangeMilliMeter,
				measurement_data[2].RangeMilliMeter);
#if 0
		for (int x=0; x<3; x++) {
			diff[x]=measurement_data[x].RangeMilliMeter-prev_meas[x];
			prev_meas[x]=measurement_data[x].RangeMilliMeter;
		}
		
		if (ena!=prev_ena) {
			//only use left and top vls for now
			start[0]=measurement_data[1].RangeMilliMeter;
			start[1]=measurement_data[0].RangeMilliMeter;
		}
#endif
//		pixel_pusher_set_speed_pos((measurement_data[1].RangeMilliMeter-start[0])*OFF_MUL, 
//				(measurement_data[0].RangeMilliMeter-start[1])*OFF_MUL,
//				diff[1]*SPEED, diff[0]*SPEED, ena);

/*
		sprintf(buff, "%d / %d / %d mm\n", measurement_data[0].RangeMilliMeter, measurement_data[1].RangeMilliMeter, measurement_data[2].RangeMilliMeter);
		UG_PutString(0, 0, buff);

		int cc=0, cr=0, cg=0, cb=0;
		ESP_ERROR_CHECK(tcs3472_get_input(tcs, &cr, &cg, &cb, &cc));
		sprintf(buff, "RGBC %d %d %d %d\n", cr, cg, cb, cc);
		UG_PutString(0, 8, buff);
*/
/*
		mpu9250_accel_tp mpumeas[128];
		uint64_t ts=esp_timer_get_time();
		int mpuct=mpu9250_read_fifo(mpu, mpumeas, 128);
		for (int i=0; i<mpuct; i++) {
			printf("%llu: %d/%d % 6d % 6d % 6d % 6d % 6d % 5d\n", esp_timer_get_time(),
				i, mpuct, mpumeas[i].accelx, mpumeas[i].accely, mpumeas[i].accelz, mpumeas[i].gyrox, mpumeas[i].gyroy, mpumeas[i].gyroz);
		}
*/
//		sprintf(buff, "Accel ct %d\n%d %d %d\n", mpuct, mpumeas[0].accelx, mpumeas[0].accely, mpumeas[0].accelz);
//		UG_PutString(0, 16, buff);

//		st7735_ugui_flush();


		prev_ena=ena;
	}
#endif
	return 0;
}

