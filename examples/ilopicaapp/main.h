/**
 ******************************************************************************
 * @file    main.h
 * @author  QQ Ding
 * @version V1.0.0
 * @date    3-Sept-2018
 * @brief   Application API header file
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

#ifndef _MAIN_H_
#define _MAIN_H_

/******************************************************************************
 *                                   Macros
 ******************************************************************************/

#define APP_DEBUG MX_DEBUG_ON
#define app_log(M, ...) MX_LOG(APP_DEBUG, "APP", M, ##__VA_ARGS__)

/******************************************************************************
 *                                 Enumerations
 ******************************************************************************/

enum {
    ILOP_HANDLE_ATTR_LightSwitch,
    ILOP_HANDLE_ATTR_ColorTemperature,
    ILOP_HANDLE_ATTR_RGBColor_Red,
    ILOP_HANDLE_ATTR_RGBColor_Green,
    ILOP_HANDLE_ATTR_RGBColor_Blue,
    ILOP_HANDLE_ATTR_TimeReset,
    ILOP_HANDLE_ATTR_ErrorCode,
    ILOP_HANDLE_ATTR_MAX,
};

/******************************************************************************
 *                             Function Declarations
 ******************************************************************************/

void ilop_main(void);
void property_register(void);
void service_register(void);
void event_register(void);
void event_task_loop(void);

#endif
