#include <esp8266.h>
#include "httpd.h"
#include "httpdespfs.h"
#include "auth.h"
#include "espfs.h"
#include "captdns.h"
#include "webpages-espfs.h"

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
#include "cgi-waveform.h"

#include "soc/timer_group_struct.h"

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}


/*
This is the main url->function dispatching data struct.
In short, it's a struct with various URLs plus their handlers. The handlers can
be 'standard' CGI functions you wrote, or 'special' CGIs requiring an argument.
They can also be auth-functions. An asterisk will match any url starting with
everything before the asterisks; "*" matches everything. The list will be
handled top-down, so make sure to put more specific rules above the more
general ones. Authorization things (like authBasic) act as a 'barrier' and
should be placed above the URLs they protect.
*/
HttpdBuiltInUrl builtInUrls[]={
	{"*", cgiRedirectApClientToHostname, "esp8266.nonet"},
	{"/", cgiRedirect, "/index.html"},
	{"/waveform.cgi", cgiWaveform, NULL},
	{"*", cgiEspFsHook, NULL}, //Catch-all cgi function for the filesystem
	{NULL, NULL, NULL}
};


//Main routine. Initialize stdout, the I/O, filesystem and the webserver and we're done.
int webserver_init(void)
{
    nvs_flash_init();
    system_init();
    tcpip_adapter_init();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    wifi_config_t sta_config = {
        .sta = {
            .ssid = "Sprite",
            .password = "pannenkoek",
            .bssid_set = false
        }
    };
    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
    ESP_ERROR_CHECK( esp_wifi_connect() );

	espFsInit((void*)(webpages_espfs_start));
	httpdInit(builtInUrls, 80);

    return 0;
}

