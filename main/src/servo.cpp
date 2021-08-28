#include "../include/servo.hpp"

#define TAG                     "Servo"
#define SERVO_MIN_PULSEWIDTH_US (1000)
#define SERVO_MAX_PULSEWIDTH_US (2000)
#define SERVO_MAX_DEGREE        (90)
#define MOVEMENT_DELAY          pdMS_TO_TICKS(1000)
#define SET_WHEELS(val)         ESP_ERROR_CHECK(mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A,\
                                                convert_servo_angle_to_duty_us(val)))


void servo_turn_left(void) {
    SET_WHEELS(-SERVO_MAX_DEGREE);
}

void servo_turn_right(void) {
    SET_WHEELS(SERVO_MAX_DEGREE);
}

void servo_turn_middle(void) {
    SET_WHEELS(0);
}

void init_servo(void) {
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, SERVO_MOTOR_PIN);

    mcpwm_config_t pwm_config;
    pwm_config.frequency    = 50, // frequency = 50Hz, i.e. for every servo motor time period should be 20ms
    pwm_config.cmpr_a       = 0,     // duty cycle of PWMxA = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER,
    pwm_config.duty_mode    = MCPWM_DUTY_MODE_0,

    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);

    servo_turn_middle();
}


static inline uint32_t convert_servo_angle_to_duty_us(int angle)
{
    return (angle + SERVO_MAX_DEGREE) * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) / (2 * SERVO_MAX_DEGREE) + SERVO_MIN_PULSEWIDTH_US;
}
