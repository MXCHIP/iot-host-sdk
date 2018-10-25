/**
 ******************************************************************************
 * @file    emh_qlink.c
 * @author  QQ Ding
 * @version V1.0.0
 * @date    3-Sept-2018
 * @brief   Alicloud qlink service AT commands API
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


#include <stdlib.h>
#include <string.h>

#include "emh_api.h"
#include "ATCmdParser/ATCmdParser.h"



/******************************************************************************
 *                              Function Definitions
 ******************************************************************************/

mx_status emh_qlink_config(const emh_qlink_config_t* config, bool force)
{
	char args[200], *arg_list[5];
	const char* format_arg = emh_arg_for_type(EMH_ARG_QLINK_FORMAT, config->product_info.format); //JSON or RAW
	
	if( false==force )
	{
		/* Check product info*/
		if (ATCmdParser_send("AT+QLINKPRODUCT?")
		&& ATCmdParser_recv("+QLINKPRODUCT:%200[^\r]\r\n",args)
		&& ATCmdParser_recv("OK\r\n")) {
			if (3 == ATCmdParser_analyse_args(args, arg_list, 3)) {
				if ( !(strcmp(arg_list[0], config->product_info.product_token)
				|| strcmp(arg_list[1], config->product_info.andlink_token)
				|| strcmp(arg_list[2], config->product_info.device_type)) ){
					goto label_version;
				}
			}
		}
	}

	if (!(ATCmdParser_send("AT+QLINKPRODUCT=%s,%s,%s", 
							config->product_info.product_token, config->product_info.andlink_token, 
							config->product_info.device_type )
		&& ATCmdParser_recv("OK\r\n"))) {
		return kGeneralErr;
	}

label_version:
	if( false==force )
	{
		/* Check version info*/
		if (ATCmdParser_send("AT+QLINKVERSION?")
		&& ATCmdParser_recv("+QLINKVERSION:%40[^\r]\r\n",args)
		&& ATCmdParser_recv("OK\r\n")) {
			if (2 == ATCmdParser_analyse_args(args, arg_list, 2)) {
				if (!(strcmp(arg_list[0], config->version_info.firmware_version)
				|| strcmp(arg_list[1], config->version_info.software_version))){
					return kNoErr;
				}
			}
		}
	}		
		 
	if (!(ATCmdParser_send("AT+QLINKVERSION=%s,%s", 
							config->version_info.firmware_version, config->version_info.software_version )
		&& ATCmdParser_recv("OK\r\n"))) {
		return kGeneralErr;
	}
	
	return kNoErr; 
}

mx_status emh_qlink_raw_start( void )
{
    if (ATCmdParser_send("AT+QLINKRAWSTART")
     && ATCmdParser_recv("OK\r\n")) {
        return kNoErr;
    }
	return kGeneralErr;
}

mx_status emh_qlink_user_start( void )
{
    if (ATCmdParser_send("AT+QLINKUSERSTART")
     && ATCmdParser_recv("OK\r\n")) {
        return kNoErr;
    }
	return kGeneralErr;
}

mx_status emh_qlink_service_start( void )
{
    if (ATCmdParser_send("AT+QLINKSTART")
     && ATCmdParser_recv("OK\r\n")) {
        return kNoErr;
    }
	return kGeneralErr;
}

emh_arg_qlink_status_t emh_qlink_get_stauts( void )
{
	char arg[20];

	if (!(ATCmdParser_send("AT+QLINKSTATUS?")
	   && ATCmdParser_recv("+QLINKSTATUS:%20[^\r]\r\n",arg)
	   && ATCmdParser_recv("OK\r\n"))) {
		return EMH_ARG_ERR;
	}
	
	return emh_arg_for_arg( EMH_ARG_QLINK_STATUS, arg);
}

mx_status emh_qlink_service_stop( void )
{
    if (ATCmdParser_send("AT+QLINKSTOP")
     && ATCmdParser_recv("OK\r\n")) {
        return kNoErr;
    }
	return kGeneralErr;
}

mx_status emh_qlink_send_json_to_cloud( char *type, uint8_t *data, uint32_t len )
{
    if (ATCmdParser_send("AT+QLINKSEND=%d", type, len)
	 && ATCmdParser_recv(">")
	 && ATCmdParser_write((char *)data, len) == len
	 && ATCmdParser_recv("OK\r\n")) {
		return kNoErr;
	}
	return kGeneralErr;
}

mx_status emh_qlink_send_data_to_uart( int res )
{
	if (  ATCmdParser_send("AT+QLINKPARAM=%d", res)
       && ATCmdParser_recv("OK\r\n")) {
        return kNoErr;
    }
	return kGeneralErr;
}

void emh_qlink_event_handler(void)
{
	mx_status err = kNoErr;
    char arg1[10], arg2[256];
    emh_arg_qlink_conn_t conn;
    emh_qlink_msg_t msg;

    require_action(ATCmdParser_recv("%10[^,],", arg1), exit, err = kMalformedErr);

	emh_arg_qlink_ev_t event = emh_arg_for_arg(EMH_ARG_QLINK_EV, arg1);
	require_action(event != EMH_ARG_ERR, exit,  err = kMalformedErr);

    /* qlink Server connection event */
	if (event == EMH_ARG_QLINK_EV_QLINK) {
		require_action(ATCmdParser_recv("%20[^\r]\r\n", arg2), exit, err = kMalformedErr);
		conn = emh_arg_for_arg(EMH_ARG_QLINK_CONN, arg2);
		require_action(conn != EMH_ARG_ERR, exit, err = kMalformedErr);
		emh_ev_qlink_connection(conn);
	}
    /* QLINK server === json value===> device */
    else if(event == EMH_ARG_QLINK_EV_SET)
    {	
		/* Read package data */
		int32_t count;
		require_action(ATCmdParser_recv("%d,", &count), exit, err = kMalformedErr);

        uint8_t *data = malloc(count+1);
		require_action(data, exit, err = kNoMemoryErr);
		require_action(ATCmdParser_read((char *)data, count) == count, exit, err = kTimeoutErr);

		*(data+count) = 0x00;
		msg.data = data;
		msg.format = EMH_ARG_QLINK_FORMAT_JSON;
		msg.len = count;

		int res = 0;
		res = emh_ev_qlink_set_local_attrs(&msg);
		if( NULL!=strstr(msg.data, "data") )
		{
			printf("ready to send back data\r\n");
			emh_qlink_send_data_to_uart(res);
		}
		printf("after sending back data\r\n");
		free(data);
    }
    /* QLINK device === json value===> server */
    else if(event == EMH_ARG_QLINK_EV_GET)
    {	
		/* Read package data */
		int32_t count;
		require_action(ATCmdParser_recv("%d,", &count), exit, err = kMalformedErr);

        uint8_t *data = malloc(count+1);
		require_action(data, exit, err = kNoMemoryErr);
		require_action(ATCmdParser_read((char *)data, count) == count, exit, err = kTimeoutErr);

		*(data+count) = 0x00;
		msg.data = data;
		msg.format = EMH_ARG_QLINK_FORMAT_JSON;
		msg.len = count;
		emh_ev_qlink_get_local_attrs(&msg);
		free(data);
    }

exit:
    if (err == kMalformedErr) emh_ev_unknown();
    return;
}
