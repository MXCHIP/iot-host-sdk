/**
 ******************************************************************************
 * @file    emh_ilop.c
 * @author  QQ Ding
 * @version V1.0.0
 * @date    3-Sept-2018
 * @brief   Alicloud ILOP service AT commands API
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
// MX_WEAK void emh_ev_ilop_connection(emh_arg_ilop_conn_t conn)
// {
//     printf("conn=%d\r\n", conn);
//     return;
// }

// MX_WEAK void emh_ev_ilop_set_local_attr(emh_ilop_msg *msg)
// {
//     if( msg->format == EMH_ARG_ILOP_FORMAT_ICA){
//         printf("msg=%s\r\n", msg->data);
//     }
//     return;
// }

mx_status emh_ilop_config( emh_ilop_config_t *config, bool force )
{
    if( config == NULL ) return kParamErr;

    char arg[10];

    if( config->dm == EMH_ARG_ILOP_DM_ICA_BY_LOCAL )
    {
        if( config->tsl_thing == NULL )
            return kParamErr;
    }

    //check data format
    if (!(ATCmdParser_send("AT+ILOPDM?")
	   && ATCmdParser_recv("+ILOPDM:%10[^\r]\r\n",arg)
	   && ATCmdParser_recv("OK\r\n"))) {
		return kReadErr;
	}

    //if dm value different, we need to be set
    if( emh_arg_for_arg( EMH_ARG_ILOP_DM, arg) != config->dm )
    {
        if ( !(ATCmdParser_send("AT+ILOPDM=%s", emh_arg_for_type(EMH_ARG_ILOP_DM, config->dm))
	        && ATCmdParser_recv("OK\r\n"))) {
		    return kReadErr;
	    }
    }

    //if dm value is raw, we shuold check tls setting
    if( config->dm == EMH_ARG_ILOP_DM_ICA_BY_LOCAL )
    {
        //check tls setting
        if ( !(ATCmdParser_send("AT+ILOPCONFIG?")
	        && ATCmdParser_recv("+ILOPCONFIG:%10[^\r]\r\n",arg)
	        && ATCmdParser_recv("OK\r\n"))) {
		    return kReadErr;
	    }

        if( (force == true) || (emh_arg_for_arg( EMH_ARG_ILOP_CONFIG, arg ) != EMH_ARG_ILOP_CONFIG_TLS) )
        {
            if( emh_ilop_set_tsl( config->tsl_thing, config->tsl_len ) != kNoErr)
                return kWriteErr;
        }
    }
    return kNoErr;
}

mx_status emh_ilop_set_tsl( const char* tsl_thing, uint32_t tls_len )
{
    int id_count=0, last = 0, i=0;
    last =  (tls_len%4096 == 0) ? 0 : 1;
    id_count = tls_len/4096 + last;

    if ( !(ATCmdParser_send("AT+ILOPTSLPRE=%d,%d", tls_len, id_count)
        && ATCmdParser_recv("OK\r\n"))) {
        return kReadErr;
    }

    for( i=0; i<id_count-last; i++ )
    {
        if ( !(ATCmdParser_send("AT+ILOPTSL=%d", i)
	        && ATCmdParser_recv(">")
	        && ATCmdParser_write((char *)tsl_thing + (i*4096) , 4096) == 4096
	        && ATCmdParser_recv("OK\r\n")
            )) {
                return kReadErr;
        }
    }

    if( last )
    {
        if ( !(ATCmdParser_send("AT+ILOPTSL=%d", i)
	        && ATCmdParser_recv(">")
	        && ATCmdParser_write((char *)tsl_thing + (i*4096) , tls_len -i*4096) == (tls_len -i*4096)
	        && ATCmdParser_recv("OK\r\n")
            )) {
                return kReadErr;
        }
    }

    return kNoErr;
}

mx_status emh_ilop_set_key( const char *pk, const char *ps, const char *ds, const char *dn )
{
    if (ATCmdParser_send("AT+ILOPSET=%s,%s,%s,%s", pk, ps, ds, dn)
	 && ATCmdParser_recv("OK\r\n")) {
		return kNoErr;
	}
	return kGeneralErr;
}

mx_status emh_ilop_get_key( char pk[EMH_ILOP_PRODUCT_KEY_MAXLEN], char ps[EMH_ILOP_PRODUCT_SECRET_MAXLEN], char ds[EMH_ILOP_DEVICE_SECRET_MAXLEN], char dn[EMH_ILOP_DEVICE_NAME_MAXLEN] )
{
    char args[200], *arg_list[5];

	if (!(ATCmdParser_send("AT+ILOPSET?")
	   && ATCmdParser_recv("+ILOPSET:%200[^\r]\r\n",args)
	   && ATCmdParser_recv("OK\r\n"))) {
		return kReadErr;
	}

    if (4 != ATCmdParser_analyse_args(args, arg_list, 4)) {
		return kMalformedErr;
	}

    strncpy( pk, arg_list[0], EMH_ILOP_PRODUCT_KEY_MAXLEN );
    strncpy( ps, arg_list[1], EMH_ILOP_PRODUCT_SECRET_MAXLEN );
    strncpy( ds, arg_list[2], EMH_ILOP_DEVICE_SECRET_MAXLEN );
    strncpy( dn, arg_list[3], EMH_ILOP_DEVICE_NAME_MAXLEN );

    return kNoErr;
}

mx_status emh_ilop_awss_start( void )
{
    if (ATCmdParser_send("AT+ILOPAWSSTART")
     && ATCmdParser_recv("OK\r\n")) {
        return kNoErr;
    }
	return kGeneralErr;
}

mx_status emh_ilop_awss_press( void )
{
    if (ATCmdParser_send("AT+ILOPAWSPRESS")
     && ATCmdParser_recv("OK\r\n")) {
        return kNoErr;
    }
	return kGeneralErr;
}

mx_status emh_ilop_service_start( void )
{
    if (ATCmdParser_send("AT+ILOPSTART")
     && ATCmdParser_recv("OK\r\n")) {
        return kNoErr;
    }
	return kGeneralErr;
}

emh_arg_ilop_status_t emh_ilop_get_stauts( void )
{
	char arg[20];

	if (!(ATCmdParser_send("AT+ILOPSTATUS?")
	   && ATCmdParser_recv("+ILOPSTATUS:%20[^\r]\r\n",arg)
	   && ATCmdParser_recv("OK\r\n"))) {
		return EMH_ARG_ERR;
	}
	
	return emh_arg_for_arg( EMH_ARG_ILOP_STATUS, arg);
}

mx_status emh_ilop_unbind( void )
{
	if (ATCmdParser_send("AT+ILOPRESET")
     && ATCmdParser_recv("OK\r\n")) {
        return kNoErr;
    }
	return kGeneralErr;
}

mx_status emh_ilop_service_stop( void )
{
    if (ATCmdParser_send("AT+ILOPSTOP")
     && ATCmdParser_recv("OK\r\n")) {
        return kNoErr;
    }
	return kGeneralErr;
}

mx_status emh_ilop_send_ica_to_cloud( const char *type, char *attr )
{
    if (ATCmdParser_send("AT+ILOPSENDICA=%s,%s", type, attr)
     && ATCmdParser_recv("OK\r\n")) {
        return kNoErr;
    }

	return kGeneralErr;
}

mx_status emh_ilop_send_raw_to_cloud( uint8_t *data, uint32_t len )
{
    if (ATCmdParser_send("AT+ILOPSENDRAW=%d", len)
	 && ATCmdParser_recv(">")
	 && ATCmdParser_write((char *)data, len) == len
	 && ATCmdParser_recv("OK\r\n")) {
		return kNoErr;
	}
	return kGeneralErr;
}

void emh_ilop_event_handler(void)
{
	mx_status err = kNoErr;
    char arg1[10], arg2[256];
    emh_arg_ilop_conn_t conn;
    emh_ilop_msg msg;

    require_action(ATCmdParser_recv("%10[^,],", arg1), exit, err = kMalformedErr);

	emh_arg_ilop_ev_t event = emh_arg_for_arg(EMH_ARG_ILOP_EV, arg1);
	require_action(event != EMH_ARG_ERR, exit,  err = kMalformedErr);

    /* ILOP Server connection event */
	if (event == EMH_ARG_ILOP_EV_ILOP) {
		require_action(ATCmdParser_recv("%20[^\r]\r\n", arg2), exit, err = kMalformedErr);
		conn = emh_arg_for_arg(EMH_ARG_ILOP_CONN, arg2);
		require_action(conn != EMH_ARG_ERR, exit, err = kMalformedErr);
		emh_ev_ilop_connection(conn);
	}
    /* ALINK server === ICA value===> device */
    else if(event == EMH_ARG_ILOP_EV_SETICA)
    {
        require_action(ATCmdParser_recv("%256[^\r]\r\n", arg2), exit, err = kMalformedErr);
        msg.format = EMH_ARG_ILOP_FORMAT_ICA;
        msg.data = (uint8_t *)&arg2;
        msg.len = strlen(arg2);
        emh_ev_ilop_set_local_attr( &msg );
    }
    /* ALINK server === RAW value===> device */
    else if(event == EMH_ARG_ILOP_EV_SETRAW)
    {	
		/* Read package data */
		int32_t count;
		require_action(ATCmdParser_recv("%d,", &count), exit, err = kMalformedErr);

        uint8_t *data = malloc(count);
		require_action(data, exit, err = kNoMemoryErr);
		require_action(ATCmdParser_read((char *)data, count) == count, exit, err = kTimeoutErr);

		msg.data = data;
		msg.format = EMH_ARG_ILOP_FORMAT_RAW;
		msg.len = count;
		emh_ev_ilop_set_local_attr(&msg);
		free(data);
    }

exit:
    if (err == kMalformedErr) emh_ev_unknown();
    return;
}
