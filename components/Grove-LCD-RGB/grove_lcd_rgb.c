#include "grove_lcd_rgb.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include "sdkconfig.h"

static const char *TAG = "GROVE_LCD_RGB";
static bool is_initialized = false;
static i2c_master_bus_handle_t i2c_bus_handle = NULL;
static i2c_master_dev_handle_t lcd_dev_handle = NULL;
static i2c_master_dev_handle_t rgb_dev_handle = NULL;

/**
 * @brief Write command to LCD using new I2C API
 */
static esp_err_t lcd_write_command(uint8_t cmd)
{
    uint8_t data[2] = {0x80, cmd}; // Co = 1, RS = 0
    return i2c_master_transmit(lcd_dev_handle, data, 2, I2C_MASTER_TIMEOUT_MS);
}

/**
 * @brief Write data to LCD using new I2C API
 */
static esp_err_t lcd_write_data(uint8_t data)
{
    uint8_t buffer[2] = {0x40, data}; // Co = 0, RS = 1
    return i2c_master_transmit(lcd_dev_handle, buffer, 2, I2C_MASTER_TIMEOUT_MS);
}

/**
 * @brief Write to RGB backlight register using new I2C API
 */
static esp_err_t rgb_write_reg(uint8_t reg, uint8_t data)
{
    uint8_t buffer[2] = {reg, data};
    return i2c_master_transmit(rgb_dev_handle, buffer, 2, I2C_MASTER_TIMEOUT_MS);
}

esp_err_t grove_lcd_rgb_init(void)
{
    ESP_LOGI(TAG, "Initializing Grove LCD RGB module with new I2C API");
    
    // Configure I2C master bus
    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_MASTER_NUM,
        .scl_io_num = CONFIG_GPIO_I2C_SCL,
        .sda_io_num = CONFIG_GPIO_I2C_SDA,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    
    esp_err_t ret = i2c_new_master_bus(&i2c_mst_config, &i2c_bus_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C master bus creation failed: %s", esp_err_to_name(ret));
        return ret;
    }
    
    // Configure LCD device
    i2c_device_config_t lcd_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = CONFIG_I2C_ADDR_LCD,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };
    
    ret = i2c_master_bus_add_device(i2c_bus_handle, &lcd_cfg, &lcd_dev_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "LCD device addition failed: %s", esp_err_to_name(ret));
        i2c_del_master_bus(i2c_bus_handle);
        return ret;
    }
    
    // Configure RGB device
    i2c_device_config_t rgb_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = CONFIG_I2C_ADDR_RGB,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };
    
    ret = i2c_master_bus_add_device(i2c_bus_handle, &rgb_cfg, &rgb_dev_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "RGB device addition failed: %s", esp_err_to_name(ret));
        i2c_master_bus_rm_device(lcd_dev_handle);
        i2c_del_master_bus(i2c_bus_handle);
        return ret;
    }
    
    // Wait for LCD to initialize
    vTaskDelay(pdMS_TO_TICKS(50));
    
    // Initialize LCD display
    ret = lcd_write_command(LCD_FUNCTIONSET | LCD_2LINE);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set LCD function: %s", esp_err_to_name(ret));
        return ret;
    }
    vTaskDelay(pdMS_TO_TICKS(5));
    
    ret = lcd_write_command(LCD_FUNCTIONSET | LCD_2LINE);
    if (ret != ESP_OK) return ret;
    vTaskDelay(pdMS_TO_TICKS(5));
    
    ret = lcd_write_command(LCD_FUNCTIONSET | LCD_2LINE);
    if (ret != ESP_OK) return ret;
    vTaskDelay(pdMS_TO_TICKS(5));
    
    // Display control
    ret = lcd_write_command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set display control: %s", esp_err_to_name(ret));
        return ret;
    }
    vTaskDelay(pdMS_TO_TICKS(5));
    
    // Clear display
    ret = lcd_write_command(LCD_CLEARDISPLAY);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to clear display: %s", esp_err_to_name(ret));
        return ret;
    }
    vTaskDelay(pdMS_TO_TICKS(5));
    
    // Entry mode set
    ret = lcd_write_command(LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set entry mode: %s", esp_err_to_name(ret));
        return ret;
    }
    vTaskDelay(pdMS_TO_TICKS(5));
    
    // Initialize RGB backlight
    ret = rgb_write_reg(REG_MODE1, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize RGB mode1: %s", esp_err_to_name(ret));
        return ret;
    }
    
    ret = rgb_write_reg(REG_OUTPUT, 0xFF);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set RGB output: %s", esp_err_to_name(ret));
        return ret;
    }
    
    ret = rgb_write_reg(REG_MODE2, 0x20);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize RGB mode2: %s", esp_err_to_name(ret));
        return ret;
    }
    
    is_initialized = true;
    ESP_LOGI(TAG, "Grove LCD RGB module initialized successfully with new I2C API");
    return ESP_OK;
}

esp_err_t grove_lcd_rgb_deinit(void)
{
    if (!is_initialized) {
        ESP_LOGW(TAG, "Grove LCD RGB module not initialized");
        return ESP_OK;
    }
    
    esp_err_t ret = ESP_OK;
    
    // Remove LCD device
    if (lcd_dev_handle != NULL) {
        ret = i2c_master_bus_rm_device(lcd_dev_handle);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to remove LCD device: %s", esp_err_to_name(ret));
        }
        lcd_dev_handle = NULL;
    }
    
    // Remove RGB device
    if (rgb_dev_handle != NULL) {
        ret = i2c_master_bus_rm_device(rgb_dev_handle);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to remove RGB device: %s", esp_err_to_name(ret));
        }
        rgb_dev_handle = NULL;
    }
    
    // Delete I2C bus
    if (i2c_bus_handle != NULL) {
        ret = i2c_del_master_bus(i2c_bus_handle);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to delete I2C master bus: %s", esp_err_to_name(ret));
        }
        i2c_bus_handle = NULL;
    }
    
    is_initialized = false;
    ESP_LOGI(TAG, "Grove LCD RGB module deinitialized");
    return ret;
}

esp_err_t grove_lcd_rgb_clear(void)
{
    if (!is_initialized) {
        ESP_LOGE(TAG, "Grove LCD RGB not initialized");
        return ESP_ERR_INVALID_STATE;
    }
    
    esp_err_t ret = lcd_write_command(LCD_CLEARDISPLAY);
    if (ret == ESP_OK) {
        vTaskDelay(pdMS_TO_TICKS(2)); // Clear command takes longer
    }
    return ret;
}

esp_err_t grove_lcd_rgb_set_cursor(uint8_t col, uint8_t row)
{
    if (!is_initialized) {
        ESP_LOGE(TAG, "Grove LCD RGB not initialized");
        return ESP_ERR_INVALID_STATE;
    }
    
    if (col > 15 || row > 1) {
        ESP_LOGE(TAG, "Invalid cursor position: col=%d, row=%d", col, row);
        return ESP_ERR_INVALID_ARG;
    }
    
    uint8_t row_offsets[] = {0x00, 0x40};
    uint8_t address = LCD_SETDDRAMADDR | (col + row_offsets[row]);
    
    return lcd_write_command(address);
}

esp_err_t grove_lcd_rgb_print(const char* str)
{
    if (!is_initialized) {
        ESP_LOGE(TAG, "Grove LCD RGB not initialized");
        return ESP_ERR_INVALID_STATE;
    }
    
    if (str == NULL) {
        ESP_LOGE(TAG, "String pointer is NULL");
        return ESP_ERR_INVALID_ARG;
    }
    
    esp_err_t ret = ESP_OK;
    size_t len = strlen(str);
    
    for (size_t i = 0; i < len && ret == ESP_OK; i++) {
        ret = lcd_write_data(str[i]);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to write character '%c' at position %d", str[i], i);
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(1)); // Small delay between characters
    }
    
    return ret;
}

esp_err_t grove_lcd_rgb_set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    if (!is_initialized) {
        ESP_LOGE(TAG, "Grove LCD RGB not initialized");
        return ESP_ERR_INVALID_STATE;
    }
    
    esp_err_t ret = rgb_write_reg(REG_RED, r);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set red value: %s", esp_err_to_name(ret));
        return ret;
    }
    
    ret = rgb_write_reg(REG_GREEN, g);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set green value: %s", esp_err_to_name(ret));
        return ret;
    }
    
    ret = rgb_write_reg(REG_BLUE, b);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set blue value: %s", esp_err_to_name(ret));
        return ret;
    }
    
    ESP_LOGI(TAG, "RGB backlight set to R=%d, G=%d, B=%d", r, g, b);
    return ESP_OK;
}

esp_err_t grove_lcd_rgb_display(bool on)
{
    if (!is_initialized) {
        ESP_LOGE(TAG, "Grove LCD RGB not initialized");
        return ESP_ERR_INVALID_STATE;
    }
    
    uint8_t cmd = LCD_DISPLAYCONTROL | LCD_CURSOROFF | LCD_BLINKOFF;
    if (on) {
        cmd |= LCD_DISPLAYON;
    } else {
        cmd |= LCD_DISPLAYOFF;
    }
    
    esp_err_t ret = lcd_write_command(cmd);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Display turned %s", on ? "on" : "off");
    }
    
    return ret;
}
