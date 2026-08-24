#include "schedule_manager.h"

#include "esp_log.h"

#include <string.h>

static const char *TAG = "SCHEDULE";

static reminder_t reminders[MAX_REMINDERS];

void schedule_manager_init(void)
{
    memset(reminders, 0, sizeof(reminders));

    // Temporary test reminder
    reminders[0].hour = 20;
    reminders[0].minute = 30;

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