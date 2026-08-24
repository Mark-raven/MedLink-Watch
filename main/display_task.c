#include "display_task.h"
#include "display_manager.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static void display_task(void *arg)
{
    while (1)
    {
        display_show_home();

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void display_task_init(void)
{
    xTaskCreate(
        display_task,
        "display_task",
        4096,
        NULL,
        5,
        NULL);
}