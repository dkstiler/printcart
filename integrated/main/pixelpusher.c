#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "printcart_genwaveform.h"
#include "printcart_i2s.h"

#define NYAN_REP_START 138
#define NYAN_REP_END 206
extern const uint8_t nyanrgb_start[]   asm("_binary_nyan_84_rgb_start");
extern const uint8_t nyanrgb_end[]     asm("_binary_nyan_84_rgb_end");

#define SCALE 4
#define CMY_OFF 32

static volatile int enabled;

static void pixel_pusher_task(void *arg) {
	printf("Ready.\n");
	int ypos[3]={0};
	uint8_t pixels[14*3];
	int t=0;
	while(1) {
		t++;
		printcart_line_clear(&pixels[0]);
		printcart_line_clear(&pixels[14]);
		printcart_line_clear(&pixels[28]);
		if (!enabled) {
			//if (ypos[0]) printf("Draw STOP. Used %d words of dma mem\n", printcart_mem_words_used);
			//LTR: Cyan is leading, magenta in the middle, yellow trailing.
			//Order of colors are (1-RGB): Cyan, magenta, yellow.
			ypos[0]=-1;
			ypos[1]=-CMY_OFF-1;
			ypos[2]=-CMY_OFF*2-1;
		} else {
			//Infinitely repeat tail
			const uint8_t *p[3];
			for (int c=0; c<3; c++) {
				ypos[c]++;
				if (ypos[c]==NYAN_REP_END*SCALE) ypos[c]=NYAN_REP_START*SCALE;
				p[c]=&nyanrgb_start[(ypos[c]/SCALE)*84*3+c];
			}
			for (int x=14; x<14+84; x++) {
				for (int c=0; c<3; c++) {
					if ((255-*p[c])>(rand()&0xff)) {
						if (ypos[c]>=0) printcart_line_set_pixel(&pixels[c*14], x, c);
					}
					p[c]+=3;
				}
			}
		}
		i2s_push_pixels(&I2S1, pixels);
	}
}

void pixel_pusher_ena(int ena) {
	enabled=ena;
}

void pixel_pusher_init() {
	xTaskCreatePinnedToCore(pixel_pusher_task, "pixpush", 1024*16, NULL, 8, NULL, 1);
}
