#include "schedule_manager.h"

#include "esp_log.h"

#include <string.h>

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

    for (int i = 0; i < MAX_REMINDERS; i++)
    {
        if (reminders[i].enabled)
        {
            *reminder = reminders[i];

            ESP_LOGI(
                TAG,
                "Next Reminder: [%d] %s %02d:%02d",
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

void schedule_clear_all(void)
{
    memset(reminders, 0, sizeof(reminders));

    ESP_LOGI(
        TAG,
        "All reminders cleared"
    );
}