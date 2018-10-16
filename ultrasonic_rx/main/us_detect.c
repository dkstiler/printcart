#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_spi_flash.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/i2s.h"
#include "driver/adc.h"

//Useful URL if you're into this DSP stuff: https://dspguru.com/dsp/tricks/

//Does an approximation of the magnitude of the vector [x,y], normally
//calculated using sqrt(x^2+y^2). See Wikipedia for more info:
//https://en.wikipedia.org/wiki/Alpha_max_plus_beta_min_algorithm
int magnitude_est(int x, int y) {
	const int alpha=30, beta=15, div=32;
	if (x<0) x=-x;
	if (y<0) y=-y;
	//use x as max, y as min
	if (x<y) {
		//swap around
		int t=y; 
		y=x;
		x=t;
	}
	//now x is max and y is min
	return (alpha*x+beta*y)/div;
}

void flush_i2s() {
	size_t sl;
	uint16_t samp;
	do {
		i2s_read(0, (void*)&samp, 2, &sl, 0);
	} while (sl==2);
}

void us_init() {
	 i2s_config_t i2s_config = {
		.mode = I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN,
		.sample_rate =	(40500*8)/16, //40KHz input signal, sample 8 times per input signal cycle
		.bits_per_sample = 16,
		.communication_format = I2S_COMM_FORMAT_I2S_MSB,
		.channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
		.intr_alloc_flags = 0,
		.dma_buf_count = 16,
		.dma_buf_len = 1024,
		.use_apll=true
	};
	//install and start i2s driver
	i2s_driver_install(0, &i2s_config, 0, NULL);
	//init ADC pad
	i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_0);
	i2s_adc_enable(0);
	i2s_stop(0);
	flush_i2s();
}



volatile int running=0;

//int64_t lastping;

void us_mark_start() {
//	int64_t nowping=esp_timer_get_time();
	if (!running) i2s_start(0);
	running=1;
//	printf("Ping delay %d\n", (int)(nowping-lastping));
//	lastping=nowping;
}

//Moving average over hlen samples
static inline void mov_avg_filter(int hlen, int16_t *samps, int samplen) {
	int16_t hist[hlen];
	memset(hist, 0, hlen*sizeof(hist[0]));
	int hpos=0;
	int avg=0;
	for (int i=0; i<samplen; i++) {
		avg-=hist[hpos];
		avg+=samps[i];
		hist[hpos]=samps[i];
		if (i>=hlen) {
			samps[i]=avg/hlen;
		} else {
			samps[i]=0;
		}
		hpos++;
		if (hpos==hlen) hpos=0;
	}
}


#define PARSE_LEN 2048


void us_run() {
	uint16_t *samp;
	int16_t *ival, *qval, *mul, *devv;
	samp=calloc(sizeof(uint16_t), PARSE_LEN);
	ival=calloc(sizeof(int16_t), PARSE_LEN);
	qval=calloc(sizeof(int16_t), PARSE_LEN);
	mul=calloc(sizeof(int16_t), PARSE_LEN);
	devv=calloc(sizeof(int16_t), PARSE_LEN);
	
//	const int8_t rphase[]={0,1,0,0,1,1,1}; //Note: each phase is held for 4 40KHz cycles
	const int8_t rphase[]={1,1,0,0}; //Note: each phase is held for 4 40KHz cycles
	const int rphase_samps=(8*8); //samples per one rphase entry
	const int rphase_len=sizeof(rphase)*rphase_samps;
	const int sinmul[8]={0, 5, 7, 5, 0, -5, -7, -5};

	while(1) {
		size_t l=0;
		//Read some samples
		i2s_read(0, (void*)samp, PARSE_LEN*2, &l, portMAX_DELAY);

#if 0
		//Fake sig
		for (int i=0; i<rphase_len; i++) {
			int m=10;
			if (rphase[i/rphase_samps]) m=-10;
			samp[(i+1000)^1]+=(m*sinmul[(i+7)&7]);
		}
#endif

		l=l/sizeof(int16_t); //bytes -> samples
		//QAM-demodulate the signal 
		int sp=0, cp=2;
		for (int i=0; i<l; i++) {
			//I is incoming signal * sin(40KHz), Q is incoming signal * cos(40KHz).
			//Because the sample freq is a nice multiple of the carrier freq, we hardcode the sine/cosine here.
			ival[i]=((int)samp[i^1]*sinmul[sp&7])/8;
			qval[i]=((int)samp[i^1]*sinmul[cp&7])/8;
			sp++; cp++;
		}



		//Filter to get the envelope of the i/q values
		mov_avg_filter(8, ival, l);
		mov_avg_filter(8, qval, l);

		int start=0;
		int val_samps=0;
		for (start=0; start<l; start++) {
			if (magnitude_est(ival[start], qval[start])>4) val_samps++;
			if (val_samps>100) {
				start-=100;
				break;
			}
		}


		//We're only interested in the phase of the signal, not the amplitude, so we normalize the I and Q streams
		//if they have any amplitude at all.
#if 1
		for (int i=0; i<l; i++) {
			int amp=magnitude_est(ival[i], qval[i]);
			if (amp>2) {
				ival[i]=(((int)ival[i])<<8)/amp;
				qval[i]=(((int)qval[i])<<8)/amp;
			} else {
				ival[i]=0; qval[i]=0;
			}
		}
#endif

		//There's no need to do cross-correlation ot a range that has no amplitude; pre-process the mul array
		//to filter those out.
		int skip=rphase_len;
		int found_zeroes=0;
		for (int i=l-1; i>=0; i--) {
			if (ival[i]+qval[i]==0) found_zeroes++; else found_zeroes=0;
			if (found_zeroes>3) skip=rphase_len*0.5;
			if (skip) skip--;
			mul[i]=skip?0:1;
		}

		//We expect the cross-correlation to be between 0 and 600 samples from the start of amplitude ramp-up.
		int valid_samps=0;
		for (int i=0; i<l; i++) {
			if (mul[i]) {
				valid_samps++;
				if (valid_samps>230) mul[i]=0;
			} else {
				valid_samps=0;
			}
		}


		//Do correlation with the known pseudo-random ping M-sequence
		//http://www.commsp.ee.ic.ac.uk/~mrt102/projects/mls/MLS%20Theory.pdf for some related info
		for (int i=0; i<l-sizeof(rphase)*rphase_samps; i++) {
			int ci, cq;
			ci=0; cq=0;
			if (mul[i]) {
				for (int j=0; j<sizeof(rphase)*rphase_samps; j+=rphase_samps) {
					if (rphase[j/rphase_samps]==0) {
						for (int k=0; k<rphase_samps; k++) {
							ci+=ival[i+j+k];
							cq+=qval[i+j+k];
						}
					} else {
						for (int k=0; k<rphase_samps; k++) {
							ci-=ival[i+j+k];
							cq-=qval[i+j+k];
						}
					}
				}
			}
			int m=magnitude_est(ci, cq)/(rphase_samps*2);
			mul[i]=m;
		}


		//Find max value
		int max=0;
		for (int j=1; j<l; j++) {
			if (mul[j]>mul[max]) max=j;
		}

		const int delta_avg=10;
		int pos_run=0, neg_run=0, prevc=0, dmax=0, dmaxpos=0;
		for (int i=delta_avg; i<l-delta_avg; i++) {
			int c;
			c=0;
			if (mul[i]) {
				for (int j=-delta_avg; j<=delta_avg; j++) {
					c=c+j*mul[i+j];
				}
			}
			devv[i]=c/4;

			if (c>0) pos_run++;
			if (c<0) neg_run++;
			if (prevc==0 && c==0) {
				pos_run=0;
				neg_run=0;
			}
			if (prevc<0 && c>0) {
				int tot=pos_run+neg_run;
				if (tot>dmax) {
					dmax=tot;
					dmaxpos=i-neg_run;
				}
				pos_run=0; neg_run=0;
			}
			prevc=c;

		}


		if (mul[max]>200) {
			printf("Found: start %d at %d max %d found_div %d div_run %d\n", start, max, mul[max], dmaxpos, dmax);

	if (start<100) {
			for (int i=0; i<l; i++) {
				printf("%d %d %d %d %d %d\n", i, samp[i^1], ival[i], qval[i], mul[i], devv[i]);
			}
	}
		}

		i2s_stop(0);
		flush_i2s();
		running=0;

	}
}


