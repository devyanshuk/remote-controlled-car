#pragma once

#include "config/dc_pwm_config.hpp"

enum current_config_e {
    CONFIG_AUTONOMOUS = 0,
    CONFIG_WALL_FOLLOW,
    CONFIG_MANUAL,
    CONFIG_ACCELEROMETER
};

typedef struct robot_config {
    int current_duty_cycle = LEDC_DUTY;
    current_config_e config_type;
} robot_config_t;