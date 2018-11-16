/**
 ******************************************************************************
 * @file    raw_protocol.c
 * @author  QQ Ding
 * @version V1.0.0
 * @date    3-Step-2018
 * @brief   AliCloud ILOP service functions and raw data parser
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

#include "raw_protocol.h"
#include "alicloud_ilop.h"

/******************************************************************************
 *                                   Macros
 ******************************************************************************/

#define MAX_ATTR 20 /**< attribute max value */

/******************************************************************************
 *                               Type Definitions
 ******************************************************************************/

typedef struct _ilop_raw_attr_t {
    const struct ilop_raw_attr_t* attr_db[MAX_ATTR];
    int changed_handle[MAX_ATTR];
    int num_handles;
} ilop_raw_context_t;

/******************************************************************************
 *                              Variable Definitions
 ******************************************************************************/

static ilop_raw_context_t property_db;
static ilop_raw_context_t event_db;
static ilop_raw_context_t service_db;
static uint32_t seed_id = 0;

/******************************************************************************
 *                              Function Definitions
 ******************************************************************************/

/* Computation and verification */
static uint8_t cal_checksum(uint8_t* buffer, uint16_t length)
{
    int32_t i;
    uint8_t check_sum = 0;

    for (i = 0; i < length; i++) {
        check_sum += buffer[i];
    }

    return check_sum;
}

static uint32_t get_seed_id(void)
{
    if (seed_id == 0xFFFFFFFF)
        seed_id = 0;

    return seed_id++;
}

/* Protocol header initialization */
static void protFrameHeadInit(data_buffer_t* buff, uint8_t cmd)
{
    uint8_t check_sum;
    raw_protocol_head_t* head = (raw_protocol_head_t*)buff->buffer;

    /* Requiring the use of big endian mode transmission */
    head->head = RAW_PROTOCOL_HEAD;
    head->ver = RAW_PROTOCOL_VER;
    head->cmd = cmd;
    head->length = hton16(buff->length - 2);
    check_sum = cal_checksum(buff->buffer, buff->length);
    buff->buffer[buff->length++] = check_sum; /* write check sum */
}

static uint32_t protocol_value_handler(const struct ilop_raw_attr_t* attr, uint8_t* addr, uint32_t length)
{
    uint16_t size = 0;
    ilop_att_val_t value;

    switch (attr->type_id) {
    case TYPE_ID_BOOL: {
        size = 1;
        value.boolValue = *addr;
        break;
    }

    case TYPE_ID_INT8: {
        size = 1;
        value.int8Value = *addr;
        break;
    }
    case TYPE_ID_UINT8: {
        size = 1;
        value.uint8Value = *addr;
        break;
    }

    case TYPE_ID_INT16: {
        size = 2;
        value.int16Value = (uint16_t)*addr;
        value.int16Value = ntoh16(value.int16Value);
        break;
    }

    case TYPE_ID_UINT16: {
        size = 2;
        value.uint16Value = (uint16_t)*addr;
        value.uint16Value = ntoh16(value.uint16Value);
        break;
    }

    case TYPE_ID_INT32: {
        size = 4;
        value.int32Value = (int32_t)*addr;
        value.int32Value = ntoh32(value.int32Value);
        break;
    }

    case TYPE_ID_UINT32: {
        size = 4;
        value.uint32Value = (uint32_t)*addr;
        value.uint32Value = ntoh32(value.uint32Value);
        break;
    }

    case TYPE_ID_FLOAT32: {
        size = 4;
        value.floatValue = (float)*addr;
        value.floatValue = ntoh32(value.floatValue);
        break;
    }

    /*8 byte*/
    case TYPE_ID_DATE:
    case TYPE_ID_UINT64: {
        size = 8;
        value.uint64Value = (uint64_t)*addr;
        value.uint64Value = ntoh64(value.uint64Value);
        break;
    }

    case TYPE_ID_INT64: {
        size = 8;
        value.int64Value = (uint64_t)*addr;
        value.int64Value = ntoh64(value.int64Value);
        break;
    }

    case TYPE_ID_FLOAT64: {
        size = 8;
        value.doubleValue = (uint64_t)*addr;
        value.doubleValue = ntoh64(value.doubleValue);
        break;
    }

    /*string*/
    case TYPE_ID_STRING: {
        size = hton16(*((uint16_t*)addr));
        value.stringValue = (char*)(addr + sizeof(size));
        break;
    }

    default: {
        size = length;
        value.dataValue = (uint8_t*)(addr);
        break;
    }
    }

    if (attr->write_func != NULL) {
        attr->write_func(value, size);
        if (attr->type_id == TYPE_ID_STRING) {
            size += sizeof(size);
        }
    }

    return size;
}

static uint32_t protocol_service_handler(const struct ilop_raw_attr_t* attr, uint8_t* addr, uint32_t size)
{
    ilop_att_val_t value;

    if (attr->write_func != NULL) {
        value.dataValue = addr;
        attr->write_func(value, size);
    }
    return size;
}

static int get_property_thing_index(uint8_t attr_id, uint8_t type_id)
{
    int i, handle = 0;

    for (i = 0; i < property_db.num_handles; i++) {
        if (property_db.attr_db[i] == NULL)
            continue;
        handle = property_db.attr_db[i]->index;

        if ((property_db.attr_db[handle]->attr_id == attr_id) && ((property_db.attr_db[handle]->type_id == type_id))) {
            ilop_log("find property index:%d", handle);
            return handle;
        }
    }

    return -1;
}

static int get_service_thing_index(uint8_t attr_id)
{
    int i, handle = 0;

    ilop_log("search attr_id:%d", attr_id);

    for (i = 0; i < service_db.num_handles; i++) {
        if (service_db.attr_db[i] == NULL)
            continue;
        handle = service_db.attr_db[i]->index;

        if (service_db.attr_db[handle]->attr_id == attr_id) {
            ilop_log("find service index:%d", handle);
            return handle;
        }
    }

    return -1;
}

/* Processing SET requests issued under cloud */
static mx_status protocol_method_handler(uint8_t method, uint8_t* data, uint16_t length)
{
    int32_t offset = 0, index = -1, ret = 0;
    raw_body_format_t* elem;

    while ((length - offset) > 0) {
        if (method == RAW_METHOD_SET) {
            elem = (raw_body_format_t*)(data + offset);
            index = get_property_thing_index(elem->attr_id, elem->type_id);
            if (index < 0) {
                ilop_log("attr_id or type_id err");
                return kGeneralErr;
            }

            offset += sizeof(raw_body_format_t) - 1;
            ret = protocol_value_handler(property_db.attr_db[index], &elem->value, length - 2);
        } else if (method == RAW_METHOD_SERVICE) {
            uint8_t attr_id = *data;
            index = get_service_thing_index(attr_id);
            if (index < 0) {
                ilop_log("attr_id err");
                return kGeneralErr;
            }
            offset += 1;
            ret = protocol_service_handler(service_db.attr_db[index], data + offset, length - 1);
        }
        if (ret == kUnsupportedErr)
            return kNotReadableErr;
        offset += ret;
    }

    return kNoErr;
}

/* Handling data transmitted by server side */
static mx_status protocol_payloa_handler(uint8_t* data, uint16_t length)
{
    raw_payload_data_head_t* payload = (raw_payload_data_head_t*)data;

    switch (payload->method) {
        /* Method of active sending from server side */
        // case RAW_METHOD_GET:
        //     protGetMethodHandler(&payload->data, length - sizeof(raw_payload_data_head_t) + 1);
        //     break;

    case RAW_METHOD_SET:
        ilop_log("raw method set");
        protocol_method_handler(RAW_METHOD_SET, &payload->data, length - sizeof(raw_payload_data_head_t) + 1);
        break;

    case RAW_METHOD_SERVICE:
        ilop_log("raw method service");
        protocol_method_handler(RAW_METHOD_SERVICE, &payload->data, length - sizeof(raw_payload_data_head_t) + 1);
        break;

    default:
        ilop_log("Method(%d) not support\n", payload->method);
        return kParamErr;
    }

    return kNoErr;
}

/* Processing protocol frames */
mx_status raw_protocol_format_handler(uint8_t* data, uint32_t length)
{
    raw_protocol_head_t* head = (raw_protocol_head_t*)data;
    uint8_t cmd = head->cmd;

    switch (cmd) {
    case RAW_DOWN_DATA:
        ilop_log("raw down data len:%d", ntoh16(head->length));
        protocol_payloa_handler(data + RAW_PROTOCOL_REGION_HEAD_LEN,
            ntoh16(head->length) - RAW_PROTOCOL_REGION_LENGTH_FIXED);
        break;

    default:
        ilop_log("Cmd %02x not support\n", cmd);
        break;
    }

    return kNoErr;
}

static void protocol_add_body(const struct ilop_raw_attr_t* attr, data_buffer_t* buf)
{
    ilop_att_val_t value;
    uint32_t len;

    ilop_log("add body attr_id:%d", attr->attr_id);

    raw_body_format_t* body = (raw_body_format_t*)(buf->buffer + buf->length);
    body->attr_id = attr->attr_id;
    body->type_id = attr->type_id;
    buf->length += sizeof(raw_body_format_t) - 1;

    attr->read_func(&value, &len);

    switch (attr->type_id) {
    /*1 byte*/
    case TYPE_ID_BOOL:
    case TYPE_ID_INT8:
    case TYPE_ID_UINT8:
        body->value = value.boolValue;
        buf->length += 1;
        break;

    /*2 byte*/
    case TYPE_ID_INT16:
    case TYPE_ID_UINT16: {
        uint16_t* p = (uint16_t*)&body->value;
        *p = hton16(value.uint16Value);
        buf->length += 2;
        break;
    }

    /*4 byte*/
    case TYPE_ID_INT32:
    case TYPE_ID_UINT32: {
        uint32_t* p = (uint32_t*)&body->value;
        *p = hton32(value.uint32Value);
        buf->length += 4;
        break;
    }

    case TYPE_ID_FLOAT32: {
        float* p = (float*)&body->value;
        *p = hton32(value.floatValue);
        buf->length += 4;
        break;
    }

    /*8 byte*/
    case TYPE_ID_DATE:
    case TYPE_ID_INT64:
    case TYPE_ID_UINT64: {
        uint64_t* p = (uint64_t*)&body->value;
        *p = hton64(value.uint64Value);
        buf->length += 4;
        break;
    }

    case TYPE_ID_FLOAT64: {
        double* p = (double*)&body->value;
        *p = hton32(value.doubleValue);
        buf->length += 4;
        break;
    }

    /*string*/
    case TYPE_ID_STRING: {
        uint8_t* p;
        uint16_t* value_len = (uint16_t*)&body->value;
        *value_len = hton16(len);
        p = &body->value + sizeof(*value_len);
        memcpy(p, value.stringValue, len);
        buf->length += (len + 2);
        break;
    }

    default: {
        uint8_t* p;
        p = &body->value;
        memcpy(p, value.dataValue, len);
        buf->length += len;
        ilop_log("default");
        break;
    }
    }
}

/*To process the report attribute, it must be called in the ilop_runloop function.*/
void ilop_raw_indicate_local_property(void)
{
    int i;
    mx_status err = kNoErr;

    if ((property_db.num_handles == 0))
        return;

    for (i = 0; i < property_db.num_handles; i++) {
        if (property_db.changed_handle[i] != ILOP_INVALID_HANDLE)
            break;
    }
    if (i >= property_db.num_handles)
        return;

    ilop_log("local property");

    int handle;

    data_buffer_t data_buff;
    memset(&data_buff, 0x00, sizeof(data_buffer_t));

    data_buff.length = RAW_PROTOCOL_REGION_HEAD_LEN;

    raw_payload_data_head_t* payload = (raw_payload_data_head_t*)(data_buff.buffer + RAW_PROTOCOL_REGION_HEAD_LEN);
    payload->method = RAW_METHOD_GET;
    payload->id = hton32(get_seed_id());

    data_buff.length += sizeof(raw_payload_data_head_t) - 1;

    for (i = 0; i < property_db.num_handles; i++) {
        if (property_db.changed_handle[i] == ILOP_INVALID_HANDLE)
            continue;

        handle = property_db.changed_handle[i];

        if (property_db.attr_db[handle]->read_func == NULL)
            continue;

        protocol_add_body(property_db.attr_db[handle], &data_buff);
    }

    protFrameHeadInit(&data_buff, RAW_UP_DATA);

    printf("\r\n");
    for (int i = 0; i < data_buff.length; i++) {
        printf("%02X", data_buff.buffer[i]);
    }
    printf("\r\n");

    err = emh_ilop_send_raw_to_cloud(data_buff.buffer, data_buff.length);
    require_noerr(err, exit);

    for (i = 0; i < property_db.num_handles; i++) {
        property_db.changed_handle[i] = ILOP_INVALID_HANDLE;
    }

exit:
    return;
}

/*To process the report events, it must be called in the ilop_runloop function*/
void ilop_raw_indicate_local_event(void)
{
    mx_status err = kNoErr;
    int i, handle = 0;

    if (event_db.num_handles == 0)
        return;

    for (i = 0; i < event_db.num_handles; i++) {
        if (event_db.changed_handle[i] != ILOP_INVALID_HANDLE) {
            handle = event_db.changed_handle[i];
            break;
        }
    }
    if (i >= event_db.num_handles)
        return;

    ilop_log("local event");

    data_buffer_t data_buff;
    memset(&data_buff, 0x00, sizeof(data_buffer_t));

    data_buff.length = RAW_PROTOCOL_REGION_HEAD_LEN;

    raw_payload_data_head_t* payload = (raw_payload_data_head_t*)(data_buff.buffer + RAW_PROTOCOL_REGION_HEAD_LEN);
    payload->method = RAW_METHOD_EVENT;
    payload->id = hton32(get_seed_id());

    data_buff.length += sizeof(raw_payload_data_head_t) - 1;

    if (event_db.attr_db[handle]->read_func == NULL)
        goto exit;

    protocol_add_body(event_db.attr_db[handle], &data_buff);

    protFrameHeadInit(&data_buff, RAW_UP_DATA);

    printf("\r\n");
    for (int i = 0; i < data_buff.length; i++) {
        printf("%02X", data_buff.buffer[i]);
    }
    printf("\r\n");

    err = emh_ilop_send_raw_to_cloud(data_buff.buffer, data_buff.length);
    require_noerr(err, exit);

    event_db.changed_handle[handle] = ILOP_INVALID_HANDLE;

exit:
    return;
}

mx_status ilop_raw_attr_init(attr_type_t attr_type, uint8_t attr_num_handls)
{
    int i;

    for (i = 0; i < MAX_ATTR; i++) {
        if (attr_type == ATTR_TYPE_PROPERTY) {
            property_db.attr_db[i] = NULL;
        } else if (attr_type == ATTR_TYPE_EVENT) {
            event_db.attr_db[i] = NULL;
        } else if (attr_type == ATTR_TYPE_SERVICE) {
            service_db.attr_db[i] = NULL;
        }
    }

    for (i = 0; i < MAX_ATTR; i++) {
        if (attr_type == ATTR_TYPE_PROPERTY) {
            property_db.changed_handle[i] = ILOP_INVALID_HANDLE;
        } else if (attr_type == ATTR_TYPE_EVENT) {
            event_db.changed_handle[i] = ILOP_INVALID_HANDLE;
        } else if (attr_type == ATTR_TYPE_SERVICE) {
            service_db.changed_handle[i] = ILOP_INVALID_HANDLE;
        }
    }

    if (attr_type == ATTR_TYPE_PROPERTY) {
        property_db.num_handles = attr_num_handls;
    } else if (attr_type == ATTR_TYPE_EVENT) {
        event_db.num_handles = attr_num_handls;
    } else if (attr_type == ATTR_TYPE_SERVICE) {
        service_db.num_handles = attr_num_handls;
    }

    return kNoErr;
}

mx_status ilop_raw_attr_register(attr_type_t attr_type, const struct ilop_raw_attr_t* attr)
{
    int handle = 0;

    if (attr_type == ATTR_TYPE_PROPERTY) {
        if (property_db.num_handles > MAX_ATTR)
            return kRangeErr;

        handle = attr->index;
        property_db.attr_db[handle] = attr;
    } else if (attr_type == ATTR_TYPE_EVENT) {
        if (event_db.num_handles > MAX_ATTR)
            return kRangeErr;

        handle = attr->index;
        event_db.attr_db[handle] = attr;
    } else if (attr_type == ATTR_TYPE_SERVICE) {
        if (service_db.num_handles > MAX_ATTR)
            return kRangeErr;

        handle = attr->index;
        service_db.attr_db[handle] = attr;
    }

    return kNoErr;
}

void ilop_raw_attr_indicate_by_handle(attr_type_t attr_type, int handle)
{
    if (attr_type == ATTR_TYPE_PROPERTY) {
        if (property_db.attr_db[handle] == NULL)
            return;
        if (property_db.attr_db[handle]->index == handle) {
            if (property_db.changed_handle[handle] == handle)
                return;
            property_db.changed_handle[handle] = handle;
        }
    } else if (attr_type == ATTR_TYPE_EVENT) {
        if (event_db.attr_db[handle] == NULL)
            return;
        if (event_db.attr_db[handle]->index == handle) {
            if (event_db.changed_handle[handle] == handle)
                return;
            event_db.changed_handle[handle] = handle;
        }
    }
}

