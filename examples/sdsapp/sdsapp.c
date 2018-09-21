#include "alicloud_sds.h"
#include "mx_common.h"
#include "mx_debug.h"

#include "emh_api.h"
#include "mx_hal.h"

#define APP_DEBUG MX_DEBUG_ON
#define app_log(M, ...) MX_LOG(APP_DEBUG, "APP", M, ##__VA_ARGS__)

static int light_on = 0;

enum {
    ALI_HANDLE_LIGHT_SWITCH,
    ALI_HANDLE_MAX,
};

const emh_alisds_config_t alisds_config = {
    .product_info = {
        .name = "alink_product",
        .module = "ALINKTEST_LIVING_LIGHT_ALINK_TEST",
        .key = "5gPFl8G4GyFZ1fPWk20m",
        .secret = "ngthgTlZ65bX5LpViKIWNsDPhOf2As9ChnoL9gQb",
        .format = EMH_ARG_ALISDS_FORMAT_JSON,
    },
    .dev_info = {
        .type = "LIGHT",
        .category = "LIVING",
        .manufacture = "ALINKTEST",
    }
};

void emh_ev_alisds_get_local_atts(emh_alisds_msg* attrs)
{
    alisds_attr_indicate_by_handle(ALI_HANDLE_LIGHT_SWITCH);
}

mx_status handle_read_cur_light_switch(alisds_att_val_t* value)
{
    (*value).boolValue = light_on;
    app_log("read switch:%d\r\n", light_on);
    return kNoErr;
}

mx_status handle_write_cur_light_switch(alisds_att_val_t value)
{
    light_on = value.boolValue;
    app_log("write switch:%d\r\n", light_on);
    return kNoErr;
}

void switch_task_init(void)
{
    alisds_attr_t attr;

    attr.name = "Switch";
    attr.type = ALI_ATT_TYPE_BOOL;
    attr.read_func = handle_read_cur_light_switch;
    attr.write_func = handle_write_cur_light_switch;
    alisds_attr_init(ALI_HANDLE_LIGHT_SWITCH, attr);
}

static void sds_task(void)
{
    mx_status err = kNoErr;

    err = alisds_init(&alisds_config, 1);
    require_noerr(err, exit);

    switch_task_init();

    while (1) {
        alisds_runloop();
    }

exit:
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
    sds_task();
    return 0;
}
