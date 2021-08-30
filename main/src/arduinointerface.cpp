#include "../include/arduinointerface.hpp"

#define AI_LOG_TAG "ArduinoInterface"

ArduinoInterface::ArduinoInterface(){}

void ArduinoInterface::setup(void) {
    
    ESP_LOGI(AI_LOG_TAG, "function setup called");

    gpio_pad_select_gpio(ENABLE_MOTOR_A);
    ESP_ERROR_CHECK(gpio_set_direction(ENABLE_MOTOR_A, GPIO_MODE_OUTPUT));

    gpio_pad_select_gpio(ENABLE_MOTOR_B);
    ESP_ERROR_CHECK(gpio_set_direction(ENABLE_MOTOR_B, GPIO_MODE_OUTPUT));

    gpio_pad_select_gpio(MOTOR_1_PIN);
    ESP_ERROR_CHECK(gpio_set_direction(MOTOR_1_PIN, GPIO_MODE_OUTPUT));

    gpio_pad_select_gpio(MOTOR_2_PIN);
    ESP_ERROR_CHECK(gpio_set_direction(MOTOR_2_PIN, GPIO_MODE_OUTPUT));

    gpio_pad_select_gpio(MOTOR_3_PIN);
    ESP_ERROR_CHECK(gpio_set_direction(MOTOR_3_PIN, GPIO_MODE_OUTPUT));

    gpio_pad_select_gpio(MOTOR_4_PIN);
    ESP_ERROR_CHECK(gpio_set_direction(MOTOR_4_PIN, GPIO_MODE_OUTPUT));

    gpio_pad_select_gpio(ULTRA_ECHO_PIN);
    ESP_ERROR_CHECK(gpio_set_direction(ULTRA_ECHO_PIN, GPIO_MODE_INPUT));

    gpio_pad_select_gpio(ULTRA_TRIG_PIN);
    ESP_ERROR_CHECK(gpio_set_direction(ULTRA_TRIG_PIN, GPIO_MODE_OUTPUT));

    ESP_LOGI(AI_LOG_TAG, "All motor pins set successfully");

    ledc_timer_config_t ledc_timer;
    ledc_timer.speed_mode           = LEDC_MODE;
    ledc_timer.timer_num            = LEDC_TIMER;
    ledc_timer.duty_resolution      = LEDC_DUTY_RES;
    ledc_timer.freq_hz              = LEDC_FREQUENCY;
    ledc_timer.clk_cfg              = LEDC_AUTO_CLK;

    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t ena;
    ena.speed_mode     = LEDC_MODE;
    ena.channel        = LEDC_CHANNEL;
    ena.timer_sel      = LEDC_TIMER;
    ena.intr_type      = LEDC_INTR_DISABLE;
    ena.gpio_num       = ENABLE_MOTOR_A;
    ena.duty           = 0;
    ena.hpoint         = 0;

    ESP_ERROR_CHECK(ledc_channel_config(&ena));

    ledc_channel_config_t enb;
    enb.speed_mode     = LEDC_MODE;
    enb.channel        = LEDC_CHANNEL;
    enb.timer_sel      = LEDC_TIMER;
    enb.intr_type      = LEDC_INTR_DISABLE;
    enb.gpio_num       = ENABLE_MOTOR_B;
    enb.duty           = 0;
    enb.hpoint         = 0;

    ESP_ERROR_CHECK(ledc_channel_config(&ena));
    ESP_ERROR_CHECK(ledc_channel_config(&enb));
    

    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, LEDC_DUTY));
    ESP_LOGI(AI_LOG_TAG, "Enabled PWM on Enable A and B pins with duty cycle %d percentage", 100);

    init_servo();

    connect_wifi();

}

void ArduinoInterface::loop(void){

    ESP_LOGI(AI_LOG_TAG, "function loop called");
}