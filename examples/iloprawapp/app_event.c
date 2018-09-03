#include "alicloud_ilop.h"
#include "main.h"
#include "mx_cli.h"
#include "mx_common.h"
#include "mx_debug.h"
#include "mx_hal.h"

static int event_post = 0;

static unsigned long long uptime_sec(void)
{
    static unsigned long long start_time = 0;

    if (start_time == 0) {
        start_time = mx_hal_ms_ticker_read();
    }

    return (mx_hal_ms_ticker_read() - start_time) / 1000;
}

static mx_status error_code_read_func(ilop_att_val_t* value, uint32_t* outlen)
{
    (*value).boolValue = 0;
    (*outlen) = sizeof((*value).boolValue);
    app_log("read value:%d", (*value).boolValue);
    return kNoErr;
}

static const struct ilop_raw_attr_t ErrorCode_event = { ILOP_EVENT_ErrorCode, 0, TYPE_ID_BOOL, error_code_read_func, NULL };

#ifdef MX_CLI_ENABLE
static void handle_event_cmd(char* pwbuf, int blen, int argc, char** argv)
{
    if (argc != 2)
        return;

    if (strcmp(argv[1], "enable") == 0) {
        event_post = 1;
    } else if (strcmp(argv[1], "disable") == 0) {
        event_post = 0;
    }
}

static struct cli_command eventcmd = { "event", "event [enable|disable]", handle_event_cmd };
#endif

void event_register(void)
{
    ilop_raw_attr_register(ATTR_TYPE_EVENT, &ErrorCode_event);
#ifdef MX_CLI_ENABLE
    cli_register_command(&eventcmd);
#endif
}

void event_task_loop(void)
{
    if (event_post == 0)
        return;

    uint32_t now = uptime_sec();
    static uint32_t pre_sec = 0;

    if (pre_sec == now)
        return;

    if ((now % 10) == 0) {
        app_log("time[%d]", now);
        ilop_raw_attr_indicate_by_handle(ATTR_TYPE_EVENT, ILOP_EVENT_ErrorCode);
    }

    pre_sec = now;
}
