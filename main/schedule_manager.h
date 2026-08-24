#ifndef SCHEDULE_MANAGER_H
#define SCHEDULE_MANAGER_H

#include <stdbool.h>
#include <stdint.h>

#define MAX_REMINDERS 10

typedef struct
{
    char medicine_name[32];
    uint8_t hour;
    uint8_t minute;
    bool enabled;
} reminder_t;

void schedule_manager_init(void);

bool schedule_get_next_reminder(reminder_t *reminder);

#endif