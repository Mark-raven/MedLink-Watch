#include "medlink_protocol.h"
#include "medlink_commands.h"

#include "esp_log.h"

static const char *TAG = "PROTOCOL";

void medlink_process_packet(uint8_t *packet,
                            uint16_t length)
{
    if (length == 0)
    {
        return;
    }

    switch (packet[0])
    {
        case CMD_PING:
            medlink_cmd_ping();
            break;

        case CMD_TIME_SYNC:
            medlink_cmd_time_sync(packet, length);
            break;
        
        case CMD_SET_SCHEDULE:
            medlink_cmd_set_schedule(packet, length);
            break;

        case CMD_ACKNOWLEDGE:
            medlink_cmd_acknowledge();
            break;

        case CMD_SNOOZE:
            medlink_cmd_snooze();
            break;

        case CMD_DISMISS:
            medlink_cmd_dismiss();
            break;

        default:
            ESP_LOGW(TAG, "Unknown command: 0x%02X", packet[0]);
            break;
    }
}