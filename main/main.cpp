#include "esp_log.h"

#include "include/arduinointerface.hpp"

#define M_TAG "Main"

extern C
void app_main(void)
{
  ESP_LOGI(M_TAG, "Attempting to start the remote controlled car");
  
  ArduinoInterface a;
  a.setup();
  a.loop();
}