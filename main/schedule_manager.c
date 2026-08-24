#include "schedule_manager.h"
#include "storage_manager.h"

#include "esp_log.h"
#include <string.h>


#define MAX_REMINDERS 10

static const char *TAG = "SCHEDULE";

static reminder_t reminders[MAX_REMINDERS];

typedef struct
{
    uint8_t hour;
    uint8_t minute;
    bool enabled;
} medicine_schedule_t;

static medicine_schedule_t schedule =
{
    .hour = 0,
    .minute = 0,
    .enabled = false
};

void schedule_manager_init(void)
{
    reminders[0].hour = 20;
    reminders[0].minute = 30;

    strcpy(reminders[0].medicine_name, "Vitamin D");

    reminders[0].enabled = true;
}


void schedule_set(uint8_t hour,
                  uint8_t minute)
{
    schedule.hour = hour;
    schedule.minute = minute;
    schedule.enabled = true;

    storage_save_schedule(hour, minute);

    ESP_LOGI(TAG,
             "Reminder Stored %02d:%02d",
             hour,
             minute);
}

bool schedule_get(uint8_t *hour,
                  uint8_t *minute)
{
    if (!schedule.enabled)
    {
        return false;
    }

    *hour = schedule.hour;
    *minute = schedule.minute;

    return true;
}

bool schedule_get_next_reminder(reminder_t *reminder)
{
    if (reminder == NULL)
    {
        return false;
    }

    *reminder = reminders[0];

    return true;
}