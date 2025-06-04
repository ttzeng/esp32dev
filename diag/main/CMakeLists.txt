idf_component_register(SRCS diag.c scan.c
                       PRIV_REQUIRES esp_driver_uart FreeRTOS-Plus-CLI esp_wifi nvs_flash
                       INCLUDE_DIRS ".")
