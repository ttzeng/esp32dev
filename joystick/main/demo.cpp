#include <math.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tft-st7735-supermini.hpp"
#include "joystick-supermini.hpp"
#include "esp_log.h"

#define PI 3.14159

// Pin definitions used in the demo
#define TFT_MOSI            GPIO_NUM_6
#define TFT_SCK             GPIO_NUM_4
#define TFT_CS              GPIO_NUM_7
#define TFT_DC              GPIO_NUM_3
#define TFT_RESET           GPIO_NUM_5
#define JOYSTICK_X_CHANNEL  ADC_CHANNEL_0  // GPIO0
#define JOYSTICK_Y_CHANNEL  ADC_CHANNEL_1  // GPIO1
#define JOYSTICK_BUTTON     GPIO_NUM_2

static const char *TAG = "Demo";

static inline void button_outlines(const char* text, int& width, int& height)
{
    width = strlen(text) * 7 + 6, height = 11 + 4;
}

static void draw_button(tft *tft, int x, int y, const char* text, bool state)
{
    int width, height;
    button_outlines(text, width, height);

    uint16_t color_darkgray  = RGB565(0x30, 0x30, 0x30),
             color_lightgray = RGB565(0x6f, 0x6f, 0x6f),
             color_btn_left_top = state? color_lightgray : color_darkgray,
             color_btn_right_bottom = state? color_darkgray : color_lightgray,
             color_btn_text = state? RGB565_YELLOW : color_lightgray;
    tft->fill(x, y, width, height, RGB565_BLUE)
        .line(x, y + height - 1, x, y, color_btn_left_top)
        .line(x, y, x + width - 1, y, color_btn_left_top)
        .line(x + width - 1, y, x + width - 1, y + height - 1, color_btn_right_bottom)
        .line(x + width - 1, y + height - 1, x, y + height - 1, color_btn_right_bottom)
        .draw_string(x + 3, y + 3, text, color_btn_text, RGB565_BLUE, &Font7x10);
}

static void demo_text(tft *tft)
{
    tft->draw_string(5, 25, "ESP32C3", RGB565_WHITE, RGB565_BLACK, &Font11x18)
        .draw_string(10, 48, "Super Mini", RGB565_CYAN, RGB565_BLACK)
        .draw_string(5, 71, "ST7735 TFT", RGB565_YELLOW, RGB565_BLACK, &Font8x8)
        .draw_string(35, 94, "SPI Master", RGB565_GREEN, RGB565_BLACK, &Font7x10)
        .draw_string(20, 117, "Graphics!", RGB565_MAGENTA, RGB565_BLACK);
}

static void demo_sinewave(tft *tft)
{
    uint16_t width = tft->width(), height = tft->height();
    int prevX = 0, prevY = height/2;
    for (int x = 0; x < width; x++) {
        int y = height/2 + (int)(50 * sin(x * PI / 30));
        if (x > 0)
            tft->line(prevX, prevY, x, y, RGB565_YELLOW);
        prevX = x, prevY = y;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

static void demo_clock(tft *tft)
{
    int cx = tft->width() / 2, cy = tft->height() / 2;

    int x = cx, y = cy;
    for (int angle = 0; angle <= 720; angle += 5) {
        tft->line(cx, cy, x, y, RGB565_BLACK);

        float rad = angle * PI / 180.0;
        x = cx + 30 * cos(rad);
        y = cy + 30 * sin(rad);
        tft->line(cx, cy, x, y, RGB565_GREEN);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

static void demo_rectangles(tft *tft)
{
    int w, h;
    button_outlines("", w, h);

    uint16_t width = tft->width(), height = tft->height();

    // Concentric rectangles
    for (int i = 0; i < 40; i += 4) {
        uint16_t color = RGB565(i*6, 255-i*6, i*3);
        tft->rect(i, h + i, width-1-2*i, height-1-2*i - h, color);
    }

    // Filled rectangles grid
    tft->fill(0, h, width, height - h, RGB565_BLACK);
    uint16_t colors[] = {RGB565_RED, RGB565_GREEN, RGB565_BLUE,
                         RGB565_YELLOW, RGB565_CYAN, RGB565_MAGENTA,
                         RGB565_ORANGE, RGB565_PURPLE, RGB565_PINK};

    w = width / 3, h = (height-h) / 3;

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            tft->rect(col*w, height - (row+1)*h, w-2, h-2, colors[row*3+col]);
        }
    }
}

typedef void (*demo_func)(tft *tft);
typedef struct {
    const char *title;
    demo_func callback;
} MenuItem;

static MenuItem menu[] = {
    { "txt",    demo_text },
    { "sin",    demo_sinewave },
    { "clk",    demo_clock },
    { "rect",   demo_rectangles },
};

static void draw_menubar(tft *tft, const MenuItem *menu, int n_items, int pos)
{
    int x, w, h;
    for (int i = x = 0; i < n_items; x += w, i++, menu++) {
        button_outlines(menu->title, w, h);
        draw_button(tft, x, 0, menu->title, i == pos);
    }
}

static void demo_task(void *arg)
{
    joystick *joystick = new joystick_supermini(JOYSTICK_X_CHANNEL, JOYSTICK_Y_CHANNEL, JOYSTICK_BUTTON);
    tft *tft = new tft_st7735_supermini(SPI2_HOST, TFT_CS, TFT_SCK, TFT_MOSI, TFT_DC, TFT_RESET, ST7735_ROTATION_90);
    tft->clear(RGB565_BLACK)
        .rect(0, 0, tft->width(), tft->height(), RGB565_YELLOW);

    int n_items = sizeof(menu) / sizeof(MenuItem);
    int current, moveTo;
    for (current = -1, moveTo = 0; 1; ) {
        int x, y;
        bool button_pressed;

        joystick->read(x, y, button_pressed, 10);
        if (x < 3)
            if (--moveTo < 0)
                moveTo = n_items - 1;
        if (x > 8)
            if (++moveTo >= n_items)
                moveTo = 0;

        if (current != moveTo)
            draw_menubar(tft, menu, n_items, current = moveTo);
        else if (button_pressed) {
            int w, h;
            button_outlines("", w, h);
            tft->fill(0, h, tft->width(), tft->height() - h, RGB565_BLACK);
            menu[current].callback(tft);
        }

        // Delay for readability
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "ESP32-C3 Joystick Reader Starting...");

    // Create joystick reading task
    xTaskCreate(demo_task, "Demo", 2048, NULL, 5, NULL);

    ESP_LOGI(TAG, "Joystick reader initialized successfully");
}
