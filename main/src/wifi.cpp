#include "../include/wifi.hpp"

#define TAG "WiFi"

#define SOFT_AP_IP_ADDRESS_1 192
#define SOFT_AP_IP_ADDRESS_2 168
#define SOFT_AP_IP_ADDRESS_3 1
#define SOFT_AP_IP_ADDRESS_4 1
 
#define SOFT_AP_GW_ADDRESS_1 192
#define SOFT_AP_GW_ADDRESS_2 168
#define SOFT_AP_GW_ADDRESS_3 1
#define SOFT_AP_GW_ADDRESS_4 1
 
#define SOFT_AP_NM_ADDRESS_1 255
#define SOFT_AP_NM_ADDRESS_2 255
#define SOFT_AP_NM_ADDRESS_3 255
#define SOFT_AP_NM_ADDRESS_4 0

void connect_wifi(void) {
    esp_log_level_set("wifi", ESP_LOG_WARN);
    ESP_ERROR_CHECK(nvs_flash_init());
    tcpip_adapter_init();
    ESP_ERROR_CHECK(tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP));
    tcpip_adapter_ip_info_t ipAddressInfo;
    memset(&ipAddressInfo, 0, sizeof(ipAddressInfo));
    IP4_ADDR(
        &ipAddressInfo.ip,
        SOFT_AP_IP_ADDRESS_1,
        SOFT_AP_IP_ADDRESS_2,
        SOFT_AP_IP_ADDRESS_3,
        SOFT_AP_IP_ADDRESS_4);
    IP4_ADDR(
        &ipAddressInfo.gw,
        SOFT_AP_GW_ADDRESS_1,
        SOFT_AP_GW_ADDRESS_2,
        SOFT_AP_GW_ADDRESS_3,
        SOFT_AP_GW_ADDRESS_4);
    IP4_ADDR(
        &ipAddressInfo.netmask,
        SOFT_AP_NM_ADDRESS_1,
        SOFT_AP_NM_ADDRESS_2,
        SOFT_AP_NM_ADDRESS_3,
        SOFT_AP_NM_ADDRESS_4);
    ESP_ERROR_CHECK(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &ipAddressInfo));
    ESP_ERROR_CHECK(tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP));
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
    wifi_init_config_t wifiConfiguration = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifiConfiguration));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    
    wifi_config_t ap_config = { };
    strcpy((char*)ap_config.ap.ssid, WIFI_SSID);
    ap_config.ap.authmode = WIFI_AUTH_OPEN;
    ap_config.ap.max_connection = 4;

    esp_wifi_set_config(WIFI_IF_AP, &ap_config);
    ESP_ERROR_CHECK(esp_wifi_start());

    tcpip_adapter_ip_info_t ip_info;
	ESP_ERROR_CHECK(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_AP, &ip_info));
	ESP_LOGI(TAG, "IP Address:  %s\n", ip4addr_ntoa(&ip_info.ip));
	ESP_LOGI(TAG, "Subnet mask: %s\n", ip4addr_ntoa(&ip_info.netmask));
	ESP_LOGI(TAG, "Gateway:     %s\n", ip4addr_ntoa(&ip_info.gw));

    start_server();
}

esp_err_t event_handler(void *ctx, system_event_t *event) {
    switch(event->event_id) {

        case SYSTEM_EVENT_AP_START:
            ESP_LOGI(TAG, "Successfully configured esp to be an access point.");
            break;

        case SYSTEM_EVENT_AP_STACONNECTED:
            ESP_LOGI(TAG, "A new station connected to the esp.");
            list_all_connected_stations();
            break;

        case SYSTEM_EVENT_AP_STADISCONNECTED:
            ESP_LOGI(TAG, "A station disconnected from the esp. Ending server");
            break;

        case SYSTEM_EVENT_AP_STOP:
            ESP_LOGI(TAG, "esp is no longer an access point");
            break;

        default:
            break;

    }
    return ESP_OK;
}

void list_all_connected_stations() {
 wifi_sta_list_t stationList;
  esp_wifi_ap_get_sta_list(&stationList);  
 
  ESP_LOGI(TAG, "Number of connected stations: %d", stationList.num);
 
  for(int i = 0; i < stationList.num; i++) {
    wifi_sta_info_t station = stationList.sta[i];
    std::stringstream mac_address;
    for(int j = 0; j < 6; j++) {
        int a = static_cast< int >(station.mac[j]);
        mac_address << std::setfill('0') << std::setw(2) << std::hex << a;
        if(j < 5) {
            mac_address << ":";
        }
    }
    ESP_LOGI(TAG, "Station mac addres : %s", mac_address.str().c_str());
  }
}