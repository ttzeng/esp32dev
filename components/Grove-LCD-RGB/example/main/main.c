#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "grove_lcd_rgb.h"

static const char *TAG = "GROVE_LCD_HELLO_WORLD";

void app_main(void)
{
    ESP_LOGI(TAG, "Starting Grove LCD RGB Hello World application");
    
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ret = nvs_flash_erase();
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to erase NVS flash: %s", esp_err_to_name(ret));
            return;
        }
        ret = nvs_flash_init();
    }
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize NVS flash: %s", esp_err_to_name(ret));
        return;
    }
    
    // Initialize Grove LCD RGB module
    ESP_LOGI(TAG, "Initializing Grove LCD RGB module");
    ret = grove_lcd_rgb_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize Grove LCD RGB module: %s", esp_err_to_name(ret));
        return;
    }
    
    // Clear the LCD display
    grove_lcd_rgb_clear();
    
    // Set RGB backlight color to green (0, 255, 0)
    grove_lcd_rgb_set_rgb(0, 255, 0);
    
    // Set cursor to position (0, 0)
    grove_lcd_rgb_set_cursor(0, 0);
    
    // Display "Hello World" message
    const char* message = "Hello World";
    grove_lcd_rgb_print(message);
    
    ESP_LOGI(TAG, "Message '%s' displayed on Grove LCD RGB module", message);
    
    // Optional: Display additional information on second line
    grove_lcd_rgb_set_cursor(0, 1);
    grove_lcd_rgb_print("ESP32-C3 Ready!");
    
    // Main application loop
    while (1) {
        // Blink the backlight every 2 seconds
        vTaskDelay(pdMS_TO_TICKS(2000));
        grove_lcd_rgb_set_rgb(255, 0, 0); // Red
        vTaskDelay(pdMS_TO_TICKS(500));
        grove_lcd_rgb_set_rgb(0, 255, 0); // Green
        vTaskDelay(pdMS_TO_TICKS(500));
        grove_lcd_rgb_set_rgb(0, 0, 255); // Blue
        vTaskDelay(pdMS_TO_TICKS(500));
        grove_lcd_rgb_set_rgb(0, 255, 0); // Back to green
        
        ESP_LOGI(TAG, "Application running... Hello World displayed");
    }
}
