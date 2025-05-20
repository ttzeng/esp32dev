#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

void app_main(void)
{
    printf("Hello world!\n");

    gpio_reset_pin(CONFIG_GPIO_LED);
    gpio_set_direction(CONFIG_GPIO_LED, GPIO_MODE_OUTPUT);

    int state = 0;
    while (1) {
        gpio_set_level(CONFIG_GPIO_LED, state ^= 1);
        printf("LED %s\n", state? "Off" : "On");

        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
    }
}
