#include "../include/ultrasonic.hpp"

#define TAG "Ultrasonic"

uint64_t micros()
{
    uint64_t us = std::chrono::duration_cast<std::chrono::microseconds>
                    (std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    return us; 
}

double get_obstacle_distance_cm(void) {
        gpio_set_level(ULTRA_TRIG_PIN, 1);
        ets_delay_us(100);
        gpio_set_level(ULTRA_TRIG_PIN, 0);

        uint64_t startTime = micros();

        while (gpio_get_level(ULTRA_ECHO_PIN) == 0 && (micros() - startTime) < 500 * 1000);

        startTime = micros();

        while (gpio_get_level(ULTRA_ECHO_PIN) == 1 && (micros() - startTime) < 500 * 1000);

        if (gpio_get_level(ULTRA_ECHO_PIN) == 0) {
            uint64_t diff = micros() - startTime;

            // Distance is TimeEchoInSeconds * SpeedOfSound / 2
            double distance = 340.29 * diff / (1000 * 1000 * 2);
            ESP_LOGI(TAG, "Distance is %f cm", distance * 100);
            return distance * 100;
        }
        else {

            ESP_LOGW(TAG, "Did not receive a response!");
            return 0;
        }
}