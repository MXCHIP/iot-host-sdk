#include "main.h"
#include "mx_common.h"
#include "mx_hal.h"

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: \"%s /dev/ttyUSB0\"\n", argv[0]);
        return 0;
    }
    printf("dev[%s]\r\n", argv[1]);
    mx_hal_uart_dev_set(argv[1]);
    ilop_main();
    return 0;
}
