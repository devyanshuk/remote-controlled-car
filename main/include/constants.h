#ifndef __RC_CAR_CONSTANTS__
#define __RC_CAR_CONSTANTS__

#include "driver/gpio.h"

// pins connected to L298N

constexpr gpio_num_t ENABLE_MOTOR_A = GPIO_NUM_32;

constexpr gpio_num_t MOTOR_1_PIN    = GPIO_NUM_33;

constexpr gpio_num_t MOTOR_2_PIN    = GPIO_NUM_25;

constexpr gpio_num_t MOTOR_3_PIN    = GPIO_NUM_26;

constexpr gpio_num_t MOTOR_4_PIN    = GPIO_NUM_27;

constexpr gpio_num_t ENABLE_MOTOR_B = GPIO_NUM_14;


// wifi credentials
#define WIFI_SSID "Remote-controlled-car"

// file partition root
#define SPIFFS_ROOT "/UI"


#endif
