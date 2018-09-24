#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define SAMPS 60000
#define WFMHDRLEN 3725
#define WFMRATE 5000000
#define USRATE 40550.0

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

int main(int argc, char **argv) {
	samp_t *samps=(samp_t*)(load_wfm(argv[1]));
	int si[SAMPS], sq[SAMPS], amp[SAMPS], phase[SAMPS];
	for (int i=0; i<SAMPS; i++) {
		double ph=((double)i*M_PI*2.0)/((double)WFMRATE/USRATE);
		si[i]=(double)samps[i].chan2*sin(ph);
		sq[i]=(double)samps[i].chan2*cos(ph);
	}
	for (int i=0; i<SAMPS; i++) {
		int ci=0, cq=0;
		for (int j=0; j<(WFMRATE/USRATE); j++) {
			if (i+j<SAMPS) {
				ci+=si[i+j];
				cq+=sq[i+j];
			}
		}
		si[i]=ci;
		sq[i]=cq;
		phase[i]=atan2(si[i], sq[i])*(360/2*M_PI);
		amp[i]=sqrt(si[i]*si[i]+sq[i]*sq[i]);
	}

	const int rphase[7]={1,-1,1,1,-1,-1,-1};
	int mul[SAMPS];
	for (int i=0; i<SAMPS; i++) {
		int ci, cq;
		ci=0; cq=0;
		int j;
		for (j=0; j<((WFMRATE*4*7)/USRATE); j++) {
			int ph=(j*USRATE)/(WFMRATE*4);
			assert(ph<7);
			if (i+j<SAMPS) {
				ci=ci+si[i+j]*rphase[ph];
				cq=cq+sq[i+j]*rphase[ph];
			}
		}
		ci=ci/j; cq=cq/j;
		mul[i]=sqrt(ci*ci+cq*cq);
	}

	int mulmax=0;
	int startidx=0;
	for (int i=1; i<SAMPS; i++) {
		if (mul[i]>mul[mulmax]) mulmax=i;
		if (startidx==0 && i>30 && samps[i].chan1-samps[i-30].chan1>20) startidx=i;
	}
	printf("Start %d highest corr %d diff %d\n", startidx, mulmax, mulmax-startidx);
	int diff=(mulmax-startidx)-1979;
	printf("--> %f mm\n", (double)diff/13.40);

	FILE *out=fopen("plot.txt", "w");
	if (!out) {
		perror("plot.txt");
		exit(1);
	}
	for (int i=0; i<SAMPS; i++) {
		fprintf(out, "%d %d %d %d %d %d %d %d\n", i, (int)samps[i].chan2, si[i], sq[i], amp[i], phase[i], mul[i], (int)samps[i].chan1);
	}
	fclose(out);
}
