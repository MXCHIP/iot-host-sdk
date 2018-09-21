#include "alicloud_ilop.h"
#include "main.h"
#include "mx_common.h"
#include "mx_debug.h"

static char* wifi_band = "2.4G";
static int8_t wifi_rssi = -40;
static char* wifi_bssid = "00:00:00:00:00:01";
static uint8_t wifi_channel = 6;
static int8_t wifi_snr = 100;

static mx_status wifi_band_read_func(ilop_att_val_t* value, uint32_t* outlen)
{
    (*value).stringValue = wifi_band;
    (*outlen) = strlen(wifi_band);
    app_log("read value:%s", wifi_band);
    return kNoErr;
}

static mx_status wifi_band_write_func(ilop_att_val_t value, uint32_t inlen)
{
    app_log("WIFI_BAND[%d]:%.*s", inlen, inlen, value.stringValue);
    return kNoErr;
}

static mx_status wifi_rssi_read_func(ilop_att_val_t* value, uint32_t* outlen)
{
    (*value).int8Value = wifi_rssi;
    (*outlen) = sizeof(wifi_rssi);
    app_log("read value:%d", wifi_rssi);
    return kNoErr;
}

static mx_status wifi_rssi_write_func(ilop_att_val_t value, uint32_t inlen)
{
    app_log("WIFI_RSSI[%d]", value.int8Value);
    return kNoErr;
}

static mx_status wifi_ap_bssid_read_func(ilop_att_val_t* value, uint32_t* outlen)
{
    (*value).stringValue = wifi_bssid;
    (*outlen) = strlen(wifi_bssid);
    app_log("read value:%s", wifi_bssid);
    return kNoErr;
}

static mx_status wifi_ap_bssid_write_func(ilop_att_val_t value, uint32_t inlen)
{
    app_log("WIFI_AP_BSSID[%d]:%.*s", inlen, inlen, value.stringValue);
    return kNoErr;
}

static mx_status wifi_channel_read_func(ilop_att_val_t* value, uint32_t* outlen)
{
    (*value).uint8Value = wifi_channel;
    (*outlen) = sizeof(wifi_channel);
    app_log("read value:%d", wifi_channel);
    return kNoErr;
}

static mx_status wifi_channel_write_func(ilop_att_val_t value, uint32_t inlen)
{
    app_log("WIFI_CHANNEL[%d]", value.uint8Value);
    return kNoErr;
}

static mx_status wifi_snr_read_func(ilop_att_val_t* value, uint32_t* outlen)
{
    (*value).int8Value = wifi_snr;
    (*outlen) = sizeof(wifi_snr);
    app_log("read value:%d", wifi_snr);
    return kNoErr;
}

static mx_status wifi_snr_write_func(ilop_att_val_t value, uint32_t inlen)
{
    app_log("WIFI_SNR[%d]", value.int8Value);
    return kNoErr;
}

static const struct ilop_raw_attr_t WIFI_Band_property = { ILOP_PROPERTY_WIFI_Band, 1, TYPE_ID_STRING, wifi_band_read_func, wifi_band_write_func };
static const struct ilop_raw_attr_t WiFI_RSSI_property = { ILOP_PROPERTY_WiFI_RSSI, 2, TYPE_ID_INT8, wifi_rssi_read_func, wifi_rssi_write_func };
static const struct ilop_raw_attr_t WIFI_AP_BSSID_property = { ILOP_PROPERTY_WIFI_AP_BSSID, 3, TYPE_ID_STRING, wifi_ap_bssid_read_func, wifi_ap_bssid_write_func };
static const struct ilop_raw_attr_t WIFI_Channel_property = { ILOP_PROPERTY_WIFI_Channel, 4, TYPE_ID_UINT8, wifi_channel_read_func, wifi_channel_write_func };
static const struct ilop_raw_attr_t WiFI_SNR_property = { ILOP_PROPERTY_WiFI_SNR, 5, TYPE_ID_INT8, wifi_snr_read_func, wifi_snr_write_func };

void property_register_wifi(void)
{
    ilop_raw_attr_register(ATTR_TYPE_PROPERTY, &WIFI_Band_property);
    ilop_raw_attr_register(ATTR_TYPE_PROPERTY, &WiFI_RSSI_property);
    ilop_raw_attr_register(ATTR_TYPE_PROPERTY, &WIFI_AP_BSSID_property);
    ilop_raw_attr_register(ATTR_TYPE_PROPERTY, &WIFI_Channel_property);
    ilop_raw_attr_register(ATTR_TYPE_PROPERTY, &WiFI_SNR_property);

    //连上服务器后 上报状态
    ilop_raw_attr_indicate_by_handle(ATTR_TYPE_PROPERTY, ILOP_PROPERTY_WIFI_Band);
    ilop_raw_attr_indicate_by_handle(ATTR_TYPE_PROPERTY, ILOP_PROPERTY_WiFI_RSSI);
    ilop_raw_attr_indicate_by_handle(ATTR_TYPE_PROPERTY, ILOP_PROPERTY_WIFI_AP_BSSID);
    ilop_raw_attr_indicate_by_handle(ATTR_TYPE_PROPERTY, ILOP_PROPERTY_WIFI_Channel);
    ilop_raw_attr_indicate_by_handle(ATTR_TYPE_PROPERTY, ILOP_PROPERTY_WiFI_SNR);
}
