#include "../include/driver.hpp"

#define LOG_TAG "Driver"

void Driver::set_motors(const MotorOutputValues & output) {
    gpio_set_level(MOTOR_1_PIN, output.motor1);
    gpio_set_level(MOTOR_2_PIN, output.motor2);
    gpio_set_level(MOTOR_3_PIN, output.motor3);
    gpio_set_level(MOTOR_4_PIN, output.motor4);
}

void Driver::stop() {
    ESP_LOGI(LOG_TAG, "Attempting to stop the motors");

    set_motors((MotorOutputValues)
                {
                    .motor1 = 0,
                    .motor2 = 0,
                    .motor3 = 0,
                    .motor4 = 0
                });
}

void Driver::forward(){
    ESP_LOGI(LOG_TAG, "Attempting to move forward");

    set_motors((MotorOutputValues)
                {
                    .motor1 = 0,
                    .motor2 = 1,
                    .motor3 = 1,
                    .motor4 = 0
                });
}

void Driver::backward() {
    ESP_LOGI(LOG_TAG, "Attempting to move backward");

    set_motors((MotorOutputValues)
                {
                    .motor1 = 1,
                    .motor2 = 0,
                    .motor3 = 0,
                    .motor4 = 1
                });
}

void Driver::clockwise() {
    ESP_LOGI(LOG_TAG, "Attempting to rotate clockwise");

    set_motors((MotorOutputValues)
                {
                    .motor1 = 0,
                    .motor2 = 1,
                    .motor3 = 0,
                    .motor4 = 0
                });
}

void Driver::counterclockwise() {
    ESP_LOGI(LOG_TAG, "Attempting to rotate counterclockwise");

    set_motors((MotorOutputValues)
                {
                    .motor1 = 0,
                    .motor2 = 0,
                    .motor3 = 1,
                    .motor4 = 0
                });
}