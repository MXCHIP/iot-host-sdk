/**
 ******************************************************************************
 * @file    alicloud_qlink.h
 * @author  QQ Ding
 * @version V1.0.0
 * @date    3-Step-2018
 * @brief   AliCloud QLINK service functions and framework header file
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

#ifndef _CLOUD_QLINK_H_
#define _CLOUD_QLINK_H_

#include "emh_api.h"

/** \addtogroup protocols */
/** @{*/

/** \addtogroup Alicloud_QLINK_Service */
/** @{*/

/******************************************************************************
 *                                   Macros
 ******************************************************************************/

#define qlink_log(M, ...) MX_LOG(CONFIG_CLOUD_DEBUG, "QLINK", M, ##__VA_ARGS__)
#define qlink_debug_log(M, ...) //MX_LOG(CONFIG_CLOUD_DEBUG, "QLINK_DEBUG", M, ##__VA_ARGS__)

/******************************************************************************
 *                                 Constants
 ******************************************************************************/

#define QLINK_INVALID_HANDLE (-1)

/******************************************************************************
 *                                 Enumerations
 ******************************************************************************/

/** Alicloud QLINK service events */
enum qlink_event_e {
    QLINK_EVENT_WLAN_CONFIG_STARTED, /**< AWS service is started to set wlan and cloud. */
    QLINK_EVENT_WLAN_CONFIG_STARTED_IN_AP_MODE,
    QLINK_EVENT_WLAN_CONNECTED, /**< Device is connected to Wi-Fi access point. */
    QLINK_EVENT_WLAN_DISCONNECTED, /**< Device is disconnected from Wi-Fi access point. */
    QLINK_EVENT_CLOUD_CONNECTED, /**< Alicloud QLINK service is connected */
    QLINK_EVENT_CLOUD_DISCONNECTED, /**< Alicloud QLINK service is disconnected. */
};
typedef uint8_t qlink_event_t; /**< Alicloud QLINK service events */

/** Alicloud sds attribute types, protocol will convert values to SDS JSON packet according to these types */
enum qlink_att_type_e {
    QLINK_ATT_TYPE_BOOL, /**< Boolean value. convert to "0 or "1" */
    QLINK_ATT_TYPE_INT, /**< Integer value. convet to "123" */
    QLINK_ATT_TYPE_FLOAT, /**< Float value.  convet to "123.3" */
    QLINK_ATT_TYPE_STRING, /**< String value. no need to convert */
    QLINK_ATT_TYPE_DATA, /**< Raw data. convert to string with base64*/
    QLINK_ATT_TYPE_NULL,
};
typedef uint8_t qlink_att_type_t; /**< Alicloud sds attribute types (see #qlink_att_type_e) */

/******************************************************************************
 *                               Type Definitions
 ******************************************************************************/

typedef int qlink_attr_handle_t;

/** Alicloud sds attribute value */
typedef union {
    bool boolValue;
    int intValue;
    double floatValue;
    char* stringValue;
    char* dateValue;
} qlink_att_val_t;

/******************************************************************************
 *                             Function Declarations
 ******************************************************************************/

/**
 * Cloud read attribute handler
 *
 * @param value[out] : point to attribute value buffer, application should write the correct value
 *
 * @return status
 */
typedef mx_status (*qlink_read_attr)(qlink_att_val_t* value);

/**
 * Cloud write attribute handler
 *
 * @param[in] value : New attribute vale sent from the SDS cloud
 *
 * @return status
 */
typedef mx_status (*qlink_write_attr)(qlink_att_val_t value);

/******************************************************************************
 *                               Type Definitions
 ******************************************************************************/
/** Cloud qlink attribute description */
typedef struct {
    char* name; /**< Attribute name */
    qlink_att_type_t type; /**< Attribute value type */
    qlink_read_attr read_func; /**< Attribute value read handler, optional if the value is readable*/
    qlink_write_attr write_func; /**< Attribute value write handler, optional if the value is writable */
} qlink_attr_t;

/******************************************************************************
 *                             Function Declarations
 ******************************************************************************/

/**
 * @brief	        Initialize EMW module with product info registered on cloud
 *                  console.
 * 
 * @param[in]       config       : qlink product info registered on cloud console
 *
 * @return      	mx_status
 *
 */
mx_status qlink_init(emh_qlink_config_t *config);

/**
 *
 * @brief        	Cloud qlink service runloop, application should called
 *                  periodically to handle events and transfer data.
 *                  To save power, also can be called when uart data is ready 
 *                  to receive or sensor data is ready to send to cloud
 * @note            Never call this function in event handler
 * 
 * @return      	mx_status
 *
 */
mx_status qlink_runloop(void);

/**
 * 
 * @brief 	Start one key  Wi-Fi configuration. EMW module start monitor mode
 * 
 * @return	mx_status
 *
 */
mx_status qlink_raw_start(void);

/**
 * @brief  Start user Wi-Fi configuration. 
 * @note  		In the monitor mode, parsing the air package, connecting the server, binding the phone
 *              
 * 
 * @return	mx_status
 */
mx_status qlink_user_start(void);

/**
 *
 * @brief	Try to procedure an unbind operation on cloud if connected and 
 *          restore module settings to default.
 * 
 * @return	mx_status
 *
 */
mx_status qlink_restore(void);

#endif //_CLOUD_QLINK_H_

/** @}*/
/** @}*/
