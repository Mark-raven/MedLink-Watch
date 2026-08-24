#include "display_manager.h"
#include "scheduler_task.h"
#include "schedule_manager.h"
#include "hardware_config.h"

#include "ssd1306.h"

#include <time.h>
#include <stdio.h>

void display_clear(void)
{
    ssd1306_clear_screen();
}

bool display_manager_init(void)
{
    ESP_LOGI("DISPLAY", "Initializing OLED...");
    init_ssd1306();

    ESP_LOGI("DISPLAY", "OLED Initialized");
    display_clear();

    return true;
}

void display_show_startup(void)
{
    ssd1306_clear_screen();
    vTaskDelay(pdMS_TO_TICKS(100));

    ssd1306_clear_screen();
    vTaskDelay(pdMS_TO_TICKS(100));


    ssd1306_print_str(0, 0, "TEST", false);

    ssd1306_display();
}

void display_show_boot(void)
{
    ssd1306_clear_screen();

    ssd1306_print_str(0,0,"MedLink",false);
    ssd1306_print_str(0,16,"Initializing...",false);

    ssd1306_display();
}

void display_show_home(void)
{
    struct tm now;
    char time_str[16];

    reminder_t reminder;
    char reminder_time[16];

    if (scheduler_get_current_time(&now))
    {
        snprintf(time_str,
                 sizeof(time_str),
                 "%02d:%02d",
                 now.tm_hour,
                 now.tm_min);
    }
    else
    {
        snprintf(time_str, sizeof(time_str), "--:--");
    }

    ssd1306_clear_screen();

    ssd1306_print_str(0, 0, time_str, false);

    if (schedule_get_next_reminder(&reminder) && reminder.enabled)
    {
        snprintf(reminder_time,
                 sizeof(reminder_time),
                 "%02d:%02d",
                 reminder.hour,
                 reminder.minute);

        ssd1306_print_str(0, 8, "Next:", false);
        ssd1306_print_str(0,16, reminder.medicine_name, false);
        ssd1306_print_str(0,24, reminder_time, false);
    }
    else
    {
        ssd1306_print_str(0,16,"No reminders",false);
    }

    ssd1306_display();
}