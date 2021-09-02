#pragma once

#include <driver/ledc.h>

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES_INT       8
#define LEDC_DUTY_RES           (static_cast< ledc_timer_bit_t >(LEDC_DUTY_RES_INT))
#define LEDC_DUTY               ((1 << LEDC_DUTY_RES_INT) - 1)
#define LEDC_FREQUENCY          (20000)