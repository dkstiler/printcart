#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define SAMPS 60000
#define WFMHDRLEN 3725
#define WFMRATE 5000000
//#define USRATE 40550.0
#define USRATE 40000.0
#define INTRATE (USRATE*4) //160KHz
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
	double off_r=((double)off/INTRATE)*WFMRATE;
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
	for (int i=0; i<INTSAMPS; i++) {
		int j=(i&3);
		if (j==0) {
			si[i]=get_samp(samps, i, 2)-get_samp(samps, i+2, 2);
			sq[i]=get_samp(samps, i+1, 2)-get_samp(samps, i+3, 2);
		} else if (j==1) {
			si[i]=get_samp(samps, i+3, 2)-get_samp(samps, i+1, 2);
			sq[i]=get_samp(samps, i, 2)-get_samp(samps, i+2, 2);
		} else if (j==2) {
			si[i]=get_samp(samps, i+2, 2)-get_samp(samps, i, 2);
			sq[i]=get_samp(samps, i+3, 2)-get_samp(samps, i+1, 2);
		} else if (j==3) {
			si[i]=get_samp(samps, i+1, 2)-get_samp(samps, i+3, 2);
			sq[i]=get_samp(samps, i+2, 2)-get_samp(samps, i, 2);
		}
	}
	for (int i=0; i<INTSAMPS; i++) {
		phase[i]=atan2(si[i], sq[i])*(360/2*M_PI);
		amp[i]=sqrt(si[i]*si[i]+sq[i]*sq[i]);
	}
	
	const int8_t rphase[]={1,-1,1,1,-1,-1,-1};
	int mul[INTSAMPS];
	for (int i=0; i<INTSAMPS; i++) {
		int ci, cq;
		ci=0; cq=0;
		int j;
		for (j=0; j<4*4*sizeof(rphase); j++) {
			int ph=j/16;
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
	printf("Start %d highest corr %d diff %d\n", startidx, mulmax, mulmax-startidx);
	int diff=(mulmax-startidx)-50.298;
	printf("--> %f mm\n", ((double)diff*2.1333));

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
