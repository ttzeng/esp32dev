idf_component_register(SRCS blinky.cpp ../../codelets/Src/led-supermini.cpp
                       PRIV_REQUIRES esp_driver_gpio esp_driver_uart
                       INCLUDE_DIRS ../../codelets/Inc)
