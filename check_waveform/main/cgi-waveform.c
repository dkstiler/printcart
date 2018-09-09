#include <esp8266.h>
#include "cgi.h"
#include "io.h"
#include "i2s_parallel.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

static i2s_parallel_buffer_desc_t bufdesc[2][2];

static TimerHandle_t stop_timer;

#define WVLEN (1024*20)

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

static void line_clear(uint8_t *l) {
	for (int i=0; i<14; i++) l[i]=0xff;
}

static void line_set_pixel(uint8_t *l, int p, int col) {
	int bo[3][14]={
		{11,2,7,12,3,8,13,4,9,0,5,10,1,6},
		{8,13,4,9,0,5,10,1,6,11,2,7,12,3},
		{0,5,10,1,6,11,2,7,12,3,8,13,4,9}
	};
	if (p<14) return;
	if (p>(7*14)) return;
	int byteno=bo[col][p%14];
	int bitno=p/14;
	l[byteno]&=~(1<<bitno);
}


int add_waveform(uint16_t *w, uint16_t *tp, uint8_t *c, uint8_t *m, uint8_t *y, int l) {
	int p=0;
	for (int j=0; j<14; j++) {
		int bit=-1;
		int ov=0;
		for (int i=0; i<l; i++) {
			uint16_t v=tp[i];
			//Increase bitno if change on 1st 2 lines
			if ((v&(1<<0))!=ov) {
				bit++;
				printf("%d\n", bit);
				ov=v&(1<<0);
			}
			
			//Set bit values for [cmy] streams
			int en=v&((1<<0)|(1<<1)); //only send out bit if bits 0 or 1 are 1
			v&=~((1<<0)|(1<<1)|(1<<2));
			if (bit<8 && bit>=0 && en) {
				if (c[j]&(1<<bit)) v|=(1<<0);
				if (m[j]&(1<<bit)) v|=(1<<1);
				if (y[j]&(1<<bit)) v|=(1<<2);
			}

			//Select correct value for seg ena
			v&=~(1<<3);
			if (j==13) { //last one
				if (v&(1<<14)) v|=(1<<3);
			} else {
				if (v&(1<<15)) v|=(1<<3);
			}
			w[p^1]=v;
			p++;
		}
		for (int i=0; i<l; i++) {
			w[p^1]=0;
			p++;
		}
	}
	for (int i=0; i<l*8; i++) {
			//note: i2s samples are stored 10325476 etc
		w[p^1]=0;
		p++;
	}

	return (p+1)&~1;
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
		uint16_t wf_desc[64]={0};
		while (*p!=0) {
			wf_desc[i]=strtol(p, NULL, 16);
			p+=5;
			i++;
		}
		int tp_len=i;
		int pos=0;
		for (int i=0; i<16; i++) {
			uint8_t c[14], m[14], y[14];
			line_clear(c);
			line_clear(m);
			line_clear(y);
			for (int j=0; j<7; j++) {
				line_set_pixel(c, i+16*j, 0);
				line_set_pixel(m, i+16*j, 1);
				line_set_pixel(y, i+16*j, 2);
			}
			pos+=add_waveform(&wv[pos], wf_desc, c, m, y, tp_len);
		}
		//make sure idle buff is clear
		uint16_t *wv2=(uint16_t*)bufdesc[1][0].memory;
		for (int i=0; i<32; i++) wv2[i]=0;

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

