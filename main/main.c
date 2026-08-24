#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "scheduler_task.h"

#include "schedule_manager.h"

#include "reminder_manager.h"
#include "motor_driver.h"

#include "esp_log.h"

#include "ble_server.h"

#include "display_manager.h"
#include "display_task.h"

static const char *TAG = "MAIN";

void app_main(void)
{
    ESP_LOGI(TAG, "====================================");
    ESP_LOGI(TAG, "      MedLink Watch Started");
    ESP_LOGI(TAG, "====================================");

    ble_server_init();

    // Initialize hardware/display BEFORE using it
    display_manager_init();

    schedule_manager_init();

    reminder_manager_init();

    motor_init();

    display_show_startup();

    display_show_boot();

    motor_startup_vibration();

    vTaskDelay(pdMS_TO_TICKS(1000));

    motor_stop_vibration();

    vTaskDelay(pdMS_TO_TICKS(2000));

    scheduler_task_init();

    display_show_home();

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}