#include "reminder_manager.h"
#include "motor_driver.h"
#include "medlink_service.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

static const char *TAG = "REMINDER";

void reminder_manager_init(void)
{
    ESP_LOGI(TAG, "Reminder Manager Initialized");
}

void reminder_start(void)
{
    ESP_LOGI(TAG, "Reminder Started");

    medlink_notify("REMINDER");

    motor_on();

    vTaskDelay(pdMS_TO_TICKS(3000));

    motor_off();

    ESP_LOGI(TAG, "Reminder Finished");
}