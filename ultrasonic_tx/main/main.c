/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */


/*
Simple and stupid program that periodically sends out both an ultrasonic ping (in the form of a bi-phase 
encoded M-code) as well as an ESPNow WiFi packet. The idea is that by measuring the time difference on the
receiver, the distance can be inferred.
*/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "driver/rmt.h"
#include "esp_now.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "espnow";

#define CHANNEL 11


static uint8_t broadcast_mac[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

static void rmt_tx_init()
{
    rmt_config_t config;
    config.rmt_mode = RMT_MODE_TX;
    config.channel = 0;
    config.gpio_num = 32;
    config.mem_block_num = 1;
    config.tx_config.loop_en = 0;
    config.tx_config.carrier_en = 0;
    config.tx_config.idle_output_en = 0;
    config.tx_config.idle_level = 0;
    config.tx_config.carrier_duty_percent = 50;
    config.clk_div = 1;

    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));

    gpio_set_direction(33, GPIO_MODE_OUTPUT);
    gpio_matrix_out(33, RMT_SIG_OUT0_IDX, 1, 0);

}

static esp_err_t wifi_event_handler(void *ctx, system_event_t *event) {
	switch(event->event_id) {
	case SYSTEM_EVENT_STA_START:
		ESP_LOGI(TAG, "WiFi started");
		break;
	default:
		break;
	}
	return ESP_OK;
}


esp_timer_handle_t sendtimer;

//define M-sequence
//const int8_t phase[]={0,1,0,0,1,1,1};
const int8_t phase[]={1,1,1,1,1,1,0,0,0,0,0,0};
#define PHASE_CYCCNT 4
#define NO_ITEMS (sizeof(phase)*PHASE_CYCCNT)
rmt_item32_t item[NO_ITEMS]={0};

static void esp_send_timer_cb(void *arg) {
	ESP_ERROR_CHECK(rmt_write_items(0, item, NO_ITEMS, true));
	printf("Tx done.\n");
}

static void send_cb(const uint8_t *mac_addr, esp_now_send_status_t status) {
	esp_timer_start_once(sendtimer, 300);
}

esp_err_t espnow_init() {
	// Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
		ESP_ERROR_CHECK( nvs_flash_erase() );
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK( ret );
	tcpip_adapter_init();
	ESP_ERROR_CHECK( esp_event_loop_init(wifi_event_handler, NULL) );
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
	ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
	ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
	ESP_ERROR_CHECK( esp_wifi_start());
	ESP_ERROR_CHECK( esp_wifi_disconnect() );

	/* Initialize ESPNOW and register sending and receiving callback function. */
	ESP_ERROR_CHECK( esp_now_init() );
	ESP_ERROR_CHECK( esp_now_register_send_cb(send_cb) );
//	ESP_ERROR_CHECK( esp_now_register_recv_cb(recv_cb) );

	/* Add broadcast peer information to peer list. */
	esp_now_peer_info_t *peer = malloc(sizeof(esp_now_peer_info_t));
	if (peer == NULL) {
		ESP_LOGE(TAG, "Malloc peer information fail");
		return ESP_FAIL;
	}
	memset(peer, 0, sizeof(esp_now_peer_info_t));
	peer->channel = CHANNEL;
	peer->ifidx = ESP_IF_WIFI_STA;
	peer->encrypt = false;
	memcpy(peer->peer_addr, broadcast_mac, ESP_NOW_ETH_ALEN);
	ESP_ERROR_CHECK( esp_now_add_peer(peer) );
	free(peer);
	return ESP_OK;
}

int app_main(void)
{
	espnow_init();

	rmt_tx_init();
	for (int i=0; i<NO_ITEMS; i++) {
		item[i].duration0=(80000000/(40000*2));
		item[i].level0=(phase[i/PHASE_CYCCNT])?1:0;
		item[i].duration1=(80000000/(40000*2));
		item[i].level1=item[i].level0?0:1;
	}
	
	const esp_timer_create_args_t arg={
		.callback=esp_send_timer_cb,
		.dispatch_method=ESP_TIMER_TASK,
		.name="sendtimer"
	};
	esp_timer_create(&arg, &sendtimer);
	
	while(1) {
		char *packet="PING!";
		esp_now_send(broadcast_mac, (uint8_t*)packet, strlen(packet));
		
		vTaskDelay(50 / portTICK_PERIOD_MS);
	}
}

