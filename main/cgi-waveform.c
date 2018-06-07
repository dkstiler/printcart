#include <esp8266.h>
#include "cgi.h"
#include "io.h"
#include "i2s_parallel.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

static i2s_parallel_buffer_desc_t bufdesc[2][2];

static TimerHandle_t stop_timer;

#define WVLEN 2048

static void my_timer_fn(void *arg) {
	i2s_parallel_flip_to_buffer(&I2S1, 1);
}


void cgiWaveformInit() {
	i2s_parallel_config_t cfg={
		.gpio_bus={2, 15, 4, 16, 27, 17,5, 18, 19, 21, 26, 25, 22, 23, -1, -1, -1},
		.gpio_clk=-1,
		.bits=I2S_PARALLEL_BITS_16,
		.clkspeed_hz=16*1000*1000,
		.bufa=bufdesc[0],
		.bufb=bufdesc[1],
	};
	bufdesc[0][0].memory=calloc(WVLEN, 2);
	bufdesc[0][0].size=WVLEN*2;
	bufdesc[1][0].memory=calloc(32, 2);
	bufdesc[1][0].size=32*2;

	bufdesc[0][1].memory=NULL;
	bufdesc[1][1].memory=NULL;
    i2s_parallel_setup(&I2S1, &cfg);
	
	stop_timer=xTimerCreate("sqstop", pdMS_TO_TICKS(1000), pdFALSE, NULL, my_timer_fn);

}

void timer_fn(void *arg) {
	i2s_parallel_flip_to_buffer(&I2S1, 1);
}

int cgiWaveform(HttpdConnData *connData) {
	int len;
	char buff[1024];
	
	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	len=httpdFindArg(connData->post->buff, "wv", buff, sizeof(buff));
	if (len>0) {
		uint16_t *wv=(uint16_t*)bufdesc[0][0].memory;
		memset(wv, 0, WVLEN*2);
		char *p=buff;
		int i=0;
		printf("Recved: %s\n", buff);
		int last=0;
		while (*p!=0) {
			//note: i2s samples are stored 103254 etc
			last=strtol(p, NULL, 16);
			wv[i^1]=last;
			p+=5;
			i++;
		}
		uint16_t *wv2=(uint16_t*)bufdesc[1][0].memory;
		for (int i=0; i<32; i++) wv2[i]=last;

		i2s_parallel_flip_to_buffer(&I2S1, 0);
		i2s_parallel_start(&I2S1, 0);
		len=httpdFindArg(connData->post->buff, "tm", buff, sizeof(buff));
		if (len>0) {
			xTimerStart(stop_timer, 10);
		}
	}

	httpdStartResponse(connData, 200);
	return HTTPD_CGI_DONE;
}

