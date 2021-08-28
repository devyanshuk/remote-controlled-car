#ifndef __RC_CAR_CONSTANTS__
#define __RC_CAR_CONSTANTS__

#include "driver/gpio.h"

// pins connected to L298N

constexpr gpio_num_t ENABLE_MOTOR_A     = GPIO_NUM_14;

constexpr gpio_num_t MOTOR_1_PIN        = GPIO_NUM_27;

constexpr gpio_num_t MOTOR_2_PIN        = GPIO_NUM_26;

constexpr gpio_num_t MOTOR_3_PIN        = GPIO_NUM_25;

constexpr gpio_num_t MOTOR_4_PIN        = GPIO_NUM_33;

constexpr gpio_num_t ENABLE_MOTOR_B     = GPIO_NUM_32;

// pins connected to the servo motor

/*
* The servo motor is powered up using the +5V output pin
* of the L298N motor driver.
*/
constexpr gpio_num_t SERVO_MOTOR_PIN    = GPIO_NUM_4;


// pins connected to the ultrasonic sensor

/*
* The ultrasonic sensor is powered up using the 5V pin of the
* esp32. The echo pin's response is a 5V signal, but the esp32
* can only take a maximum of 3.3V. So, we use voltage divider to
* reduce the 5V to 3.3V.

    [Echo pin] 5V--------
                        |
                        |
                        [1KΩ]
                        |
                        |-----------> 3.3V      5V * (2KΩ / (1KΩ + 2KΩ)) = 5V * (2/3) ~ 3.3V
                        |
                        [2KΩ]
                        |___________> GND

*/
constexpr gpio_num_t ULTRA_ECHO_PIN = GPIO_NUM_13;

constexpr gpio_num_t ULTRA_TRIG_PIN = GPIO_NUM_12; 



// wifi credentials
#define WIFI_SSID "Remote-controlled-car"

// file partition root
#define SPIFFS_ROOT "/UI"


#endif
