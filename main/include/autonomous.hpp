#pragma once

#include <functional>

#include "servo.hpp"
#include "driver.hpp"
#include "ultrasonic.hpp"

extern  TaskHandle_t autonomous_task_handler;
double  look_and_get_val(std::function<void ()> servo_turn);
void    autonomous_movement_logic(void*);
void    start_autonomous_movement(void);
void    stop_autonomous_movement(void);