/**
 ******************************************************************************
 * @file    mx_stdio.c
 * @author  QQ Ding
 * @version V1.0.0
 * @date    3-Sept-2018
 * @brief   Initialize uart for printf output
 ******************************************************************************
 *
 * Copyright (c) 2009-2018 MXCHIP Co.,Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */
#include <pthread.h>

#include "mx_common.h"
#include "mx_hal.h"
#include "mx_ringbuffer.h"

/******************************************************************************
 *                              Variable Definitions
 ******************************************************************************/

static uint8_t cli_buffer[MX_CLI_RX_BUF_SIZE];
static struct ringbuffer stdio_rx;
static pthread_mutex_t stdio_mtx;
extern int cli_init(void);

/******************************************************************************
 *                               Type Definitions
 ******************************************************************************/

static uart_dev_t cli_uart_dev = {
    STDIO_UART_PORT,
    {
        STDIO_UART_BAUDRATE,
        DATA_WIDTH_8BIT,
        NO_PARITY,
        STOP_BITS_1,
        FLOW_CONTROL_DISABLED,
        MODE_TX_RX,
    },
};

/******************************************************************************
 *                              Function Definitions
 ******************************************************************************/

#ifdef MX_CLI_ENABLE
static void* cli_recv_task(void* arg)
{
    int ret = 0;
    uint8_t RXData;
    uint32_t recv_len = 0;

    while (1) {
        ret = mx_hal_uart_recv(&cli_uart_dev, &RXData, 1, &recv_len, 0);
        if ((ret == 0) && (recv_len == 1)) {
            pthread_mutex_lock(&stdio_mtx);
            ringbuffer_put(&stdio_rx, RXData);
            pthread_mutex_unlock(&stdio_mtx);
        }
    }
    return NULL;
}

static int32_t at_async_read(uint8_t* const buf, const uint16_t length)
{
    uint16_t was_read = 0;
    uint32_t num;

    if (buf == 0 || length == 0)
        return 0;

    pthread_mutex_lock(&stdio_mtx);
    num = ringbuffer_num(&stdio_rx);
    pthread_mutex_unlock(&stdio_mtx);

    while ((was_read < num) && (was_read < length)) {
        ringbuffer_get(&stdio_rx, &buf[was_read++]);
    }

    return (int32_t)was_read;
}
#endif

void mx_hal_stdio_init(void)
{
    mx_status err;

    ringbuffer_init(&stdio_rx, cli_buffer, MX_CLI_RX_BUF_SIZE);

    err = mx_hal_uart_init(&cli_uart_dev);
    require_noerr(err, exit);

#ifdef MX_CLI_ENABLE
    pthread_t ntid;

    pthread_mutex_init(&stdio_mtx, NULL);

    err = pthread_create(&ntid, NULL, cli_recv_task, NULL);
    require_noerr(err, exit);

    err = cli_init();
    require_noerr(err, exit);
#endif

exit:
    return;
}

#ifdef MX_CLI_ENABLE
int mx_hal_cli_putstr(const char* str, uint32_t strlen)
{
    int ret = 0;
    ret = mx_hal_uart_send(&cli_uart_dev, str, strlen, 0);
    return ret;
}

int mx_hal_cli_getc(void)
{
    uint8_t ch;

    if (at_async_read(&ch, 1) == 1) {
        return ch;
    } else {
        return -1;
    }
}

#endif

