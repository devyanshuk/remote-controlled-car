#pragma once

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "wifi.hpp"
#include "driver.hpp"
#include "driver/ledc.h"
#include "constants.h"
#include "errormacros.hpp"

#define C "C"

class ArduinoInterface final {

    public:

        ArduinoInterface                ();
        ArduinoInterface                (ArduinoInterface &&)       = delete;
        ArduinoInterface                (ArduinoInterface const &)  = delete;
        ArduinoInterface& operator =    (ArduinoInterface const &)  = delete;
        ArduinoInterface& operator =    (ArduinoInterface &&)       = delete;

        void setup (void);
        void loop  (void);
};