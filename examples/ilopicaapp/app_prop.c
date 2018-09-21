#include "alicloud_ilop.h"
#include "main.h"
#include "mx_common.h"
#include "mx_debug.h"

typedef struct _app_prop_t {
    char switch_status[2];
    char color_temp[5];
    char rgb_red[4];
    char rgb_green[4];
    char rgb_blue[4];
} app_prop_t;

static app_prop_t prop;

static mx_status light_switch_write_func(char* value)
{
    strcpy(prop.switch_status, value);
    app_log("write LightSwitch:%s", prop.switch_status);
    ilop_ica_attr_indicate_by_handle(ILOP_HANDLE_ATTR_LightSwitch);
    return kNoErr;
}

static mx_status light_switch_read_func(char** value)
{
    (*value) = prop.switch_status;
    app_log("read LightSwitch:%s", prop.switch_status);
    return kNoErr;
}

static mx_status color_temp_write_func(char* value)
{
    strcpy(prop.color_temp, value);
    app_log("write ColorTemperature:%s", prop.color_temp);
    ilop_ica_attr_indicate_by_handle(ILOP_HANDLE_ATTR_ColorTemperature);
    return kNoErr;
}

static mx_status color_temp_read_func(char** value)
{
    (*value) = prop.color_temp;
    app_log("read ColorTemperature:%s", prop.color_temp);
    return kNoErr;
}

static mx_status rgb_red_write_func(char* value)
{
    strcpy(prop.rgb_red, value);
    app_log("write R:%s", prop.rgb_red);
    ilop_ica_attr_indicate_by_handle(ILOP_HANDLE_ATTR_RGBColor_Red);
    return kNoErr;
}

static mx_status rgb_red_read_func(char** value)
{
    (*value) = prop.rgb_red;
    app_log("read R:%s", prop.rgb_red);
    return kNoErr;
}

static mx_status rgb_green_write_func(char* value)
{
    strcpy(prop.rgb_green, value);
    app_log("write G:%s", prop.rgb_green);
    ilop_ica_attr_indicate_by_handle(ILOP_HANDLE_ATTR_RGBColor_Green);
    return kNoErr;
}

static mx_status rgb_green_read_func(char** value)
{
    (*value) = prop.rgb_green;
    app_log("read G:%s", prop.rgb_green);
    return kNoErr;
}

static mx_status rgb_blue_write_func(char* value)
{
    strcpy(prop.rgb_blue, value);
    app_log("write B:%s", prop.rgb_blue);
    ilop_ica_attr_indicate_by_handle(ILOP_HANDLE_ATTR_RGBColor_Blue);
    return kNoErr;
}

static mx_status rgb_blue_read_func(char** value)
{
    (*value) = prop.rgb_blue;
    app_log("read B:%s", prop.rgb_blue);
    return kNoErr;
}

static const struct ilop_ica_attr_t LightSwitch_property = { ILOP_HANDLE_ATTR_LightSwitch, "LightSwitch", EMH_ARG_ILOP_VT_PROPERTY, light_switch_read_func, light_switch_write_func };
static const struct ilop_ica_attr_t ColorTemperature_property = { ILOP_HANDLE_ATTR_ColorTemperature, "ColorTemperature", EMH_ARG_ILOP_VT_PROPERTY, color_temp_read_func, color_temp_write_func };
static const struct ilop_ica_attr_t RGBColor_Red_property = { ILOP_HANDLE_ATTR_RGBColor_Red, "RGBColor.Red", EMH_ARG_ILOP_VT_PROPERTY, rgb_red_read_func, rgb_red_write_func };
static const struct ilop_ica_attr_t RGBColor_Green_property = { ILOP_HANDLE_ATTR_RGBColor_Green, "RGBColor.Green", EMH_ARG_ILOP_VT_PROPERTY, rgb_green_read_func, rgb_green_write_func };
static const struct ilop_ica_attr_t RGBColor_Blue_property = { ILOP_HANDLE_ATTR_RGBColor_Blue, "RGBColor.Blue", EMH_ARG_ILOP_VT_PROPERTY, rgb_blue_read_func, rgb_blue_write_func };

void property_register(void)
{
    memset(&prop, 0x00, sizeof(app_prop_t));
    strcpy(prop.switch_status, "0");
    strcpy(prop.color_temp, "4000");
    strcpy(prop.rgb_red, "0");
    strcpy(prop.rgb_green, "2");
    strcpy(prop.rgb_blue, "100");

    ilop_ica_attr_register(&LightSwitch_property);
    ilop_ica_attr_register(&ColorTemperature_property);
    ilop_ica_attr_register(&RGBColor_Red_property);
    ilop_ica_attr_register(&RGBColor_Green_property);
    ilop_ica_attr_register(&RGBColor_Blue_property);

    //连上服务器后 上报状态
    ilop_ica_attr_indicate_by_handle(ILOP_HANDLE_ATTR_LightSwitch);
    ilop_ica_attr_indicate_by_handle(ILOP_HANDLE_ATTR_ColorTemperature);
    ilop_ica_attr_indicate_by_handle(ILOP_HANDLE_ATTR_RGBColor_Red);
    ilop_ica_attr_indicate_by_handle(ILOP_HANDLE_ATTR_RGBColor_Green);
    ilop_ica_attr_indicate_by_handle(ILOP_HANDLE_ATTR_RGBColor_Blue);
}
