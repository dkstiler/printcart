#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>

//Header in the wfm file. No idea what's in there and we don't care, just skip iut.
#define DATA_OFF 0xC90


/* Read in (actually, mmap) the wfm file. Returns pointer to the buffer, fills len with length of the thing. */
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

//Grab a bit from one of the lines in the wfm file:
//Bit indicates channel:
//0=ch1, 1=ch2, 2 tm 18=la1 tm la15
//pos indicates position in waveform
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

//This gets the average time of a clock period. It only counts clock cycles smaller than `max`.
//Wfm is the waveform, len is its length, bit is the channel to check.
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
			//We get here if the line was low and is now high.
			if (s<max) {
				//Add this clock pulse to the average.
				ct++;
				if (ct>0) avg+=s;
			}
			s=0;
			trigger=0;
		}
	}
	return avg/ct;
}

//Returns 1 if the indicated bit went from 0 to 1 at pos.
int rising_edge(uint8_t *wfm, int pos, int bit) {
	if (get_bit(wfm, pos-1, bit)!=0) return 0;
	if (get_bit(wfm, pos, bit)!=1) return 0;
	return 1;
}

//Set bit i in array bits to value b
void set_bit(uint8_t *bits, int i, int b) {
	int m=(1<<(i&7));
	if (b) bits[i/8]|=m; else bits[i/8]&=~m;
}

//bits is the array of bits as 
void print_bits(uint8_t *bits, int len, int alt_order, uint8_t *power) {
	//Bit order for each color. Each color has the sequence of bits slightly
	//different, hence the three arrays (the data in each is just a shifted version
	//of the others.)

	int bo_a[3][14]={ //M, C, Y
		{11,2,7,12,3,8,13,4,9,0,5,10,1,6},
		{8,13,4,9,0,5,10,1,6,11,2,7,12,3},
		{0,5,10,1,6,11,2,7,12,3,8,13,4,9}
	};
	int bo_n[3][14]={
		{7,12,3,8,13,4,9,0,5,10,1,6,11,2},
		{10,1,6,11,2,7,12,3,8,13,4,9,0,5},
		{4,9,0,5,10,1,6,11,2,7,12,3,8,13}
	};

	int *bo[3];
	for (int x=0; x<3; x++) bo[x]=alt_order?bo_a[x]:bo_n[x];

	int lut[8]={
		97, //white bgnd
		95, //M
		96, //  C
		94, //M+C
		93,  //    Y
		91,//M + Y
		92, //  C+Y
		37, //M+C+Y
	};

	//Decode the bits in the order needed
	int j;
	for (int bit=0; bit<8; bit++) {
		if (alt_order) {
			j=(1<<bit);
		} else {
			j=(1<<(7-bit));
		}
		for (int i=0; i<14; i++) {
			int col=0;
			for (int c=0; c<3; c++) {
				if (bits[c*14+bo[c][i]] & j) col|=(1<<c);
			}
			printf("\033[%dm", lut[7-col]);
			printf(col==7?"..":"██");
		}
		printf(" ");
	}
	printf("\033[37m%s", alt_order?"A":"N");

#if 0
	printf("\n");
	for (int j=0; j<8; j++) {
		for (int i=0; i<14; i++) {
			int idx=bo[2][i];
			if (j<4) {
				printf("%s", (power[idx]&2)?"PP":"  ");
			} else {
				printf("%s", (power[idx]&1)?"PP":"  ");
			}
		}
		printf(" ");
	}
#endif
	printf("\n");

}

typedef struct {
	int c;
	int bit;
	int order;
} bw_nozinfo_t;

bw_nozinfo_t ni[]={
	{2,0,1},
	{2,1,1},
	{0,0,1},
	{0,1,1},
	{1,0,1},
	{1,1,1},
	{2,4,1},
	{2,5,1},
	{0,4,1},
	{0,5,1},
	{1,4,1},
	{1,5,1},
	{2,2,0},
	{2,3,0},
	{0,2,0},
	{0,3,0},
	{1,2,0},
	{1,3,0},
	{2,6,0},
	{2,7,0},
	{0,6,0},
	{0,7,0},
	{1,6,0},
	{1,7,0},
	{-1,-1,-1}
};

void print_bits_bw(uint8_t *bits, int alt_order, uint8_t *power) {
	if (alt_order) alt_order=1;
	int bo[2][14]={
		{4,12,10,2,8,0,6,13,7,1,9,3,11,5},
		{13,7,1,9,3,11,5,4,12,10,2,8,0,6},
	};

	//Decode the bits in the order needed

	for (int j=0; ni[j].c!=-1; j++) {
		int bmsk=(1<<ni[j].bit);
		for (int i=0; i<14; i++) {
			printf((bits[ni[j].c*14+bo[ni[j].order][i^alt_order]] & bmsk)?"%02d":"██", i);
		}
	}

//	printf("\033[37m%s", alt_order?"A":"N");
	printf("\n");
}



//LA channels for various signals
#define BIT_M 2
#define BIT_C 3
#define BIT_Y 4
#define BIT_CLK 10

int main(int argc, char **argv) {
	if (argc<2) {
		printf("Usage: %s file.wmv [colidx|'b'] > out.txt\n", argv[0]);
		exit(0);
	}
	int len=0;
	uint8_t *data=get_wv_mem(argv[1], &len);
	uint8_t *wfm=data+DATA_OFF;
	len=(len-DATA_OFF)/4;

	//We need some type of time reference because not all the signals are aligned to the clock edge,
	//some are a fraction earlier or later.
	//The clock is idle for long times, so just taking the average won't do. We get around this by 
	//first getting the average clock (which is way too long because of the idle periods), then
	//re-calculating the average but discarding all periods longer than the previously calculated
	//average. As the idle periods are longer than this value and the active periods shorter, we should
	//end up with the amount of clock cycles of the actual active clock.
	int clk_avg=get_clk_avg(wfm, len, BIT_CLK, 999999);
	int clk=get_clk_avg(wfm, len, BIT_CLK, clk_avg);
	int dofs=(clk/4); //data offset

	//Select which channel to decode: magenta, cyan or yellow.
	int mybits[]={BIT_M, BIT_C, BIT_Y};
	int colmask=7;
	if (argc>=3) {
		colmask=atoi(argv[2]);
		if (argv[2][0]=='b') {
			colmask=0xff;
		}
	}


	//Decode
	int ov=0;
	int l=0;
	int bit=0;
	int last_byte=0;
	int alt_order=0;
	//This array will contain the bits that are clocked out on the selected line. No re-ordering is done here yet; the bits are stored here sequentially.
	uint8_t bits[14*3];
	uint8_t power[14]={0};
	for (int i=1; i<len; i++) {
		if (rising_edge(wfm, i, BIT_CLK)) { //clock goes up
			l=0;
			for (int c=0; c<3; c++) {
				if (colmask&(1<<c)) {
					//Data at rising clock edge
					set_bit(&bits[c*14], bit, get_bit(wfm, i+dofs, mybits[c]));
					//Data at falling clock edge
					set_bit(&bits[c*14], bit+1, get_bit(wfm, i+dofs+(clk/2), mybits[c]));
				} else {
					set_bit(&bits[c*14], bit, 1);
					set_bit(&bits[c*14], bit+1, 1);
				}
			}
			bit+=2;
			if ((bit&7)==0 && last_byte) {
				if (bit==14*8) {
					if (colmask==0xff) {
						print_bits_bw(bits, alt_order>2, power);
					} else {
						print_bits(bits, bit, alt_order>2, power);
					}
				}
				bit=0;
				last_byte=0;
				alt_order=0;
				memset(power, 0, 14);
			}
		}
		if (rising_edge(wfm, i, 8)) {
			if (get_bit(wfm, i+(clk/4), 5)) alt_order++;
		}
		if (rising_edge(wfm, i, 9)) {
			if (get_bit(wfm, i+(clk/32), 5)) { //was clk/4
				last_byte=1;
			}
		}
		if (rising_edge(wfm, i+(clk*3), 0)) power[bit/8]|=1;
		if (rising_edge(wfm, i+(clk*3), 1)) power[bit/8]|=2;
		if (rising_edge(wfm, i+(clk*3), 16)) power[bit/8]|=1;
		if (rising_edge(wfm, i+(clk*3), 17)) power[bit/8]|=2;

		if (l>clk_avg*4) bit&=~7;
		l++;
	}
}

