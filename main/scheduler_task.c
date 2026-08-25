#include "scheduler_task.h"
#include "schedule_manager.h"
#include "reminder_manager.h"
#include "display_manager.h"

#include <time.h>
#include <stdbool.h>

#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "SCHEDULER";

static bool reminder_triggered = false;
static struct tm current_time;

static void scheduler_task(void *arg)
{
    while (1)
    {
        time_t now;
        time(&now);

        localtime_r(&now, &current_time);

        ESP_LOGI(TAG,
                 "ESP32 Time: %02d:%02d:%02d",
                 current_time.tm_hour,
                 current_time.tm_min,
                 current_time.tm_sec);

        reminder_t reminder;

        if (schedule_get_next_reminder(&reminder))
        {
            ESP_LOGI(TAG,
                     "Checking Reminder: %s %02d:%02d",
                     reminder.medicine_name,
                     reminder.hour,
                     reminder.minute);

            if (current_time.tm_hour == reminder.hour &&
                current_time.tm_min == reminder.minute)
            {
                if (!reminder_triggered)
                {
                    reminder_triggered = true;

                    ESP_LOGI(TAG,
                             "Reminder Time Reached: %s",
                             reminder.medicine_name);

                    reminder_start();
                }
            }
            else
            {
                reminder_triggered = false;
            }
        }

        display_show_home();

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void scheduler_task_init(void)
{
    ESP_LOGI(TAG, "Scheduler Task Started");

    xTaskCreate(
        scheduler_task,
        "scheduler_task",
        4096,
        NULL,
        5,
        NULL);
}

bool scheduler_get_current_time(struct tm *time)
{
    if (time == NULL)
    {
        return false;
    }

    *time = current_time;

    return true;
}