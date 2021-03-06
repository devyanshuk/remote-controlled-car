#include "../include/ultrasonic.hpp"

#define TAG                 "Ultrasonic"
#define SOUND_SPEED         (340.3) //~340.3 m/s
#define MAX_LOOKOUT_TIME    (500*1000)

using namespace std::chrono;

uint64_t micros()
{
    return duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count(); 
}

double get_obstacle_distance_cm(void) {
        gpio_set_level(ULTRA_TRIG_PIN, 1);
        ets_delay_us(100);
        gpio_set_level(ULTRA_TRIG_PIN, 0);

        uint64_t startTime = micros();

        while (gpio_get_level(ULTRA_ECHO_PIN) == 0 && (micros() - startTime) < MAX_LOOKOUT_TIME);

        startTime = micros();

        while (gpio_get_level(ULTRA_ECHO_PIN) == 1 && (micros() - startTime) < MAX_LOOKOUT_TIME);

        if (gpio_get_level(ULTRA_ECHO_PIN) == 0) {
            uint64_t diff = micros() - startTime;
            // distance = ((time difference(in seconds) * sound speed) / 2) metres
            double distance_m = (SOUND_SPEED * (diff/1000000.0)) / 2.0;
            return distance_m * 100;
        }
        else {
            ESP_LOGW(TAG, "Did not receive a response!");
            return 0;
        }
}