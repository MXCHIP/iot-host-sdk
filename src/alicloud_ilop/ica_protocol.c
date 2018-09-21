/**
 ******************************************************************************
 * @file    ica_protocol.c
 * @author  QQ Ding
 * @version V1.0.0
 * @date    3-Step-2018
 * @brief   AliCloud ILOP service functions and ica data parser
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

#include "ica_protocol.h"
#include "alicloud_ilop.h"

/******************************************************************************
 *                                   Macros
 ******************************************************************************/

#define MAX_ATTR 20 /**< attribute max value */

/******************************************************************************
 *                               Type Definitions
 ******************************************************************************/

typedef struct _ilop_ica_attr_t {
    const struct ilop_ica_attr_t* attr_db[MAX_ATTR]; /**< attribute db */
    int changed_handle[MAX_ATTR]; /**< ILOP characteristics numbers registered on cloud */
    int num_handles; /**< MAX, ILOP characteristics numbers registered on cloud */
} ilop_ica_context_t;

/******************************************************************************
 *                              Variable Definitions
 ******************************************************************************/

static ilop_ica_context_t ica_db;

/******************************************************************************
 *                              Function Definitions
 ******************************************************************************/

void ica_protocol_format_handler(char* type, char* value, char* key)
{
    int i, handle = 0;

    for (i = 0; i < ica_db.num_handles; i++) {
        if (ica_db.attr_db[i] == NULL)
            continue;
        handle = ica_db.attr_db[i]->index;

        if (strcmp(value, ica_db.attr_db[handle]->name) == 0) {
            if (emh_arg_for_arg(EMH_ARG_ILOP_VT, type) == ica_db.attr_db[handle]->type) {
                if (ica_db.attr_db[handle]->write_func != NULL) {
                    ica_db.attr_db[handle]->write_func(key);
                }
                break;
            }
        }
    }
}

void ilop_ica_indicate_local_atts(void)
{
    mx_status err = kNoErr;
    int i, handle = 0;

    if (ica_db.num_handles == 0)
        return;

    for (i = 0; i < ica_db.num_handles; i++) {
        if (ica_db.changed_handle[i] != ILOP_INVALID_HANDLE) {
            handle = ica_db.changed_handle[i];
            break;
        }
    }
    if (i >= ica_db.num_handles)
        return;

    char prop_buff[512] = { 0 };
    char event_buff[100] = { 0 };
    const char* type;
    char* value;
    int prop_index = 0, event_index = 0;

    for (i = 0; i < ica_db.num_handles; i++) {
        if (ica_db.changed_handle[i] == ILOP_INVALID_HANDLE)
            continue;

        handle = ica_db.changed_handle[i];

        if (ica_db.attr_db[handle]->read_func == NULL)
            continue;

        if (ica_db.attr_db[handle]->type == EMH_ARG_ILOP_VT_PROPERTY) {
            err = ica_db.attr_db[handle]->read_func(&value);
            require_noerr(err, exit);
            prop_index += sprintf(prop_buff + prop_index, "%s,%s,", ica_db.attr_db[handle]->name, value);
        }

        if (ica_db.attr_db[handle]->type == EMH_ARG_ILOP_VT_EVENT) {
            err = ica_db.attr_db[handle]->read_func(&value);
            require_noerr(err, exit);
            event_index += sprintf(event_buff + event_index, "%s,%s,", ica_db.attr_db[handle]->name, value);
        }
    }

    if (prop_index > 0) {
        prop_buff[prop_index - 1] = '\0';
        ilop_log("Send Property to cloud %d bytes > %s", prop_index - 1, prop_buff);
        type = emh_arg_for_type(EMH_ARG_ILOP_VT, EMH_ARG_ILOP_VT_PROPERTY);
        err = emh_ilop_send_ica_to_cloud(type, prop_buff);
        require_noerr(err, exit);
    }

    if (event_index > 0) {
        event_buff[event_index - 1] = '\0';
        ilop_log("Send Event to cloud %d bytes > %s", event_index - 1, event_buff);
        type = emh_arg_for_type(EMH_ARG_ILOP_VT, EMH_ARG_ILOP_VT_EVENT);
        err = emh_ilop_send_ica_to_cloud(type, event_buff);
        require_noerr(err, exit);
    }

exit:

    for (i = 0; i < ica_db.num_handles; i++) {
        ica_db.changed_handle[i] = ILOP_INVALID_HANDLE;
    }
    return;
}

mx_status ilop_ica_attr_init(uint8_t attr_num_handls)
{
    int i;

    for (i = 0; i < MAX_ATTR; i++) {
        ica_db.attr_db[i] = NULL;
    }

    for (i = 0; i < MAX_ATTR; i++) {
        ica_db.changed_handle[i] = ILOP_INVALID_HANDLE;
    }

    ica_db.num_handles = attr_num_handls;

    return kNoErr;
}

mx_status ilop_ica_attr_register(const struct ilop_ica_attr_t* attr)
{
    int handle = 0;

    if (ica_db.num_handles > MAX_ATTR)
        return kRangeErr;

    handle = attr->index;
    ica_db.attr_db[handle] = attr;
    return kNoErr;
}

void ilop_ica_attr_indicate_by_handle(int handle)
{
    if (ica_db.attr_db[handle] == NULL)
        return;
    if (ica_db.attr_db[handle]->index == handle) {
        if (ica_db.changed_handle[handle] == handle)
            return;
        ica_db.changed_handle[handle] = handle;
    }
}
