#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "FreeRTOS_CLI.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "sdkconfig.h"

esp_ip4_addr_t g_IPaddr, g_Gateway, g_Netmask;

static EventGroupHandle_t evtGroupWiFi = NULL;

#define WIFI_FAILED         (1 << 0)    // Event bit for WiFi failed to connect
#define WIFI_CONNECTING     (1 << 1)    // Event bit for WiFi connection in progress
#define WIFI_CONNECTED      (1 << 2)    // Event bit for WiFi connected

static void wifi_event_handler (void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_STA_START:
                // WiFi station started, initiate connection
                xEventGroupSetBits(evtGroupWiFi, WIFI_CONNECTING);
                break;
            case WIFI_EVENT_STA_DISCONNECTED:
                // WiFi station disconnected, set the failed bit
                xEventGroupSetBits(evtGroupWiFi, WIFI_FAILED);
                break;
            default:
                break;
        }
    } else if (event_base == IP_EVENT) {
        switch (event_id) {
            case IP_EVENT_STA_GOT_IP:
                // WiFi station got IP address, set the connected bit
                xEventGroupSetBits(evtGroupWiFi, WIFI_CONNECTED);
                ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
                g_IPaddr  = event->ip_info.ip;
                g_Gateway = event->ip_info.gw;
                g_Netmask = event->ip_info.netmask;
                break;
            default:
                break;
        }
    }
}

static portBASE_TYPE xConnect (char* pcOutBuf, size_t xOutBufLen, const char* pcCmdStr)
{
    if (evtGroupWiFi == NULL) {
        if ((evtGroupWiFi = xEventGroupCreate()) == NULL) {
            snprintf(pcOutBuf, xOutBufLen, "Failed to create event group for WiFi.\r\n");
            return pdFALSE;
        }

        ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                            ESP_EVENT_ANY_ID,
                                                            &wifi_event_handler,
                                                            NULL,
                                                            NULL));
        ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                            IP_EVENT_STA_GOT_IP,
                                                            &wifi_event_handler,
                                                            NULL,
                                                            NULL));

        char ssid[32], password[32];
        sscanf(pcCmdStr, "connect %s %s", ssid, password);
        wifi_config_t wifi_config = {
            .sta = {
                .threshold.authmode = WIFI_AUTH_WPA2_PSK,
                .pmf_cfg = {
                    .capable = true,
                    .required = false
                },
            }
        };
        strncpy((char*)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid) - 1);
        strncpy((char*)wifi_config.sta.password, password, sizeof(wifi_config.sta.password) - 1);
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
        ESP_ERROR_CHECK(esp_wifi_start());
    } else {
        EventBits_t events = xEventGroupWaitBits(evtGroupWiFi,
            WIFI_CONNECTED | WIFI_FAILED | WIFI_CONNECTING,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

        if (events & WIFI_CONNECTING) {
            xEventGroupClearBits(evtGroupWiFi, WIFI_CONNECTING);
            esp_wifi_connect();
            snprintf(pcOutBuf, xOutBufLen, "Connecting to WiFi ...\r\n");
        } else if (events & WIFI_CONNECTED) {
            snprintf(pcOutBuf, xOutBufLen, "Connected to WiFi network.\r\nIP: [" IPSTR "] Gateway: [" IPSTR "] Netmask: [" IPSTR "]\r\n",
                     IP2STR(&g_IPaddr), IP2STR(&g_Gateway), IP2STR(&g_Netmask));
            return pdFALSE;
        } else if (events & WIFI_FAILED) {
            snprintf(pcOutBuf, xOutBufLen, "Failed to connect to WiFi network.\r\n");
            vEventGroupDelete(evtGroupWiFi);
            evtGroupWiFi = NULL;
            ESP_ERROR_CHECK(esp_wifi_stop());
            return pdFALSE;
        }
    }

    return pdTRUE;
}

static const CLI_Command_Definition_t xConnectCommand = {
    "connect",
    "connect <ssid> <password>:\r\n\tConnects to WiFi network.\r\n",
    xConnect,
    2
};

void AddConnectCommand (void)
{
    FreeRTOS_CLIRegisterCommand(&xConnectCommand);
}
