#include <stdio.h>
#include <stdint.h>
#include "rom/ets_sys.h"
#include "esp_attr.h"
#include <string.h>

//Clear a line of pixels
void printcart_line_clear(uint8_t *l) {
	memset(l, 0xff, 14);
}

//In a set of bits representing the bits being shifted out to the cartridge, this function sets
//the p'th pixel from the top of the inkjet spouts. Needs the color as well as the order
//of spouts in the data is slightly different for c, m and y.
void IRAM_ATTR printcart_line_set_pixel(uint8_t *l, int p, int col) {
	//Byte order for the three colors. Note that these arrays are
	//just shifted versions of eachother.
	int bo[3][14]={
		{8,13,4,9,0,5,10,1,6,11,2,7,12,3},
		{11,2,7,12,3,8,13,4,9,0,5,10,1,6},
		{0,5,10,1,6,11,2,7,12,3,8,13,4,9}
	};
	if (p<14) return;
	if (p>(7*14)) return;
	int byteno=bo[col][p%14];
	int bitno=p/14;
	l[byteno]&=~(1<<bitno);
}


//This takes a pointer to a buffer of words to send out to the cartridge after eachother.
//It then uses a template (tp) with length l to generate the base signals to control the 
//inkjet. It then uses the bitstreams in c, m and y to add in the image data into the
//buffer. It finally returns the length written.
//
//Note that the 16-bit words in w will be written with every pair of words swapped;
//the I2S peripheral sends them out that way (so the order is 1032547698 etc)
int IRAM_ATTR printcart_add_waveform(uint16_t *w, const uint16_t *tp, const uint8_t *c, const uint8_t *m, const uint8_t *y, int l) {
	int p=0;

	int i;
	for (i=0; i<14; i++) {
		if (c[i]!=0xff) break;
		if (m[i]!=0xff) break;
		if (y[i]!=0xff) break;
	}
	int is_empty=0;
	if (i==14) is_empty=1;
	//ets_printf("%d\n", is_empty);

	for (int j=0; j<14; j++) {
		int bit=-1;
		int ov=0;
		int power=(c[j]&m[j]&y[j]);
		for (int i=0; i<l; i++) {
			uint16_t v=tp[i];
			//Increase bitno if change on 1st 2 lines
			if ((v&(1<<0))!=ov) {
				bit++;
				ov=v&(1<<0);
			}
			
			//Set bit values for [cmy] streams
			int en=v&((1<<0)|(1<<1)); //only send out bit if bits 0 or 1 are 1
			v&=~((1<<0)|(1<<1)|(1<<2));
			if (en && bit<8 && bit>=0) {
				if (c[j]&(1<<bit)) v|=(1<<0);
				if (m[j]&(1<<bit)) v|=(1<<1);
				if (y[j]&(1<<bit)) v|=(1<<2);
			}

			//Only enable power if needed
			if ((power&0x0f)==0x0f) v&=~(1<<10);
			if ((power&0xf0)==0xF0) v&=~(1<<11);

			//Select correct value for seg ena
			v&=~(1<<3);
			if (j==13) { //last one
				if (v&(1<<14)) v|=(1<<3);
			} else {
				if (v&(1<<15)) v|=(1<<3);
			}
			if (is_empty) v=0;
			w[p^1]=v;
			p++;
		}
		for (int i=0; i<l; i++) {
			w[p^1]=0;
			p++;
		}
	}
	//Always return an even amount of words
	return (p+1)&~1;
}
