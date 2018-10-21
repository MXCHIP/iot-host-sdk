#include "user_qlink_include.h"

const emh_qlink_config_t config = {
    .product_info = {
        .product_token = "PXmsE6kQmxEgc0e3",
        .andlink_token = "ZxMo0zJvvZMWP3Az",
        .device_type   = "30531",
        .format = EMH_ARG_QLINK_FORMAT_JSON,
    },
    .version_info = {
        .firmware_version = "1.4.3",
        .software_version = "1.0.1",
    }
};

void qlink_event_handler(qlink_event_t event)
{
    switch (event) {
        case QLINK_EVENT_WLAN_CONFIG_STARTED: {
            app_log("Wi-Fi config....");
            break;
        }
        case QLINK_EVENT_WLAN_CONFIG_STARTED_IN_AP_MODE: {
            app_log("WiFi config in AP mode......");
        break;
        }
        case QLINK_EVENT_WLAN_CONNECTED: {
            app_log("Wi-Fi connected");
            break;
        }
        case QLINK_EVENT_WLAN_DISCONNECTED: {
            app_log("Wi-Fi disconnected");
            break;
        }
        case QLINK_EVENT_CLOUD_CONNECTED: {
            app_log("Cloud connected");
            break;
        }
        case QLINK_EVENT_CLOUD_DISCONNECTED: {
            app_log("Cloud disconnected");
            break;
        }
    }
}

//cloud want to get data from device
void emh_ev_qlink_get_local_attrs(emh_qlink_msg_t* msg)
{
    app_log("get attrs");
    app_log("data is :%s  len is :%d", msg->data, msg->len);

}

void emh_ev_qlink_set_local_attrs(emh_qlink_msg_t* msg)
{
    app_log("set attrs");
    app_log("data is :%s  len is :%d", msg->data, msg->len);

    //unbind

    //reset

    //factory

    //set data,  if this field also need to be upload, this function return 1, otherwise 0 will be returned.

}

#ifdef MX_CLI_ENABLE
static void handle_raw_cmd(char* pwbuf, int blen, int argc, char** argv)
{
    if (argc != 2)
        return;

    if (strcmp(argv[1], "start") == 0) {
        qlink_raw_start();
    } else if (strcmp(argv[1], "stop") == 0) {
      
    }
}

static void handle_user_cmd(char* pwbuf, int blen, int argc, char** argv)
{
    app_log("enter handle_user_cmd()!");
    if (argc != 2)
        return;

    if (strcmp(argv[1], "start") == 0) {
        qlink_user_start();
    } else if (strcmp(argv[1], "stop") == 0) {

    }
}

struct cli_command qlinkcmds[] = {
    { "raw", "raw [start|stop]", handle_raw_cmd },
    { "user", "start|stop", handle_user_cmd },
};
#endif

void qlink_main(void)
{
    mx_status err = kNoErr;
    int nums = 0;

     /* System initialization， ticker, stdio */
    mx_hal_ms_ticker_init();
    mx_hal_stdio_init();

#ifdef MX_CLI_ENABLE
    app_log("open cli function");
    cli_register_commands(qlinkcmds, sizeof(qlinkcmds) / sizeof(struct cli_command));
#endif

label_reinit:
    /* qlink service initialization */
    err = qlink_init(&config);
    if (err != kNoErr){
        app_log("qlink init err");
        if( nums<3 ){
            nums++;
            goto label_reinit;
        }
    }

    while (1) {
        qlink_runloop();
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
    qlink_main();
    return 0;
}

