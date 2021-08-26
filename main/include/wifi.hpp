#pragma once

#include <string>
#include <iomanip>
#include <sstream>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include <esp_http_server.h>
#include <tcpip_adapter.h>

#include "server.hpp"
#include "constants.h"

esp_err_t           init_access_point               (void);
httpd_handle_t      start_webserver                 (void);
void                connect_wifi                    (void);
static esp_err_t    event_handler                   (void *, system_event_t *);
void                list_all_connected_stations     ();