#ifndef _USER_QLINK_INCLUDE_H_
#define _USER_QLINK_INCLUDE_H_

#include "emh_api.h"
#include "mx_cli.h"
#include "mx_common.h"
#include "mx_debug.h"
#include "mx_hal.h"
#include "cloud_qlink.h"

#define app_log(M, ...) MX_LOG(APP_DEBUG, "APP", M, ##__VA_ARGS__)

#define APP_DEBUG                            MX_DEBUG_ON

#define QLINK_EVENT_TYPE_INFORM              "Inform"   //send data initiate
#define QLINK_EVENT_TYPE_FILE                "File"
#define QLINK_EVENT_TYPE_PARAMCHANGE         "ParamChange"  //send data after triggering by hand
#define QLINK_EVENT_TYPE_DATA                "Data"         //send data as a response


void qlink_main(void);

#endif
