#include "alicloud_ilop.h"
#include "main.h"
#include "mx_common.h"
#include "mx_debug.h"

static uint8_t switch_status = 0;
static uint16_t color_temp = 3000;
static uint8_t rgb_color[3] = { 0, 0, 0 };

static mx_status light_switch_write_func(ilop_att_val_t value, uint32_t inlen)
{
    switch_status = value.boolValue;
    app_log("write value:%d", switch_status);
    ilop_raw_attr_indicate_by_handle(ATTR_TYPE_PROPERTY, ILOP_PROPERTY_LightSwitch);
    return kNoErr;
}

static mx_status light_switch_read_func(ilop_att_val_t* value, uint32_t* outlen)
{
    (*value).boolValue = switch_status;
    (*outlen) = sizeof(switch_status);
    app_log("read value:%d", switch_status);
    return kNoErr;
}

static mx_status color_temp_write_func(ilop_att_val_t value, uint32_t inlen)
{
    color_temp = value.uint16Value;
    app_log("write value:%d", color_temp);
    ilop_raw_attr_indicate_by_handle(ATTR_TYPE_PROPERTY, ILOP_PROPERTY_ColorTemperature);
    return kNoErr;
}

static mx_status color_temp_read_func(ilop_att_val_t* value, uint32_t* outlen)
{
    (*value).uint16Value = color_temp;
    (*outlen) = sizeof(color_temp);
    app_log("read value:%d", color_temp);
    return kNoErr;
}

static mx_status rgb_color_write_func(ilop_att_val_t value, uint32_t inlen)
{
    rgb_color[0] = value.dataValue[0];
    rgb_color[1] = value.dataValue[1];
    rgb_color[2] = value.dataValue[2];
    app_log("write[%d] R[%d] G[%d] B[%d]", inlen, rgb_color[0], rgb_color[1], rgb_color[2]);
    ilop_raw_attr_indicate_by_handle(ATTR_TYPE_PROPERTY, ILOP_PROPERTY_RGBColor);
    return kNoErr;
}

static mx_status rgb_color_read_func(ilop_att_val_t* value, uint32_t* outlen)
{
    (*value).dataValue = rgb_color;
    (*outlen) = sizeof(rgb_color);
    app_log("read[%d] R[%d] G[%d] B[%d]", (*outlen), (*value).dataValue[0], (*value).dataValue[1], (*value).dataValue[2]);
    return kNoErr;
}

static const struct ilop_raw_attr_t LightSwitch_property = { ILOP_PROPERTY_LightSwitch, 0, TYPE_ID_BOOL, light_switch_read_func, light_switch_write_func };
static const struct ilop_raw_attr_t ColorTemperature_property = { ILOP_PROPERTY_ColorTemperature, 6, TYPE_ID_UINT16, color_temp_read_func, color_temp_write_func };
static const struct ilop_raw_attr_t RGBColor_property = { ILOP_PROPERTY_RGBColor, 7, 30, rgb_color_read_func, rgb_color_write_func };

void property_register_light(void)
{
    ilop_raw_attr_register(ATTR_TYPE_PROPERTY, &LightSwitch_property);
    ilop_raw_attr_register(ATTR_TYPE_PROPERTY, &ColorTemperature_property);
    ilop_raw_attr_register(ATTR_TYPE_PROPERTY, &RGBColor_property);

    //连上服务器后 上报状态
    ilop_raw_attr_indicate_by_handle(ATTR_TYPE_PROPERTY, ILOP_PROPERTY_LightSwitch);
    ilop_raw_attr_indicate_by_handle(ATTR_TYPE_PROPERTY, ILOP_PROPERTY_ColorTemperature);
    ilop_raw_attr_indicate_by_handle(ATTR_TYPE_PROPERTY, ILOP_PROPERTY_RGBColor);
}
