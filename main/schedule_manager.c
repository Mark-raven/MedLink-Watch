#include "schedule_manager.h"

#include "esp_log.h"

#include <string.h>

static const char *TAG = "SCHEDULE";

static reminder_t reminders[MAX_REMINDERS];

void schedule_manager_init(void)
{
    memset(reminders, 0, sizeof(reminders));

    // Temporary test reminder
    reminders[0].hour = 00;
    reminders[0].minute = 29;

    strcpy(reminders[0].medicine_name, "Vitamin D");

    reminders[0].enabled = true;

    ESP_LOGI(TAG,
             "Test Reminder Loaded: %s %02d:%02d",
             reminders[0].medicine_name,
             reminders[0].hour,
             reminders[0].minute);
}

bool schedule_get_next_reminder(reminder_t *reminder)
{
    if (reminder == NULL)
    {
        return false;
    }

    if (!reminders[0].enabled)
    {
        return false;
    }

    *reminder = reminders[0];

    return true;
}

void schedule_set_reminder(
    uint8_t hour,
    uint8_t minute,
    const char *medicine_name
)
{
    reminders[0].hour = hour;
    reminders[0].minute = minute;

    strncpy(
        reminders[0].medicine_name,
        medicine_name,
        sizeof(reminders[0].medicine_name) - 1
    );

    reminders[0].medicine_name[
        sizeof(reminders[0].medicine_name) - 1
    ] = '\0';

    reminders[0].enabled = true;

    ESP_LOGI(
        TAG,
        "Reminder Updated: %s %02d:%02d",
        reminders[0].medicine_name,
        reminders[0].hour,
        reminders[0].minute
    );
}