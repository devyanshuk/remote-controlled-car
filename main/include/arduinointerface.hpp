#pragma once

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/ledc.h>

#include "wifi.hpp"
#include "servo.hpp"
#include "driver.hpp"
#include "errormacros.hpp"
#include "config/gpio_config.hpp"
#include "config/dc_pwm_config.hpp"

#define C "C"

class ArduinoInterface final {

    public:

        ArduinoInterface              ();
        ArduinoInterface              (ArduinoInterface &&)       = delete;
        ArduinoInterface              (ArduinoInterface const &)  = delete;
        ArduinoInterface& operator =  (ArduinoInterface const &)  = delete;
		ArduinoInterface& operator =  (ArduinoInterface &&)       = delete;

        void setup (void);
        void loop  (void);
};