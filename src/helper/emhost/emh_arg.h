/**
 ******************************************************************************
 * @file    emh_arg.c
 * @author  QQ Ding
 * @version V1.0.0
 * @date    3-Sept-2018
 * @brief   AT command arguments to enum type convert header file
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

#ifndef _EMH_ARG_H_
#define _EMH_ARG_H_

#include "mx_common.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** \addtogroup emhost */
/** @{*/

/******************************************************************************
 *                                 Constants
 ******************************************************************************/

#define EMH_ARG_ERR 0xFF

/******************************************************************************
 *                              Alicoud SDS service
 ******************************************************************************/

/** \addtogroup alicloud_sds */
/** @{*/
enum{
	EMH_ARG_ALISDS_CONN_DISCONNECTED,
	EMH_ARG_ALISDS_CONN_CONNECTED,
	EMH_ARG_ALISDS_CONN_MAX,
};
typedef uint8_t emh_arg_alisds_conn_t;
extern const char* EMH_ARG_ALISDS_CONN[];

enum{
	EMH_ARG_ALISDS_STATUS_UNINITIALIZED,
	EMH_ARG_ALISDS_STATUS_INITIALIZED,
	EMH_ARG_ALISDS_STATUS_CONNECTED,
	EMH_ARG_ALISDS_STATUS_DISCONNECTED,
	EMH_ARG_ALISDS_STATUS_MAX,
};
typedef uint8_t emh_arg_alisds_status_t;
extern const char* EMH_ARG_ALISDS_STATUS[];

enum{
	EMH_ARG_ALISDS_FORMAT_JSON,
	EMH_ARG_ALISDS_FORMAT_RAW,
	EMH_ARG_ALISDS_FORMAT_MAX,
};
typedef uint8_t emh_arg_alisds_format_t;
extern const char* EMH_ARG_ALISDS_FORMAT[];

enum{
	EMH_ARG_ALISDS_EV_ALINK,
	EMH_ARG_ALISDS_EV_SET,
	EMH_ARG_ALISDS_EV_GET,
	EMH_ARG_ALISDS_EV_MAX,
};
typedef uint8_t emh_arg_alisds_ev_t;
extern const char* EMH_ARG_ALISDS_EV[];
/** @}*/

/******************************************************************************
 *                              WiFi service
 ******************************************************************************/
/** \addtogroup wlan */
/** @{*/
enum{
	EMH_ARG_WLAN_EV_UAP_ESTABLISHED,
	EMH_ARG_WLAN_EV_UAP_DESTROYED,
	EMH_ARG_WLAN_EV_STA_CONNECTED,
	EMH_ARG_WLAN_EV_STA_DISCONNECTED,
	EMH_ARG_WLAN_EV_MAX,
};
typedef uint8_t emh_arg_wlan_ev_t;
extern const char* EMH_ARG_WLAN_EV[];

enum{
	EMH_ARG_WLAN_STA_CONNECTED,
	EMH_ARG_WLAN_STA_DISCONNECTED,
	EMH_ARG_WLAN_STA_CONNECTING,
	EMH_ARG_WLAN_STA_MAX,
};
typedef uint8_t emh_arg_wlan_sta_t;      /**< wlan connection status under station mode */
extern const char* EMH_ARG_WLAN_STA[];
/** @}*/

/******************************************************************************
 *                              Alicoud ILOP service
 ******************************************************************************/
/** \addtogroup ilop */
/** @{*/
enum{
	EMH_ARG_ILOP_DM_RAW,
	EMH_ARG_ILOP_DM_ICA_BY_CLOUD,
	EMH_ARG_ILOP_DM_ICA_BY_LOCAL,
	EMH_ARG_ILOP_DM_MAX,
};
typedef uint8_t emh_arg_ilop_dm_t;
extern const char* EMH_ARG_ILOP_DM[];

enum{
	EMH_ARG_ILOP_CONFIG_NONE,
	EMH_ARG_ILOP_CONFIG_TLS,
	EMH_ARG_ILOP_CONFIG_MAX,
};
typedef uint8_t emh_arg_ilop_config_t;
extern const char* EMH_ARG_ILOP_CONFIG[];

enum{
	EMH_ARG_ILOP_STATUS_UNINITIALIZED,
	EMH_ARG_ILOP_STATUS_INITIALIZED,
	EMH_ARG_ILOP_STATUS_CONNECTED,
	EMH_ARG_ILOP_STATUS_DISCONNECTED,
	EMH_ARG_ILOP_STATUS_MAX,
};
typedef uint8_t emh_arg_ilop_status_t;
extern const char* EMH_ARG_ILOP_STATUS[];

enum{
	EMH_ARG_ILOP_CONN_CLOUD_CONNECTED,
	EMH_ARG_ILOP_CONN_CLOUD_DISCONNECTED,
	EMH_ARG_ILOP_CONN_LOCAL_CONNECTED,
	EMH_ARG_ILOP_CONN_LOCAL_DISCONNECTED,	
	EMH_ARG_ILOP_CONN_MAX,
};
typedef uint8_t emh_arg_ilop_conn_t;
extern const char* EMH_ARG_ILOP_CONN[];

enum{
	EMH_ARG_ILOP_EV_ILOP,
	EMH_ARG_ILOP_EV_SETICA,
	EMH_ARG_ILOP_EV_SETRAW,
	EMH_ARG_ILOP_EV_MAX,
};
typedef uint8_t emh_arg_ilop_ev_t;
extern const char* EMH_ARG_ILOP_EV[];

enum{
	EMH_ARG_ILOP_FORMAT_ICA,
	EMH_ARG_ILOP_FORMAT_RAW,
	EMH_ARG_ILOP_FORMAT_MAX,
};
typedef uint8_t emh_arg_ilop_format_t;

enum{
	EMH_ARG_ILOP_VT_PROPERTY,
	EMH_ARG_ILOP_VT_EVENT,
	EMH_ARG_ILOP_VT_SERVICE,
	EMH_ARG_ILOP_VT_MAX,
};
typedef uint8_t emh_arg_ilop_vt_t;
extern const char *EMH_ARG_ILOP_VT[];
/** @}*/

/******************************************************************************
 *                             Function Declarations
 ******************************************************************************/

/**
 * @brief 			Find string type argument from enum 
 * 
 * @param[in] 		emh_arg: Arg convert list array
 * @param[in] 		type: enum argument type
 *
 * @return 			Argument string type
 */
const char* emh_arg_for_type(const char* emh_arg[], uint8_t type);

/**
 * @brief 			Find enum type argument from string 
 * 
 * @param[in] 		emh_arg: Arg convert list array
 * @param[in] 		type: string argument read from AT command
 *
 * @return 			Argument enum type
 */
uint8_t emh_arg_for_arg(const char* emh_arg[], char *arg);

/** @}*/
/** @}*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
