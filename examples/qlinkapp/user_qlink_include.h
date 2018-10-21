#ifndef _USER_QLINK_INCLUDE_H_
#define _USER_QLINK_INCLUDE_H_

#include "emh_api.h"
#include "mx_cli.h"
#include "mx_common.h"
#include "mx_debug.h"
#include "mx_hal.h"
#include "cloud_qlink.h"

#define app_log(M, ...) MX_LOG(APP_DEBUG, "APP", M, ##__VA_ARGS__)

// #define MX_CLI_ENABLE            1
#define APP_DEBUG                MX_DEBUG_ON


void qlink_main(void);

#endif
