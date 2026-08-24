#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

#include "driver/gpio.h"

/* ===========================
 * I2C Configuration
 * =========================== */

#define I2C_SDA_PIN        GPIO_NUM_8
#define I2C_SCL_PIN        GPIO_NUM_9

#define OLED_I2C_ADDRESS   0x3C

/* ===========================
 * Vibration Motor
 * =========================== */

#define MOTOR_GPIO         GPIO_NUM_3

/* ===========================
 * Future Expansion
 * =========================== */

#define BUTTON1_GPIO       GPIO_NUM_4
#define BUTTON2_GPIO       GPIO_NUM_5

#endif