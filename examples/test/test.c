#include "mx_common.h"
#include "mx_debug.h"

#include "emh_api.h"
#include "mx_hal.h"

#define APP_DEBUG MX_DEBUG_ON
#define app_log(M, ...) MX_LOG(APP_DEBUG, "APP", M, ##__VA_ARGS__)

static void at_task(void)
{
    mx_status err = kNoErr;

    mx_hal_ms_ticker_init();
    mx_hal_stdio_init();

    err = emh_init();
    if (err == kNoErr) {
        app_log("FW version: %s", emh_module_get_fw_version());
        app_log("System tick: %d", (int)emh_module_get_tick());
    }

    while (1) {
        emh_runloop();
    }

    return;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: \"%s /dev/ttyUSB0\"\n", argv[0]);
        return 0;
    }
    printf("dev[%s]\r\n", argv[1]);
    mx_hal_uart_dev_set(argv[1]);
    at_task();
    return 0;
}