/**
 ******************************************************************************
 * @file    alicloud_ilop.c
 * @author  QQ Ding
 * @version V1.0.0
 * @date    3-Step-2018
 * @brief   AliCloud ILOP service functions and framework
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
#include "ATCmdParser/ATCmdParser.h"
#include "mx_hal.h"

/******************************************************************************
 *                                 Enumerations
 ******************************************************************************/

typedef enum {
    Ilop_eState_M1_initialize = 1, /**< State machine: Reset and initialize module */
    Ilop_eState_M2_provision = 2, /**< State machine: Waiting for WiFi config and cloud connection */
    Ilop_eState_M3_normal = 3, /**< State machine: Connected to cloud, application running  */
    Ilop_eState_M4_disconnected = 4, /**< State machine: Disconnect to cloud, all data transmission should stop */
    Ilop_eState_M5_fault = 5, /**< State machine: Drop in an unexpected error */
} ilop_device_state_e;

/******************************************************************************
 *                               Type Definitions
 ******************************************************************************/

typedef struct {
    ilop_device_state_e device_state; /**< Device state machine. */
    emh_arg_ilop_conn_t cloud_state; /**< SDS service connection state. */
} ilop_context_t;

/******************************************************************************
 *                              Variable Definitions
 ******************************************************************************/

static const ilop_device_key_t* device_key = NULL;
static emh_ilop_config_t* ilop_config = NULL;
static ilop_context_t context;

#ifdef ILOP_USE_ICA
extern void ilop_ica_indicate_local_atts(void);
extern void ica_protocol_format_handler(char* type, char* value, char* key);
#endif

#ifdef ILOP_USE_RAW
mx_status raw_protocol_format_handler(uint8_t* data, uint32_t length);
void ilop_raw_indicate_local_property(void);
void ilop_raw_indicate_local_event(void);
#endif

/******************************************************************************
 *                              Function Definitions
 ******************************************************************************/

MX_WEAK void ilop_event_handler(ilop_event_t event)
{
    switch (event) {
    case ILOP_EVENT_WLAN_CONFIG_STARTED: {
        ilop_log("Wi-Fi config....");
        break;
    }
    case ILOP_EVENT_WLAN_CONNECTED: {
        ilop_log("Wi-Fi connected");
        break;
    }
    case ILOP_EVENT_WLAN_DISCONNECTED: {
        ilop_log("Wi-Fi disconnected");
        break;
    }
    case ILOP_EVENT_CLOUD_CONNECTED: {
        ilop_log("Cloud connected");
        break;
    }
    case ILOP_EVENT_CLOUD_DISCONNECTED: {
        ilop_log("Cloud disconnected");
        break;
    }
    }
}

static void ilop_device_key_printf(const char* pk, const char* ps, const char* ds, const char* dn)
{
    ilop_log("product key    :[%s]", pk);
    ilop_log("product secret :[%s]", ps);
    ilop_log("device secret  :[%s]", ds);
    ilop_log("device name    :[%s]", dn);
}

mx_status ilop_init(const emh_ilop_config_t* config)
{
    mx_status err = kNoErr;

    context.device_state = Ilop_eState_M1_initialize;
    context.cloud_state = EMH_ARG_ILOP_CONN_CLOUD_DISCONNECTED;

    ilop_config = (emh_ilop_config_t*)config;

    err = emh_init();
    require_noerr(err, exit);

exit:
    return err;
}

mx_status ilop_set_device_key(const ilop_device_key_t* key)
{
    device_key = key;
    return kNoErr;
}

static mx_status _handle_state_initialize(void)
{
    mx_status err = kNoErr;
    char ssid[33] = { 0 }, pwd[33];
    char pk[EMH_ILOP_PRODUCT_KEY_MAXLEN] = { 0 };
    char ps[EMH_ILOP_PRODUCT_SECRET_MAXLEN] = { 0 };
    char ds[EMH_ILOP_DEVICE_SECRET_MAXLEN] = { 0 };
    char dn[EMH_ILOP_DEVICE_NAME_MAXLEN] = { 0 };

    ilop_log("FW version: %s", emh_module_get_fw_version());

    err = emh_ilop_config(ilop_config, false);
    require_noerr(err, exit);

    err = emh_ilop_get_key(pk, ps, ds, dn);
    require_noerr(err, exit);

    if ((strlen(pk) == 0) || (strlen(ps) == 0) || (strlen(ds) == 0) || (strlen(dn) == 0)) {
        if (device_key != NULL) {
            err = emh_ilop_set_key(device_key->product_key, device_key->product_secret, device_key->device_secret, device_key->device_name);
            require_noerr(err, exit);
            ilop_device_key_printf(device_key->product_key, device_key->product_secret, device_key->device_secret, device_key->device_name);
        } else {
            ilop_log("not find device key...");
            err = kIDErr;
            require_noerr(err, exit);
        }
    } else if (device_key != NULL) {
        if ((strcmp(pk, device_key->product_key) != 0) || (strcmp(ps, device_key->product_secret) != 0) || (strcmp(ds, device_key->device_secret) != 0) || (strcmp(dn, device_key->device_name) != 0)) {
            ilop_log("device key different, need to set");
            err = emh_ilop_set_key(device_key->product_key, device_key->product_secret, device_key->device_secret, device_key->device_name);
            require_noerr(err, exit);
            ilop_device_key_printf(device_key->product_key, device_key->product_secret, device_key->device_secret, device_key->device_name);
        } else {
            ilop_log("device key the same");
            ilop_device_key_printf(pk, ps, ds, dn);
        }
    }

    /* Start alisds daemon service*/
    err = emh_ilop_service_start();
    require_noerr(err, exit);

    /* Check Wi-Fi configuration */
    err = emh_wlan_get_para(ssid, pwd);
    require_noerr(err, exit);

    if (strlen(ssid)) {
        ilop_log("SSID: %s, PWD: %s", ssid, pwd);
        if (EMH_ARG_ILOP_STATUS_CONNECTED == emh_ilop_get_stauts()) {
            ilop_log("Alicloud connected.");
            mx_hal_delay_ms(200);
            context.device_state = Ilop_eState_M3_normal;
        } else {
            ilop_log("Waiting for Alicloud connection");
            context.device_state = Ilop_eState_M4_disconnected;
        }
    } else {
        ilop_log("Wlan unconfigured, start config mode");

        /* Start alisds Wi-Fi configuration */
        err = emh_ilop_awss_start();
        require_noerr(err, exit);

        ilop_event_handler(ILOP_EVENT_WLAN_CONFIG_STARTED);
        context.device_state = Ilop_eState_M2_provision;
    }

exit:
    return err;
}

mx_status ilop_runloop(void)
{
    mx_status err = kNoErr;

    if (context.device_state == Ilop_eState_M3_normal) {
#ifdef ILOP_USE_ICA
        ilop_ica_indicate_local_atts();
#endif

#ifdef ILOP_USE_RAW
        ilop_raw_indicate_local_event();
        ilop_raw_indicate_local_property();
#endif
    }

    switch (context.device_state) {
    case Ilop_eState_M1_initialize: {
        err = _handle_state_initialize();
        require_noerr_action(err, exit, context.device_state = Ilop_eState_M5_fault);
        break;
    }

    case Ilop_eState_M2_provision: {
        break;
    }

    case Ilop_eState_M3_normal:
    case Ilop_eState_M4_disconnected: {
        break;
    }

    case Ilop_eState_M5_fault: {
        break;
    }

    default:
        ilop_log("STATE ERROR");
        err = kNoErr;
    }

    emh_runloop();
exit:
    return err;
}

void emh_ev_wlan(emh_arg_wlan_ev_t event)
{
    ilop_log("Wlan event: %s", emh_arg_for_type(EMH_ARG_WLAN_EV, event));
    if (event == EMH_ARG_WLAN_EV_STA_CONNECTED) {
        ilop_event_handler(ILOP_EVENT_WLAN_CONNECTED);
    } else if (event == EMH_ARG_WLAN_EV_STA_DISCONNECTED) {
        ilop_event_handler(ILOP_EVENT_WLAN_DISCONNECTED);
    }
}

void emh_ev_ilop_connection(emh_arg_alisds_conn_t conn)
{
    ilop_log("AliCloud event: %s", emh_arg_for_type(EMH_ARG_ILOP_CONN, conn));

    context.cloud_state = conn;

    if (conn == EMH_ARG_ILOP_CONN_CLOUD_CONNECTED) {

        /* EMW3080 has unexpected uart data lost after cloud connection, should fix in future */
        mx_hal_delay_ms(1000);
        ilop_event_handler(ILOP_EVENT_CLOUD_CONNECTED);

        context.device_state = Ilop_eState_M3_normal;
    }

    if (conn == EMH_ARG_ILOP_CONN_CLOUD_DISCONNECTED) {
        ilop_event_handler(ILOP_EVENT_CLOUD_DISCONNECTED);
        context.device_state = Ilop_eState_M4_disconnected;
    }
}

void emh_ev_ilop_set_local_attr(emh_ilop_msg* msg)
{
#define ARG_LIST_NUM 13
    int num;
    char* arg_list[ARG_LIST_NUM];

    if (msg->format == EMH_ARG_ILOP_FORMAT_ICA) {
        ilop_log("Set local attrs format ICA");
        num = ATCmdParser_analyse_args((char*)msg->data, arg_list, ARG_LIST_NUM);
        if ((num < 3) || (num > ARG_LIST_NUM)) {
            ilop_log("ERR analyse args num: %d", num);
            goto exit;
        }
#ifdef ILOP_USE_ICA
        for (int i = 1; i < num; i += 2) {
            ica_protocol_format_handler(arg_list[0], arg_list[i], arg_list[i + 1]);
        }
#endif
    } else if (msg->format == EMH_ARG_ILOP_FORMAT_RAW) {
        ilop_log("Set local attrs format RAW");
        printf("\r\n");
        for (int i = 0; i < msg->len; i++) {
            printf("%02X", msg->data[i]);
        }
        printf("\r\n");
#ifdef ILOP_USE_RAW
        raw_protocol_format_handler(msg->data, msg->len);
#endif
    }

exit:
    return;
}

mx_status ilop_awss_start(void)
{
    mx_status err;

    err = emh_ilop_service_stop();
    require_noerr(err, exit);

    err = emh_module_restore_settings();
    require_noerr(err, exit);

    context.device_state = Ilop_eState_M1_initialize;
exit:
    return err;
}

mx_status ilop_awss_press(void)
{
    if (context.device_state == Ilop_eState_M2_provision) {
        return emh_ilop_awss_press();
    }

    return kGeneralErr;
}

mx_status ilop_restore(void)
{
    mx_status err;
    if (context.device_state == Ilop_eState_M3_normal) {
        err = emh_ilop_unbind();
        require_noerr(err, exit);
    }

    err = emh_module_restore_settings();
    require_noerr(err, exit);

    context.device_state = Ilop_eState_M1_initialize;
exit:
    return err;
}
