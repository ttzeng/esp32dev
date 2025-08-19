#ifndef GROVE_LCD_RGB_H
#define GROVE_LCD_RGB_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"
#include "driver/i2c_master.h"

#ifdef __cplusplus
extern "C" {
#endif

#define I2C_MASTER_NUM           0     // I2C master port number
#define I2C_MASTER_FREQ_HZ       100000 // I2C master clock frequency
#define I2C_MASTER_TIMEOUT_MS    1000

// LCD commands
#define LCD_CLEARDISPLAY         0x01
#define LCD_RETURNHOME           0x02
#define LCD_ENTRYMODESET         0x04
#define LCD_DISPLAYCONTROL       0x08
#define LCD_CURSORSHIFT          0x10
#define LCD_FUNCTIONSET          0x20
#define LCD_SETCGRAMADDR         0x40
#define LCD_SETDDRAMADDR         0x80

// LCD display control flags
#define LCD_DISPLAYON            0x04
#define LCD_DISPLAYOFF           0x00
#define LCD_CURSORON             0x02
#define LCD_CURSOROFF            0x00
#define LCD_BLINKON              0x01
#define LCD_BLINKOFF             0x00

// LCD entry mode flags
#define LCD_ENTRYRIGHT           0x00
#define LCD_ENTRYLEFT            0x02
#define LCD_ENTRYSHIFTINCREMENT  0x01
#define LCD_ENTRYSHIFTDECREMENT  0x00

// LCD function set flags
#define LCD_8BITMODE             0x10
#define LCD_4BITMODE             0x00
#define LCD_2LINE                0x08
#define LCD_1LINE                0x00
#define LCD_5x10DOTS             0x04
#define LCD_5x8DOTS              0x00

// RGB registers
#define REG_RED                  0x04
#define REG_GREEN                0x03
#define REG_BLUE                 0x02
#define REG_MODE1                0x00
#define REG_MODE2                0x01
#define REG_OUTPUT               0x08

/**
 * @brief Initialize Grove LCD RGB module using new I2C API
 * 
 * @return esp_err_t ESP_OK on success, error code otherwise
 */
esp_err_t grove_lcd_rgb_init(void);

/**
 * @brief Deinitialize Grove LCD RGB module and cleanup I2C resources
 * 
 * @return esp_err_t ESP_OK on success, error code otherwise
 */
esp_err_t grove_lcd_rgb_deinit(void);

/**
 * @brief Clear the LCD display
 * 
 * @return esp_err_t ESP_OK on success, error code otherwise
 */
esp_err_t grove_lcd_rgb_clear(void);

/**
 * @brief Set cursor position
 * 
 * @param col Column position (0-15)
 * @param row Row position (0-1)
 * @return esp_err_t ESP_OK on success, error code otherwise
 */
esp_err_t grove_lcd_rgb_set_cursor(uint8_t col, uint8_t row);

/**
 * @brief Print string to LCD
 * 
 * @param str String to print
 * @return esp_err_t ESP_OK on success, error code otherwise
 */
esp_err_t grove_lcd_rgb_print(const char* str);

/**
 * @brief Set RGB backlight color
 * 
 * @param r Red value (0-255)
 * @param g Green value (0-255)
 * @param b Blue value (0-255)
 * @return esp_err_t ESP_OK on success, error code otherwise
 */
esp_err_t grove_lcd_rgb_set_rgb(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Turn display on/off
 * 
 * @param on true to turn on, false to turn off
 * @return esp_err_t ESP_OK on success, error code otherwise
 */
esp_err_t grove_lcd_rgb_display(bool on);

#ifdef __cplusplus
}
#endif

#endif // GROVE_LCD_RGB_H
