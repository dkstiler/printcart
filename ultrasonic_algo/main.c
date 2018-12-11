#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

//Test code to see if we can find the start of transmission (or any stable timing point, actually) in the received ultrasonic
//waveforms.

//If you really see anything you want to re-use:
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */


#define SAMPS 60000
#define WFMHDRLEN 3725
#define WFMRATE 5000000
//#define USRATE 40550.0
#define USRATE 40000.0
#define INTRATE (USRATE*8) //320KHz
#define INTSAMPS (int)((SAMPS*INTRATE)/WFMRATE)

uint8_t *load_wfm(char *wfmname) {
	FILE *f=fopen(wfmname, "r");
	if (!f) {
		perror(wfmname);
		exit(1);
	}
	uint8_t *ret=calloc(SAMPS, 2);
	fseek(f, WFMHDRLEN, SEEK_SET);
	size_t n=fread(ret, 2, SAMPS, f);
	if (n!=SAMPS) {
		printf("%s: short read: %d!\n", wfmname, n);
		exit(1);
	}
	fclose(f);
	return ret;
}

typedef struct {
	uint8_t chan2;
	uint8_t chan1;
} samp_t;

int get_samp(samp_t *s, int off, int chan) {
	double off_r=((double)off/INTRATE)*(double)WFMRATE;
	int off_p=floor(off_r);
	double frac=off_r-off_p;
	if (off_p+1>=SAMPS) return 0;
	int a, b;
	if (chan==1) {
		a=s[off_p].chan1;
		b=s[off_p+1].chan1;
	} else if (chan==2) {
		a=s[off_p].chan2;
		b=s[off_p+1].chan2;
	} else {
		abort();
	}
	double r=(double)a*(1.0-frac)+(double)b*frac;
	return floor(r+0.5);
}

/*
Note: We expect:
Speed of sound: 340m/s = 340mm/ms = 2.941 us/mm
40KHz -> 25uS/wave -> 1 wave = 8.5mm
*/

int main(int argc, char **argv) {
	samp_t *samps=(samp_t*)(load_wfm(argv[1]));
	int si[INTSAMPS], sq[INTSAMPS], amp[INTSAMPS], phase[INTSAMPS];
	//demodulate
	for (int i=0; i<INTSAMPS; i++) {
		const int qsin[8]={0,10,14,10,0,-10,-14,-10};
		const int qcos[8]={14,10,0,-10,-14,-10,0,10};
		si[i]=get_samp(samps, i, 2)*qsin[i&7];
		sq[i]=get_samp(samps, i, 2)*qcos[i&7];
	}
	//filter
	int histi[8]={0};
	int histq[8]={0};
	int ci=0, cq=0;
	for (int i=0; i<INTSAMPS; i++) {
		ci-=histi[i&7];
		cq-=histq[i&7];
		histi[i&7]=si[i];
		histq[i&7]=sq[i];
		ci+=si[i];
		cq+=sq[i];
		si[i]=ci/14;
		sq[i]=cq/14;
	}

	//calc samp and ampl
	for (int i=0; i<INTSAMPS; i++) {
		phase[i]=atan2(si[i], sq[i])*(360/2*M_PI);
		amp[i]=sqrt(si[i]*si[i]+sq[i]*sq[i]);
	}
	
	//do correlation
	const int8_t rphase[]={1,-1,1,1,-1,-1,-1};
	int mul[INTSAMPS];
	for (int i=0; i<INTSAMPS; i++) {
		int ci, cq;
		ci=0; cq=0;
		int j;
		for (j=0; j<8*4*sizeof(rphase); j++) {
			int ph=j/(8*4);
			assert(ph<7);
			if (i+j<INTSAMPS) {
				if (amp[i+j]>2) {
					ci=ci+(si[i+j]*rphase[ph]*500)/amp[i+j];
					cq=cq+(sq[i+j]*rphase[ph]*500)/amp[i+j];
				}
			}
		}
		ci=ci/j; cq=cq/j;
		mul[i]=sqrt(ci*ci+cq*cq);
	}

	int mulmax=0;
	int startidx=0;
	for (int i=1; i<INTSAMPS; i++) {
		if (mul[i]>mul[mulmax]) mulmax=i;
		if (startidx==0 && i>30 && get_samp(samps, i, 1)-get_samp(samps, i-10, 1)>20) startidx=i;
	}
	printf("%s: Start %d highest corr %d diff %d ", argv[1], startidx, mulmax, mulmax-startidx);
	double diff=(mulmax-startidx)-109.3006;
	printf("--> %f mm\n", (diff/0.9191471181));

	FILE *out=fopen("plot.txt", "w");
	if (!out) {
		perror("plot.txt");
		exit(1);
	}
	for (int i=0; i<INTSAMPS-4; i++) {
		fprintf(out, "%d %d %d %d %d %d %d %d\n", i, get_samp(samps, i, 2), si[i], sq[i], amp[i], phase[i], mul[i], get_samp(samps, i, 1));
	}
	fclose(out);
}

