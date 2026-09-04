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

/*
 * Stores the index of the reminder that was already triggered.
 *
 * Example:
 *   0 = Vigra already triggered
 *   1 = Para already triggered
 *   2 = hitty already triggered
 *
 * -1 means no reminder has been triggered yet.
 */
static int last_triggered_reminder = -1;

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
            ESP_LOGI(TAG,
                     "Checking Reminder: [%d] %s %02d:%02d",
                     matched_index,
                     reminder.medicine_name,
                     reminder.hour,
                     reminder.minute);

            /*
             * Trigger only if this particular reminder
             * has not already been triggered.
             */
            if (matched_index != last_triggered_reminder)
            {
                last_triggered_reminder = matched_index;

                ESP_LOGI(TAG,
                         "Reminder Time Reached: %s",
                         reminder.medicine_name);

                reminder_start();
            }
        }
        else
        {
            /*
             * No reminder matches the current time.
             *
             * Reset the state so the next reminder can
             * trigger normally.
             */
            last_triggered_reminder = -1;
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