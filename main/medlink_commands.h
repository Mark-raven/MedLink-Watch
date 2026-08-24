#ifndef MEDLINK_COMMANDS_H
#define MEDLINK_COMMANDS_H

#include <stdint.h>

void medlink_cmd_ping(void);

void medlink_cmd_time_sync(uint8_t *packet,
                           uint16_t length);

void medlink_cmd_get_battery(void);

void medlink_cmd_acknowledge(void);

void medlink_cmd_snooze(void);

void medlink_cmd_dismiss(void);

#endif