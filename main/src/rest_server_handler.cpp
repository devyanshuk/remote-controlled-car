#include "../include/rest_server_handler.hpp"

#define TAG "Rest server handler"

#define FILE_BUF_SIZE 256

#define FORWARD_URI "/Forward"
#define ROOT_URI "/*"
#define PWM_URI "/PWM/*"
#define BACKWARD_URI "/Backward"
#define CLOCKWISE_URI "/Clockwise"
#define COUNTERCLOCKWISE_URI "/Counterclockwise"
#define STOP_URI "/Stop"

#define SEND_OK(req) do { \
    std::stringstream message; \
    message << "{\"" << req->uri << "\": \"ok\"}"; \
    std::string m = message.str(); \
    httpd_resp_send(req, m.c_str(), m.length()); \
} while(0) \

#define CHECK_FILE_EXTENSION(str, ext) (str.substr(str.find_last_of(".") + 1) == ext)

void register_callbacks(httpd_handle_t * server) {

    httpd_uri_t root =
        {
            .uri = ROOT_URI,
            .method = HTTP_GET,
            .handler = root_handler,
            .user_ctx = (void*)0
        };

    httpd_uri_t pwm =
        {
            .uri = PWM_URI,
            .method = HTTP_POST,
            .handler = pwm_handler,
            .user_ctx = (void*)0
        };


    httpd_uri_t straight = 
        {
            .uri = FORWARD_URI,
            .method = HTTP_POST,
            .handler = forward_handler,
            .user_ctx = (void*)0
        };

    httpd_uri_t back = 
        {
            .uri = BACKWARD_URI,
            .method = HTTP_POST,
            .handler = backward_handler,
            .user_ctx = (void*)0
        };

    httpd_uri_t left = 
        {
            .uri = COUNTERCLOCKWISE_URI,
            .method = HTTP_POST,
            .handler = left_handler,
            .user_ctx = (void*)0
        };

    httpd_uri_t right =
        {
            .uri = CLOCKWISE_URI,
            .method = HTTP_POST,
            .handler = right_handler,
            .user_ctx = (void*)0
        };

    httpd_uri_t stop =
        {
            .uri = STOP_URI,
            .method = HTTP_POST,
            .handler = stop_handler,
            .user_ctx = (void*)0
        };

    httpd_register_uri_handler(*server, &root);
    ESP_LOGI(TAG, "Registered /* uri handler");
    httpd_register_uri_handler(*server, &pwm);
    ESP_LOGI(TAG, "Registered /PWM/* uri handler");
    httpd_register_uri_handler(*server, &straight);
    ESP_LOGI(TAG, "Registered /forward uri handler");
    httpd_register_uri_handler(*server, &back);
    ESP_LOGI(TAG, "Registered /forward uri handler");
    httpd_register_uri_handler(*server, &left);
    ESP_LOGI(TAG, "Registered /counterclockwise uri handler");
    httpd_register_uri_handler(*server, &right);
    ESP_LOGI(TAG, "Registered /clockwise uri handler");
    httpd_register_uri_handler(*server, &stop);
    ESP_LOGI(TAG, "Registered /stop uri handler");
    
}

esp_err_t right_handler(httpd_req_t *req) {
    Driver::clockwise();
    SEND_OK(req);
    return ESP_OK;
}

esp_err_t left_handler(httpd_req_t *req) {
    Driver::counterclockwise();
    SEND_OK(req);
    return ESP_OK;

}

esp_err_t stop_handler(httpd_req_t *req) {
    Driver::stop();
    SEND_OK(req);
    return ESP_OK;

}

esp_err_t forward_handler(httpd_req_t *req) {
    Driver::forward();
    SEND_OK(req);
    return ESP_OK;

}

esp_err_t backward_handler(httpd_req_t *req) {
    Driver::backward();
    SEND_OK(req);
    return ESP_OK;
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
    ESP_LOGI(TAG, "URI %s was hit", req->uri);

    std::string str(req->uri);
    size_t i = 0;
    
    // Parse: /PWM/integer -> integer
    for ( ; i < str.length(); i++ ){ if ( isdigit(str[i]) ) break; }
    str = str.substr(i, str.length() - i );
    int pwm_val = atoi(str.c_str());

    ESP_LOGI(TAG, "got pwm value : %d", pwm_val);

    // set new duty cycle
    float duty_cycle_percentage = (pwm_val / 255.0) * 100.0;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, pwm_val);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

    ESP_LOGI(TAG, "changed duty cycle to %.2f percent", duty_cycle_percentage );

    SEND_OK(req);
    return ESP_OK;
}


esp_err_t root_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "url %s was hit", req->uri);

    std::string file_path(SPIFFS_ROOT);

    ESP_LOGI(TAG, "Base path = %s", file_path.c_str());

    if (req->uri[strlen(req->uri) - 1] == '/') {
        file_path += "/index.html";
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
