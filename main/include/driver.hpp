#pragma once

#include "esp_log.h"
#include "driver/gpio.h"

#include "errormacros.hpp"
#include "config/gpio_config.hpp"

class Driver final {

    private:

        struct MotorOutputValues {
            uint32_t motor1;
            uint32_t motor2;
            uint32_t motor3;
            uint32_t motor4;
        };

        static void set_motors(const MotorOutputValues &);

    public:
        static void stop();
        static void forward();
        static void backward();
        static void clockwise();
        static void counterclockwise();
};