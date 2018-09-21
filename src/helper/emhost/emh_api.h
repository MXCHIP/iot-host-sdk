/**
 ******************************************************************************
 * @file    emh_arg.c
 * @author  QQ Ding
 * @version V1.0.0
 * @date    3-Sept-2018
 * @brief   Emhost SDK aAPIs
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

#ifndef _EMH_API_H_
#define _EMH_API_H_

#include "emh_arg.h"
#include "mx_common.h"
#include "mx_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup emhost */
/** @{*/

/******************************************************************************
 *                             Module controller
 ******************************************************************************/

/** \addtogroup module */
/** @{*/

/**
 * @brief	Emhost SDK initialize and EMW module initialize
 * 
 * @return	status
 */
mx_status emh_init(void);

/**
 * @brief	Emhost SDK initialize and EMW module initialize
 * 
 * @return	none
 */
void emh_runloop(void);

/**
 * @brief	Software reset EMW module
 * 
 * @return	status
 */
mx_status emh_module_reset(void);

mx_status emh_module_check_ready(void);

/**
 * @brief	Read EMW module firmware version
 * 
 * @return	Point to firmware version string
 */
const char* emh_module_get_fw_version(void);

/**
 * @brief	Read EMW module internal tick
 * 
 * @return	Tick value
 */
uint32_t emh_module_get_tick(void);

/**
 * @brief	Read EMW module internal tick
 * 
 * @return	status
 */
mx_status emh_module_restore_settings(void);

/**
 * @brief	Receive an unknown event, or data format error
 * 
 * @return	none
 */
MX_WEAK void emh_ev_unknown(void);

/** @}*/
/******************************************************************************
 *                              Wlan management
 ******************************************************************************/

/** \addtogroup wlan */
/** @{*/

/**
 * @brief		Read current configured AP's SSID and Password
 * 
 * @param[out] 	timeout: AT receive timeout, also changable by  #ATCmdParser_set_timeout
 * @param[out]	debug: Enable debug mode or not
 * 
 * @return	status
 */
mx_status emh_wlan_get_para(char ssid[33], char pwd[33]);

/**
 * @brief		Read wlan connection status
 * 
 * @return		Refer to #emh_arg_wlan_sta_t
 */
emh_arg_wlan_sta_t emh_wlan_get_sta_status(void);

/**
 * @brief		Event: wlan connection status is changed
 * 
 * @return		none
 */
MX_WEAK void emh_ev_wlan(emh_arg_wlan_ev_t event);

/** @}*/
/******************************************************************************
 *                              Alicoud SDS service
 ******************************************************************************/

/** \addtogroup alicloud_sds */
/** @{*/

/**
 *  Alicloud sds service product information registered on cloud console
 */
typedef struct {
    struct {
        const char* name; /**< Reference to product TRD document */
        const char* module; /**< Reference to product TRD document */
        const char* key; /**< Reference to product TRD document */
        const char* secret; /**< Reference to product TRD document */
        emh_arg_alisds_format_t format; /**< Reference to product TRD document */
    } product_info;
    struct {
        const char* type; /**< Reference to product TRD document */
        const char* category; /**< Reference to product TRD document */
        const char* manufacture; /**< Reference to product TRD document */
    } dev_info;
} emh_alisds_config_t;

/**
 *  Alicloud sds service message
 */
typedef struct {
    int32_t len; /**< message length */
    uint8_t* data; /**< point to the buffer store the message */
    emh_arg_alisds_format_t format; /**< message format, json or raw data */
} emh_alisds_msg;

/**
 * @brief		Read product info data from module, write new data if not equal
 * 
 * @param[in] 	config: Product information
 * 
 * @return		status
 */
mx_status emh_alisds_config(const emh_alisds_config_t* config);

/**
 * @brief		Start cloud service on module
 * 
 * @return		status
 */
mx_status emh_alisds_start_service(void);

/**
 * @brief		Start or stop AWS Wi-Fi configuration, cloud provision procedure bond module to APP
 * @warning		After bound device to APP, the device key can no longer used in other SDS products         
 * 
 * @param[in] 	on: true for start and false for stop 
 * 
 * @return		status
 */
mx_status emh_alisds_provision(bool on);

/**
 * @brief		Get current alicloud service connection status, 
 * 
 * @return		connection status
 */
emh_arg_alisds_status_t emh_alisds_get_status(void);

/**
 * @brief		Send message to cloud from local
 * 
 * @param[in] 	format: message format, json or raw data
 * @param[in] 	data: point to the buffer store the outgoing message
 * @param[in] 	len: outgoing message length
 * 
 * @return		status
 */
mx_status emh_alisds_set_cloud_atts(emh_arg_alisds_format_t format, uint8_t* data, int32_t len);

/**
 * @brief		Set device key, device key is used to connect to SDS service.
 * @note  		The default key is stored in EMW module, use this function to write the new key.
 * 	            A factory restore command remove the new key and use the default key
 * 
 * @param[in] 	dev_key: device key
 * @param[in] 	dev_sec: device security
 * 
 * @return		status
 */
mx_status emh_alisds_set_key(const char* dev_key, const char* dev_sec);

/**
 * @brief		Un-bond device from APP, on works when connected to cloud
 * 
 * @return		status
 */
mx_status emh_alisds_unbound(void);

/**
 * @brief		Event: Alicloud SDS service connection status is changed
 * 
 * @return		none
 */
MX_WEAK void emh_ev_alisds_connection(emh_arg_alisds_conn_t conn);

/**
 * @brief		Event: Alicloud SDS service is requesting data from local device
 * 
 * @param[in] 	attrs: SDS service msg
 * 
 * @return		none
 */
MX_WEAK void emh_ev_alisds_get_local_atts(emh_alisds_msg* attrs);

/**
 * @brief		Event: Alicloud SDS service is writing data to local device
 * 
 * @param[in] 	attrs: SDS service msg
 * 
 * @return		none
 */
MX_WEAK void emh_ev_alisds_set_local_atts(emh_alisds_msg* attrs);

/** @}*/
/******************************************************************************
 *                              Alicoud ILOP service
 ******************************************************************************/

/** \addtogroup alicloud_ilop */
/** @{*/

#define EMH_ILOP_PRODUCT_KEY_MAXLEN (20 + 1)
#define EMH_ILOP_PRODUCT_SECRET_MAXLEN (64 + 1)
#define EMH_ILOP_DEVICE_NAME_MAXLEN (32 + 1)
#define EMH_ILOP_DEVICE_SECRET_MAXLEN (64 + 1)

/**
 *  Alicloud ilop service product information registered on cloud console
 */
typedef struct {
    const char* tsl_thing; /**< TSL model defined by Alibaba cloud platform(json string). if dm=EMH_ARG_ILOP_DM_RAW, tls_thing=NULL */
    uint32_t tsl_len; /**< The JSON string length of the TSL model. if dm=EMH_ARG_ILOP_DM_RAW, tls_len=0 */
    emh_arg_ilop_dm_t dm; /**< The method to access the TLS, raw , local, cloud.*/
} emh_ilop_config_t;

/**
 *  Alicloud ilop service message
 */
typedef struct {
    int32_t len; /**< message length */
    uint8_t* data; /**< point to the buffer store the message */
    emh_arg_ilop_format_t format; /**< message format, ica or raw data */
} emh_ilop_msg;

/**
 * @brief		Read product info data from module, write new data if not equal
 * 
 * @param[in] 	config: Product information
 * @param[in] 	force: Force setting
 * 
 * @return		status
 */
mx_status emh_ilop_config(emh_ilop_config_t *config, bool force);

/**
 * @brief		Set up TSL(json string) from AliCloud ilop platform
 * 
 * @param[in] 	tsl_thing: TSL string
 * @param[in] 	tsl_len: TSL string length
 * 
 * @return		status
 */
mx_status emh_ilop_set_tsl(const char* tsl_thing, uint32_t tsl_len);

/**
 * @brief		Set product key, product secret, device secret, device name is used to connect to ILOP service.
 * @note  		The default key is stored in EMW module, use this function to write the new key.
 * 
 * @param[in] 	pk: product key
 * @param[in] 	ps: product secret
 * @param[in] 	ds: device secret
 * @param[in] 	dn: device name
 * 
 * @return		status
 */
mx_status emh_ilop_set_key(const char* pk, const char* ps, const char* ds, const char* dn);

/**
 * @brief		Get product key, product secret, device secret, device name form EMW module.
 * 
 * @param[in] 	pk: product key
 * @param[in] 	ps: product secret
 * @param[in] 	ds: device secret
 * @param[in] 	dn: device name
 * 
 * @return		status
 */
mx_status emh_ilop_get_key(char pk[EMH_ILOP_PRODUCT_KEY_MAXLEN], char ps[EMH_ILOP_PRODUCT_SECRET_MAXLEN], char ds[EMH_ILOP_DEVICE_SECRET_MAXLEN], char dn[EMH_ILOP_DEVICE_NAME_MAXLEN]);

/**
 * @brief		Start AWSS Wi-Fi configuration. EMW module start monitor mode
 * @note  		In the monitor mode, only capture packets.
 * 
 * @return		status
 */
mx_status emh_qlink_raw_start(void);

/**
 * @brief		Start AWSS press
 * @note  		In the monitor mode, parsing the air package, connecting the server, binding the phone
 * 
 * @return		status
 */
mx_status emh_qlink_user_start(void);

/**
 * @brief		Start cloud service on module
 * 
 * @return		status
 */
mx_status emh_ilop_service_start(void);

/**
 * @brief		Get current alicloud ILOP service connection status.
 * 
 * @return		connection status
 */
emh_arg_ilop_status_t emh_ilop_get_stauts(void);

/**
 * @brief		To unbind the user, the device must be connected to the service.
 * 
 * @return		mx_status
 */
mx_status emh_ilop_unbind(void);

/**
 * @brief		Stop cloud service on module
 * 
 * @return		mx_status
 */
mx_status emh_ilop_service_stop(void);

/**
 * @brief		Send message to cloud from local
 * 
 * @param[in] 	type: Attributes type. see: emh_arg_ilop_vt_t
 * @param[in] 	attr: outgoing message, format: key,value...
 * 
 * @return		mx_status
 */
mx_status emh_ilop_send_ica_to_cloud(const char* type, char* attr);

/**
 * @brief		Send message to cloud from local
 * 
 * @param[in] 	data: point to the buffer store the outgoing message
 * @param[in] 	len: outgoing message length
 * 
 * @return		mx_status
 */
mx_status emh_ilop_send_raw_to_cloud(uint8_t* data, uint32_t len);

/**
 * @brief		Event hadnle, incomming oob packet prefix.
 * 
 * @param[in] 	attrs: SDS service msg
 * 
 * @return		none
 */
void emh_ilop_event_handler(void);

/**
 * @brief		Event: Alicloud ILOP service connection status is changed
 * 
 * @param[in] 	conn: service connection status
 * 
 * @return		none
 */
MX_WEAK void emh_ev_ilop_connection(emh_arg_ilop_conn_t conn);

/**
 * @brief		Event: Alicloud ILOP service is writing data to local device
 * 
 * @param[in] 	msg: ILOP service msg
 * 
 * @return		none
 */
MX_WEAK void emh_ev_ilop_set_local_attr(emh_ilop_msg* msg);

/** @}*/
/******************************************************************************
 *                              Coud QLINK service
 ******************************************************************************/

/** \addtogroup alicloud_ilop */
/** @{*/

#define EMH_QLINK_PRODUCT_TOKEN_MAXLEN    (16 + 1)
#define EMH_QLINK_ANDLINK_TOKEN_MAXLEN    (16 + 1)
#define EMH_QLINK_DEVICE_TYPE_MAXLEN      (10 + 1)

typedef struct {
    struct {
        const char* product_token; /**< Reference to product TRD document */
        const char* andlink_token; /**< Reference to product TRD document */
        const char* device_type; /**< Reference to product TRD document */
        emh_arg_qlink_format_t format; /**< Reference to product TRD document */
    } product_info;
    struct {
        const char* firmware_version; /**< Reference to product TRD document */
        const char* software_version; /**< Reference to product TRD document */
    } version_info;
} emh_qlink_config_t;

/**
 *  Alicloud ilop service message
 */
typedef struct {
    int32_t len; /**< message length */
    uint8_t* data; /**< point to the buffer store the message */
    emh_arg_qlink_format_t format; /**< message format, ica or raw data */
} emh_qlink_msg_t;

/**
 * @brief		Read product info data from module, write new data if not equal
 * 
 * @param[in] 	config: Product information
 * @param[in] 	force: Force setting
 * 
 * @return		status
 */
mx_status emh_qlink_config(const emh_qlink_config_t* config, bool force);

/**
 * @brief		Start AWSS Wi-Fi configuration. EMW module start monitor mode
 * @note  		In the monitor mode, only capture packets.
 * 
 * @return		status
 */
mx_status emh_qlink_raw_start(void);

/**
 * @brief		Start AWSS press
 * @note  		In the monitor mode, parsing the air package, connecting the server, binding the phone
 * 
 * @return		status
 */
mx_status emh_qlink_user_start(void);

/**
 * @brief		Start cloud service on module
 * 
 * @return		status
 */
mx_status emh_qlink_service_start(void);

/**
 * @brief		Get current alicloud ILOP service connection status.
 * 
 * @return		connection status
 */
emh_arg_ilop_status_t emh_qlink_get_stauts(void);

/**
 * @brief		Stop cloud service on module
 * 
 * @return		mx_status
 */
mx_status emh_qlink_service_stop(void);

/**
 * @brief		Send message to cloud from local
 * 
 * @param[in] 	data: point to the buffer store the outgoing message
 * @param[in] 	len: outgoing message length
 * 
 * @return		mx_status
 */
mx_status emh_qlink_send_json_to_cloud( char *type, uint8_t *data, uint32_t len );

/**
 * @brief		Event hadnle, incomming oob packet prefix.
 * 
 * @param[in] 	attrs: SDS service msg
 * 
 * @return		none
 */
void emh_qlink_event_handler(void);

/**
 * @brief		Event: Alicloud ILOP service connection status is changed
 * 
 * @param[in] 	conn: service connection status
 * 
 * @return		none
 */
MX_WEAK void emh_ev_qlink_connection(emh_arg_qlink_conn_t conn);

/**
 * @brief		Event: Alicloud SDS service is requesting data from local device
 * 
 * @param[in] 	attrs: SDS service msg
 * 
 * @return		none
 */
MX_WEAK void emh_ev_qlink_get_local_attrs(emh_qlink_msg_t* attrs);

/**
 * @brief		Event: Alicloud ILOP service is writing data to local device
 * 
 * @param[in] 	msg: ILOP service msg
 * 
 * @return		none
 */
MX_WEAK void emh_ev_qlink_set_local_attrs(emh_qlink_msg_t* msg);

/** @}*/
/** @}*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
