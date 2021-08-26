#ifndef __ERRORMACROS_H__
#define __ERRORMACROS_H__

#include "esp_log.h"

#define E_TAG "ErrorMacros"

#define CHECK_INIT(res, pin_number) \
	if((res != ESP_OK)) { \
		ESP_LOGE(E_TAG, "error setting %d's direction to OUTPUT", pin_number); \
	} \
	else { \
		ESP_LOGI(E_TAG, "pin %d's direction set to OUTPUT successfully", pin_number); \
} \


#define CHECK_OUTPUT(res, pin_number) \
	if((res != ESP_OK)) { \
		ESP_LOGE(E_TAG, "error writing to pin %d", pin_number); \
	} \
	else { \
		ESP_LOGI(E_TAG, "pin %d has been provided with output voltage", pin_number); \
	} \

#endif