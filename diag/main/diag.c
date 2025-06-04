#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "FreeRTOS_CLI.h"
#include "driver/uart.h"
#include "hal/efuse_hal.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "sdkconfig.h"

extern void AddScanCommand(void);

#define RX_BUF_SIZE        512
#define MAX_INPUT_LENGTH   40
#define MAX_OUTPUT_LENGTH  1024
#define PROMPT_STRING      "diag> "

enum {
    CHAR_BS = 8,
    CHAR_LF = 10,
    CHAR_CR = 13,
};

static QueueHandle_t uart0_queue;

static portBASE_TYPE xVersion(char* pcOutBuf, size_t xOutBufLen, const char* pcCmdStr)
{
    uint32_t revision = efuse_hal_chip_revision();
    sprintf(pcOutBuf, "Chip version: v%ld.%ld\r\n", revision / 100, revision % 100);
    return pdFALSE;
}

static const CLI_Command_Definition_t xVersionCommand = {
    "version",
    "version: Show chip version information.\r\n",
    xVersion,
    0
};

void task_diagnostic(void *pvParameter)
{
    uart_event_t event;
    uint8_t ch, index = 0;
    char pcInputString[MAX_INPUT_LENGTH], pcOutputString[MAX_OUTPUT_LENGTH];
    BaseType_t more;

    // Initialize the UART for output
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_0, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, RX_BUF_SIZE, 0, CONFIG_UART_QUEUE_SIZE, &uart0_queue, 0));

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize Wi-Fi
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    printf("Hello from the Diagnostic task!\n");

    FreeRTOS_CLIRegisterCommand(&xVersionCommand);

    AddScanCommand();

    while (1) {
        // Wait for UART events
        if (xQueueReceive(uart0_queue, &event, portMAX_DELAY)) {
            switch (event.type) {
                case UART_DATA:
                    uart_read_bytes(UART_NUM_0, &ch, 1, portMAX_DELAY);
                    switch (ch) {
                        case CHAR_LF: // Line Feed
                        case CHAR_CR: // Carriage Return
                            uart_write_bytes(UART_NUM_0, "\r\n", 2);
                            pcInputString[index] = 0;
                            do {
                                more = FreeRTOS_CLIProcessCommand(pcInputString, pcOutputString, sizeof(pcOutputString));
                                uart_write_bytes(UART_NUM_0, pcOutputString, strlen(pcOutputString));
                            } while (more == pdTRUE);
                            index = 0;
                            uart_write_bytes(UART_NUM_0, PROMPT_STRING, strlen(PROMPT_STRING));
                            break;
                        default:
                            if (ch == CHAR_BS && index > 0) { // Handle backspace
                                index--;
                                uart_write_bytes(UART_NUM_0, "\b \b", 3); // Erase character from terminal
                            } else if (ch >= 32 && ch <= 126) { // Printable characters
                                if (index < MAX_INPUT_LENGTH - 1) {
                                    pcInputString[index++] = ch;
                                    uart_write_bytes(UART_NUM_0, &ch, 1);
                                }
                            }
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void app_main(void)
{
    xTaskCreate(task_diagnostic, "The diagnostic task", 8192, NULL, 5, NULL);
}
