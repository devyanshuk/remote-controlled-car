#include "../include/rest_server_handler.hpp"

#define TAG "Rest server handler"

#define SEND_OK(req) do { \
    std::stringstream message; \
    message << "{\"" << req->uri << "\": \"completed\"}"; \
    std::string m = message.str(); \
    httpd_resp_send(req, m.c_str(), m.length()); \
    return ESP_OK; \
} while(0) \

#define CHECK_FILE_EXTENSION(str, ext) (str.substr(str.find_last_of(".") + 1) == ext)

esp_err_t   set_content_type_from_file  (httpd_req_t *req, std::string filepath);
int         get_integer_value_from_uri  (const char* uri);

robot_config_t robot_config;

template<typename T>
std::string toString(T val)
{
    std::stringstream ss;
    ss << val;
    return ss.str();
}

void register_callbacks(httpd_handle_t * server) {

    httpd_uri_t root =
        {
            .uri = ROOT_URI,
            .method = HTTP_GET,
            .handler = root_handler,
            .user_ctx = NULL_CTX
        };

    httpd_uri_t pwm =
        {
            .uri = PWM_URI,
            .method = HTTP_POST,
            .handler = pwm_handler,
            .user_ctx = NULL_CTX
        };


    httpd_uri_t straight = 
        {
            .uri = FORWARD_URI,
            .method = HTTP_POST,
            .handler = forward_handler,
            .user_ctx = NULL_CTX
        };

    httpd_uri_t back = 
        {
            .uri = BACKWARD_URI,
            .method = HTTP_POST,
            .handler = backward_handler,
            .user_ctx = NULL_CTX
        };

    httpd_uri_t left = 
        {
            .uri = COUNTERCLOCKWISE_URI,
            .method = HTTP_POST,
            .handler = left_handler,
            .user_ctx = NULL_CTX
        };

    httpd_uri_t right =
        {
            .uri = CLOCKWISE_URI,
            .method = HTTP_POST,
            .handler = right_handler,
            .user_ctx = NULL_CTX
        };

    httpd_uri_t stop =
        {
            .uri = STOP_URI,
            .method = HTTP_POST,
            .handler = stop_handler,
            .user_ctx = NULL_CTX
        };

    httpd_uri_t autodrive =
        {
            .uri = AUTO_DRIVE_URI,
            .method = HTTP_POST,
            .handler = autodrive_handler,
            .user_ctx = NULL_CTX
        };

    httpd_uri_t wall_follow =
        {
            .uri = WALL_FOLLOW_URI,
            .method = HTTP_POST,
            .handler = wall_follow_handler,
            .user_ctx = NULL_CTX
        };

    httpd_uri_t servo =
        {
            .uri = SERVO_URI,
            .method = HTTP_POST,
            .handler = servo_handler,
            .user_ctx = NULL_CTX
        };

    httpd_register_uri_handler(*server, &root);
    ESP_LOGI(TAG, "Registered %s uri handler", ROOT_URI);
    httpd_register_uri_handler(*server, &pwm);
    ESP_LOGI(TAG, "Registered %s uri handler", PWM_URI);
    httpd_register_uri_handler(*server, &straight);
    ESP_LOGI(TAG, "Registered %s uri handler", FORWARD_URI);
    httpd_register_uri_handler(*server, &back);
    ESP_LOGI(TAG, "Registered %s uri handler", BACKWARD_URI);
    httpd_register_uri_handler(*server, &left);
    ESP_LOGI(TAG, "Registered %s uri handler", COUNTERCLOCKWISE_URI);
    httpd_register_uri_handler(*server, &right);
    ESP_LOGI(TAG, "Registered %s uri handler", CLOCKWISE_URI);
    httpd_register_uri_handler(*server, &stop);
    ESP_LOGI(TAG, "Registered %s uri handler", STOP_URI);
    httpd_register_uri_handler(*server, &autodrive);
    ESP_LOGI(TAG, "Registered %s uri handler", AUTO_DRIVE_URI);
    httpd_register_uri_handler(*server, &servo);
    ESP_LOGI(TAG, "Registered %s uri handler", SERVO_URI);
    httpd_register_uri_handler(*server, &wall_follow);
    ESP_LOGI(TAG, "Registered %s uri handler", WALL_FOLLOW_URI);
}

void set_config_to_manual(robot_config_t * config) {
    config->config_type = CONFIG_MANUAL;
    stop_autonomous_movement();
    stop_wall_follow();
}

esp_err_t right_handler(httpd_req_t *req) {
    set_config_to_manual(&robot_config);
    Driver::clockwise();
    SEND_OK(req);
}

esp_err_t left_handler(httpd_req_t *req) {
    set_config_to_manual(&robot_config);
    Driver::counterclockwise();
    SEND_OK(req);
}

esp_err_t stop_handler(httpd_req_t *req) {
    set_config_to_manual(&robot_config);
    Driver::stop();
    SEND_OK(req);
}

esp_err_t forward_handler(httpd_req_t *req) {
    set_config_to_manual(&robot_config);
    Driver::forward();
    SEND_OK(req);
}

esp_err_t backward_handler(httpd_req_t *req) {
    set_config_to_manual(&robot_config);
    Driver::backward();
    SEND_OK(req);
}

esp_err_t set_content_type_from_file(httpd_req_t *req, std::string filepath)
{
    const char *type = TEXT_PLAIN;
    if (CHECK_FILE_EXTENSION(filepath, HTML)) {
        type = TEXT_HTML;
    }
    else if (CHECK_FILE_EXTENSION(filepath, JS)) {
        type = APPLICATION_JAVASCRIPT;
    }
    else if (CHECK_FILE_EXTENSION(filepath, CSS)) {
        type = TEXT_CSS;
    }
    ESP_LOGI(TAG, "type %s returned", type);

    return httpd_resp_set_type(req, type);
}

int get_integer_value_from_uri(const char* uri) {
    std::string str(uri);
    size_t i = 0;
    
    // Parse: /String/integer -> integer
    for ( ; i < str.length() && (str[i] != '-' && !isdigit(str[i])); i++ );
    str = str.substr(i, str.length() - i );
    ESP_LOGI(TAG, "str value = %s", str.c_str());
    return atoi(str.c_str());
}


esp_err_t pwm_handler(httpd_req_t * req) {
    ESP_LOGI(TAG, "uri %s was hit", req->uri);

    int pwm_val = get_integer_value_from_uri(req->uri);

    ESP_LOGI(TAG, "got pwm value : %d", pwm_val);

    // set new duty cycle
    float duty_cycle_percentage = (pwm_val / 255.0) * 100.0;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, pwm_val);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

    robot_config.current_duty_cycle = pwm_val;

    ESP_LOGI(TAG, "changed duty cycle to %.2f percent", duty_cycle_percentage );

    SEND_OK(req);
}

esp_err_t servo_handler(httpd_req_t * req) {
    ESP_LOGI(TAG, "uri %s was hit", req->uri);

    int servo_val = get_integer_value_from_uri(req->uri);

    ESP_LOGI(TAG, "got servo value : %d", servo_val);

    custom_turn(servo_val);

    robot_config.current_servo_angle = servo_val;

    SEND_OK(req);
}


esp_err_t root_handler(httpd_req_t *req)
{
    if (strcmp(req->uri, UPDATE_URI) == 0) {
        return update_handler(req);
    }

    ESP_LOGI(TAG, "uri %s was hit", req->uri);

    std::string file_path(SPIFFS_ROOT);

    ESP_LOGI(TAG, "Base path = %s", file_path.c_str());

    if (req->uri[strlen(req->uri) - 1] == '/') {
        file_path += INDEX_HTML;
    } else {
        file_path += req->uri;
    }

    ESP_LOGI(TAG, "file path = %s", file_path.c_str());

    set_content_type_from_file(req, file_path);

    char lineRead[FILE_BUF_SIZE];
    
    FILE *file = fopen(file_path.c_str(), "r");
    while (fgets(lineRead, sizeof(lineRead), file))
    {
       httpd_resp_sendstr_chunk(req, lineRead);
    }

    httpd_resp_sendstr_chunk(req, NULL);

    fclose(file);
    return ESP_OK;
}


esp_err_t update_handler(httpd_req_t * req) {
    bool auto_drive = robot_config.config_type == CONFIG_OBSTACLE_AVOIDANCE;
    bool wall_follow = robot_config.config_type == CONFIG_WALL_FOLLOW;
    std::stringstream outputJson;
    outputJson  << "{\"auto_drive\":\"" << toString<bool>(auto_drive) << "\","
                << "\"wall_follow\":\"" << toString<bool>(wall_follow) << "\","
                << "\"distance\":\"" << toString<double>(get_obstacle_distance_cm()) << "\"}";

    httpd_resp_sendstr_chunk(req, outputJson.str().c_str());
    httpd_resp_sendstr_chunk(req, NULL);
    return ESP_OK;
}

esp_err_t autodrive_handler(httpd_req_t * req) {
    ESP_LOGI(TAG, "uri %s was hit", req->uri);

    //Parse /Autonomous/bool -> bool
    char *ptr = strrchr(req->uri, '/');
    if (strcmp(ptr + 1, "true") == 0) {
        robot_config.config_type = CONFIG_OBSTACLE_AVOIDANCE;
        start_autonomous_movement();
    }
    else {
        robot_config.config_type = CONFIG_MANUAL;
        stop_autonomous_movement();
    }

    SEND_OK(req);
}

esp_err_t wall_follow_handler(httpd_req_t * req) {
    ESP_LOGI(TAG, "uri %s was hit", req->uri);

    //Parse /Autonomous/bool -> bool
    char *ptr = strrchr(req->uri, '/');
    if (strcmp(ptr + 1, "true") == 0) {
        robot_config.config_type = CONFIG_WALL_FOLLOW;
        start_wall_follow();
    }
    else {
        robot_config.config_type = CONFIG_MANUAL;
        stop_wall_follow();
    }

    SEND_OK(req);
}
