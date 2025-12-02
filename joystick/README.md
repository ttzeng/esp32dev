# ESP32-C3 Super Mini TFT Menu Demo with Joystick Control

A comprehensive demo program featuring an interactive menu system controlled by an analog joystick, displayed on an ST7735 TFT screen for the ESP32-C3 Super Mini board.

*Note, though the driver and the demo program were developed by myself, the code was reviewed by the `claude-sonnet-4-5-20250929` model and generated this document.*

## Overview

This project showcases a complete graphical user interface system that combines:
- **ST7735 TFT Display** - 128x160 pixel color display
- **Analog Joystick** - 2-axis control with button
- **Interactive Menu System** - Navigate and select visual demos
- **Multiple Graphics Demos** - Text rendering, sine wave, geometric shapes

## Features

- ✅ **Joystick-Driven Navigation** - Intuitive left/right menu selection
- ✅ **Button Press Activation** - Execute selected demo with button press
- ✅ **3D-Style Buttons** - Visual feedback with raised/pressed states
- ✅ **Multiple Demo Modes:**
  - Text rendering with multiple fonts
  - Animated sine wave visualization
  - Geometric shapes and patterns
- ✅ **Modular Design** - Easy to add new menu items and demos
- ✅ **FreeRTOS Integration** - Responsive real-time operation

## Hardware Requirements

- ESP32-C3 Super Mini development board
- ST7735 TFT Display (80x160 or 128x160)
- 2-Axis Analog Joystick with button
- Connecting wires

## Pin Configuration

### TFT Display Connections

| TFT Pin | ESP32-C3 GPIO | Function |
|---------|---------------|----------|
| MOSI    | GPIO 6        | SPI Data Out |
| SCK     | GPIO 4        | SPI Clock |
| CS      | GPIO 7        | Chip Select |
| DC      | GPIO 3        | Data/Command |
| RESET   | GPIO 5        | Display Reset |
| VCC     | 3.3V          | Power |
| GND     | GND           | Ground |
| BL      | 3.3V          | Backlight (optional) |

### Joystick Connections

| Joystick Pin | ESP32-C3 GPIO | Function |
|--------------|---------------|----------|
| VRx (X-axis) | GPIO 0        | ADC Channel 0 |
| VRy (Y-axis) | GPIO 1        | ADC Channel 1 |
| SW (Button)  | GPIO 2        | Digital Input |
| VCC          | 3.3V          | Power |
| GND          | GND           | Ground |

## Wiring Diagram

```
ESP32-C3 Super Mini          ST7735 TFT Display
┌──────────────────┐        ┌─────────────────┐
│                  │        │                 │
│  GPIO 6 (MOSI)  ●──────────●  MOSI/SDA      │
│  GPIO 4 (SCK)   ●──────────●  SCK           │
│  GPIO 7 (CS)    ●──────────●  CS            │
│  GPIO 3 (DC)    ●──────────●  DC/RS         │
│  GPIO 5 (RST)   ●──────────●  RESET         │
│  3.3V           ●──────────●  VCC           │
│  GND            ●──────────●  GND           │
│                  │        └─────────────────┘
│                  │
│                  │        Analog Joystick
│                  │        ┌─────────────────┐
│  GPIO 0 (ADC0)  ●──────────●  VRx (X-axis)  │
│  GPIO 1 (ADC1)  ●──────────●  VRy (Y-axis)  │
│  GPIO 2 (BTN)   ●──────────●  SW (Button)   │
│  3.3V           ●──────────●  VCC           │
│  GND            ●──────────●  GND           │
└──────────────────┘        └─────────────────┘
```

## Software Requirements

- ESP-IDF v5.0 or later
- FreeRTOS (included with ESP-IDF)

## Project Structure

```
joystick_demo/
├── main/
│   ├── demo.cpp                      # Main demo application
│   └── CMakeLists.txt
├── CMakeLists.txt
└── README.md
```

## Building and Flashing

### Prerequisites

Ensure ESP-IDF is properly installed and configured:

```bash
# Set up ESP-IDF environment
. $HOME/esp/esp-idf/export.sh
```

### Build Steps

1. **Clone the project**

2. **Configure the project:**
```bash
idf.py set-target esp32c3
idf.py menuconfig
```

3. **Build the project:**
```bash
idf.py build
```

4. **Flash to ESP32-C3:**
```bash
idf.py -p /dev/ttyUSB0 flash monitor
```

Or use the USB serial/JTAG interface:
```bash
idf.py -p /dev/ttyACM0 flash monitor
```

## Usage

### Basic Operation

1. **Power On**
   - The display shows a yellow border with menu buttons at the top
   - Default selection is the first menu item (highlighted)

2. **Navigate Menu**
   - Move joystick **LEFT** to select previous menu item
   - Move joystick **RIGHT** to select next menu item
   - Menu wraps around (last → first, first → last)

3. **Execute Demo**
   - **Press joystick button** to run the selected demo
   - The display area below the menu shows the demo output

4. **Return to Menu**
   - Navigate to a different menu item to clear the demo area

### Menu Items

#### 1. "txt" - Text Rendering Demo
Displays multiple text strings with different:
- Font sizes (7x10, 8x8, 11x18)
- Colors (White, Cyan, Yellow, Green, Magenta)
- Text content showcasing the device capabilities

**Sample Output:**
```
ESP32C3          (11x18 font, white)
  Super Mini     (default font, cyan)
ST7735 TFT       (8x8 font, yellow)
    SPI Master   (7x10 font, green)
  Graphics!      (default font, magenta)
```

#### 2. "sin" - Sine Wave Demo
Generates an animated sine wave visualization:
- Smooth yellow line
- Mathematical sine function: `y = 50 × sin(x × π / 30)`
- Spans entire display width
- Demonstrates line drawing and mathematical graphics

#### 3. "clk" - Analog Clock Demo
- Demonstrates animated line rotate

#### 4. "rect" - Rectangle Demo
Shows two patterns:

**Pattern 1: Concentric Rectangles**
- 10 nested rectangles
- Gradient color transition
- Demonstrates precise geometry

**Pattern 2: Color Grid**
- 3×3 grid of colored rectangles
- 9 different colors: Red, Green, Blue, Yellow, Cyan, Magenta, Orange, Purple, Pink
- Demonstrates fill operations and color palette

### Joystick Operation

#### Reading Values

The joystick driver provides normalized values:

```cpp
int x, y;              // Position values
bool button_pressed;   // Button state
joystick->read(x, y, button_pressed, 10);
```

**Parameters:**
- `x`: Horizontal position (0-10 when normalized to 10)
- `y`: Vertical position (0-10 when normalized to 10)
- `button_pressed`: True when button is pressed
- `normalized`: Value range (10 = scale 0-4096 to 0-10)

#### Navigation Thresholds

```cpp
if (x < 3)   // Joystick moved LEFT
    moveTo--;

if (x > 8)   // Joystick moved RIGHT
    moveTo++;
```

**Neutral Zone:** x = 3 to 8 (no navigation)

## Code Architecture

### Main Components

#### 1. Menu System

```cpp
typedef struct {
    const char *title;   // Menu button text
    demo_func callback;  // Function pointer to demo
} MenuItem;

static MenuItem menu[] = {
    { "text", demo_text },
    { "sin",  demo_sinewave },
    { "clk",  demo_clock },
    { "rect", demo_rectangles },
};
```

**Adding New Menu Items:**

```cpp
static void my_new_demo(tft *tft)
{
    // Your demo code here
    tft->draw_string(10, 50, "Hello!", RGB565_WHITE, RGB565_BLACK);
}

// Add to menu array:
{ "new", my_new_demo },
```

#### 2. Button Rendering

The program features 3D-style buttons with visual feedback:

```cpp
static void draw_button(tft *tft, int x, int y, const char* text, bool state)
```

**Button States:**
- **Normal (false):** Dark gray edges (pressed appearance)
- **Selected (true):** Light gray edges (raised appearance), yellow text

**Visual Design:**
- Top and left edges: Light or dark gray
- Bottom and right edges: Opposite shade for 3D effect
- Text color changes based on state
- Blue background

#### 3. Demo Functions

Each demo follows this pattern:

```cpp
static void demo_name(tft *tft)
{
    // Clear or prepare display
    // Draw graphics using TFT API
    // Use available colors and primitives
}
```

**Available TFT Functions:**
- `clear(color)` - Fill entire screen
- `draw_string(x, y, text, fg, bg, font)` - Render text
- `line(x1, y1, x2, y2, color)` - Draw line
- `rect(x, y, w, h, color)` - Draw rectangle outline
- `fill(x, y, w, h, color)` - Draw filled rectangle

### Main Task Flow

```cpp
demo_task()
├── Initialize joystick and TFT
├── Clear screen with yellow border
└── Main Loop:
    ├── Read joystick position and button
    ├── Update menu position based on X-axis
    ├── Redraw menu bar if position changed
    ├── Execute demo if button pressed
    └── Delay 100ms (10Hz update rate)
```

## Color Definitions

The program uses RGB565 format (16-bit color):

```cpp
RGB565(r, g, b)  // r, g, b: 0-255

// Predefined colors:
RGB565_BLACK     // 0x0000
RGB565_WHITE     // 0xFFFF
RGB565_RED       // 0xF800
RGB565_GREEN     // 0x07E0
RGB565_BLUE      // 0x001F
RGB565_YELLOW    // 0xFFE0
RGB565_CYAN      // 0x07FF
RGB565_MAGENTA   // 0xF81F
RGB565_ORANGE    // Custom
RGB565_PURPLE    // Custom
RGB565_PINK      // Custom
```

## Customization Guide

### Change Menu Layout

```cpp
// Modify button size calculation in button_outlines():
static inline void button_outlines(const char* text, int& width, int& height)
{
    width = strlen(text) * 7 + 6;  // Adjust multiplier for wider buttons
    height = 11 + 4;                // Adjust for taller buttons
}
```

### Adjust Navigation Sensitivity

```cpp
// Change thresholds in demo_task():
if (x < 2)   // More sensitive (triggers earlier)
if (x > 9)   // Less sensitive (requires more movement)
```

### Modify Update Rate

```cpp
// Change delay at end of main loop:
vTaskDelay(pdMS_TO_TICKS(50));   // 20Hz (faster)
vTaskDelay(pdMS_TO_TICKS(200));  // 5Hz (slower)
```

### Add Custom Fonts

If your TFT library supports additional fonts:

```cpp
extern const GFXfont CustomFont12x16;

tft->draw_string(10, 50, "Text", RGB565_WHITE, 
                 RGB565_BLACK, &CustomFont12x16);
```

## API Reference

### Joystick Driver API

```cpp
class joystick {
public:
    virtual void read(int& x, int& y, bool& button, int normalized);
};
```

**Method: read()**

Reads the current joystick state.

**Parameters:**
- `x` (output): Horizontal axis position
- `y` (output): Vertical axis position  
- `button` (output): Button press state (true = pressed)
- `normalized` (input): Normalization range

**Raw Values:** 0-4095 (12-bit ADC)

**Normalized Values:** 0-normalized parameter
- Example: `normalized = 10` → returns 0-10
- Example: `normalized = 100` → returns 0-100

**Usage Examples:**

```cpp
// Read with normalization to 0-10 range
int x, y;
bool pressed;
joystick->read(x, y, pressed, 10);

// Check directions
if (x < 3) {
    // Moved LEFT
}
if (x > 7) {
    // Moved RIGHT
}
if (y < 3) {
    // Moved DOWN
}
if (y > 7) {
    // Moved UP
}
if (pressed) {
    // Button is pressed
}

// Read raw values (no normalization)
joystick->read(x, y, pressed, 4095);
// x and y now range from 0-4095
```

### TFT Driver API (Brief)

For complete API documentation, refer to the TFT driver documentation.

**Common Methods:**
```cpp
tft->clear(color);
tft->draw_string(x, y, text, fg_color, bg_color, font);
tft->line(x1, y1, x2, y2, color);
tft->rect(x, y, width, height, color);
tft->fill(x, y, width, height, color);
uint16_t width();
uint16_t height();
```

## Performance Considerations

### Update Rate

- **Current:** 100ms (10Hz)
- **Recommended range:** 50-200ms (5-20Hz)
- **Too fast:** Joystick becomes too sensitive
- **Too slow:** Unresponsive user experience

### Memory Usage

- **Flash:** ~50KB (including drivers and fonts)
- **RAM:** ~10KB (frame buffers, task stack)
- **Task Stack:** 2048 bytes (adjustable)

### CPU Usage

- **Idle:** <5%
- **Drawing:** 10-30% (depends on complexity)
- **SPI Transfer:** DMA-capable for efficiency

## Advanced Features

### Adding Animated Demos

```cpp
static void demo_animated(tft *tft)
{
    for (int frame = 0; frame < 100; frame++) {
        // Clear previous frame
        tft->clear(RGB565_BLACK);
        
        // Draw animation frame
        int x = frame;
        int y = 50 + 20 * sin(frame * 0.1);
        tft->fill(x, y, 5, 5, RGB565_RED);
        
        // Small delay for animation speed
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
```

### Using Y-Axis for Additional Control

```cpp
// In demo_task(), add:
if (y < 3) {
    // Joystick moved DOWN - implement action
}
if (y > 7) {
    // Joystick moved UP - implement action
}
```

### Multi-Level Menus

```cpp
typedef struct {
    const char *title;
    MenuItem *submenu;    // NULL for demos, points to submenu items
    demo_func callback;
} MenuItemExtended;
```

### State Persistence

```cpp
// Save current menu position to NVS
#include "nvs_flash.h"

void save_menu_position(int pos) {
    nvs_handle_t handle;
    nvs_open("storage", NVS_READWRITE, &handle);
    nvs_set_i32(handle, "menu_pos", pos);
    nvs_commit(handle);
    nvs_close(handle);
}
```

## Example Applications

### Drawing Application

```cpp
static void demo_drawing(tft *tft)
{
    int prev_x = 0, prev_y = 0;
    bool first = true;
    
    while (1) {
        int x, y;
        bool button;
        joystick->read(x, y, button, 10);
        
        // Map joystick to screen coordinates
        int screen_x = x * tft->width() / 10;
        int screen_y = y * tft->height() / 10;
        
        if (!first && button) {
            // Draw line from previous position
            tft->line(prev_x, prev_y, screen_x, screen_y, RGB565_WHITE);
        }
        
        prev_x = screen_x;
        prev_y = screen_y;
        first = !button;
        
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
```

## References

- [ESP32-C3 Technical Reference](https://www.espressif.com/sites/default/files/documentation/esp32-c3_technical_reference_manual_en.pdf)
- [ST7735 Datasheet](https://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf)
- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/)
- [FreeRTOS Documentation](https://www.freertos.org/Documentation/RTOS_book.html)

---

**Developed for ESP32-C3 Super Mini with ST7735 TFT Display**

Happy Coding! 🎮🖥️
