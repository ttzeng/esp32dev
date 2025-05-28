# My ESP32 Projects

This repository contains source code and resources for my ESP32  projects. It also documents what I've learned while exploring the capabilities of ESP32 microcontrollers.

## Ask Perplexity

### Why designing with ESP32-C3 Super Mini chips instead of STM32F103 chips?

When choosing between the [ESP32-C3 Super Mini][1] and the [STM32F103][2] for new designs, several key technical and application-driven factors come into play, here’s a comprehensive comparison to highlight why designers might opt for the ESP32-C3 Super Mini over the STM32F103:
<sup>[[7],[8]]</sup>

|Feature|ESP32-C3 Super Mini|STM32F103|
|-------|:-----------------:|:-------:|
|Core Architecture|[RISC-V][3] 32-bit, single-core|ARM Cortex-M3 32-bit, single-core|
|Max Clock Frequency|160 MHz|72 MHz|
|Wireless Connectivity|Wi-Fi (802.11 b/g/n),<br>BLE 5.0|None|
|RAM|400 KB SRAM|64 KB SRAM|
|Flash Support|Up to 4 MB external|512 KB internal|
|GPIO|22|Up to 112|
|Security Features|AES, SHA, RSA, Secure Boot, Encryption|Basic, less advanced|
|Power Consumption|Optimized for low power, deep sleep|Low power, but fewer sleep modes|
|Package Size|Compact SMD/SMT modules|Larger LQFP-144|
|Typical Applications|IoT, wearables, gateways, smart home|Industrial, motor control, PLC|

#### Key Reasons to Choose ESP32-C3 Super Mini

1. Integrated Wireless Connectivity

    The ESP32-C3 Super Mini comes with built-in Wi-Fi and Bluetooth Low Energy (BLE 5.0), making it ideal for IoT and connected applications without the need for external modules.
    <sup>[[4],[5],[6]]</sup>

    The STM32F103 lacks any wireless capability, requiring additional hardware for connectivity.

2. Modern RISC-V Architecture

    ESP32-C3 uses a RISC-V core, which is open-source, modern, and gaining rapid industry adoption. This can future-proof designs and provide access to a growing ecosystem.

    STM32F103 is based on the older ARM Cortex-M3, which, while proven, is not as open or modern as RISC-V.

3. Enhanced Security

    ESP32-C3 features hardware-accelerated encryption, secure boot, and flash encryption, crucial for secure IoT deployments.

    STM32F103 offers basic security, but lacks advanced hardware-based security features.

4. Power Efficiency

    Designed for low-power applications, the ESP32-C3 supports deep sleep modes with ultra-low current draw, extending battery life in portable and remote devices.

    STM32F103 is efficient but does not match the ESP32-C3’s deep sleep capabilities.

5. Cost and Size

    ESP32-C3 Super Mini modules are compact and cost-effective, suitable for space-constrained and budget-sensitive projects.

    STM32F103, especially in LQFP-144, is physically larger and may be more expensive for designs that don’t need extensive I/O.

6. Sufficient Performance for IoT

    While not as powerful as dual-core ESP32 variants, the ESP32-C3’s 160 MHz clock and 400 KB SRAM are more than adequate for most IoT, sensor, and automation tasks.

    STM32F103 offers more I/O and timers, making it better for complex industrial control, but overkill for typical IoT needs.

#### When to Prefer STM32F103

If your application requires a large number of I/O pins (up to 112), advanced timers, or industrial interfaces like CAN, the STM32F103 is more suitable.

For legacy projects or where ARM Cortex-M3 compatibility is required, STM32F103 remains a strong choice.

#### Summary

Designers are increasingly choosing the ESP32-C3 Super Mini over the STM32F103 when:

* Wireless connectivity (Wi-Fi/BLE) is essential.

* Security, low power, and compact size are priorities.

* The project is IoT-focused, such as smart home devices, wearables, or gateways.

* Cost and ease of integration are important.

STM32F103 is still preferred for high I/O, industrial, or legacy control applications where wireless is not needed and robust peripheral support is required.

### Any free RTOS supports ESP32-C3 chips?

Yes, there are several free RTOS (Real-Time Operating System) options that support the ESP32-C3 chip:

#### FreeRTOS

* Official Support: ESP32-C3 is [officially supported][10] by FreeRTOS, and Espressif’s ESP-IDF (the official development framework for ESP chips) uses [a customized version of FreeRTOS][9] as its core RTOS. The port in ESP-IDF is stable for single-core operation (as required by ESP32-C3).

* Amazon FreeRTOS / Amazon SMP FreeRTOS: Amazon FreeRTOS (now called "FreeRTOS with AWS IoT integration") also supports the ESP32-C3, including reference integrations for IoT applications. However, the SMP (Symmetric Multiprocessing) features are mainly relevant for dual-core chips; the ESP32-C3 is single-core, so SMP options are not applicable here.

* Configuration: All FreeRTOS kernel configuration for ESP32-C3 is managed through the ESP-IDF’s menuconfig system, not by directly editing FreeRTOSConfig.h.

#### Other Free RTOS Options

* Zephyr: Zephyr RTOS is under active development for RISC-V and ESP32-C3, but support may not be as mature as FreeRTOS.

* NuttX: NuttX is another open-source RTOS that is being ported to ESP32-C3, though it is less commonly used than FreeRTOS for this chip.

#### Summary Table

|RTOS|ESP32-C3 Support|Notes|
|----|:--------------:|:---:|
|FreeRTOS|Yes (official)|Default in ESP-IDF; Amazon IoT support|
|Zephyr|In progress|Experimental, not as mature|
|NuttX|In progress|Experimental, less common|

#### Conclusion

FreeRTOS is the most widely used and officially supported free RTOS for the ESP32-C3, both via Espressif’s ESP-IDF and Amazon’s IoT integrations. Zephyr and NuttX are also being developed for ESP32-C3, but FreeRTOS remains the standard choice for most projects.

## Setting up ESP-IDF

ESP-IDF is the software libraries provided by Espressif to build applications for the ESP32 family, including the ESP32-C3 microcontroller. Clone the [ESP-IDF repository][16] and install the toolchain as follows 
<sup>[[14]]</sup> :

```
$ mkdir -p ~/esp
$ cd ~/esp
$ git clone --recursive https://github.com/espressif/esp-idf.git
$ cd ~/esp/esp-idf
$ ./install.sh esp32c3 
```
Before building a project, set up the environment variables with the `export.sh` script:

```
$ . $HOME/esp/esp-idf/export.sh
```

Navigate to the project folder, set the chip target for the build, optional run the project configuration utility `menuconfig`, and build the project with the following commands:

```
$ idf.py set-target esp32c3
$ idf.py menuconfig
$ idf.py build
```

On a successful build, flash the binaries to run the code, and monitor the output with the following commands:

```
$ idf.py -p <PORT> flash
$ idf.py -p <PORT> monitor
```

## ESP32-C3 Super Mini

<img src="https://michiel.vanderwulp.be/domotica/Modules/ESP32-C3-SuperMini/conns.jpg" width="500px"/>

![][18]

## Resources

* [Getting Started Guide][13]
* [API Reference][15]
* [Tutorials - Blinky with ESP-IDF in VS Code on ESP32-C3][11]
* [Blinky on the ESP32-C3 SuperMini board using Arduino IDE][17]
* [Configure and debug a ESP32-C3 code using Eclipse / command line][12]
* [ESP32-C3 series datasheet][20]

[1]: https://dl.artronshop.co.th/ESP32-C3%20SuperMini%20datasheet.pdf "Tutorial: ESP32-C3 Super Mini"
[2]: https://stm32-base.org/boards/STM32F103C8T6-Black-Pill.html "STM32-base: Black Pill"
[3]: https://www.synopsys.com/glossary/what-is-risc-v.html "Synopsys: What is RISC-V?"
[4]: https://openelab.io/blogs/learn/understanding-esp32-c3-and-esp32-s3 "Understanding ESP32-C3 and ESP32-S3: A Comprehensive Guide to Espressif’s IoT Powerhouses"
[5]: https://www.teachmemicro.com/esp32-c3-overview/ "ESP32-C3: IoT Microcontroller Overview"
[6]: https://done.land/components/microcontroller/families/esp/esp32/developmentboards/esp32-c3/c3supermini/ "ESP32-C3 Super Mini - Compact And Affordable Microcontroller Board With Great Functionality"
[7]: https://www.embedic.com/technology/details/esp32-vs-stm32--which-is-better-and-how-to-choose-2022 "ESP32 vs STM32, Which is Better and How to Choose 2023"
[8]: https://www.nevsemi.com/blog/esp32-vs-stm32 "ESP32 vs. STM32"
[9]: https://docs.espressif.com/projects/esp-idf/en/stable/esp32c3/api-reference/system/freertos.html "Espressif API Reference: FreeRTOS Overview"
[10]: https://freertos.org/Documentation/03-Libraries/08-Featured-integrations/02-Espressif-ESP32c3 "Featured FreeRTOS IoT Integration"
[11]: https://hutscape.com/tutorials/blinky-esp-idf-esp32c3 "Tutorials - Blinky with ESP-IDF in VS Code on ESP32-C3 - Hutscape"
[12]: https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-guides/jtag-debugging/using-debugger.html "Using Debugger"
[13]: https://docs.espressif.com/projects/esp-idf/en/stable/esp32c3/get-started/index.html "ESP32-C3 Getting Started Guide"
[14]: https://docs.espressif.com/projects/esp-idf/en/stable/esp32c3/get-started/linux-macos-setup.html "ESP-IDF Toolchain Setup"
[15]: https://docs.espressif.com/projects/esp-idf/en/stable/esp32c3/api-reference/index.html "ESP32-C3 API Reference"
[16]: https://github.com/espressif/esp-idf "Espressif IoT Development Framework Repository"
[17]: https://michiel.vanderwulp.be/post-esp32c3supermini-blink-arduino-ide.html "ESP32-C3 SuperMini - Blink - Arduino"
[18]: https://michiel.vanderwulp.be/domotica/Modules/ESP32-C3-SuperMini/schema-C3.jpg "ESP32-C3 Super Mini Schematics"
[19]: https://michiel.vanderwulp.be/domotica/Modules/ESP32-C3-SuperMini/conns.jpg "ESP32-C3 Super Mini Connections"
[20]: https://michiel.vanderwulp.be/domotica/Modules/ESP32-C3-SuperMini/esp32-c3_datasheet_en.pdf "ESP32-C3 Super Mini Datasheet"
