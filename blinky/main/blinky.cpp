#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "sdkconfig.h"
#include "led-supermini.hpp"

#define RX_BUF_SIZE 1024

void task_blinky(void *pvParameter)
{
    printf("Hello from the Blinky task!\n");

    Led *led = new Led_Esp32c3_SuperMini((gpio_num_t)CONFIG_GPIO_LED);

    int state = 0;
    while (1) {
        char str[10];
        *led = state ^= 1;
        snprintf(str, sizeof(str), "LED %s\r\n", state ? "Off" : "On");
        printf("%s", str);

        uart_write_bytes(UART_NUM_0, str, strlen(str));

        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
    }
}

extern "C" void app_main(void)
{
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
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, RX_BUF_SIZE, 0, 0, NULL, 0));

    xTaskCreate(task_blinky, "task_blinky", 4096, NULL, 5, NULL);
}
