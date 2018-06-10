#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>

#define DATA_OFF 0xC90

void *get_wv_mem(char *file, int *len) {
	struct stat statbuf;
	int f=open(file, O_RDONLY);
	if (f<=0) {
		perror(file);
		exit(0);
	}
	fstat(f, &statbuf);
	*len=(int)statbuf.st_size;
	void *r=mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, f, 0);
	if (r==MAP_FAILED) {
		perror(file);
		exit(0);
	}
	return r;
}

int get_bit(uint8_t *wfm, int pos, int bit) {
	int r;
	if (bit==0) {
		r=wfm[pos*4]>64;
	} else if (bit==1) {
		r=wfm[pos*4+3]>64;
	} else if (bit<10) {
		r=wfm[pos*4+2]&(1<<(bit-2));
	} else {
		r=wfm[pos*4+1]&(1<<(bit-10));
	}
	return r?1:0;
}

int get_clk_avg(uint8_t *wfm, int len, int bit, int max) {
	int avg=0;
	int ct=-1;
	int s=0;
	int trigger=0;
	for (int i=0; i<len; i++) {
		s++;
		int v=get_bit(wfm, i, bit);
		if (v==0) {
			trigger=1;
		} else if (trigger) {
			if (s<max) {
				ct++;
				if (ct>0) avg+=s;
			}
			s=0;
			trigger=0;
		}
	}
	return avg/ct;
}

int rising_edge(uint8_t *wfm, int pos, int bit) {
	if (get_bit(wfm, pos-1, bit)!=0) return 0;
	if (get_bit(wfm, pos, bit)!=1) return 0;
	return 1;
}

void set_bit(uint8_t *bits, int i, int b) {
	int m=(1<<(i&7));
	if (b) bits[i/8]|=m; else bits[i/8]&=~m;
}

void print_bits(uint8_t *bits, int len) {
	int bo[]={11,2,7,12,3,8,13,4,9,0,5,10,1,6};

	for (int j=1; j!=(1<<8); j<<=1) {
		for (int i=0; i<14; i++) {
			printf((bits[bo[i]]&j)?"..":"██");
		}
		printf(" ");
	}
	printf("\n");
}

#define BIT_M 2
#define BIT_C 3
#define BIT_Y 4
#define BIT_CLK 10

int main(int argc, char **argv) {
	if (argc<2) {
		printf("Usage: %s file.wmv > out.txt\n", argv[0]);
		exit(0);
	}
	int len=0;
	uint8_t *data=get_wv_mem(argv[1], &len);
	uint8_t *wfm=data+DATA_OFF;
	len=(len-DATA_OFF)/4;

	int clk_avg=get_clk_avg(wfm, len, BIT_CLK, 999999);
	int clk=get_clk_avg(wfm, len, BIT_CLK, clk_avg);
	int dofs=(clk/4);

	int mybits[]={BIT_M, BIT_C, BIT_Y};
	int mybit=BIT_M;
	if (argc>2) mybit=mybits[atoi(argv[2])];

	int ov=0;
	int l=0;
	int bit=0;
	int last_byte=0;
	uint8_t bits[32];
	for (int i=1; i<len; i++) {
		if (rising_edge(wfm, i, BIT_CLK)) { //clock goes up
			l=0;
			set_bit(bits, bit++, get_bit(wfm, i+dofs, mybit));
			set_bit(bits, bit++, get_bit(wfm, i+dofs+(clk/2), mybit));
			if ((bit&7)==0 && last_byte) {
//			if (bit==14*8) {
				print_bits(bits, bit);
				bit=0;
				last_byte=0;
			}
		}
		if (rising_edge(wfm, i, 6)) {
			if (get_bit(wfm, i+(clk/4), 5)) last_byte=1;
		}
		if (l>clk_avg*4) bit&=~7;
		l++;
	}
}

