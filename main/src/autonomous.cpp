#include "../include/autonomous.hpp"

#define TAG         "Autonomous"
#define PC_NAME     "Autonomous movement"
#define STACK_DEPTH 2048
#define UX_PRIORITY 5

TaskHandle_t autonomous_task_handler = NULL;

double look_and_get_val(std::function<void ()> servo_turn) {
    servo_turn();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    return get_obstacle_distance_cm();
}

double look_right(void) {
    return look_and_get_val(servo_turn_right);
}

double look_left(void) {
    return look_and_get_val(servo_turn_left);
}

double look_middle(void) {
    return look_and_get_val(servo_turn_middle);
}

void autonomous_movement_logic(void* param) {
    double right, left, middle, distance;

    while(1) {
        distance = look_middle();

        if (distance <= 9) {
            ESP_LOGW(TAG, "DISTANCE = %f", distance);
            Driver::stop();
            vTaskDelay(200 / portTICK_PERIOD_MS);
            Driver::backward();
            vTaskDelay(200 / portTICK_PERIOD_MS);
            Driver::stop();
            vTaskDelay(200 / portTICK_PERIOD_MS);
            left = look_left();
            vTaskDelay(200 / portTICK_PERIOD_MS);
            right = look_right();
            vTaskDelay(200 / portTICK_PERIOD_MS);

            if (left <= distance) {
                Driver::clockwise();
                vTaskDelay(300 / portTICK_PERIOD_MS);
                Driver::stop();
            }
            else {
                Driver::counterclockwise();
                vTaskDelay(300 / portTICK_PERIOD_MS);
                Driver::stop();
            }
        }
        else {
            ESP_LOGW(TAG, "GOING FORWARD");
            Driver::forward();
        }
        servo_turn_middle();
    }
}

void start_autonomous_movement(void) {
    esp_log_level_set("Driver", ESP_LOG_WARN);
    xTaskCreate(&autonomous_movement_logic, PC_NAME, STACK_DEPTH, NULL, UX_PRIORITY, &autonomous_task_handler);
}

void stop_autonomous_movement(void) {
    esp_log_level_set("Driver", ESP_LOG_INFO);
    servo_turn_middle();

    if (autonomous_task_handler != NULL) {
        vTaskDelete(autonomous_task_handler);
    }

}