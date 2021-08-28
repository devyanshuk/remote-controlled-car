#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <driver/mcpwm.h>

#include "constants.h"

static inline
uint32_t        convert_servo_angle_to_duty_us(int angle);

void            init_servo(void);
void            servo_turn_left(void);
void            servo_turn_right(void);
void            servo_turn_middle(void);