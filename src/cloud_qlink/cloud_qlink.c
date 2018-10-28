/**
 ******************************************************************************
 * @file    alicloud_qlink.c
 * @author  QQ Ding
 * @version V1.0.0
 * @date    3-Step-2018
 * @brief   AliCloud qlink service functions and framework
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

#include "cloud_qlink.h"
#include "ATCmdParser/ATCmdParser.h"
#include "mx_hal.h"

/******************************************************************************
 *                                 Enumerations
 ******************************************************************************/

typedef enum {
    eState_M1_initialize = 1, /**< State machine: Reset and initialize module */
    eState_M2_provision  = 2, /**< State machine: Waiting for WiFi config */
    eState_M2_provision_ap = 3,/** connect to WiFi successfully, waiting for cloud connection  */
    eState_M3_normal = 4, /**< State machine: Connected to cloud, application running  */
    eState_M4_disconnected = 5, /**< State machine: Disconnect to cloud, all data transmission should stop */
    eState_M5_fault = 6, /**< State machine: Drop in an unexpected error */
} qlink_device_state_e;

/******************************************************************************
 *                               Type Definitions
 ******************************************************************************/

typedef struct {
    qlink_device_state_e device_state; /**< Device state machine. */
    emh_arg_qlink_conn_t cloud_state; /**< QLINK service connection state. */
} qlink_context_t;

/******************************************************************************
 *                              Variable Definitions
 ******************************************************************************/

static emh_qlink_config_t* qlink_config = NULL;
static qlink_context_t qlink_context;


/******************************************************************************
 *                           Static Function Declarations
 ******************************************************************************/

/******************************************************************************
 *                              Function Definitions
 ******************************************************************************/

MX_WEAK void qlink_event_handler(qlink_event_t event)
{
    switch (event) {
    case QLINK_EVENT_WLAN_CONFIG_STARTED: {
        qlink_log("Wi-Fi config....");
        break;
    }
    case QLINK_EVENT_WLAN_CONFIG_STARTED_IN_AP_MODE: {
        qlink_log("WiFi config in AP mode......");
        break;
    }
    case QLINK_EVENT_WLAN_CONNECTED: {
        qlink_log("Wi-Fi connected");
        break;
    }
    case QLINK_EVENT_WLAN_DISCONNECTED: {
        qlink_log("Wi-Fi disconnected");
        break;
    }
    case QLINK_EVENT_CLOUD_CONNECTED: {
        qlink_log("Cloud connected");
        break;
    }
    case QLINK_EVENT_CLOUD_DISCONNECTED: {
        qlink_log("Cloud disconnected");
        break;
    }
    }
}

mx_status qlink_init(emh_qlink_config_t *config)
{
    mx_status err = kNoErr;

    qlink_context.device_state = eState_M1_initialize;
    qlink_context.cloud_state = EMH_ARG_QLINK_CONN_CLOUD_DISCONNECTED;

    qlink_config = config;

    err = emh_init();
    require_noerr(err, exit);

exit:
    return err;
}

static mx_status _handle_state_initialize(void)
{
    mx_status err = kNoErr;

    qlink_log("FW version: %s", emh_module_get_fw_version());

    err = emh_qlink_config(qlink_config, true);
    require_noerr(err, exit);

    qlink_context.device_state = eState_M2_provision;

exit:
    return err;
}

mx_status qlink_runloop(void)
{
    mx_status err = kNoErr;

    switch (qlink_context.device_state) {
        case eState_M1_initialize: {
            err = _handle_state_initialize();
            require_noerr_action(err, exit, qlink_context.device_state = eState_M5_fault);
            break;
        }

        case eState_M2_provision: {//waitint for connect to ap
            break;
        }

        case eState_M2_provision_ap:{  //connect to ap successfully, buf fail to connect to cloud
            qlink_context.device_state = eState_M4_disconnected;
            emh_qlink_service_start();
            break;
        }

        case eState_M3_normal:
        case eState_M4_disconnected: {
            break;
        }

        case eState_M5_fault: {
            break;
        }

        default:
            qlink_log("STATE ERROR");
            err = kNoErr;
    }

    emh_runloop();
exit:
    return err;
}

void emh_ev_wlan(emh_arg_wlan_ev_t event)
{
    qlink_debug_log("Wlan event: %s", emh_arg_for_type(EMH_ARG_WLAN_EV, event));
    if (event == EMH_ARG_WLAN_EV_STA_CONNECTED) {
        qlink_event_handler(QLINK_EVENT_WLAN_CONNECTED);
        qlink_context.device_state = eState_M2_provision_ap;
    } else if (event == EMH_ARG_WLAN_EV_STA_DISCONNECTED) {
        qlink_event_handler(QLINK_EVENT_WLAN_DISCONNECTED);
    } else if (event == EMH_ARG_WLAN_EV_UAP_ESTABLISHED) {
        qlink_event_handler(QLINK_EVENT_WLAN_CONFIG_STARTED_IN_AP_MODE);
    }
}

void emh_ev_qlink_connection(emh_arg_qlink_conn_t conn)
{
    qlink_debug_log("AliCloud event: %s", emh_arg_for_type(EMH_ARG_QLINK_CONN, conn));

    qlink_context.cloud_state = conn;

    if (conn == EMH_ARG_QLINK_CONN_CLOUD_CONNECTED) {

        /* EMW3080 has unexpected uart data lost after cloud connection, should fix in future */
        mx_hal_delay_ms(1000);
        qlink_event_handler(QLINK_EVENT_CLOUD_CONNECTED);

        qlink_context.device_state = eState_M3_normal;
    }

    if (conn == EMH_ARG_QLINK_CONN_CLOUD_DISCONNECTED) {
        qlink_event_handler(QLINK_EVENT_CLOUD_DISCONNECTED);
        qlink_context.device_state = eState_M4_disconnected;
    }
}

MX_WEAK void emh_ev_qlink_get_local_attrs(emh_qlink_msg_t* msg)
{
    qlink_log("please redeclare this function in application!!!");
}

MX_WEAK int emh_ev_qlink_set_local_attrs(emh_qlink_msg_t* msg)
{
    return 0;
}

mx_status qlink_raw_start(void)
{
    return emh_qlink_raw_start();
}

mx_status qlink_user_start(void)
{
    return emh_qlink_user_start();
}

mx_status qlink_send_data( char *type, uint8_t *data, uint32_t len )
{
    return emh_qlink_send_json_to_cloud( type, data, len );
}