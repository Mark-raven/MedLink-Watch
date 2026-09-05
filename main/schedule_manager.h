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
    bool triggered;
} reminder_t;

void schedule_manager_init(void);

bool schedule_get_next_reminder(reminder_t *reminder);

bool schedule_get_reminder_at_time(
    uint8_t hour,
    uint8_t minute,
    reminder_t *reminder,
    int *matched_index
);

void schedule_set_reminder(
    uint8_t hour,
    uint8_t minute,
    const char *medicine_name
);

void schedule_mark_triggered(int index);

void schedule_reset_triggered(void);

void schedule_clear_all(void);

#endif