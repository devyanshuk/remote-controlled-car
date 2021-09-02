#pragma once

#include "config/servo_config.hpp"
#include "config/dc_pwm_config.hpp"


enum current_config_e {
    CONFIG_OBSTACLE_AVOIDANCE = 0,
    CONFIG_WALL_FOLLOW,
    CONFIG_MANUAL,
};

typedef struct robot_config {
    int current_duty_cycle = LEDC_DUTY;
    int current_servo_angle = INITIAL_SERVO_ANGLE;
    current_config_e config_type = CONFIG_MANUAL;
} robot_config_t;