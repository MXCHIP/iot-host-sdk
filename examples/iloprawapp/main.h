
#ifndef _MAIN_H_
#define _MAIN_H_

#define APP_DEBUG MX_DEBUG_ON
#define app_log(M, ...) MX_LOG(APP_DEBUG, "APP", M, ##__VA_ARGS__)

//property
enum {
    ILOP_PROPERTY_LightSwitch,
    ILOP_PROPERTY_WIFI_Band,
    ILOP_PROPERTY_WiFI_RSSI,
    ILOP_PROPERTY_WIFI_AP_BSSID,
    ILOP_PROPERTY_WIFI_Channel,
    ILOP_PROPERTY_WiFI_SNR,
    ILOP_PROPERTY_ColorTemperature,
    ILOP_PROPERTY_RGBColor,
    ILOP_PROPERTY_MAX,
};

//event
enum {
    ILOP_EVENT_ErrorCode,
    ILOP_EVENT_MAX,
};

//sevice
enum {
    ILOP_SERVICE_TimeReset,
    ILOP_SERVICE_MAX,
};

void ilop_main(void);

void event_register(void);
void event_task_loop(void);

void property_register_light(void);
void property_register_wifi(void);

void service_register(void);

#endif
