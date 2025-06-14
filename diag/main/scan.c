#include <string.h>
#include "freertos/FreeRTOS.h"
#include "FreeRTOS_CLI.h"
#include "esp_wifi.h"
#include "sdkconfig.h"

static portBASE_TYPE xScan(char* pcOutBuf, size_t xOutBufLen, const char* pcCmdStr)
{
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    sprintf(pcOutBuf, "Scanning for Wi-Fi networks...\r\n");

    // Scan all available APs
    ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, true));

    // Get the number of scanned APs
    uint16_t ap_count;
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
    sprintf(pcOutBuf + strlen(pcOutBuf), "Found %d Wi-Fi networks:\r\n", ap_count);

    // Iterate through the scanned APs
    for (uint16_t i = 0; i < ap_count; i++) {
        wifi_ap_record_t ap_record;
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_record(&ap_record));
        // Print the AP details
        sprintf(pcOutBuf + strlen(pcOutBuf), "SSID: %s, RSSI: %d dBm\r\n", ap_record.ssid, ap_record.rssi);
    }

    ESP_ERROR_CHECK(esp_wifi_stop());

    return pdFALSE;
}

static const CLI_Command_Definition_t xScanCommand = {
    "scan",
    "scan:\r\n\tScan for available Wi-Fi networks.\r\n",
    xScan,
    0
};

void AddScanCommand(void)
{
    FreeRTOS_CLIRegisterCommand(&xScanCommand);
}
