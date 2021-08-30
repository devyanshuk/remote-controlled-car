#pragma once

#include <esp_log.h>
#include <esp_vfs_semihost.h>

#include "rest_server_handler.hpp" 

extern httpd_handle_t http_server_instance;

#define SCRATCH_BUFSIZE (10240)

typedef struct rest_server_context {
    char base_path[ESP_VFS_PATH_MAX + 1];
    char scratch[SCRATCH_BUFSIZE];
} rest_server_context_t;


void start_server();
void stop_server ();