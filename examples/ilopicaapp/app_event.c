#include "alicloud_ilop.h"
#include "main.h"
#include "mx_cli.h"
#include "mx_common.h"
#include "mx_debug.h"
#include "mx_hal.h"

static char* err_code = "0";
static int event_post = 0;

static unsigned long long uptime_sec(void)
{
    static unsigned long long start_time = 0;

    if (start_time == 0) {
        start_time = mx_hal_ms_ticker_read();
    }

    return (mx_hal_ms_ticker_read() - start_time) / 1000;
}

static mx_status error_code_read_func(char** value)
{
    (*value) = err_code;
    app_log("read value:%s", err_code);
    return kNoErr;
}

static const struct ilop_ica_attr_t ErrorCode_event = { ILOP_HANDLE_ATTR_ErrorCode, "Error.ErrorCode", EMH_ARG_ILOP_VT_EVENT, error_code_read_func, NULL };

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
    ilop_ica_attr_register(&ErrorCode_event);
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
        ilop_ica_attr_indicate_by_handle(ILOP_HANDLE_ATTR_ErrorCode);
    }

    pre_sec = now;
}
