# Overview

This is a complete ESP32-C3 IoT embedded project that demonstrates interfacing with a Grove LCD RGB display module. The project successfully displays "Hello World" on a 16x2 LCD with dynamic RGB backlight effects. Built using the ESP-IDF framework with a component-based architecture, it serves as a comprehensive example for embedded developers working with ESP32-C3 microcontrollers and Grove ecosystem components.

**✅ HARDWARE VALIDATION CONFIRMED**: Project successfully tested and working on real ESP32-C3 hardware with Grove LCD RGB module (August 18, 2025).

## Recent Changes (August 18, 2025)
- ✓ **CORRECTED ESP32-C3 GPIO PINS**: Updated I2C pins to SCL=GPIO9, SDA=GPIO8 for proper ESP32-C3 compatibility
- ✓ **UPGRADED TO NEW I2C API**: Migrated from legacy I2C API to ESP-IDF v5.1+ new I2C master API
- ✓ Added i2c_master_bus_handle_t and separate device handles for LCD/RGB controllers  
- ✓ Implemented grove_lcd_rgb_deinit() for proper I2C resource cleanup
- ✓ Updated all I2C operations to use i2c_master_transmit() instead of deprecated functions
- ✓ Enhanced error handling with new I2C API error reporting
- ✓ Fixed header includes (added stdint.h, stdbool.h for uint8_t and bool types)
- ✓ Corrected error handling in main.c to avoid return value issues in void functions
- ✓ Created comprehensive build validation system with syntax checking
- ✓ Updated README.md with new I2C API documentation and troubleshooting
- ✓ All C code syntax validation passed with new I2C API
- ✓ Validated modern I2C API usage and removed all deprecated API calls
- ✓ **FIXED NVS_FLASH DEPENDENCY**: Added nvs_flash component to main/CMakeLists.txt dependencies
- ✓ Build configuration now includes required nvs_flash component for ESP-IDF compilation
- ✅ **HARDWARE VALIDATION SUCCESS**: Project confirmed working on real ESP32-C3 with Grove LCD RGB module
- ✅ All technical implementation decisions validated through successful hardware deployment

# User Preferences

Preferred communication style: Simple, everyday language.

# System Architecture

## Embedded Framework Architecture
The project uses ESP-IDF (Espressif IoT Development Framework) as the core development platform. This provides:
- Hardware abstraction layer for ESP32 microcontroller
- FreeRTOS real-time operating system
- Driver libraries for peripheral management
- Build system integration with CMake

## Component-Based Design
The architecture follows ESP-IDF's component model with clear separation of concerns:
- **Main Application Component**: Contains the primary application logic in `main.c`
- **Grove LCD RGB Component**: Custom driver component for the Grove LCD RGB display module
- **Dependency Management**: Explicit component dependencies defined through CMake

## Hardware Integration Layer
The Grove LCD RGB component acts as a hardware abstraction layer that:
- Interfaces with the Grove LCD RGB display hardware using modern I2C master API
- Provides a simplified API for display operations with proper resource management
- Handles low-level I2C communication using separate device handles for LCD and RGB controllers
- Abstracts hardware-specific implementation details from the main application
- Implements proper I2C bus and device lifecycle management with init/deinit functions

## Build System Architecture
Uses CMake-based build configuration following ESP-IDF conventions:
- Project-level CMake configuration for overall build settings
- Component-level CMake files for modular compilation
- Automatic dependency resolution between components
- Integration with ESP-IDF toolchain and flash utilities

# External Dependencies

## ESP-IDF Framework
- **Espressif IoT Development Framework**: Core embedded development platform
- **FreeRTOS**: Real-time operating system provided by ESP-IDF
- **ESP32 HAL Drivers**: Hardware abstraction layer for ESP32 peripherals

## Hardware Components
- **Grove LCD RGB Display**: External display module using Grove connector interface
- **ESP32 Microcontroller**: Target hardware platform

## Build Tools
- **CMake**: Build system generator (minimum version 3.16 required)
- **ESP-IDF Toolchain**: Compiler and flashing tools for ESP32 development