#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include "scheduler.h"

bool display_manager_init(void);

void display_show_boot(void);
void display_show_startup(void);
void display_show_home(void);
//void display_show_reminder(const reminder_t *reminder);

void display_show_ble_connected(void);
void display_show_ble_disconnected(void);

#endif