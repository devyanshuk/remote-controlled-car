#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <driver/mcpwm.h>

#include "config/gpio_config.hpp"
#include "config/servo_config.hpp"

extern int current_servo_pos;

static inline
uint32_t        convert_servo_angle_to_duty_us  (int angle);
void            init_servo                      (void);
void            servo_turn_left                 (void);
void            servo_turn_right                (void);
void            servo_turn_middle               (void);
void            custom_turn                     (int angle);