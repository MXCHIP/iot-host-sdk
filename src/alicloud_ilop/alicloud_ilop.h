/**
 ******************************************************************************
 * @file    alicloud_ilop.h
 * @author  QQ Ding
 * @version V1.0.0
 * @date    3-Step-2018
 * @brief   AliCloud ILOP service functions and framework header file
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

#ifndef _ALICLOUD_ILOP_H_
#define _ALICLOUD_ILOP_H_

#include "emh_api.h"
#include "ica_protocol.h"
#include "raw_protocol.h"

/** \addtogroup protocols */
/** @{*/

/** \addtogroup Alicloud_ILOP_Service */
/** @{*/

/******************************************************************************
 *                                   Macros
 ******************************************************************************/

#define ilop_log(M, ...) MX_LOG(CONFIG_CLOUD_DEBUG, "ILOP", M, ##__VA_ARGS__)

/******************************************************************************
 *                                 Constants
 ******************************************************************************/

#define ILOP_INVALID_HANDLE (-1)

/******************************************************************************
 *                                 Enumerations
 ******************************************************************************/

/** Alicloud ilop service events */
enum ilop_event_e {
    ILOP_EVENT_WLAN_CONFIG_STARTED, /**< AWS service is started to set wlan and cloud. */
    ILOP_EVENT_WLAN_CONNECTED, /**< Device is connected to Wi-Fi access point. */
    ILOP_EVENT_WLAN_DISCONNECTED, /**< Device is disconnected from Wi-Fi access point. */
    ILOP_EVENT_CLOUD_CONNECTED, /**< Alicloud ilop service is connected */
    ILOP_EVENT_CLOUD_DISCONNECTED, /**< Alicloud ilop service is disconnected. */
};
typedef uint8_t ilop_event_t; /**< Alicloud ilop service events */

/******************************************************************************
 *                               Type Definitions
 ******************************************************************************/

typedef struct _ilop_device_key_t {
    char product_key[EMH_ILOP_PRODUCT_KEY_MAXLEN];
    char product_secret[EMH_ILOP_PRODUCT_SECRET_MAXLEN];
    char device_name[EMH_ILOP_DEVICE_NAME_MAXLEN];
    char device_secret[EMH_ILOP_DEVICE_SECRET_MAXLEN];
} ilop_device_key_t;

/******************************************************************************
 *                             Function Declarations
 ******************************************************************************/

/**
 * @brief	        Initialize EMW module with product info registered on cloud
 *                  console.
 * 
 * @param[in]       config       : ILOP product info registered on cloud console
 *
 * @return      	mx_status
 *
 */
mx_status ilop_init(const emh_ilop_config_t* config);

/**
 *
 * @brief        	Alicould ilop service runloop, application should called
 *                  periodically to handle events and transfer data.
 *                  To save power, also can be called when uart data is ready 
 *                  to receive or sensor data is ready to send to cloud
 * @note            Never call this function in event handler
 * 
 * @return      	mx_status
 *
 */
mx_status ilop_runloop(void);

/**
 * 
 * @brief 	Start AWSS Wi-Fi configuration. EMW module start monitor mode
 * 
 * @return	mx_status
 *
 */
mx_status ilop_awss_start(void);

/**
 * @brief  Start AWSS press, called after ilop_awss_start()
 * @note  		In the monitor mode, parsing the air package, connecting the server, binding the phone
 *              
 * 
 * @return	mx_status
 */
mx_status ilop_awss_press(void);

/**
 *
 * @brief	Try to procedure an unbind operation on cloud if connected and 
 *          restore module settings to default.
 * 
 * @return	mx_status
 *
 */
mx_status ilop_restore(void);

/**
 * @brief		Set product key, product secret, device secret, device name is used to connect to ILOP service.
 * @note  		The default key is stored in EMW module, use this function to write the new key.
 * 
 * @param[in] 	key: see ilop_device_key_t
 * 
 * @return		status
 */
mx_status ilop_set_device_key(const ilop_device_key_t* key);

/******************************************************************************
 *                              ICA Data Function
 ******************************************************************************/

/**
 * @brief        	ILOP ICA Attribute initialization
 * 
 * @param[in]       attr_num_handls	: attribute handle Maximum value
 * @return      	mx_status
 *
 */
mx_status ilop_ica_attr_init(uint8_t attr_num_handls);

/**
 * @brief        	ILOP ICA Attribute register to database
 * 
 * @param[in]       attr 	: Alicloud ilop attribute description
 * @return      	mx_status
 *
 */
mx_status ilop_ica_attr_register(const struct ilop_ica_attr_t* attr);

/**
 *
 * @brief           Prepare to send attribute to cloud, attribute value will 
 *                  be read and send in ILOP runloop ilop_runloop
 * @note            This function do not send data use AT command, so it can 
 * 	                be called inside the event handler. #ilop_runloop will 
 *                  do the actually read and send function.
 * 
 * @param[in]       handle	: attribute handle
 * @return          none
 *
 */
void ilop_ica_attr_indicate_by_handle(int handle);

/******************************************************************************
 *                              RAW Data Function
 ******************************************************************************/

/**
 * @brief        	ILOP RAW Attribute initialization
 * 
 * @param[in]       attr_type       : attribute type
 * @param[in]       attr_num_handls	: attribute handle Maximum value 
 * @return      	mx_status
 *
 */
mx_status ilop_raw_attr_init(attr_type_t attr_type, uint8_t attr_num_handls);

/**
 * @brief        	ILOP RAW Attribute register to database
 * 
 * @param[in]       attr_type       : attribute type
 * @param[in]       attr 	        : Alicloud ilop attribute description
 * @return      	mx_status
 *
 */
mx_status ilop_raw_attr_register(attr_type_t attr_type, const struct ilop_raw_attr_t* attr);

/**
 *
 * @brief           Prepare to send attribute to cloud, attribute value will 
 *                  be read and send in ILOP runloop ilop_runloop
 * @note            This function do not send data use AT command, so it can 
 * 	                be called inside the event handler. #ilop_runloop will 
 *                  do the actually read and send function.
 * 
 * @param[in]       attr_type       : attribute type
 * @param[in]       handle	        : attribute handle, see #ilop_raw_attr_t
 * @return          none
 *
 */
void ilop_raw_attr_indicate_by_handle(attr_type_t attr_type, int handle);

#endif //_ALICLOUD_ILOP_H_

/** @}*/
/** @}*/
