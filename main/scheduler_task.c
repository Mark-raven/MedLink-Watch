#include "scheduler_task.h"

#include "schedule_manager.h"
#include "reminder_manager.h"
#include "display_manager.h"

#include <time.h>

#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "SCHEDULER";

static struct tm current_time;

static int last_day = -1;


static void scheduler_task(void *arg)
{
    while (1)
    {
        time_t now;

        time(&now);
        localtime_r(&now, &current_time);

        /* =========================================================
         * DAILY RESET
        * ========================================================= */

        if (last_day == -1)
        {
            last_day = current_time.tm_yday;
        }
        else if (current_time.tm_yday != last_day)
        {
            ESP_LOGI(
                TAG,
                "New day detected - resetting reminder trigger states"
            );

            schedule_reset_triggered();

            last_day = current_time.tm_yday;
        }


        ESP_LOGI(
            TAG,
            "ESP32 Time: %02d:%02d:%02d",
            current_time.tm_hour,
            current_time.tm_min,
            current_time.tm_sec
        );

        reminder_t reminder;
        int matched_index = -1;

        /*
         * Check whether a reminder exists for the
         * current hour and minute.
         */
        if (schedule_get_reminder_at_time(
                current_time.tm_hour,
                current_time.tm_min,
                &reminder,
                &matched_index))
        {
            ESP_LOGI(
                TAG,
                "Checking Reminder: [%d] %s %02d:%02d",
                matched_index,
                reminder.medicine_name,
                reminder.hour,
                reminder.minute
            );

            /*
             * Check whether this reminder has already
             * been triggered.
             */
            if (!reminder.triggered)
            {
                schedule_mark_triggered(matched_index);

                ESP_LOGI(
                        TAG,
                        "Reminder Time Reached: %s",
                        reminder.medicine_name
                );

                reminder_start();
            }
        }

        display_show_home();

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void scheduler_task_init(void)
{
    ESP_LOGI(
        TAG,
        "Scheduler Task Started"
    );

    xTaskCreate(
        scheduler_task,
        "scheduler_task",
        4096,
        NULL,
        5,
        NULL
    );
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