idf_component_register(SRCS diag.c scan.c connect.c
                       PRIV_REQUIRES esp_driver_uart FreeRTOS-Plus-CLI esp_wifi esp_netif esp_event nvs_flash
                       INCLUDE_DIRS ".")
