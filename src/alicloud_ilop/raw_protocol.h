/**
 ******************************************************************************
 * @file    raw_protocol.h
 * @author  QQ Ding
 * @version V1.0.0
 * @date    3-Step-2018
 * @brief   AliCloud ILOP Raw Data parser header file
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

#ifndef __RAW_PROTOCOL_H__
#define __RAW_PROTOCOL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "mx_common.h"

/******************************************************************************
 *                                 Enumerations
 ******************************************************************************/

typedef enum{
    ATTR_TYPE_PROPERTY,
    ATTR_TYPE_EVENT,
    ATTR_TYPE_SERVICE,
}attr_type_t;

typedef enum {
    /* Basic data type */
    TYPE_ID_BOOL = 0, /* Boolean: 0 or 1 */
    TYPE_ID_INT8 = 1, /* Integer: -128 ~ 127 */
    TYPE_ID_UINT8 = 2, /* Integer: 0 ~ 255 */
    TYPE_ID_INT16 = 3, /* Integer: -32768 ~ 32767 */
    TYPE_ID_UINT16 = 4, /* Integer 0 ~ 65535 */
    TYPE_ID_INT32 = 5, /* Integer */
    TYPE_ID_UINT32 = 6,
    TYPE_ID_INT64 = 7,
    TYPE_ID_UINT64 = 8,
    TYPE_ID_FLOAT32 = 9, /* Float */
    TYPE_ID_FLOAT64 = 10,
    TYPE_ID_STRING = 11, /* String */
    TYPE_ID_DATE = 12, /* DATE */

    /* Extended data type */
    TYPE_ID_ARRAY_BOOL = 16,
    TYPE_ID_ARRAY_INT8 = 17,
    TYPE_ID_ARRAY_UINT8 = 18,
    TYPE_ID_ARRAY_INT16 = 19,
    TYPE_ID_ARRAY_UINT16 = 20,
    TYPE_ID_ARRAY_INT32 = 21,
    TYPE_ID_ARRAY_UINT32 = 22,
    TYPE_ID_ARRAY_INT64 = 23,
    TYPE_ID_ARRAY_UINT64 = 24,
    TYPE_ID_ARRAY_FLOAT32 = 25,
    TYPE_ID_ARRAY_FLOAT64 = 26,
    TYPE_ID_ARRAY_STRING = 27,

    NUM_TYPE_ID
} data_type_t;

typedef union {
    bool boolValue;
    uint8_t uint8Value;
    int8_t int8Value;
    uint16_t uint16Value;
    int16_t int16Value;
    uint32_t uint32Value;
    int32_t int32Value;
    float floatValue;
    double doubleValue;
    uint64_t uint64Value;
    int64_t int64Value;
    char* stringValue;
    uint8_t* dataValue;
} ilop_att_val_t;

/**
 * Cloud read attribute handler
 *
 * @param value[out] : point to attribute value buffer, application should write the correct value
 *
 * @return status
 */
typedef mx_status (*ilop_raw_read_attr)(ilop_att_val_t* value, uint32_t* outlen);

/**
 * Cloud write attribute handler
 *
 * @param[in] value : New attribute vale sent from the ILOP cloud
 *
 * @return status
 */
typedef mx_status (*ilop_raw_write_attr)(ilop_att_val_t value, uint32_t inlen);

struct ilop_raw_attr_t {
    uint8_t index;
    uint8_t attr_id;
    uint8_t type_id;
    ilop_raw_read_attr read_func;
    ilop_raw_write_attr write_func;
};

/* ============================================================================================== */
/*                                   MACRO & TYPE DEFINITION REGION                               */
/* ============================================================================================== */
/*
 * 帧结构定义如下：
 *   ----------------------------------------------------------------
 *  |  HEAD  |   LENGTH   |  VER  |  CMD  |    PAYLOAD    |  CHKSUM  |
 *   ----------------------------------------------------------------
 *      1Byte      2Byte     1Byte   1Byte      n Byte       1Byte
 */

/*
 * PAYLOAD 帧结构定义如下：
 *   -----------------------------
 *  |  method  |   id    |  data  |
 *   -----------------------------
 *      1Byte     4Byte    n Byte
 */

/*
 * set/report 的 data 帧结构定义如下：(len 仅在类型为数组和文本(text)的情况下需要，表示长度)
 *   -------------------------------------------
 *  |  typeid  |   attrid   |  [len]  |  value  |
 *   -------------------------------------------
 *      1Byte      1Byte       2Byte     n Byte
 */

#define RAW_PROTOCOL_HEAD 0xAA
#define RAW_PROTOCOL_REGION_HEAD_LEN 5 /* HEAD + LENGTH + VER + CMD */
#define RAW_PROTOCOL_REGION_LENGTH_FIXED 3 /* LENGTH(VER+CMD+CHKSUM) */
#define RAW_PROTOCOL_FRAME_LEN_FIXED (RAW_PROTOCOL_REGION_HEAD_LEN + 1) /* Fixed length in frames (except for other fields in DATA domain) */
#define RAW_PROTOCOL_VER 0x01

#define CONFIG_MAX_PKG_SIZE 256

typedef enum {
    RAW_UP_DATA = 0x00,
    RAW_DOWN_DATA = 0x80,
} raw_elem_cmd_t;

typedef enum {
    /* Downlink method */
    RAW_METHOD_GET = 0x00,
    RAW_METHOD_SET = 0x01,
    RAW_METHOD_SERVICE = 0x02,

    /* Uplink method */
    RAW_METHOD_REPORT = 0x80,
    RAW_METHOD_EVENT = 0x81,

    /* General response */
    RAW_METHOD_ACK = 0xFF
} raw_data_method_t;

/* ============================================================================================== */
/*                                   DATA STRUCTURE DEFINITION REGION                             */
/* ============================================================================================== */

/* 一般缓冲区 */
typedef struct {
    uint8_t buffer[CONFIG_MAX_PKG_SIZE];
    uint16_t size; /* Cache size */
    uint16_t length; /* Cache length */
} data_buffer_t;

#pragma pack(1) /* 1 byte alignment */

typedef struct
{
    uint8_t head;
    uint16_t length;
    uint8_t ver;
    uint8_t cmd;
} raw_protocol_head_t;

/* Transparent (Passthrough) frame payload domain header */
typedef struct
{
    uint8_t method;
    uint32_t id;
    uint8_t data;
} raw_payload_data_head_t;

typedef struct
{
    uint8_t type_id;
    uint8_t attr_id;
    uint8_t value;
} raw_body_format_t;

typedef struct
{
    uint8_t service_id;
    uint8_t data;
} raw_service_body_format_t;

#pragma pack() /* Cancel custom byte alignment */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
