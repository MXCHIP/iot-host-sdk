/**
 ******************************************************************************
 * @file    ica_protocol.h
 * @author  QQ Ding
 * @version V1.0.0
 * @date    3-Step-2018
 * @brief   AliCloud ILOP ICA Data parser header file
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

#ifndef __ICA_PROTOCOL_H__
#define __ICA_PROTOCOL_H__

#include "emh_api.h"

/** \addtogroup protocols */
/** @{*/

/** \addtogroup Alicloud_ILOP_Service */
/** @{*/

/**
 * Cloud read attribute handler
 *
 * @param value[out] : point to attribute value buffer, application should write the correct value
 *
 * @return mx_status
 */
typedef mx_status (*ilop_ica_read_attr)(char** value);

/**
 * Cloud write attribute handler
 *
 * @param[in] value : New attribute vale sent from the ilop cloud
 *
 * @return mx_status
 */
typedef mx_status (*ilop_ica_write_attr)(char* value);

/** Alicloud ilop attribute description */
struct ilop_ica_attr_t {
    uint8_t index; /**< characteristics numbers registered on cloud */
    char* name; /**< Attribute name */
    emh_arg_ilop_vt_t type; /**< Attribute value type */
    ilop_ica_read_attr read_func; /**< Attribute value read handler, optional if the value is readable*/
    ilop_ica_write_attr write_func; /**< Attribute value write handler, optional if the value is writable */
};

#endif

/** @}*/
/** @}*/
