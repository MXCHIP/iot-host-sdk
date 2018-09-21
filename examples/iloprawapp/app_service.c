#include "alicloud_ilop.h"
#include "main.h"
#include "mx_common.h"
#include "mx_debug.h"

static mx_status timereset_write_func(ilop_att_val_t value, uint32_t inlen)
{
    app_log("TimeReset[%d]:%s", inlen, (char*)value.dataValue);
    return kNoErr;
}

static const struct ilop_raw_attr_t TimeReset_service = { ILOP_SERVICE_TimeReset, 0, TYPE_ID_STRING, NULL, timereset_write_func };

void service_register(void)
{
    ilop_raw_attr_register(ATTR_TYPE_SERVICE, &TimeReset_service);
}
