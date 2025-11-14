#include <math.h>
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "tft-st7735-supermini.hpp"
#include "sdkconfig.h"

#define RX_BUF_SIZE 1024

// Pin definitions for ESP32-C3 Super Mini
#define PIN_MOSI    6
#define PIN_SCK     4
#define PIN_CS      7
#define PIN_DC      1
#define PIN_RESET   0

static void mdelay(uint32_t msec)
{
    vTaskDelay(msec / portTICK_PERIOD_MS);
}

static void demo_text(tft *tft)
{
    tft->clear(RGB565_BLACK);

    tft->draw_string(5, 5, "ESP32C3", RGB565_WHITE, RGB565_BLACK, &Font11x18);
    tft->draw_string(10, 28, "Super Mini", RGB565_CYAN, RGB565_BLACK);
    tft->draw_string(5, 51, "ST7735 TFT", RGB565_YELLOW, RGB565_BLACK, &Font8x8);
    tft->draw_string(35, 74, "SPI Master", RGB565_GREEN, RGB565_BLACK, &Font7x10);
    tft->draw_string(20, 97, "Graphics!", RGB565_MAGENTA, RGB565_BLACK);

    mdelay(2000);
}

static void demo_fillScreen(tft *tft)
{
    tft->clear(RGB565_BLACK);
    tft->draw_string(10, 10, "Fill Screen", RGB565_WHITE, RGB565_BLACK);

    mdelay(500);
    tft->clear(RGB565_RED);
    mdelay(500);
    tft->clear(RGB565_GREEN);
    mdelay(500);
    tft->clear(RGB565_BLUE);
    mdelay(500);
    tft->clear(RGB565_YELLOW);
    mdelay(500);
    tft->clear(RGB565_CYAN);
    mdelay(500);
    tft->clear(RGB565_MAGENTA);
    mdelay(500);
}

static void demo_lines(tft *tft)
{
    tft->clear(RGB565_BLACK);
    tft->draw_string(10, 10, "Lines", RGB565_WHITE, RGB565_BLACK);

    uint16_t width = tft->width(), height = tft->height();

    // Horizontal lines
    for (int y = 0; y < height; y += 8) {
        tft->line(0, y, width-1, y, RGB565_RED);
    }
    mdelay(1000);

    tft->clear(RGB565_BLACK);
    // Vertical lines
    for (int x = 0; x < width; x += 8) {
        tft->line(x, 0, x, height-1, RGB565_GREEN);
    }
    mdelay(1000);

    // Diagonal lines from corners
    tft->clear(RGB565_BLACK);
    for (int i = 0; i < width; i += 8) {
        tft->line(0, 0, i, height-1, RGB565_BLUE);
        tft->line(width-1, 0, width-1-i, height-1, RGB565_YELLOW);
    }
    for (int i = 0; i < height; i += 8) {
        tft->line(0, 0, width-1, i, RGB565_CYAN);
        tft->line(width-1, 0, 0, height-1-i, RGB565_MAGENTA);
    }
    mdelay(1500);
}

static void demo_rectangles(tft *tft)
{
    tft->clear(RGB565_BLACK);
    tft->draw_string(10, 10, "Rectangles", RGB565_WHITE, RGB565_BLACK);

    uint16_t width = tft->width(), height = tft->height();

    // Concentric rectangles
    for (int i = 0; i < 40; i += 4) {
        uint16_t color = RGB565(i*6, 255-i*6, i*3);
        tft->rect(i, i, width-1-2*i, height-1-2*i, color);
    }
    mdelay(1500);

    // Filled rectangles grid
    tft->clear(RGB565_BLACK);
    uint16_t colors[] = {RGB565_RED, RGB565_GREEN, RGB565_BLUE,
                         RGB565_YELLOW, RGB565_CYAN, RGB565_MAGENTA,
                         RGB565_ORANGE, RGB565_PURPLE, RGB565_PINK};

    int w = width / 3;
    int h = height / 3;

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            tft->rect(col*w, row*h, w-2, h-2, colors[row*3+col]);
        }
    }
    mdelay(1500);
}

static void demo_circles(tft *tft)
{
    tft->clear(RGB565_BLACK);
    tft->draw_string(10, 10, "Circles", RGB565_WHITE, RGB565_BLACK);

    uint16_t width = tft->width(), height = tft->height();

    // Concentric circles
    int centerX = tft->width() / 2;
    int centerY = tft->height() / 2;

    for (int r = 10; r < 60; r += 8) {
        uint16_t color = RGB565(r*4, 255-r*4, 128);
        tft->circle(centerX, centerY, r, color);
    }
    mdelay(1500);

    // Filled circles pattern
    tft->clear(RGB565_BLACK);
    tft->fill(40, 40, 30, RGB565_RED);
    tft->fill(120, 40, 30, RGB565_GREEN);
    tft->fill(40, 88, 30, RGB565_BLUE);
    tft->fill(120, 88, 30, RGB565_YELLOW);
    tft->fill(80, 64, 25, RGB565_MAGENTA);
    mdelay(1500);
}

static void demo_colorPalette(tft *tft)
{
    tft->clear(RGB565_BLACK);
    tft->draw_string(10, 10, "Color Palette", RGB565_WHITE, RGB565_BLACK);

    uint16_t width = tft->width(), height = tft->height();

    // RGB color bars
    int barHeight = height / 3;

    for (int x = 0; x < width; x++) {
        uint8_t intensity = (x * 255) / width;

        // Red gradient
        tft->line(x, 0, x, barHeight-1, RGB565(intensity, 0, 0));

        // Green gradient
        tft->line(x, barHeight, x, 2*barHeight-1, RGB565(0, intensity, 0));

        // Blue gradient
        tft->line(x, 2*barHeight, x, height-1, RGB565(0, 0, intensity));
    }

    mdelay(2000);
}

static void demo_gradient(tft *tft)
{
    tft->clear(RGB565_BLACK);
    tft->draw_string(10, 10, "Gradient", RGB565_WHITE, RGB565_BLACK);

    uint16_t width = tft->width(), height = tft->height();

    // Horizontal gradient (red to blue)
    for (int x = 0; x < width; x++) {
        uint8_t red = 255 - (x * 255) / width;
        uint8_t blue = (x * 255) / width;
        uint16_t color = RGB565(red, 0, blue);
        tft->line(x, 0, x, height/2, color);
    }

    // Vertical gradient (green to yellow)
    for (int y = height/2; y < height; y++) {
        uint8_t red = ((y - height/2) * 255) / (height/2);
        uint16_t color = RGB565(red, 255, 0);
        tft->line(0, y, width-1, y, color);
    }

    mdelay(2000);
}

static void demo_sinewave(tft *tft)
{
    tft->clear(RGB565_BLACK);
    tft->draw_string(10, 10, "Sine Wave", RGB565_WHITE, RGB565_BLACK);

    uint16_t width = tft->width(), height = tft->height();
    int prevX = 0, prevY = height/2;
    for (int x = 0; x < width; x++) {
        int y = height/2 + (int)(50 * sin(x * 3.14159 / 30));
        if (x > 0)
            tft->line(prevX, prevY, x, y, RGB565_YELLOW);
        prevX = x;
        prevY = y;
    }
    mdelay(2000);
}

static void task_demo(void *pvParameter)
{
    printf("Hello from the TFT demo task!\n");

    tft *tft = new tft_st7735_supermini(SPI2_HOST, PIN_CS, PIN_SCK, PIN_MOSI, PIN_DC, PIN_RESET);
    while (1) {
        demo_text(tft);
        demo_fillScreen(tft);
        demo_lines(tft);
        demo_rectangles(tft);
        demo_circles(tft);
        demo_colorPalette(tft);
        demo_gradient(tft);
        demo_sinewave(tft);
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

    xTaskCreate(task_demo, "Demo", 4096, NULL, 5, NULL);
}
