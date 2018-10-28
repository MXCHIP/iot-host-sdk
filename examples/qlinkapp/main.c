#include "user_qlink_include.h"

const emh_qlink_config_t config = {
    .product_info = {
        .product_token = "PXmsE6kQmxEgc0e3",
        .andlink_token = "ZxMo0zJvvZMWP3Az",
        .product_id   = "30531",
        .format = EMH_ARG_QLINK_FORMAT_JSON,
    },
    .version_info = {
        .firmware_version = "1.4.3",
        .software_version = "1.0.1",
    }
};
    // .product_info = {
    // .product_token = "38Szyd6i1240elbV",
    // .andlink_token = "wWppJIdROfQkcsMW",
    // .product_id   = "30413",
    // .format = EMH_ARG_QLINK_FORMAT_JSON,
    // },
    
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

int emh_ev_qlink_set_local_attrs(emh_qlink_msg_t* msg)
{
    app_log("set attrs");

    //REBOOT, reboot device
    if( 0==strncmp(msg->data, "REBOOT", msg->len ) )
    {
        app_log("please reboot device.");
    }else if( 0==strncmp(msg->data, "FACTORY", msg->len) ) //FACTOR, restore factory settings and clear all parameters of then system.
    {
        app_log("please restore device.");
    }else if( 0==strncmp(msg->data, "UNBIND", msg->len) ) //UNBIND 
    {
        app_log("this device has already bend unbinded");
    }else  //set data,  if this field also need to be upload, this function return 1, otherwise 0 will be returned.
    {
        app_log("data is :%s  len is :%d", msg->data, msg->len);
        // return 1;
    }
    return 0;
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

static void handle_reboot_cmd(char* pwbuf, int blen, int argc, char** argv)
{

    if (argc != 1)
    {
        app_log("input param error!!!");
        return;
    }
    
    emh_module_reset();

}

// {"deviceId":"CMCC-30531-B0F89327CDF4","childDeviceId":"","eventType":"Inform","timestamp":0,"data":{"params":[{"paramCode":"permitJoining","paramValue":"0"},{"paramCode":"firmware","paramValue":"1.6.0"},{"paramCode":"softVersion","paramValue":"V1.0.6"}]}}
static void handle_send_cmd(char* pwbuf, int blen, int argc, char** argv)
{
    if (argc != 3)
    {
        app_log("input param error!!!");
        return;
    }
    app_log("type is:%s\r\n data is:%s\r\ndata's length is:%d", argv[1], argv[2], strlen(argv[2]) );

    qlink_send_data( argv[1], argv[2], strlen(argv[2]) );
}

struct cli_command qlinkcmds[] = {
    { "raw", "raw [start|stop]", handle_raw_cmd },
    { "user", "start|stop", handle_user_cmd },
    { "reboot", "reboot", handle_reboot_cmd },
    { "send", "send <type>", handle_send_cmd },
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

