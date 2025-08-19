***The code and documents in this folder are generated primarily by the [Replit Agent][1], the AI coding agent service provided by [replit.com][2] to create the code based on user's requests using natural language.***

---

# ESP32-C3 Grove LCD RGB "Hello World" Project

This project demonstrates how to interface an ESP32-C3 microcontroller with a Grove LCD RGB backlight display module using the ESP-IDF framework with the **new I2C master API**. The application displays "Hello World" on the LCD with colorful RGB backlight effects.

## Features

- **Target Hardware**: ESP32-C3 microcontroller
- **Display**: Grove LCD RGB backlight module (16x2 characters)
- **Communication**: I2C protocol for both LCD and RGB control
- **Visual Effects**: Dynamic RGB backlight color cycling
- **Architecture**: Component-based ESP-IDF structure

## Hardware Setup

### Required Components
- ESP32-C3 development board
- Grove LCD RGB backlight display module
- Grove connector cable or jumper wires

### Pin Connections
```
ESP32-C3    Grove LCD RGB
--------    -------------
GPIO9       SCL (Clock)
GPIO8       SDA (Data)  
5V          VCC
GND         GND
```

### Grove LCD RGB Module
- **LCD Address**: 0x3E (display control)
- **RGB Address**: 0x62 (backlight control) 
- **Interface**: I2C
- **Display**: 16x2 characters with RGB backlight

## Software Architecture

### Component Structure
```
└── components/
    └── Grove-LCD-RGB/          # Grove LCD RGB driver component
        ├── CMakeLists.txt      # Component build configuration
        ├── grove_lcd_rgb.h     # Driver header file
        ├── grove_lcd_rgb.c     # Driver implementation
        ├── Kconfig.projbuild   # I2C configuration options
        └── example/
            ├── main/
            │   ├── CMakeLists.txt  # Main component build config
            │   └── main.c          # Main application code
            └── CMakeLists.txt      # Project build configuration
```

### Key Features

#### Grove LCD RGB Driver Component (New I2C API)
- **Modern I2C Integration**: Uses ESP-IDF's new I2C master API with separate bus and device handles
- **Dual Device Support**: Independent handles for LCD display and RGB backlight controllers  
- **Initialization**: Configures I2C bus and adds LCD/RGB devices with proper error handling
- **Display Control**: Clear screen, set cursor position, print text using new API
- **RGB Backlight**: Set custom RGB colors (0-255 for each channel) with dedicated device handle
- **Resource Management**: Proper cleanup with grove_lcd_rgb_deinit() function
- **Error Handling**: Comprehensive error checking and logging with new API error codes

#### Example Code
- **Startup Sequence**: NVS initialization and LCD setup
- **Display Content**: Shows "Hello World" and "ESP32-C3 Ready!"
- **Visual Effects**: Cycles through red, green, and blue backlight colors
- **Continuous Operation**: Runs indefinitely with status logging

## API Reference

### Grove LCD RGB Functions (New I2C API)

```c
// Initialize the Grove LCD RGB module using new I2C master API
// Creates I2C bus and adds LCD/RGB device handles
esp_err_t grove_lcd_rgb_init(void);

// Deinitialize and cleanup I2C resources (NEW)
// Removes devices and deletes I2C bus handle
esp_err_t grove_lcd_rgb_deinit(void);

// Clear the LCD display
esp_err_t grove_lcd_rgb_clear(void);

// Set cursor position (col: 0-15, row: 0-1)
esp_err_t grove_lcd_rgb_set_cursor(uint8_t col, uint8_t row);

// Print string to LCD
esp_err_t grove_lcd_rgb_print(const char* str);

// Set RGB backlight color (r, g, b: 0-255)
esp_err_t grove_lcd_rgb_set_rgb(uint8_t r, uint8_t g, uint8_t b);

// Turn display on/off
esp_err_t grove_lcd_rgb_display(bool on);
```

### New I2C API Implementation Details

The driver now uses ESP-IDF's modern I2C master API with these key improvements:

- **Bus Handle**: `i2c_master_bus_handle_t` for I2C bus management
- **Device Handles**: Separate `i2c_master_dev_handle_t` for LCD and RGB controllers
- **Resource Management**: Proper cleanup with dedicated deinit function
- **Enhanced Error Handling**: Better error reporting with new API error codes

## Building and Flashing

### Prerequisites
- **ESP-IDF v5.1 or later** (required for new I2C master API)
- ESP32-C3 development board
- USB cable for programming

> **Note**: This project uses ESP-IDF's new I2C master API introduced in v5.1. For older ESP-IDF versions, the legacy I2C API would need to be used instead.

### Build Commands
```bash
cd example/

# Configure for ESP32-C3
idf.py set-target esp32c3

# Build the project
idf.py build

# Flash to device
idf.py -p /dev/ttyUSB0 flash monitor
```

## Expected Output

### Serial Console
```
I (xxx) GROVE_LCD_HELLO_WORLD: Starting Grove LCD RGB Hello World application
I (xxx) GROVE_LCD_HELLO_WORLD: Initializing Grove LCD RGB module
I (xxx) GROVE_LCD_RGB: Initializing Grove LCD RGB module with new I2C API
I (xxx) GROVE_LCD_RGB: Grove LCD RGB module initialized successfully with new I2C API
I (xxx) GROVE_LCD_HELLO_WORLD: Message 'Hello World' displayed on Grove LCD RGB module
I (xxx) GROVE_LCD_HELLO_WORLD: Application running... Hello World displayed
```

### LCD Display
```
Line 1: Hello World
Line 2: ESP32-C3 Ready!
```
*RGB backlight cycles through green → red → blue → green every 2-3 seconds*

## Troubleshooting

### Common Issues

1. **I2C Communication Errors**
   - Check wiring connections (SDA/SCL pins)
   - Verify 5V power supply to LCD module
   - Ensure pull-up resistors on I2C lines

2. **LCD Not Displaying**
   - Confirm LCD I2C address (0x3E)
   - Check if LCD initialization succeeded in logs
   - Try adjusting I2C clock frequency

3. **RGB Backlight Not Working**
   - Verify RGB controller address (0x62)
   - Check RGB initialization in logs with new I2C API
   - Test with different color values
   - Ensure RGB device handle was created successfully

4. **New I2C API Migration Issues**
   - Ensure ESP-IDF v5.1+ is being used
   - Check that `driver/i2c_master.h` is included instead of `driver/i2c.h`
   - Verify bus handle and device handles are properly created
   - Use `grove_lcd_rgb_deinit()` for proper cleanup

### Debug Tips
- Enable verbose logging: `idf.py menuconfig` → Component config → Log output → Verbose
- Use I2C scanner to detect connected devices
- Monitor serial output for detailed error messages

## License

This project is provided as-is for educational and development purposes.

## Contributing

This is a basic example project. Feel free to extend it with additional features:
- Custom characters and graphics
- Menu systems
- Sensor data display
- Network connectivity
- Real-time clock integration

[1]: https://docs.replit.com/replitai/agent "Replit Agent"
[2]: https://replit.com/ "repl.it"