#ifndef MEDLINK_PROTOCOL_H
#define MEDLINK_PROTOCOL_H

#include <stdint.h>

#define CMD_PING            0x01
#define CMD_TIME_SYNC       0x02
#define CMD_SET_SCHEDULE    0x03
#define CMD_ACKNOWLEDGE     0x04
#define CMD_SNOOZE          0x05
#define CMD_DISMISS         0x06
#define CMD_CLEAR_SCHEDULE  0x07

void medlink_process_packet(uint8_t *packet,
                            uint16_t length);

void medlink_cmd_set_schedule(uint8_t *packet,
                              uint16_t length);

void medlink_cmd_clear_schedule(void);

#endif