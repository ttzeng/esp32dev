# 1.8" ST7735-based TFT Display Demo

A graphics demo of driving a 1.8" TFT LCD displays using ESP32-C3 Super Mini board.
The demo is similar to the [one][1] created by Claude model for the STM32F103C8 Black Pill board, as the driver object is derived from the same [TFT][2] base class.

## Hardware Connections

```
ST7735 Display          STM32F103C8 Black Pill
┌─────────────┐        ┌────────────────┐
│             │        │                │
│   VCC   ────┼────────┼─ 3.3V          │
│   GND   ────┼────────┼─ GND           │
│   CS    ────┼────────┼─ GPIO7         │
│   RESET ────┼────────┼─ GPIO0         │
│   A0/DC ────┼────────┼─ GPIO1         │
│   SDA   ────┼────────┼─ GPIO6 (MOSI)  │
│   SCK   ────┼────────┼─ GPIO4 (SCK)   │
│   LED   ────┼────────┼─ 3.3V          │
│             │        │                │
└─────────────┘        └────────────────┘
```

## Demo Programs
1. **Fill Screen** - Solid color transitions
2. **Line Patterns** - Horizontal, vertical, diagonal lines
3. **Rectangles** - Concentric and grid patterns
4. **Circles** - Outlined and filled circles
5. **Color Palette** - RGB gradient bars
6. **Gradient Effects** - Smooth color transitions
7. **Text Display** - Multi-line text rendering

[1]: https://github.com/ttzeng/stm32dev/tree/main/tftdemo-st7735
[2]: https://github.com/ttzeng/code-snippet/blob/main/tft.md
