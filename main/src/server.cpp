#include "../include/server.hpp"

#define TAG "Server"

esp_err_t init_fs(void);

httpd_handle_t http_server_instance = NULL;

void start_server() {
    httpd_config_t httpServerConfiguration = HTTPD_DEFAULT_CONFIG();
    httpServerConfiguration.lru_purge_enable = true;
    httpServerConfiguration.max_uri_handlers = 15;
    httpServerConfiguration.uri_match_fn = httpd_uri_match_wildcard;
    
    if(httpd_start(&http_server_instance, &httpServerConfiguration) != ESP_OK){
        ESP_LOGW(TAG, "Could not start http server");
    }
    else {
        ESP_LOGI(TAG, "Started a new server on the esp32 chip");
    }

    init_fs();
    ESP_LOGI(TAG, "Succesfully initialized SPIFFS");

    register_callbacks(&http_server_instance);
}

void stop_server() {
    ESP_LOGI(TAG, "Stopping http server");

    if (http_server_instance != NULL) {
        ESP_ERROR_CHECK(httpd_stop(&http_server_instance));
    }
}