// main.c - Simple I2C Scanner for ESP32-C3 Super Mini
// Uses ESP-IDF v5.1+ new I2C master API
// Results are printed to serial console/log only

#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "driver/i2c_master.h"

static const char *TAG = "I2C_SCANNER";

// I2C Scanner configuration for ESP32-C3 Super Mini
#define I2C_MASTER_SCL_IO           9       /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           8       /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_FREQ_HZ          100000  /*!< I2C master clock frequency */
#define I2C_MASTER_PORT             I2C_NUM_0

// I2C scanner range
#define I2C_SCAN_START_ADDR         0x08    /*!< Start scanning from address 0x08 */
#define I2C_SCAN_END_ADDR           0x77    /*!< End scanning at address 0x77 */
#define I2C_PROBE_TIMEOUT_MS        100     /*!< Timeout for device probe */

// Global I2C bus handle
static i2c_master_bus_handle_t bus_handle = NULL;

/**
 * @brief Initialize I2C bus for scanning
 */
static esp_err_t i2c_scanner_init(void)
{
    esp_err_t ret = ESP_OK;
    
    ESP_LOGI(TAG, "Initializing I2C bus for device scanning");
    ESP_LOGI(TAG, "I2C Configuration: SDA=GPIO%d, SCL=GPIO%d, Freq=%dHz", 
             I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO, I2C_MASTER_FREQ_HZ);
    
    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_MASTER_PORT,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    
    ret = i2c_new_master_bus(&i2c_mst_config, &bus_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create I2C bus handle: %s", esp_err_to_name(ret));
        return ret;
    }
    
    ESP_LOGI(TAG, "I2C bus initialized successfully");
    return ESP_OK;
}

/**
 * @brief Deinitialize I2C bus
 */
static void i2c_scanner_deinit(void)
{
    if (bus_handle != NULL) {
        esp_err_t ret = i2c_del_master_bus(bus_handle);
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "I2C bus deinitialized successfully");
        } else {
            ESP_LOGE(TAG, "Failed to deinitialize I2C bus: %s", esp_err_to_name(ret));
        }
        bus_handle = NULL;
    }
}

/**
 * @brief Get device type description for known I2C addresses
 */
static const char* get_device_description(uint8_t addr)
{
    switch (addr) {
        case 0x3E: return "Grove LCD Display";
        case 0x62: return "Grove LCD RGB Backlight";
        case 0x68: case 0x69: return "RTC DS1307/DS3231 or MPU6050/MPU9250";
        case 0x48: case 0x49: case 0x4A: case 0x4B: return "ADS1115/ADS1015 ADC";
        case 0x76: case 0x77: return "BMP280/BME280/BMP180 Sensor";
        case 0x5A: return "MLX90614 IR Temperature Sensor";
        case 0x1D: case 0x53: return "ADXL345 Accelerometer";
        case 0x3C: case 0x3D: return "SSD1306 OLED Display";
        case 0x40: case 0x41: case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: return "PCA9685 PWM Driver";
        case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27: return "PCF8574 I/O Expander";
        case 0x50: case 0x51: case 0x52: case 0x54: case 0x55: case 0x56: case 0x57: return "EEPROM 24C32/24C64";
        case 0x39: return "TSL2561 Light Sensor";
        case 0x29: return "VL53L0X Distance Sensor";
        case 0x1E: return "HMC5883L Magnetometer";
        case 0x44: return "SHT30/SHT31 Humidity Sensor";
        case 0x78: case 0x7A: return "SSD1306 OLED (alternative address)";
        default: return "Unknown Device";
    }
}

/**
 * @brief Scan for I2C devices and print results
 */
static void scan_and_print_i2c_devices(void)
{
    uint8_t device_count = 0;
    uint8_t found_addresses[128]; // Store all found addresses
    
    ESP_LOGI(TAG, "==========================================");
    ESP_LOGI(TAG, "Starting I2C Device Scan");
    ESP_LOGI(TAG, "Scanning address range: 0x%02X to 0x%02X", I2C_SCAN_START_ADDR, I2C_SCAN_END_ADDR);
    ESP_LOGI(TAG, "==========================================");
    
    // Scan all addresses in range
    for (uint8_t addr = I2C_SCAN_START_ADDR; addr <= I2C_SCAN_END_ADDR; addr++) {
        if (i2c_master_probe(bus_handle, addr, I2C_PROBE_TIMEOUT_MS) == ESP_OK) {
            found_addresses[device_count] = addr;
            device_count++;
            
            const char* description = get_device_description(addr);
            ESP_LOGI(TAG, "✓ Device found at 0x%02X: %s", addr, description);
        }
        
        // Small delay to avoid overwhelming the I2C bus
        vTaskDelay(pdMS_TO_TICKS(5));
    }
    
    ESP_LOGI(TAG, "==========================================");
    ESP_LOGI(TAG, "Scan Complete - Summary:");
    ESP_LOGI(TAG, "Total devices found: %d", device_count);
    
    if (device_count == 0) {
        ESP_LOGW(TAG, "No I2C devices detected!");
        ESP_LOGI(TAG, "Possible issues:");
        ESP_LOGI(TAG, "  - Check wiring (SDA=GPIO%d, SCL=GPIO%d)", I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO);
        ESP_LOGI(TAG, "  - Verify device power supply");
        ESP_LOGI(TAG, "  - Ensure pull-up resistors are present");
        ESP_LOGI(TAG, "  - Check if devices are properly connected");
    } else {
        ESP_LOGI(TAG, "Found devices at addresses:");
        for (uint8_t i = 0; i < device_count; i++) {
            ESP_LOGI(TAG, "  [%d] 0x%02X (%s)", 
                     i + 1, found_addresses[i], get_device_description(found_addresses[i]));
        }
    }
    ESP_LOGI(TAG, "==========================================");
    
    // Print ASCII art representation
    printf("\nI2C Address Map (X = device found, . = no device):\n");
    printf("     0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
    
    for (uint8_t row = 0; row < 8; row++) {
        printf("%02X: ", row * 16);
        for (uint8_t col = 0; col < 16; col++) {
            uint8_t addr = row * 16 + col;
            if (addr < I2C_SCAN_START_ADDR || addr > I2C_SCAN_END_ADDR) {
                printf("   ");
            } else {
                bool found = false;
                for (uint8_t i = 0; i < device_count; i++) {
                    if (found_addresses[i] == addr) {
                        found = true;
                        break;
                    }
                }
                printf(" %c ", found ? 'X' : '.');
            }
        }
        printf("\n");
    }
    printf("\n");
}

void app_main(void)
{
    ESP_LOGI(TAG, "ESP32-C3 Super Mini I2C Scanner Starting...");
    ESP_LOGI(TAG, "Using ESP-IDF New I2C Master API");
    
    // Initialize NVS (required for some ESP-IDF components)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "NVS Flash initialized");
    
    // Set log levels to reduce I2C noise
    ESP_LOGI(TAG, "Configuring log levels to reduce I2C noise during scanning");
    esp_log_level_set("i2c.master", ESP_LOG_ERROR); // Only critical errors
    esp_log_level_set("i2c", ESP_LOG_ERROR);        // Only critical errors
    esp_log_level_set("gpio", ESP_LOG_WARN);        // Reduce GPIO noise
    
    // Initialize I2C scanner
    ret = i2c_scanner_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize I2C scanner. Aborting.");
        return;
    }
    
    ESP_LOGI(TAG, "I2C Scanner initialized successfully");
    ESP_LOGI(TAG, "Starting continuous scanning (every 10 seconds)...");
    ESP_LOGI(TAG, "Press Ctrl+C in monitor to stop");
    
    // Main scanning loop
    uint32_t scan_count = 0;
    while (1) {
        scan_count++;
        ESP_LOGI(TAG, "\n--- Scan #%lu ---", scan_count);
        
        // Perform I2C device scan
        scan_and_print_i2c_devices();
        
        ESP_LOGI(TAG, "Next scan in 10 seconds...\n");
        vTaskDelay(pdMS_TO_TICKS(10000)); // Wait 10 seconds before next scan
    }
    
    // Cleanup (won't be reached in this implementation)
    i2c_scanner_deinit();
}
