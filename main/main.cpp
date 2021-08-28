#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "include/autonomous.hpp"

#include "include/arduinointerface.hpp"

#define M_TAG "Main"

extern C
void app_main(void)
{
  ESP_LOGI(M_TAG, "Attempting to start the remote controlled car");
  
  ArduinoInterface a;
  a.setup();
  a.loop();

  start_autonomous_movement();
}