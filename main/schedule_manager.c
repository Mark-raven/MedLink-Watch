#include "schedule_manager.h"

#include "esp_log.h"

#include <string.h>
#include <time.h>

static const char *TAG = "SCHEDULE";

static reminder_t reminders[MAX_REMINDERS];

void schedule_manager_init(void)
{
    memset(reminders, 0, sizeof(reminders));

    ESP_LOGI(TAG, "Schedule Manager Initialized");
}

bool schedule_get_next_reminder(reminder_t *reminder)
{
    if (reminder == NULL)
    {
        return false;
    }

    time_t now;
    struct tm current_time;

    time(&now);
    localtime_r(&now, &current_time);

    int current_minutes =
        current_time.tm_hour * 60 +
        current_time.tm_min;

    int best_index = -1;
    int best_difference = 24 * 60 + 1;

    for (int i = 0; i < MAX_REMINDERS; i++)
    {
        if (!reminders[i].enabled)
        {
            continue;
        }

        int reminder_minutes =
            reminders[i].hour * 60 +
            reminders[i].minute;

        int difference =
            reminder_minutes - current_minutes;

        /*
         * Ignore reminders that have already passed today.
         */
        if (difference < 0)
        {
            continue;
        }

        /*
         * Find the closest upcoming reminder.
         */
        if (difference < best_difference)
        {
            best_difference = difference;
            best_index = i;
        }
    }

    /*
     * No upcoming reminder found.
     */
    if (best_index == -1)
    {
        ESP_LOGI(
            TAG,
            "No upcoming reminders"
        );

        return false;
    }

    *reminder = reminders[best_index];

    ESP_LOGI(
        TAG,
        "Next Reminder: [%d] %s %02d:%02d",
        best_index,
        reminder->medicine_name,
        reminder->hour,
        reminder->minute
    );

    return true;
}

void schedule_set_reminder(
    uint8_t hour,
    uint8_t minute,
    const char *medicine_name
)
{
    if (medicine_name == NULL)
    {
        ESP_LOGE(TAG, "Medicine name is NULL");
        return;
    }

    for (int i = 0; i < MAX_REMINDERS; i++)
    {
        if (!reminders[i].enabled)
        {
            reminders[i].hour = hour;
            reminders[i].minute = minute;

            strncpy(
                reminders[i].medicine_name,
                medicine_name,
                sizeof(reminders[i].medicine_name) - 1
            );

            reminders[i].medicine_name[
                sizeof(reminders[i].medicine_name) - 1
            ] = '\0';

            reminders[i].enabled = true;

            ESP_LOGI(
                TAG,
                "Reminder Stored: [%d] %s %02d:%02d",
                i,
                reminders[i].medicine_name,
                reminders[i].hour,
                reminders[i].minute
            );

            return;
        }
    }

    ESP_LOGE(
        TAG,
        "No free reminder slots available"
    );
}

bool schedule_get_reminder_at_time(
    uint8_t hour,
    uint8_t minute,
    reminder_t *reminder,
    int *matched_index
)
{
    if (reminder == NULL)
    {
        return false;
    }

    for (int i = 0; i < MAX_REMINDERS; i++)
    {
        if (reminders[i].enabled &&
            reminders[i].hour == hour &&
            reminders[i].minute == minute)
        {
            *reminder = reminders[i];

            if (matched_index != NULL)
            {
                *matched_index = i;
            }

            ESP_LOGI(
                TAG,
                "Matching Reminder: [%d] %s %02d:%02d",
                i,
                reminder->medicine_name,
                reminder->hour,
                reminder->minute
            );

            return true;
        }
    }

    return false;
}


void schedule_clear_all(void)
{
    memset(reminders, 0, sizeof(reminders));

    ESP_LOGI(
        TAG,
        "All reminders cleared"
    );
}