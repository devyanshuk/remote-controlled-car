#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_err.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include <chrono>

#include "constants.h"

uint64_t    micros();
double      get_obstacle_distance_cm(void);