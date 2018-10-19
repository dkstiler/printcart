#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "soc/i2s_struct.h"
#include "soc/i2s_reg.h"
#include "driver/periph_ctrl.h"
#include "soc/io_mux_reg.h"
#include "rom/lldesc.h"
#include "esp_heap_caps.h"
#include "printcart_i2s.h"
#include "esp_attr.h"

#include "printcart_genwaveform.h"

typedef struct {
    volatile lldesc_t *dmadesc_a, *dmadesc_b;
    int desccount_a, desccount_b;
	QueueHandle_t pixq;
} i2s_parallel_state_t;

static i2s_parallel_state_t *i2s_state[2]={NULL, NULL};

static void i2s_int_hdl(void *arg);

#define DMA_MAX (4096-4)

//Calculate the amount of dma descs needed for a buffer desc
static int calc_needed_dma_descs_for(i2s_parallel_buffer_desc_t *desc) {
    int ret=0;
    for (int i=0; desc[i].memory!=NULL; i++) {
        ret+=(desc[i].size+DMA_MAX-1)/DMA_MAX;
    }
    return ret;
}

static int fill_dma_desc(volatile lldesc_t *dmadesc, i2s_parallel_buffer_desc_t *bufdesc) {
    int n=0;
    for (int i=0; bufdesc[i].memory!=NULL; i++) {
        int len=bufdesc[i].size;
        uint8_t *data=(uint8_t*)bufdesc[i].memory;
        while(len) {
            int dmalen=len;
            if (dmalen>DMA_MAX) dmalen=DMA_MAX;
            dmadesc[n].size=dmalen;
            dmadesc[n].length=dmalen;
            dmadesc[n].buf=data;
            dmadesc[n].eof=0;
            dmadesc[n].sosf=0;
            dmadesc[n].owner=1;
            dmadesc[n].qe.stqe_next=(lldesc_t*)&dmadesc[n+1];
            dmadesc[n].offset=0;
            len-=dmalen;
            data+=dmalen;
            n++;
        }
    }
    printf("fill_dma_desc: filled %d descriptors\n", n);
    return n;
}

static void gpio_setup_out(int gpio, int sig) {
    if (gpio==-1) return;
    PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[gpio], PIN_FUNC_GPIO);
    gpio_set_direction(gpio, GPIO_MODE_DEF_OUTPUT);
    gpio_matrix_out(gpio, sig, false, false);
}


static void dma_reset(volatile i2s_dev_t *dev) {
    dev->lc_conf.in_rst=1; dev->lc_conf.in_rst=0;
    dev->lc_conf.out_rst=1; dev->lc_conf.out_rst=0;
}

static void fifo_reset(volatile i2s_dev_t *dev) {
    dev->conf.rx_fifo_reset=1; dev->conf.rx_fifo_reset=0;
    dev->conf.tx_fifo_reset=1; dev->conf.tx_fifo_reset=0;
}

static int i2snum(volatile i2s_dev_t *dev) {
    return (dev==&I2S0)?0:1;
}

void i2s_parallel_setup(volatile i2s_dev_t *dev, const i2s_parallel_config_t *cfg) {
    //Figure out which signal numbers to use for routing
    printf("Setting up parallel I2S bus at I2S%d\n", i2snum(dev));
    int sig_data_base, sig_clk;

    //Power on dev
    if (dev==&I2S0) {
        periph_module_enable(PERIPH_I2S0_MODULE);
    } else {
        periph_module_enable(PERIPH_I2S1_MODULE);
    }

    if (dev==&I2S0) {
        sig_data_base=I2S0O_DATA_OUT0_IDX;
        sig_clk=I2S0O_WS_OUT_IDX;
    } else {
        if (cfg->bits==I2S_PARALLEL_BITS_32) {
            sig_data_base=I2S1O_DATA_OUT0_IDX;
        } else {
            //Because of... reasons... the 16-bit values for i2s1 appear on d8...d23
            sig_data_base=I2S1O_DATA_OUT8_IDX;
        }
        sig_clk=I2S1O_WS_OUT_IDX;
    }
    
    //Route the signals
    for (int x=0; x<cfg->bits; x++) {
        gpio_setup_out(cfg->gpio_bus[x], sig_data_base+x);
    }
    //ToDo: Clk/WS may need inversion?
    gpio_setup_out(cfg->gpio_clk, sig_clk);
    
    //Initialize I2S dev
    fifo_reset(dev);
    dma_reset(dev);
    dev->conf.rx_reset=1; dev->conf.tx_reset=1;
    dev->conf.rx_reset=0; dev->conf.tx_reset=0;
    
    //Enable LCD mode
    dev->conf2.val=0;
    dev->conf2.lcd_en=1;
    
    dev->sample_rate_conf.val=0;
    dev->sample_rate_conf.rx_bits_mod=cfg->bits; //don't really matter
    dev->sample_rate_conf.tx_bits_mod=cfg->bits;
    dev->sample_rate_conf.rx_bck_div_num=4; //ToDo: Unsure about what this does...
    dev->sample_rate_conf.tx_bck_div_num=4;
    
    dev->clkm_conf.val=0;
    dev->clkm_conf.clka_en=0;
    dev->clkm_conf.clkm_div_a=0;
    dev->clkm_conf.clkm_div_b=0;
    //We ignore the possibility for fractional division here.
    dev->clkm_conf.clkm_div_num=1;//80000000L/cfg->clkspeed_hz;
    
    dev->fifo_conf.val=0;
    dev->fifo_conf.rx_fifo_mod_force_en=1;
    dev->fifo_conf.tx_fifo_mod_force_en=1;
    dev->fifo_conf.tx_fifo_mod=1;
    dev->fifo_conf.tx_fifo_mod=1;
    dev->fifo_conf.rx_data_num=32; //Thresholds. 
    dev->fifo_conf.tx_data_num=32;
    dev->fifo_conf.dscr_en=1;
    
    dev->conf1.val=0;
    dev->conf1.tx_stop_en=1;
    dev->conf1.tx_pcm_bypass=1;
    
    dev->conf_chan.val=0;
    dev->conf_chan.tx_chan_mod=1;
    dev->conf_chan.rx_chan_mod=1;
    
    //Invert ws to be active-low... ToDo: make this configurable
    dev->conf.tx_right_first=1;
    dev->conf.rx_right_first=1;
    
    dev->timing.val=0;
    
    //Allocate DMA descriptors
    i2s_state[i2snum(dev)]=malloc(sizeof(i2s_parallel_state_t));
    i2s_parallel_state_t *st=i2s_state[i2snum(dev)];
    st->desccount_a=calc_needed_dma_descs_for(cfg->bufa);
    st->desccount_b=calc_needed_dma_descs_for(cfg->bufb);
    st->dmadesc_a=heap_caps_malloc(st->desccount_a*sizeof(lldesc_t), MALLOC_CAP_DMA);
    st->dmadesc_b=heap_caps_malloc(st->desccount_b*sizeof(lldesc_t), MALLOC_CAP_DMA);
    
    //and fill them
    int cta=fill_dma_desc(st->dmadesc_a, cfg->bufa);
    int ctb=fill_dma_desc(st->dmadesc_b, cfg->bufb);
	st->dmadesc_a[cta-1].eof=1;
	st->dmadesc_b[ctb-1].eof=1;
	st->dmadesc_a[cta-1].qe.stqe_next=(lldesc_t*)&st->dmadesc_b[0];
	st->dmadesc_b[ctb-1].qe.stqe_next=(lldesc_t*)&st->dmadesc_a[0];

	//Set int hdl
	esp_intr_alloc(ETS_I2S0_INTR_SOURCE + i2snum(dev), 0, i2s_int_hdl, (void*)dev, NULL);

	st->pixq=xQueueCreate(32, 14*3);

	//Note: call i2s_parallel_start to start transmission.
}

void i2s_push_pixels(volatile i2s_dev_t *dev, uint8_t *pixbuf) {
	xQueueSend(i2s_state[i2snum(dev)]->pixq, pixbuf, portMAX_DELAY);
}

static uint16_t waveform_tpl[]={
//	0x100,0x105,0x105,0x45,0x46,0x46,0x146,0x105,0x4105,0x4015,0x4016,0x4016,0x4116,0x505,0x4505,0x4205,0x4206,0x4206,0x4306,0x105,0x4105,0x4005,0x4026,0x4026,0x4026,0x820,0x4800,0x4080,0x4080,0x4080,0x80,0x4
//	0x100,0x105,0xc005,0xc045,0xc046,0xc046,0xc146,0x105,0x4005,0x4015,0x4016,0x4016,0x4116,0x505,0x4405,0x4205,0x4206,0x4206,0x4306,0x105,0x4005,0x4005,0x4026,0x4026,0x4026,0x820,0x4800,0x4080,0x4080,0x4080,0x80,0x4
//	0x100,0x105,0xc005,0xc045,0xc046,0xc046,0xc146,0x105,0x4005,0x4015,0x4016,0x4016,0x4116,0x105,0x4405,0x4605,0x4206,0x4206,0x4306,0x105,0x4005,0x4005,0x4026,0x4026,0x4026,0x820,0x4800,0x4080,0x4080,0x4080,0x80,0x4
//	0x0,0x105,0xc105,0xc145,0xc046,0xc046,0x46,0x105,0x4105,0x4115,0x4016,0x4016,0x16,0x505,0x4505,0x4305,0x4206,0x4206,0x206,0x105,0x4105,0x4105,0x4026,0x4026,0x26,0x820,0x4800,0x4080,0x4080,0x4080,0x80,0x4
//	0x0,0x105,0xc105,0xc045,0xc046,0xc046,0x46,0x105,0x4115,0x4015,0x4016,0x4016,0x6,0x505,0x4705,0x4205,0x4206,0x4206,0x6,0x105,0x4105,0x4025,0x4026,0x4026,0x26,0x800,0x4880,0x4080,0x4080,0x4080,0x0,0x4
	//0x0,0x105,0x105,0x45,0x46,0x46,0x46,0x105,0x4115,0x4015,0x4016,0x4016,0x6,0x505,0x4705,0x4205,0x4206,0x4206,0x6,0x105,0x4105,0x4025,0x4026,0x4026,0x26,0x800,0x4880,0x4080,0x4080,0x4080,0x0,0x4
//	0x0,0xc105,0xc145,0xc045,0x46,0x6,0x4006,0x4115,0x4115,0x15,0x6,0x6,0x4006,0x4705,0x4705,0x205,0x6,0x6,0x4006,0x4125,0x4125,0x25,0x6,0x6,0x4006,0x4880,0x4880,0x80,0x0,0x0,0x0,0x4
//	0x0,0xc105,0xc145,0xc145,0x46,0x6,0x4006,0x4115,0x4115,0x115,0x6,0x6,0x4006,0x4705,0x4705,0x305,0x6,0x6,0x4006,0x4125,0x4125,0x125,0x6,0x6,0x4006,0x4880,0x4880,0x80,0x0,0x0,0x0,0x4
	0x0,0xc145,0xc145,0xc145,0xc046,0x6,0x6,0x4115,0x4115,0x4115,0x4016,0x6,0x6,0x4305,0x4305,0x4705,0x4606,0x6,0x6,0x4125,0x4125,0x4125,0x4026,0x6,0x6,0x4080,0x4080,0x4880,0x4880,0x0,0x0,0x4
};

int printcart_mem_words_used=0;

		uint8_t colors[14*3];

#define NO_ROWS 1

static void IRAM_ATTR i2s_int_hdl(void *arg) {
    portBASE_TYPE high_priority_task_awoken = 0;
	volatile i2s_dev_t* dev = arg;
	int devno=i2snum(dev);
	if (dev->int_st.out_eof) {
		volatile lldesc_t *finish_desc = (lldesc_t*) dev->out_eof_des_addr;
		int p=0;
		for (int i=0; i<NO_ROWS; i++) {
			memset(colors, 0xff, 14*3);
			xQueueReceiveFromISR(i2s_state[devno]->pixq, colors, &high_priority_task_awoken);
			p+=printcart_add_waveform((uint16_t*)&finish_desc->buf[p], waveform_tpl, &colors[0], &colors[14], &colors[28], sizeof(waveform_tpl)/2);
		}
		printcart_mem_words_used=p;
	}
    dev->int_clr.val = dev->int_st.val;
    if (high_priority_task_awoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

void i2s_parallel_start(volatile i2s_dev_t *dev) {
   //Reset FIFO/DMA -> needed? Doesn't dma_reset/fifo_reset do this?
    dev->out_link.stop=1;
    dev->out_link.start=0;
    dev->conf.tx_start=0;
	vTaskDelay(2);
	dma_reset(dev); fifo_reset(dev);
    dev->lc_conf.in_rst=1; dev->lc_conf.out_rst=1; dev->lc_conf.ahbm_rst=1; dev->lc_conf.ahbm_fifo_rst=1;
    dev->lc_conf.in_rst=0; dev->lc_conf.out_rst=0; dev->lc_conf.ahbm_rst=0; dev->lc_conf.ahbm_fifo_rst=0;
    dev->conf.tx_reset=1; dev->conf.tx_fifo_reset=1; dev->conf.rx_fifo_reset=1;
    dev->conf.tx_reset=0; dev->conf.tx_fifo_reset=0; dev->conf.rx_fifo_reset=0;

    fifo_reset(dev);
    dma_reset(dev);
    dev->conf.rx_reset=1; dev->conf.tx_reset=1;
    dev->conf.rx_reset=0; dev->conf.tx_reset=0;

	vTaskDelay(5);
    dev->lc_conf.val=I2S_OUT_DATA_BURST_EN | I2S_OUTDSCR_BURST_EN | I2S_OUT_DATA_BURST_EN;
    dev->out_link.addr=((uint32_t)(&i2s_state[i2snum(dev)]->dmadesc_a[0]));
	vTaskDelay(5);
    dev->out_link.start=1;
	vTaskDelay(5);
    dev->conf.tx_start=1;
    dev->int_ena.out_eof = 1;
}

