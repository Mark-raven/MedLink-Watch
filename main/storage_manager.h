#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

bool storage_manager_init(void);

bool storage_save_schedule(uint8_t hour,
                           uint8_t minute);

bool storage_load_schedule(uint8_t *hour,
                           uint8_t *minute);

#endif