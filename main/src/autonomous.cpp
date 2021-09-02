#include "../include/autonomous.hpp"

#define TAG         "Autonomous"
#define DRIVER_TAG  "Driver"
#define PC_NAME     "Autonomous movement"
#define WF_NAME     "Wall follow"
#define STACK_DEPTH 2048
#define UX_PRIORITY 5

TaskHandle_t autonomous_task_handler = NULL;
TaskHandle_t wall_follow_task_handler = NULL;

double look_and_get_val(std::function<void ()> servo_turn) {
    servo_turn();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    return get_obstacle_distance_cm();
}

double distance_right(void) {
    return look_and_get_val(servo_turn_right);
}

double distance_left(void) {
    return look_and_get_val(servo_turn_left);
}

double distance_middle(void) {
    return look_and_get_val(servo_turn_middle);
}

void autonomous_movement_logic(void* param) {
    double right, left, middle, distance;

    while(1) {
        distance = distance_middle();

        if (distance <= 25) {
            ESP_LOGW(TAG, "DISTANCE = %f", distance);
            Driver::stop();
            vTaskDelay(200 / portTICK_PERIOD_MS);
            Driver::backward();
            vTaskDelay(400 / portTICK_PERIOD_MS);
            Driver::stop();
            vTaskDelay(200 / portTICK_PERIOD_MS);
            left = distance_left();
            right = distance_right();

            if (left <= distance) {
                Driver::clockwise();
                vTaskDelay(500 / portTICK_PERIOD_MS);
                Driver::stop();
            }
            else {
                Driver::counterclockwise();
                vTaskDelay(500 / portTICK_PERIOD_MS);
                Driver::stop();
            }
        }
        else {
            Driver::forward();
        }
        servo_turn_middle();
    }
}

void wall_follow_logic(void * param) {
    const int distance = 30;
    int left = distance_left();
    int middle = distance_middle();
    int right = distance_right();
    bool wall_on_left = left <= distance;
    bool wall_on_right = right <= distance;
    bool wall_ahead = middle <= distance;
    servo_turn_middle();

    while(1) {

        if (wall_ahead) {
            Driver::stop();
            left = distance_left();
            right = distance_right();
            if (left < right) {
                wall_on_left = true;
                Driver::clockwise();
                vTaskDelay(500 / portTICK_PERIOD_MS);
                Driver::stop();
                wall_on_right = false;
            }
            else {
                wall_on_right = true;
                Driver::counterclockwise();
                vTaskDelay(500 / portTICK_PERIOD_MS);
                Driver::stop();
                wall_on_left = false;
            }
        }

        else if (!wall_on_left && !wall_on_right) {
            Driver::forward();
        }

        else if (wall_on_right) {
            if (distance_right() <= distance) {
                Driver::counterclockwise();
                vTaskDelay(200 / portTICK_PERIOD_MS);
                Driver::stop();
                vTaskDelay(200 / portTICK_PERIOD_MS);
                Driver::forward();
            }
            else {
                Driver::clockwise();
                vTaskDelay(200 / portTICK_PERIOD_MS);
                Driver::stop();
                vTaskDelay(200 / portTICK_PERIOD_MS);
                Driver::forward();
            }

        }
        else {
            if (distance_left() <= distance) {
                Driver::clockwise();
                vTaskDelay(200 / portTICK_PERIOD_MS);
                Driver::stop();
                vTaskDelay(200 / portTICK_PERIOD_MS);
                Driver::forward();
            }
            else {
                Driver::counterclockwise();
                vTaskDelay(200 / portTICK_PERIOD_MS);
                Driver::stop();
                vTaskDelay(200 / portTICK_PERIOD_MS);
                Driver::forward();
            }
        }
        wall_ahead = distance_middle() <= distance;
    }
}

void start_autonomous_movement(void) {
    stop_wall_follow();
    esp_log_level_set(DRIVER_TAG, ESP_LOG_WARN);
    xTaskCreate(&autonomous_movement_logic, PC_NAME, STACK_DEPTH, NULL, UX_PRIORITY, &autonomous_task_handler);
}

void stop_autonomous_movement(void) {
    esp_log_level_set(DRIVER_TAG, ESP_LOG_INFO);
    servo_turn_middle();

    if (autonomous_task_handler != NULL) {
        vTaskDelete(autonomous_task_handler);
        autonomous_task_handler = NULL;
        Driver::stop();
    }
}

void start_wall_follow(void) {
    stop_autonomous_movement();
    esp_log_level_set(DRIVER_TAG, ESP_LOG_WARN);
    xTaskCreate(&wall_follow_logic, WF_NAME, STACK_DEPTH, NULL, UX_PRIORITY, &wall_follow_task_handler);
}

void stop_wall_follow(void) {
    esp_log_level_set(DRIVER_TAG, ESP_LOG_INFO);
    servo_turn_middle();

    if (wall_follow_task_handler != NULL) {
        vTaskDelete(wall_follow_task_handler);
        wall_follow_task_handler = NULL;
        Driver::stop();
    }
}