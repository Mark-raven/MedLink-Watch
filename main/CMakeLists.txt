idf_component_register(
    SRCS
        "hardware_config.c"
        "ui_manager.c"
        "display_task.c"
        "display_manager.c"
        "storage_manager.c"
        "motor_driver.c"
        "reminder_manager.c"
        "scheduler_task.c"
        "schedule_manager.c"
        "medlink_commands.c"
        "medlink_protocol.c"
        "medlink_service.c"
        "ble_server.c"
        "main.c"

    INCLUDE_DIRS "."

    REQUIRES ssd1306
             esp_driver_gpio
             esp_driver_i2c
             nvs_flash
             bt
)