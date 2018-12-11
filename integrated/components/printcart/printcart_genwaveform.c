//This contains the routines to convert nozzle data to drive waveforms.

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */

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
//	if (p<14) return;
//	if (p>(7*14)) return;
	if (p>(8*14)) return;
	int byteno=bo[col][p%14];
	int bitno=p/14;
	l[byteno+(14*col)]&=~(1<<bitno);
}

typedef struct {
	int c;
	int bit;
	int order;
} bw_nozinfo_t;

const bw_nozinfo_t ni[]={
	{2,0,1}, {2,1,1}, {1,0,1}, {1,1,1},
	{0,0,1}, {0,1,1}, {2,4,1}, {2,5,1},
	{1,4,1}, {1,5,1}, {0,4,1}, {0,5,1},
	{2,2,0}, {2,3,0}, {1,2,0}, {1,3,0},
	{0,2,0}, {0,3,0}, {2,6,0}, {2,7,0},
	{1,6,0}, {1,7,0}, {0,6,0}, {0,7,0},
};

//In a set of bits representing the bits being shifted out to the cartridge, this function sets
//the enable bit for the p'th nozzle from the top of the inkjet nozzles. The black cartridge has two rows,
//the 2nd one is slightly offset in the X direction and interleaved with the 1st (offset by half
//a nozzle).
//Note that the 2 first and last nozzles of each 168-nozzle row are not connected (giving a total 
//of 324 nozzles in the combined two rows).
void IRAM_ATTR printcart_line_set_pixel_mono(uint8_t *l, int p, int row) {
	if (row) p+=168;
	int j=p/14;
	int k=13-(p%14);

	const int bo[2][14]={
		{4,12,10,2,8,0,6,13,7,1,9,3,11,5},
		{13,7,1,9,3,11,5,4,12,10,2,8,0,6},
	};

	l[ni[j].c*14 + bo[ni[j].order][k]] &=~ (1<<ni[j].bit);
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
	//See if we actually need to output anything
	for (i=0; i<14; i++) {
		if (c[i]!=0xff) break;
		if (m[i]!=0xff) break;
		if (y[i]!=0xff) break;
	}
	int is_empty=0;
	if (i==14) is_empty=1;
	//ets_printf("%d\n", is_empty);

	for (int j=0; j<14; j++) {
		uint16_t bit=0;
		int ov=0;
		int power=(c[j]&m[j]&y[j]);
		//Only enable power if needed
		uint16_t mask=0xffff;
		mask&=~(1<<3); //seg ena is generated, so mask out in template
		mask&=~((1<<0)|(1<<1)|(1<<2)); //same for cmy lines
		//mask out power lines if not needed
//		if ((power&0x0f)==0x0f) mask&=~(1<<10);
//		if ((power&0xf0)==0xF0) mask&=~(1<<11);
		uint16_t optdata_sel=0;
		if (j==13) { //last one
			optdata_sel=(1<<14);
		} else {
			optdata_sel=(1<<15);
		}

		for (int i=0; i<l; i++) {
			if (is_empty) {
				//no need to output anything
				w[p^1]=0;
				p++;
				continue;
			}
			uint16_t v=tp[i];
			//Increase bitno if change on 1st 2 lines
			if ((v&(1<<0))!=ov) {
				if (!bit) {
					bit=1; 
				} else {
					bit<<=1;
				}
				ov=v&(1<<0);
			}
			int en=v&((1<<0)|(1<<1)); //only send out bit if bits 0 or 1 are 1
			
			v&=mask;
			
			//Set bit values for [cmy] streams
			if (en) {
				if (c[j]&bit) v|=(1<<0);
				if (m[j]&bit) v|=(1<<1);
				if (y[j]&bit) v|=(1<<2);
			}
			//Select correct value for seg ena
			if (v&optdata_sel) v|=(1<<3);
			w[p^1]=v;
			p++;
		}
		i+=8; //space after data
	}
	//Always return an even amount of words
	return (p+1)&~1;
}
