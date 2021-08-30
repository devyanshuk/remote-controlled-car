#include "../include/rest_server_handler.hpp"

#define TAG                     "Rest server handler"

#define SEND_OK(req) do { \
    std::stringstream message; \
    message << "{\"" << req->uri << "\": \"ok\"}"; \
    std::string m = message.str(); \
    httpd_resp_send(req, m.c_str(), m.length()); \
    return ESP_OK; \
} while(0) \

#define CHECK_FILE_EXTENSION(str, ext) (str.substr(str.find_last_of(".") + 1) == ext)

template<typename T>
std::string toString(T val)
{
    std::stringstream ss("");
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
}

esp_err_t right_handler(httpd_req_t *req) {
    Driver::clockwise();
    SEND_OK(req);
}

esp_err_t left_handler(httpd_req_t *req) {
    Driver::counterclockwise();
    SEND_OK(req);
}

esp_err_t stop_handler(httpd_req_t *req) {
    Driver::stop();
    SEND_OK(req);
}

esp_err_t forward_handler(httpd_req_t *req) {
    Driver::forward();
    SEND_OK(req);
}

esp_err_t backward_handler(httpd_req_t *req) {
    Driver::backward();
    SEND_OK(req);
}

esp_err_t set_content_type_from_file(httpd_req_t *req, std::string filepath)
{
    const char *type = "text/plain";
    if (CHECK_FILE_EXTENSION(filepath, "html")) {
        type = "text/html";
    }
    else if (CHECK_FILE_EXTENSION(filepath, "js")) {
        type = "application/javascript";
    }
    else if (CHECK_FILE_EXTENSION(filepath, "css")) {
        type = "text/css";
    }
    ESP_LOGI(TAG, "type %s returned", type);

    return httpd_resp_set_type(req, type);
}


esp_err_t pwm_handler(httpd_req_t * req) {
    ESP_LOGI(TAG, "uri %s was hit", req->uri);

    std::string str(req->uri);
    size_t i = 0;
    
    // Parse: /PWM/integer -> integer
    for ( ; i < str.length() && !isdigit(str[i]); i++ );
    str = str.substr(i, str.length() - i );
    int pwm_val = atoi(str.c_str());

    ESP_LOGI(TAG, "got pwm value : %d", pwm_val);

    // set new duty cycle
    float duty_cycle_percentage = (pwm_val / 255.0) * 100.0;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, pwm_val);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

    ESP_LOGI(TAG, "changed duty cycle to %.2f percent", duty_cycle_percentage );

    SEND_OK(req);
}


esp_err_t root_handler(httpd_req_t *req)
{
    if (strcmp(req->uri, DISTANCE_URI) == 0) {
        return distance_handler(req);
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

esp_err_t distance_handler(httpd_req_t * req) {
    ESP_LOGI(TAG, "uri %s was hit", req->uri);
    httpd_resp_sendstr_chunk(req, toString<double>(get_obstacle_distance_cm()).c_str());
    httpd_resp_sendstr_chunk(req, NULL);
    return ESP_OK;
}

esp_err_t autodrive_handler(httpd_req_t * req) {
    ESP_LOGI(TAG, "uri %s was hit", req->uri);

    //Parse /Autonomous/bool -> bool
    char *ptr = strrchr(req->uri, '/');
    if (strcmp(ptr + 1, "true") == 0) {
        start_autonomous_movement();
    }
    else {
        stop_autonomous_movement();
    }

    SEND_OK(req);
}
