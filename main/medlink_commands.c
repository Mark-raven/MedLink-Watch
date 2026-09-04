#include "medlink_commands.h"
#include "medlink_service.h"

#include "schedule_manager.h"

#include <time.h>
#include <sys/time.h>

#include "esp_log.h"
#include <string.h>

static const char *TAG = "COMMANDS";

void medlink_cmd_ping(void)
{
    ESP_LOGI(TAG, "PING Command");

    medlink_notify("PONG");
}

void medlink_cmd_time_sync(uint8_t *packet,
                           uint16_t length)
{
    if (length < 8)
    {
        ESP_LOGE(TAG, "Invalid Time Sync Packet");
        return;
    }

    uint16_t year = (packet[1] << 8) | packet[2];

    uint8_t month  = packet[3];
    uint8_t day    = packet[4];
    uint8_t hour   = packet[5];
    uint8_t minute = packet[6];
    uint8_t second = packet[7];

    if (month < 1 || month > 12 ||
        day < 1 || day > 31 ||
        hour > 23 ||
        minute > 59 ||
        second > 59)
    {
        ESP_LOGE(TAG, "Invalid date/time received");
        return;
    }

    ESP_LOGI(TAG,
         "Received Time: %04d-%02d-%02d %02d:%02d:%02d",
         year,
         month,
         day,
         hour,
         minute,
         second);

    setenv("TZ", "IST-5:30", 1);
    tzset();

    struct tm tm_time = {0};

    tm_time.tm_year = year - 1900;
    tm_time.tm_mon  = month - 1;
    tm_time.tm_mday = day;
    tm_time.tm_hour = hour;
    tm_time.tm_min  = minute;
    tm_time.tm_sec  = second;


    time_t now = mktime(&tm_time);

    struct timeval tv =
    {
        .tv_sec = now,
        .tv_usec = 0,
    };

    if (settimeofday(&tv, NULL) != 0)
    {
        ESP_LOGE(TAG, "Failed to set system time");
        return;
    }

    time_t current;
    time(&current);

    ESP_LOGI(TAG,
             "Epoch = %lld",
             (long long)current);

    struct tm current_time;

    localtime_r(&current, &current_time);

    ESP_LOGI(TAG,
             "RTC Time %04d-%02d-%02d %02d:%02d:%02d",
             current_time.tm_year + 1900,
             current_time.tm_mon + 1,
             current_time.tm_mday,
             current_time.tm_hour,
             current_time.tm_min,
             current_time.tm_sec);

    medlink_notify("TIME OK");
}

void medlink_cmd_set_schedule(uint8_t *packet,
                              uint16_t length)
{
    if (length < 4)
    {
        ESP_LOGE(TAG, "Invalid Schedule Packet");
        return;
    }

    uint8_t hour = packet[1];
    uint8_t minute = packet[2];

    if (hour > 23 || minute > 59)
    {
        ESP_LOGE(TAG, "Invalid Schedule");
        return;
    }

    char medicine_name[32] = {0};

    uint16_t medicine_length = length - 3;

    if (medicine_length >= sizeof(medicine_name))
    {
        medicine_length = sizeof(medicine_name) - 1;
    }

    memcpy(
        medicine_name,
        &packet[3],
        medicine_length
    );

    medicine_name[medicine_length] = '\0';

    ESP_LOGI(TAG,
             "SET_SCHEDULE %02d:%02d - %s",
             hour,
             minute,
             medicine_name);

    schedule_set_reminder(
        hour,
        minute,
        medicine_name
    );

    medlink_notify("SCHEDULE OK");
}

void medlink_cmd_clear_schedule(void)
{
    ESP_LOGI(TAG, "CLEAR_SCHEDULE received");

    schedule_clear_all();

    medlink_notify("CLEAR OK");
}

void medlink_cmd_get_battery(void)
{
    ESP_LOGI(TAG, "BATTERY not implemented");
}

void medlink_cmd_acknowledge(void)
{
    ESP_LOGI(TAG, "Medicine Taken");

    medlink_notify("ACK OK");
}

void medlink_cmd_snooze(void)
{
    ESP_LOGI(TAG, "Reminder Snoozed");

    medlink_notify("SNOOZE OK");
}

void medlink_cmd_dismiss(void)
{
    ESP_LOGI(TAG, "Reminder Dismissed");

    medlink_notify("DISMISS OK");
}