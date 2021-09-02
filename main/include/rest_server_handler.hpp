#pragma once

#include <string>
#include <sstream>
#include <esp_log.h>
#include <driver/ledc.h>
#include <esp_http_server.h>

#include "servo.hpp"
#include "driver.hpp"
#include "ultrasonic.hpp"
#include "autonomous.hpp"
#include "robot_config.hpp"
#include "config/spiffs_config.hpp"
#include "config/rest_server_config.hpp"


extern          robot_config_t              robot_config;
void            register_callbacks          (httpd_handle_t * server);
esp_err_t       root_handler                (httpd_req_t *);
esp_err_t       pwm_handler                 (httpd_req_t *);
esp_err_t       stop_handler                (httpd_req_t *);
esp_err_t       servo_handler               (httpd_req_t *);
esp_err_t       left_handler                (httpd_req_t *);
esp_err_t       right_handler               (httpd_req_t *);
esp_err_t       forward_handler             (httpd_req_t *);
esp_err_t       backward_handler            (httpd_req_t *);
esp_err_t       autodrive_handler           (httpd_req_t *);
esp_err_t       update_handler              (httpd_req_t *);
esp_err_t       wall_follow_handler         (httpd_req_t *);