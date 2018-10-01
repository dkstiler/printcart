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
//We use 61/64 for alpha and 3/8 (=24/64) for beta.
int magnitude_est(int x, int y) {
	const int alpha=61, beta=24, div=64;
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

static int64_t us_start_time=0;
static int64_t us_ping_start=0;
static int64_t us_tot_samples=0;

void us_init() {
	 i2s_config_t i2s_config = {
		.mode = I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN,
		.sample_rate =	(40000*8)/16, //40KHz input signal, sample 8 times per input signal cycle
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
	us_start_time=esp_timer_get_time();
	//init ADC pad
	i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_0);
	i2s_adc_enable(0);
}



void us_mark_start() {
	us_ping_start=esp_timer_get_time();
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


#define SNIFF_LEN 256
#define PARSE_LEN 1024


void us_run() {
	uint16_t *samp;
	int16_t *ival, *qval, *mul;
	samp=calloc(sizeof(uint16_t), PARSE_LEN);
	ival=calloc(sizeof(int16_t), PARSE_LEN);
	qval=calloc(sizeof(int16_t), PARSE_LEN);
	mul=calloc(sizeof(int16_t), PARSE_LEN);
	
	while(1) {
		size_t l=0, sl=0;
		//Read some samples, see if there's activity
		while(1) {
			i2s_read(0, (void*)samp, SNIFF_LEN*2, &sl, portMAX_DELAY);
			sl=sl/sizeof(int16_t); //to samples
			//This is a very quick and dirty qam demodulation and averaging, just to see if there's any
			//40KHz signal in the sample we took.
			int ri=0, rq=0;
			for (int i=0; i<sl; i++) {
				if ((i+0)&4) ri+=samp[i]; else ri-=samp[i];
				if ((i+2)&4) rq+=samp[i]; else rq-=samp[i];
			}
			int m=magnitude_est(ri, rq);

			if (m>1000) {
				printf("Tresh %d\n", m);
				break;
			} else {
				//Clean bitbucket, try again.
				us_tot_samples+=sl;
			}
		}
		//Read the rest of the samples
		i2s_read(0, (void*)&samp[sl], (PARSE_LEN-sl)*2, &l, portMAX_DELAY);

		l=(l/sizeof(int16_t))+sl; //bytes -> samples, add sniffed len as well
		//QAM-demodulate the signal 
		int sp=0, cp=2;
		const int sinmul[8]={0, 5, 7, 5, 0, -5, -7, -5};
		for (int i=0; i<l; i++) {
			//I is incoming signal * sin(40KHz), Q is incoming signal * cos(40KHz).
			//Because the sample freq is a nice multiple of the carrier freq, we hardcode the sine/cosine here.
			ival[i]=((int)samp[i]*sinmul[sp&7])/8;
			qval[i]=((int)samp[i]*sinmul[cp&7])/8;
			sp++; cp++;
		}

		//Filter to get the envelope of the i/q values
		mov_avg_filter(8, ival, l);
		mov_avg_filter(8, qval, l);

		//We're only interested in the phase of the signal, not the amplitude, so we normalize the I and Q streams
		//if they have any amplitude at all.
#if 0
		for (int i=0; i<l; i++) {
			int amp=magnitude_est(ival[i], qval[i]);
			if (amp>8) {
				ival[i]=(((int)ival[i])<<8)/amp;
				qval[i]=(((int)qval[i])<<8)/amp;
			}
		}
#endif

		//Do correlation with the known pseudo-random ping M-sequence
		//http://www.commsp.ee.ic.ac.uk/~mrt102/projects/mls/MLS%20Theory.pdf for some related info
		const int8_t rphase[]={1,-1,1,1,-1,-1,-1}; //Note: each phase is held for 4 40KHz cycles
		const int rphase_samps=(8*4); //samples per one rphase entry
		for (int i=0; i<l-sizeof(rphase)*rphase_samps; i++) {
			int ci, cq;
			ci=0; cq=0;
			for (int j=0; j<sizeof(rphase)*rphase_samps; j+=rphase_samps) {
					if (rphase[j/rphase_samps]==1) {
						for (int k=0; k<rphase_samps; k++) {
							ci+=ival[i+j+k];
							cq+=qval[i+j+k];
						}
					} else {
						for (int k=0; k<rphase_samps; k++) {
							ci-=ival[i+j];
							cq-=qval[i+j];
						}
				}
			}
			mul[i]=magnitude_est(ci, cq);
		}
//		for (int i=0; i<l; i++) printf("%d ", mul[i]);

		//Find max value
		int max=0;
		for (int j=1; j<l; j++) {
			if (mul[j]>mul[max]) max=j;
		}

		uint64_t timestamp=((us_tot_samples+max)*1000000ULL)/(320000ULL)+us_start_time;
		if (mul[max]>300) printf("Found: at %d max %d, %d us\n", max, mul[max], (int)(timestamp-us_ping_start));
		us_tot_samples+=l;
		int sps=(((esp_timer_get_time()-us_start_time)*320)/1000);
		printf("Samps %d should be %d diff %d\n", (int)us_tot_samples, sps, sps-(int)us_tot_samples);

	}
}


