#ifndef SCHEDULER_TASK_H
#define SCHEDULER_TASK_H

#include <stdbool.h>
#include <time.h>    

void scheduler_task_init(void);

bool scheduler_get_current_time(struct tm *time);

#endif