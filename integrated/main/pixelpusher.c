#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "printcart_genwaveform.h"
#include "printcart_i2s.h"
#include "pixelpusher.h"
#include <string.h>

#define SCALE 4
#define NYAN_REP_START (138*SCALE)
#define NYAN_REP_END (206*SCALE)
extern const uint8_t nyanrgb_start[]   asm("_binary_nyan_84_rgb_start");
extern const uint8_t nyanrgb_end[]     asm("_binary_nyan_84_rgb_end");

extern const uint8_t brush_start[]   asm("_binary_brush_raw_start");
extern const uint8_t brush_end[]     asm("_binary_brush_raw_end");

extern const uint8_t mona_start[]   asm("_binary_monalisa_rgb_start");
extern const uint8_t mona_end[]     asm("_binary_monalisa_rgb_end");

extern const uint8_t whe_start[]   asm("_binary_whe_mono_start");
extern const uint8_t whe_end[]     asm("_binary_whe_mono_end");

extern const uint8_t mine_start[]   asm("_binary_mine_raw_start");
extern const uint8_t mine_end[]     asm("_binary_mine_raw_end");

#define CMY_OFF 32

typedef struct{
	int x;
	int y;
	int vx;
	int vy;
	int ena;
	int end_x;
	int end_y;
} pixelpusher_state_t;

static pixelpusher_state_t ppstates[2];
static pixelpusher_state_t * volatile ppstate=&ppstates[0];

static int wraparound(int val, int start, int end) {
	if (val<start) {
		return val;
	} else {
		return ((val-start)%(end-start))+start;
	}
}

typedef struct {
	int r;
	int g;
	int b;
} color_t;

color_t gradient[2];

void pixel_pusher_color_set(int r, int g, int b, int pos) {
	gradient[pos].r=r;
	gradient[pos].g=g;
	gradient[pos].b=b;
}

static void pixel_pusher_color(pixelpusher_state_t *st, uint8_t *pixels) {
	int xpos[3];
	if (st->ena) {
		xpos[0]=st->x >> FP_SHIFT;
		xpos[1]=(xpos[0]-CMY_OFF);
		xpos[2]=(xpos[0]-CMY_OFF*2);
		for (int i=0; i<3; i++) {
			xpos[i]/=SCALE;
			if (xpos[i]>42) xpos[i]=42;
		}
	} else {
		xpos[0]=(st->x - st->end_x) >> FP_SHIFT;
		xpos[1]=(xpos[0]-CMY_OFF);
		xpos[2]=(xpos[0]-CMY_OFF*2);
		for (int i=0; i<3; i++) {
			xpos[i]/=SCALE;
			if (xpos[i]<42) xpos[i]=42;
		}
	}

	int col[3]={gradient[0].r, gradient[1].g, gradient[2].b};

	const uint8_t *p[3]; //position in image data for all 3 nozzles
	for (int c=0; c<3; c++) {
		p[c]=&brush_start[xpos[c]*84];
	}
	for (int x=14; x<14+84; x++) {
		for (int c=0; c<3; c++) {
			if (xpos[c]>=0 && xpos[c]<84) {
				int cm=((255-col[c]) * (255-p[c][x-14]))>>8;
				if ((cm)>(rand()&0x1ff)) { //dither
					printcart_line_set_pixel(pixels, x, c);
				}
			}
		}
	}
}

static void pixel_pusher_nyancat(pixelpusher_state_t *st, uint8_t *pixels) {
	int xpos[3];
	if (!st->ena) return;
	//Offsets for nozzles
	xpos[0]=st->x >> FP_SHIFT;
	//Repeat tail infinitely
	xpos[2]=wraparound(xpos[0]-CMY_OFF*2, NYAN_REP_START, NYAN_REP_END)/SCALE;
	xpos[1]=wraparound(xpos[0]-CMY_OFF, NYAN_REP_START, NYAN_REP_END)/SCALE;
	xpos[0]=wraparound(xpos[0], NYAN_REP_START, NYAN_REP_END)/SCALE;

	const uint8_t *p[3]; //position in image data for all 3 nozzles
	for (int c=0; c<3; c++) {
		p[c]=&nyanrgb_start[(xpos[c])*84*3+c];
	}
	for (int x=14; x<14+84; x++) {
		for (int c=0; c<3; c++) {
			if ((255-*p[c])>(rand()&0xff)) { //dither
				if (xpos[c]>=0) printcart_line_set_pixel(pixels, x, c);
			}
			p[c]+=3;
		}
	}
}

#define MONO_OFF 20
#define WHE_REP_START (370*SCALE)
#define WHE_REP_END (515*SCALE)

static void pixel_pusher_whee(pixelpusher_state_t *st, uint8_t *pixels) {
	int xpos[2];
	if (!st->ena) return;
	//Offsets for nozzles
	xpos[0]=st->x >> FP_SHIFT;
	//Repeat last 'E' infinitely
	xpos[1]=wraparound(xpos[0], WHE_REP_START, WHE_REP_END)/SCALE;
	xpos[0]=wraparound(xpos[0]-MONO_OFF, WHE_REP_START, WHE_REP_END)/SCALE;

	const uint8_t *p[2]; //position in image data for all 3 nozzles
	int mask[2];
	for (int r=0; r<2; r++) {
		p[r]=&whe_start[(xpos[r])/8];
		mask[r]=1<<(xpos[r]&7);
	}
	for (int x=2; x<164; x++) {
		for (int r=0; r<2; r++) {
			if (rand()&1) { //dither
				if (((*p[r])&mask[r])!=0) printcart_line_set_pixel_mono(pixels, (168-x), r);
			}
			p[r]+=65;
		}
	}
}



static void pixel_pusher_mine(pixelpusher_state_t *st, uint8_t *pixels) {
	int xpos[3];
	if (!st->ena) return;
	//Offsets for nozzles
	xpos[0]=st->x >> FP_SHIFT;
	//Repeat tail infinitely
	xpos[2]=wraparound(xpos[0]-CMY_OFF*2, 317*SCALE, 318*SCALE)/SCALE;
	xpos[1]=wraparound(xpos[0]-CMY_OFF, 317*SCALE, 318*SCALE)/SCALE;
	xpos[0]=wraparound(xpos[0], 317*SCALE, 318*SCALE)/SCALE;

	const uint8_t *p[3]; //position in image data for all 3 nozzles
	for (int c=0; c<3; c++) {
		p[c]=&mine_start[xpos[c]*84];
	}
	for (int x=14; x<14+84; x++) {
		for (int c=0; c<3; c++) {
			if ((255-*p[c])>(rand()&0xff)) { //dither
				if (xpos[c]>=0) printcart_line_set_pixel(pixels, x, c);
			}
			p[c]++;
		}
	}
}


static int mona_stripe=0;
void pixel_pusher_mona_set_stripe(int stripe) {
	mona_stripe=5-(stripe%5);
}

static void pixel_pusher_mona(pixelpusher_state_t *st, uint8_t *pixels) {
	int xpos[3];
	if (!st->ena) return;
	//Offsets for nozzles
	xpos[0]=st->x >> FP_SHIFT;
	xpos[2]=(xpos[0]-CMY_OFF*2)/SCALE;
	xpos[1]=(xpos[0]-CMY_OFF)/SCALE;
	xpos[0]=xpos[0]/SCALE;

	const uint8_t *p[3]; //position in image data for all 3 nozzles
	for (int c=0; c<3; c++) {
		p[c]=&mona_start[(xpos[c])*560*3+c + mona_stripe*3*84];
	}
	for (int x=14; x<14+84; x++) {
		for (int c=0; c<3; c++) {
			if ((255-*p[c])>(rand()&0x1ff)) { //dither
				if (xpos[c]>=0 && xpos[c]<864) printcart_line_set_pixel(pixels, x, c);
			}
			p[c]+=3;
		}
	}
}


static void pixel_pusher_mona_imu(pixelpusher_state_t *st, uint8_t *pixels) {
	int xpos[3];
	if (!st->ena) return;
	//Offsets for nozzles
	xpos[0]=(st->x*20) >> FP_SHIFT;
	xpos[2]=(xpos[0]-CMY_OFF*2);
	xpos[1]=(xpos[0]-CMY_OFF);
	int ypos=((st->y*20) >> FP_SHIFT);
//	printf("Mona: %d %d\n", xpos[0], ypos);
	const uint8_t *p[3]; //position in image data for all 3 nozzles
	for (int c=0; c<3; c++) {
		p[c]=&mona_start[(xpos[c])*560*3+c + ypos*3];
	}
	for (int x=14; x<14+84; x++) {
		for (int c=0; c<3; c++) {
			if ((255-*p[c])>(rand()&0xff)) { //dither
				if (xpos[c]>=0 && xpos[c]<864 && ypos>=0 && ypos<560) {
					printcart_line_set_pixel(pixels, x, c);
				}
			}
			p[c]+=3;
		}
		ypos++;
	}
}



volatile int sel_type=0;

void pixel_pusher_set_type(int type) {
	sel_type=type;
}

static void pixel_pusher_task(void *arg) {
	uint8_t pixels[14*3];
	int prev_x=-1;
	int old_ena=0;
	while(1) {
		pixelpusher_state_t * volatile st;
		st=ppstate; //get current state so other task can write into the other one
		//clear pixel data
		memset(pixels, 0xff, 14*3);
//		if (((st->x >> (FP_SHIFT-2))!=(prev_x >> (FP_SHIFT-2)))) {
			if (sel_type==PP_NYANCAT) pixel_pusher_nyancat(st, pixels);
			if (sel_type==PP_COLOR) pixel_pusher_color(st, pixels);
			if (sel_type==PP_MONA) pixel_pusher_mona(st, pixels);
			if (sel_type==PP_MINE) pixel_pusher_mine(st, pixels);
			if (sel_type==PP_MONA_IMU) pixel_pusher_mona_imu(st, pixels);
			if (sel_type==PP_WHEE) pixel_pusher_whee(st, pixels);
//		}
		i2s_push_pixels(&I2S1, pixels);
		prev_x=st->x;
		st->x+=st->vx;
		st->y+=st->vy;
		if (st->ena==0 && old_ena==1) {
			st->end_x=st->x;
			st->end_y=st->y;
		}
		old_ena=st->ena;
	}
}

//Note: These are 24.8 fixed point values.
void pixel_pusher_set_speed_pos(int x, int y, int vx, int vy, int ena) {
	static int last_x, last_y;
	pixelpusher_state_t *ws;
	if (ppstate==&ppstates[0]) ws=&ppstates[1]; else ws=&ppstates[0];
	ws->x=x;
	ws->y=y;
	ws->vx=vx/186;
	ws->vy=vy/186;
	ws->ena=ena;
//	printf("Diff hyp vx % 5d vs real % 5d, % 5d vs % 5d vx %d vy %d ena %d x %d y %d\n", ppstate->x-last_x, x-last_x, ppstate->y-last_y, y-last_y, vx, vy, ws->ena, x>>FP_SHIFT, y>>FP_SHIFT);
//	printf("Diff hyp vx % 5d vs real % 5d, hyp vy % 5d vs real % 5d\n", ppstate->x-last_x, vx, ppstate->y-last_y, vy);
//	printf("x %d y %d", x>>FP_SHIFT, y>>FP_SHIFT);
	ppstate=ws; //swap
	last_x=x;
	last_y=y;
}

void pixel_pusher_init() {
	xTaskCreatePinnedToCore(pixel_pusher_task, "pixpush", 1024*16, NULL, 8, NULL, 1);
}
