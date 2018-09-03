/**
 ******************************************************************************
 * @file    ilopmain.c
 * @author  QQ Ding
 * @version V1.0.0
 * @date    3-Sept-2018
 * @brief   ILOP Service Main function
 ******************************************************************************
 *
 * Copyright (c) 2009-2018 MXCHIP Co.,Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

#include "alicloud_ilop.h"
#include "mx_common.h"
#include "mx_debug.h"

#include "emh_api.h"
#include "main.h"
#include "mx_cli.h"
#include "mx_hal.h"

const char thing[] = "{\"schema\":\"https://iotx-tsl.oss-ap-southeast-1.aliyuncs.com/schema.json\",\"profile\":{\"productKey\":\"a1MeLuIJPYL\"},\"services\":[{\"outputData\":[],\"identifier\":\"set\",\"inputData\":[{\"identifier\":\"LightSwitch\",\"dataType\":{\"specs\":{\"0\":\"关闭\",\"1\":\"开启\"},\"type\":\"bool\"},\"name\":\"主灯开关\"},{\"identifier\":\"ColorTemperature\",\"dataType\":{\"specs\":{\"unit\":\"K\",\"min\":\"2700\",\"unitName\":\"开尔文\",\"max\":\"6500\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"冷暖色温\"},{\"identifier\":\"NightLightSwitch\",\"dataType\":{\"specs\":{\"0\":\"关闭\",\"1\":\"开启\"},\"type\":\"bool\"},\"name\":\"夜灯开关\"},{\"identifier\":\"WorkMode\",\"dataType\":{\"specs\":{\"0\":\"手动\",\"1\":\"阅读\",\"2\":\"影院\",\"3\":\"夜灯\",\"4\":\"生活\",\"5\":\"柔和\"},\"type\":\"enum\"},\"name\":\"工作模式\"},{\"identifier\":\"RGBColor\",\"dataType\":{\"specs\":[{\"identifier\":\"Red\",\"dataType\":{\"specs\":{\"min\":\"0\",\"unitName\":\"无\",\"max\":\"255\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"红色\"},{\"identifier\":\"Green\",\"dataType\":{\"specs\":{\"min\":\"0\",\"unitName\":\"无\",\"max\":\"255\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"绿色\"},{\"identifier\":\"Blue\",\"dataType\":{\"specs\":{\"min\":\"0\",\"unitName\":\"无\",\"max\":\"255\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"蓝色\"}],\"type\":\"struct\"},\"name\":\"RGB调色\"}],\"method\":\"thing.service.property.set\",\"name\":\"set\",\"required\":true,\"callType\":\"async\",\"desc\":\"属性设置\"},{\"outputData\":[{\"identifier\":\"LightSwitch\",\"dataType\":{\"specs\":{\"0\":\"关闭\",\"1\":\"开启\"},\"type\":\"bool\"},\"name\":\"主灯开关\"},{\"identifier\":\"ColorTemperature\",\"dataType\":{\"specs\":{\"unit\":\"K\",\"min\":\"2700\",\"unitName\":\"开尔文\",\"max\":\"6500\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"冷暖色温\"},{\"identifier\":\"NightLightSwitch\",\"dataType\":{\"specs\":{\"0\":\"关闭\",\"1\":\"开启\"},\"type\":\"bool\"},\"name\":\"夜灯开关\"},{\"identifier\":\"WorkMode\",\"dataType\":{\"specs\":{\"0\":\"手动\",\"1\":\"阅读\",\"2\":\"影院\",\"3\":\"夜灯\",\"4\":\"生活\",\"5\":\"柔和\"},\"type\":\"enum\"},\"name\":\"工作模式\"},{\"identifier\":\"RGBColor\",\"dataType\":{\"specs\":[{\"identifier\":\"Red\",\"dataType\":{\"specs\":{\"min\":\"0\",\"unitName\":\"无\",\"max\":\"255\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"红色\"},{\"identifier\":\"Green\",\"dataType\":{\"specs\":{\"min\":\"0\",\"unitName\":\"无\",\"max\":\"255\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"绿色\"},{\"identifier\":\"Blue\",\"dataType\":{\"specs\":{\"min\":\"0\",\"unitName\":\"无\",\"max\":\"255\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"蓝色\"}],\"type\":\"struct\"},\"name\":\"RGB调色\"}],\"identifier\":\"get\",\"inputData\":[\"LightSwitch\",\"ColorTemperature\",\"NightLightSwitch\",\"WorkMode\",\"RGBColor\"],\"method\":\"thing.service.property.get\",\"name\":\"get\",\"required\":true,\"callType\":\"async\",\"desc\":\"属性获取\"},{\"outputData\":[],\"identifier\":\"TimeReset\",\"inputData\":[{\"identifier\":\"TimeReset\",\"dataType\":{\"specs\":{\"length\":\"255\"},\"type\":\"text\"},\"name\":\"TimeReset\"}],\"method\":\"thing.service.TimeReset\",\"name\":\"设备校时服务\",\"required\":false,\"callType\":\"async\"}],\"properties\":[{\"identifier\":\"LightSwitch\",\"dataType\":{\"specs\":{\"0\":\"关闭\",\"1\":\"开启\"},\"type\":\"bool\"},\"name\":\"主灯开关\",\"accessMode\":\"rw\",\"required\":true},{\"identifier\":\"ColorTemperature\",\"dataType\":{\"specs\":{\"unit\":\"K\",\"min\":\"2700\",\"unitName\":\"开尔文\",\"max\":\"6500\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"冷暖色温\",\"accessMode\":\"rw\",\"required\":false},{\"identifier\":\"NightLightSwitch\",\"dataType\":{\"specs\":{\"0\":\"关闭\",\"1\":\"开启\"},\"type\":\"bool\"},\"name\":\"夜灯开关\",\"accessMode\":\"rw\",\"required\":false},{\"identifier\":\"WorkMode\",\"dataType\":{\"specs\":{\"0\":\"手动\",\"1\":\"阅读\",\"2\":\"影院\",\"3\":\"夜灯\",\"4\":\"生活\",\"5\":\"柔和\"},\"type\":\"enum\"},\"name\":\"工作模式\",\"accessMode\":\"rw\",\"required\":false},{\"identifier\":\"RGBColor\",\"dataType\":{\"specs\":[{\"identifier\":\"Red\",\"dataType\":{\"specs\":{\"min\":\"0\",\"unitName\":\"无\",\"max\":\"255\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"红色\"},{\"identifier\":\"Green\",\"dataType\":{\"specs\":{\"min\":\"0\",\"unitName\":\"无\",\"max\":\"255\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"绿色\"},{\"identifier\":\"Blue\",\"dataType\":{\"specs\":{\"min\":\"0\",\"unitName\":\"无\",\"max\":\"255\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"蓝色\"}],\"type\":\"struct\"},\"name\":\"RGB调色\",\"accessMode\":\"rw\",\"required\":false}],\"events\":[{\"outputData\":[{\"identifier\":\"LightSwitch\",\"dataType\":{\"specs\":{\"0\":\"关闭\",\"1\":\"开启\"},\"type\":\"bool\"},\"name\":\"主灯开关\"},{\"identifier\":\"ColorTemperature\",\"dataType\":{\"specs\":{\"unit\":\"K\",\"min\":\"2700\",\"unitName\":\"开尔文\",\"max\":\"6500\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"冷暖色温\"},{\"identifier\":\"NightLightSwitch\",\"dataType\":{\"specs\":{\"0\":\"关闭\",\"1\":\"开启\"},\"type\":\"bool\"},\"name\":\"夜灯开关\"},{\"identifier\":\"WorkMode\",\"dataType\":{\"specs\":{\"0\":\"手动\",\"1\":\"阅读\",\"2\":\"影院\",\"3\":\"夜灯\",\"4\":\"生活\",\"5\":\"柔和\"},\"type\":\"enum\"},\"name\":\"工作模式\"},{\"identifier\":\"RGBColor\",\"dataType\":{\"specs\":[{\"identifier\":\"Red\",\"dataType\":{\"specs\":{\"min\":\"0\",\"unitName\":\"无\",\"max\":\"255\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"红色\"},{\"identifier\":\"Green\",\"dataType\":{\"specs\":{\"min\":\"0\",\"unitName\":\"无\",\"max\":\"255\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"绿色\"},{\"identifier\":\"Blue\",\"dataType\":{\"specs\":{\"min\":\"0\",\"unitName\":\"无\",\"max\":\"255\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"蓝色\"}],\"type\":\"struct\"},\"name\":\"RGB调色\"}],\"identifier\":\"post\",\"method\":\"thing.event.property.post\",\"name\":\"post\",\"type\":\"info\",\"required\":true,\"desc\":\"属性上报\"},{\"outputData\":[{\"identifier\":\"ErrorCode\",\"dataType\":{\"specs\":{\"0\":\"恢复正常\"},\"type\":\"enum\"},\"name\":\"故障代码\"}],\"identifier\":\"Error\",\"method\":\"thing.event.Error.post\",\"name\":\"故障上报\",\"type\":\"error\",\"required\":true}]}";

static int awss_start = 0;
static int awss_press = 0;
static int reset = 0;

const emh_ilop_config_t ilop_config = {
    .tsl_thing = thing,
    .tsl_len = strlen(thing),
    .dm = EMH_ARG_ILOP_DM_ICA_BY_LOCAL,
};

const ilop_device_key_t device_key = {
    .product_key = "a1MeLuIJPYL",
    .product_secret = "SegyIuWW6xBQGuIc",
    .device_name = "aQI0eEM5g2nlSr2njD0x",
    .device_secret = "o7EZShzl6z8F3xq8qF3vN9P7HFGslRAV",
};

static void user_tast_loop(void)
{
    if (awss_start == 1) {
        ilop_awss_start();
        awss_start = 0;
    }
    if (awss_press == 1) {
        ilop_awss_press();
        awss_press = 0;
    }
    if (reset == 1) {
        ilop_restore();
        reset = 0;
    }
}

#ifdef MX_CLI_ENABLE
static void handle_aws_cmd(char* pwbuf, int blen, int argc, char** argv)
{
    if (argc != 2)
        return;

    if (strcmp(argv[1], "start") == 0) {
        awss_start = 1;
    } else if (strcmp(argv[1], "press") == 0) {
        awss_press = 1;
    }
}

static void handle_reset_cmd(char* pwbuf, int blen, int argc, char** argv)
{
    reset = 1;
}

static struct cli_command ilopcmds[] = {
    { "aws", "aws [start|press]", handle_aws_cmd },
    { "reset", "clean wifi module and ilop service", handle_reset_cmd },
};
#endif

void ilop_main(void)
{
    mx_status err = kNoErr;

    /* System initialization， ticker, stdio */
    mx_hal_ms_ticker_init();
    mx_hal_stdio_init();

#ifdef MX_CLI_ENABLE
    cli_register_commands(ilopcmds, sizeof(ilopcmds) / sizeof(struct cli_command));
#endif

    /* ILOP service initialization */
    err = ilop_init(&ilop_config);
    if (err != kNoErr)
        app_log("ilop init err");

    /* Set the ILOP three tuple. If the module is pre burned, delete the function */
    ilop_set_device_key(&device_key);

    /* database initialization */
    ilop_ica_attr_init(ILOP_HANDLE_ATTR_MAX);

    property_register();
    service_register();
    event_register();

    while (1) {
        ilop_runloop();
        user_tast_loop();
        event_task_loop();
    }
}
