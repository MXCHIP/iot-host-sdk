#include "alicloud_ilop.h"
#include "main.h"
#include "mx_common.h"
#include "mx_debug.h"

static mx_status timereset_write_func(char* value)
{
    app_log("TimeReset:%s", value);
    return kNoErr;
}

static const struct ilop_ica_attr_t TimeReset_service = { ILOP_HANDLE_ATTR_TimeReset, "TimeReset", EMH_ARG_ILOP_VT_SERVICE, NULL, timereset_write_func };

void service_register(void)
{
    ilop_ica_attr_register(&TimeReset_service);
}
