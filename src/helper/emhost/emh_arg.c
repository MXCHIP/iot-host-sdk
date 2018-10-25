/**
 ******************************************************************************
 * @file    emh_arg.c
 * @author  William Xu
 * @version V1.0.0
 * @date    9-Apr-2018
 * @brief   AT command arguments to enum type convert
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

#include "emh_arg.h"

/******************************************************************************
 *                                 Constants
 ******************************************************************************/

/******************************************************************************
 *                              Alicoud SDS service
 ******************************************************************************/

#ifdef AT_SUPPORT_ALISDS
const char* EMH_ARG_ALISDS_FORMAT[] =
{
	[EMH_ARG_ALISDS_FORMAT_JSON] = "JSON",
	[EMH_ARG_ALISDS_FORMAT_RAW]  = "RAW",
	[EMH_ARG_ALISDS_FORMAT_MAX]  = "\0",
};

const char* EMH_ARG_ALISDS_CONN[] =
{
	[EMH_ARG_ALISDS_CONN_CONNECTED]     = "CONNECT",
	[EMH_ARG_ALISDS_CONN_DISCONNECTED]  = "DISCONNECT",
	[EMH_ARG_ALISDS_CONN_MAX]           = "\0",
};

const char* EMH_ARG_ALISDS_STATUS[] =
{
	[EMH_ARG_ALISDS_STATUS_UNINITIALIZED]	= "NONE",
	[EMH_ARG_ALISDS_STATUS_INITIALIZED]		= "INIT",
	[EMH_ARG_ALISDS_STATUS_CONNECTED]		= "CONNECT",
	[EMH_ARG_ALISDS_STATUS_DISCONNECTED]	= "DISCONNECT",
	[EMH_ARG_ALISDS_STATUS_MAX]				= "\0",
};

const char* EMH_ARG_ALISDS_EV[] =
{
	[EMH_ARG_ALISDS_EV_ALINK]     = "ALINK",
	[EMH_ARG_ALISDS_EV_GET]       = "GET",
	[EMH_ARG_ALISDS_EV_SET]       = "SET",
	[EMH_ARG_ALISDS_EV_MAX]       = "\0",
};
#endif

/******************************************************************************
 *                              WiFi service
 ******************************************************************************/

const char* EMH_ARG_WLAN_EV[] =
{
	[EMH_ARG_WLAN_EV_UAP_ESTABLISHED]     = "AP_UP",
	[EMH_ARG_WLAN_EV_UAP_DESTROYED]       = "AP_DOWN",
	[EMH_ARG_WLAN_EV_STA_CONNECTED]       = "STATION_UP",
	[EMH_ARG_WLAN_EV_STA_DISCONNECTED]    = "STATION_DOWN",
	[EMH_ARG_WLAN_EV_MAX]                 = "\0",
};

const char* EMH_ARG_WLAN_STA[] =
{
	[EMH_ARG_WLAN_STA_CONNECTED]       = "STATION_UP",
	[EMH_ARG_WLAN_STA_DISCONNECTED]    = "STATION_DOWN",
	[EMH_ARG_WLAN_STA_DISCONNECTED]    = "CONNECTING",
	[EMH_ARG_WLAN_STA_MAX]             = "\0",
};

/******************************************************************************
 *                              Alicoud ILOP service
 ******************************************************************************/

#ifdef AT_SUPPORT_ILOP
const char* EMH_ARG_ILOP_DM[] =
{
	[EMH_ARG_ILOP_DM_RAW]			= "0",
	[EMH_ARG_ILOP_DM_ICA_BY_CLOUD]  = "1",
	[EMH_ARG_ILOP_DM_ICA_BY_LOCAL]	= "2",
	[EMH_ARG_ILOP_DM_MAX]			= "\0",
};

const char* EMH_ARG_ILOP_DOMAIN[] =
{
	[EMH_ARG_ILOP_DOMAIN_SHANGHAI]			= "0",
	[EMH_ARG_ILOP_DOMAIN_SINGAPORE]         = "1",
	[EMH_ARG_ILOP_DOMAIN_JAPAN]	            = "2",
	[EMH_ARG_ILOP_DOMAIN_AMERICA]			= "3",
	[EMH_ARG_ILOP_DOMAIN_GERMANY]			= "4",
	[EMH_ARG_ILOP_DOMAIN_MAX]			    = "\0",
};

const char* EMH_ARG_ILOP_CONFIG[] = 
{
	[EMH_ARG_ILOP_CONFIG_NONE]		= "NONE",
	[EMH_ARG_ILOP_CONFIG_TLS]		= "CONFIG",
	[EMH_ARG_ILOP_CONFIG_MAX]		= "\0",
};

const char* EMH_ARG_ILOP_STATUS[] =
{
	[EMH_ARG_ILOP_STATUS_UNINITIALIZED]		= "NONE",
	[EMH_ARG_ILOP_STATUS_INITIALIZED]		= "INIT",
	[EMH_ARG_ILOP_STATUS_CONNECTED]			= "CONNECT",
	[EMH_ARG_ILOP_STATUS_DISCONNECTED]		= "DISCONNECT",
	[EMH_ARG_ILOP_STATUS_MAX]     		  	= "\0",
};

const char* EMH_ARG_ILOP_CONN[] =
{
	[EMH_ARG_ILOP_CONN_CLOUD_CONNECTED]     = "CLOUD_CONNECT",
	[EMH_ARG_ILOP_CONN_CLOUD_DISCONNECTED]  = "CLOUD_DISCONNECT",
	[EMH_ARG_ILOP_CONN_LOCAL_CONNECTED]     = "LOCAL_CONNECT",
	[EMH_ARG_ILOP_CONN_LOCAL_DISCONNECTED]  = "LOCAL_DISCONNECT",
	[EMH_ARG_ILOP_CONN_MAX]           		= "\0",
};

const char* EMH_ARG_ILOP_EV[] =
{
	[EMH_ARG_ILOP_EV_ILOP]			= "ILOP",
	[EMH_ARG_ILOP_EV_SETICA]		= "SETICA",
	[EMH_ARG_ILOP_EV_SETRAW]        = "SETRAW",
	[EMH_ARG_ILOP_EV_MAX]       	= "\0",
};

const char *EMH_ARG_ILOP_VT[] =
{
	[EMH_ARG_ILOP_VT_PROPERTY] 		= "property",
	[EMH_ARG_ILOP_VT_EVENT]			= "event",
	[EMH_ARG_ILOP_VT_SERVICE]		= "service",
	[EMH_ARG_ILOP_VT_MAX]			= "\0",
};
#endif

/******************************************************************************
 *                              Function Definitions
 ******************************************************************************/

const char* emh_arg_for_type(const char* emh_arg[], uint8_t type)
{
	return emh_arg[type];
}

uint8_t emh_arg_for_arg(const  char* emh_arg[], char *arg)
{
	for (uint8_t i = 0; i < EMH_ARG_ERR; i++) {
		if (*emh_arg[i] == 0x0) break;
		if (memcmp(emh_arg[i], arg, strlen(emh_arg[i])) == 0) {
			return i;
		}
		
	}
	return EMH_ARG_ERR;
}


