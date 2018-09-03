#include "alicloud_ilop.h"
#include "emh_api.h"
#include "main.h"
#include "mx_cli.h"
#include "mx_common.h"
#include "mx_debug.h"
#include "mx_hal.h"

static int awss_start = 0;
static int awss_press = 0;
static int reset = 0;

const emh_ilop_config_t ilop_config = {
    .tls_thing = NULL,
    .tls_len = 0,
    .dm = EMH_ARG_ILOP_DM_RAW,
};

const ilop_device_key_t device_key = {
    .product_key = "b1CcQqrXoVR",
    .product_secret = "yOrZrmtDcHLgVOJc",
    .device_name = "sPhDWW7vvaSyUfNBOvM9",
    .device_secret = "R90glnJXJNAUsKGSRRudMdlhyvA12OEM",
};

static void awss_tast_loop(void)
{
    if (awss_start == 1) {
        ilop_awss_start();
        awss_start = 0;
    }
    if (awss_press == 1) {
        ilop_awss_press();
        awss_press = 0;
    }
    if( reset == 1 )
    {
        ilop_restore();
        reset = 0;
    }
}

#ifdef MX_CLI_ENABLE
static void handle_aws_cmd(char* pwbuf, int blen, int argc, char** argv)
{
    if (argc != 2)
        return;

    if (strcmp(argv[1], "start") == 0) {
        awss_start = 1;
    } else if (strcmp(argv[1], "press") == 0) {
        awss_press = 1;
    }
}

static void handle_reset_cmd(char* pwbuf, int blen, int argc, char** argv)
{
    reset = 1;
}

static struct cli_command ilopcmds[] = {
    { "aws", "aws [start|press]", handle_aws_cmd },
    { "reset", "clean wifi module and ilop service", handle_reset_cmd },
};
#endif

void ilop_main(void)
{
    mx_status err = kNoErr;

     /* System initialization， ticker, stdio */
    mx_hal_ms_ticker_init();
    mx_hal_stdio_init();

#ifdef MX_CLI_ENABLE
    cli_register_commands(ilopcmds, sizeof(ilopcmds) / sizeof(struct cli_command));
#endif

    /* ILOP service initialization */
    err = ilop_init(&ilop_config);
    if (err != kNoErr)
        app_log("ilop init err");

    /* Set the ILOP three tuple. If the module is pre burned, delete the function */
    ilop_set_device_key(&device_key);

    /* database initialization */
    ilop_raw_attr_init(ATTR_TYPE_PROPERTY, ILOP_PROPERTY_MAX);
    ilop_raw_attr_init(ATTR_TYPE_EVENT, ILOP_EVENT_MAX);
    ilop_raw_attr_init(ATTR_TYPE_SERVICE, ILOP_SERVICE_MAX);

    event_register();
    property_register_light();
    property_register_wifi();
    service_register();

    while (1) {
        ilop_runloop();
        awss_tast_loop();
        event_task_loop();
    }

    return;
}
