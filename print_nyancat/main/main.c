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
#include "soc/timer_group_struct.h"
#include "printcart_i2s.h"
#include "printcart_genwaveform.h"

#define NYAN_REP_START 138
#define NYAN_REP_END 206
extern const uint8_t nyanrgb_start[]   asm("_binary_nyan_84_rgb_start");
extern const uint8_t nyanrgb_end[]     asm("_binary_ nyan_84_rgb_end");

#define SCALE 4

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

#define GPIO_BTN (32UL)


extern int printcart_mem_words_used;

#define CMY_OFF 32

void pixel_pusher_task(void *arg) {
	printf("Ready.\n");
	int ypos[3]={0};
	uint8_t pixels[14*3];
	int t=0;
	while(1) {
		t++;
		printcart_line_clear(&pixels[0]);
		printcart_line_clear(&pixels[14]);
		printcart_line_clear(&pixels[28]);
		if (gpio_get_level(GPIO_BTN)) {
			//if (ypos[0]) printf("Draw STOP. Used %d words of dma mem\n", printcart_mem_words_used);
			//LTR: Cyan is leading, magenta in the middle, yellow trailing.
			//Order of colors are (1-RGB): Cyan, magenta, yellow.
			ypos[0]=-1;
			ypos[1]=-CMY_OFF-1;
			ypos[2]=-CMY_OFF*2-1;
		} else {
			//Infinitely repeat tail
			const uint8_t *p[3];
			for (int c=0; c<3; c++) {
				ypos[c]++;
				if (ypos[c]==NYAN_REP_END*SCALE) ypos[c]=NYAN_REP_START*SCALE;
				p[c]=&nyanrgb_start[(ypos[c]/SCALE)*84*3+c];
			}
			for (int x=14; x<14+84; x++) {
				for (int c=0; c<3; c++) {
					if ((255-*p[c])>(rand()&0x3ff)) {
						if (ypos[c]>=0) printcart_line_set_pixel(&pixels[c*14], x, c);
					}
					p[c]+=3;
				}
			}
		}
		i2s_push_pixels(&I2S1, pixels);
	}
}


int app_main(void)
{
#if 0
    nvs_flash_init();
    tcpip_adapter_init();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    wifi_config_t sta_config = {
        .sta = {
            .ssid = "Sprite",
            .password = "pannenkoek",
            .bssid_set = false
        }
    };
    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
    ESP_ERROR_CHECK( esp_wifi_connect() );
#endif

	gpio_config_t io_conf={
		.mode=GPIO_MODE_INPUT,
		.pin_bit_mask=(1ULL<<GPIO_BTN),
		.pull_up_en=1,
	};
	gpio_config(&io_conf);

	i2s_parallel_buffer_desc_t bufdesc[2][2];

	i2s_parallel_config_t i2scfg={
		.gpio_bus={15, 2, 4, 16, 27, 17,5, 18, 19, 21, 26, 25, 22, 23, -1, -1, -1},
		.gpio_clk=-1,
		.bits=I2S_PARALLEL_BITS_16,
		.clkspeed_hz=16*1000*1000,
		.bufa=bufdesc[0],
		.bufb=bufdesc[1],
	};

#define WVLEN 900
	uint16_t *mema=calloc(WVLEN, 2);
	uint16_t *memb=calloc(WVLEN, 2);
	bufdesc[0][0].memory=mema;
	bufdesc[0][0].size=WVLEN*2;
	bufdesc[1][0].memory=memb;
	bufdesc[1][0].size=WVLEN*2;
	bufdesc[0][1].memory=NULL;
	bufdesc[1][1].memory=NULL;

    i2s_parallel_setup(&I2S1, &i2scfg);
	vTaskDelay(5);
	i2s_parallel_start(&I2S1);

	xTaskCreatePinnedToCore(pixel_pusher_task, "pixpush", 1024*16, NULL, 7, NULL, 1);
	return 0;
}

