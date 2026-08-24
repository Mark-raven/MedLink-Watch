#include "motor_driver.h"
#include "hardware_config.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#define MOTOR_GPIO GPIO_NUM_3     // Change to your motor GPIO

static const char *TAG = "MOTOR";

void motor_init(void)
{
    gpio_config_t io_conf = {
    .mode = GPIO_MODE_OUTPUT,
    .pin_bit_mask = (1ULL << MOTOR_GPIO),
    };

    gpio_config(&io_conf);

    gpio_set_level(MOTOR_GPIO, 0);

    ESP_LOGI(TAG, "Motor Driver Initialized");
}

void motor_on(void)
{
    gpio_set_level(MOTOR_GPIO, 1);

    ESP_LOGI(TAG, "Motor ON");
}

void motor_off(void)
{
    gpio_set_level(MOTOR_GPIO, 0);

    ESP_LOGI(TAG, "Motor OFF");
}

void motor_startup_vibration(void)
{
    motor_on();
}

void motor_stop_vibration(void)
{
    motor_off();
}