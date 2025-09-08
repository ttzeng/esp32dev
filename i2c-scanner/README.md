# ESP32-C3 I2C Scanner

A comprehensive I2C device scanner for ESP32-C3 Super Mini board.

*Note, the code and the documentation in this folder were created in collaboration with Claude Sonnet 4.*

## Features

- **Target Hardware**: ESP32-C3 Super Mini development board
- **Scan Range**: I2C addresses 0x08 to 0x77
- **Device Recognition**: Identifies 20+ common I2C device types
- **Continuous Scanning**: Automatic rescanning every 10 seconds
- **Clean Output**: Suppressed I2C error messages for readable logs

## Supported I2C Devices

The scanner can identify these common I2C devices:

| Address | Device Type | Description |
|---------|-------------|-------------|
| 0x3E | Grove LCD Display | 16x2 character LCD |
| 0x62 | Grove LCD RGB Backlight | RGB backlight controller |
| 0x68-0x69 | RTC/IMU | DS1307/DS3231 RTC or MPU6050/MPU9250 IMU |
| 0x48-0x4B | ADC | ADS1115/ADS1015 analog-to-digital converter |
| 0x76-0x77 | Environmental Sensor | BMP280/BME280/BMP180 pressure/humidity sensor |
| 0x3C-0x3D | OLED Display | SSD1306 OLED display |
| 0x5A | Temperature Sensor | MLX90614 IR temperature sensor |
| 0x40-0x41, 0x70-0x75 | PWM Driver | PCA9685 16-channel PWM driver |
| 0x20-0x27 | I/O Expander | PCF8574 8-bit I/O expander |
| 0x50-0x57 | EEPROM | 24C32/24C64 serial EEPROM |
| 0x39 | Light Sensor | TSL2561 light-to-digital converter |
| 0x29 | Distance Sensor | VL53L0X time-of-flight distance sensor |
| 0x1E | Magnetometer | HMC5883L 3-axis magnetometer |
| 0x44 | Humidity Sensor | SHT30/SHT31 temperature and humidity sensor |

## API Reference

### I2C Scanner Functions
```c
// Get shared I2C bus handle from Grove LCD driver
static esp_err_t i2c_scanner_init(void);

// Scan for I2C devices and print results
static void scan_and_print_i2c_devices(void)
```

## Building and Flashing

### Prerequisites
- **ESP-IDF v5.1 or later** (required for new I2C master API)
- ESP32-C3 Super Mini development board
- USB cable for programming

### Build Commands
```bash
# Navigate to project directory
cd i2c-scanner/

# Configure for ESP32-C3
idf.py set-target esp32c3

# Build the project
idf.py build

# Flash to device and monitor output
idf.py -p /dev/ttyACM0 flash monitor
```
## Expected Output

### Serial Console
```
I (340) I2C_SCANNER: Starting I2C Scanner with Grove LCD RGB Display
I (345) I2C_SCANNER: Using shared I2C bus handle via i2c_master_get_bus_handle()
I (355) I2C_SCANNER: NVS Flash initialized
I (360) I2C_SCANNER: Initializing Grove LCD RGB display...
I (410) I2C_SCANNER: Grove LCD RGB initialized successfully
I (420) I2C_SCANNER: Successfully obtained shared I2C bus handle
I (425) I2C_SCANNER: Starting new I2C scan cycle...
I (430) I2C_SCANNER: Starting I2C device scan (0x08 to 0x77)...
I (445) I2C_SCANNER: ✓ Device found at 0x3E: Grove LCD Display
I (465) I2C_SCANNER: ✓ Device found at 0x62: Grove LCD RGB Backlight
I (2890) I2C_SCANNER: Scan complete. Found 2 devices
I (2895) I2C_SCANNER: ==========================================
I (2900) I2C_SCANNER: I2C Scan Results Summary:
I (2905) I2C_SCANNER: Scanned range: 0x08 to 0x77
I (2910) I2C_SCANNER: Found 2 device(s):
I (2915) I2C_SCANNER:   [1] 0x3E - Grove LCD Display
I (2920) I2C_SCANNER:   [2] 0x62 - Grove LCD RGB Backlight
I (2925) I2C_SCANNER: ==========================================

I2C Address Map (X = device found, . = no device):
     0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
00:             .  .  .  .  .  .  .  .  .  .  .  . 
10:  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . 
20:  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . 
30:  .  .  .  .  .  .  .  .  .  .  .  .  .  .  X  . 
40:  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . 
50:  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . 
60:  .  .  X  .  .  .  .  .  .  .  .  .  .  .  .  . 
70:  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . 

I (2965) I2C_SCANNER: Waiting 10 seconds before next scan...
```

## Troubleshooting

### Common Issues

1. **I2C Communication Errors**
   - Check wiring connections (SDA=GPIO8, SCL=GPIO9)
   - Verify power supply to I2C devices (3.3V or 5V)
   - Ensure pull-up resistors are present (enabled internally)

2. **No Devices Found**
   - Confirm I2C devices are properly powered
   - Check device addresses are in scan range (0x08-0x77)
   - Test with known working I2C devices
   - Verify GPIO pin assignments match hardware

### Debug Tips
- Enable verbose logging temporarily for debugging: `esp_log_level_set("I2C_SCANNER", ESP_LOG_DEBUG)`
- Use multimeter to verify 3.3V/5V on I2C devices
- Monitor I2C bus with oscilloscope if available
- Try different I2C clock frequencies if communication fails

## Configuration Options

### I2C Scanner Settings
```c
#define I2C_MASTER_PORT             I2C_NUM_0
#define I2C_MASTER_FREQ_HZ          100000
#define I2C_SCAN_START_ADDR         0x08
#define I2C_SCAN_END_ADDR           0x77
#define I2C_PROBE_TIMEOUT_MS        100
```

### Pin Configuration (ESP32-C3 Super Mini)
```c
#define I2C_MASTER_SCL_IO           9       // GPIO9 for SCL
#define I2C_MASTER_SDA_IO           8       // GPIO8 for SDA
```

## License

This project is provided as-is for educational and development purposes.

---

**Note**: This project requires ESP-IDF v5.1+ for the new I2C master API. For older versions, the legacy I2C API would need to be used instead.
