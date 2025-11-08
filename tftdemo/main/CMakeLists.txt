idf_component_register(SRCS "tftdemo.cpp"
                            "../../codelets/Src/tft.cpp"
                            "../../codelets/Src/tft-st7735.cpp"
                            "../../codelets/Src/tft-st7735-supermini.cpp"
                       PRIV_REQUIRES esp_driver_spi esp_driver_gpio esp_driver_uart
                       INCLUDE_DIRS ../../codelets/Inc)
