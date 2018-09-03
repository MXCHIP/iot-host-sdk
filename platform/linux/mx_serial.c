/**
 ******************************************************************************
 * @file    mx_serial.c
 * @author  QQ Ding
 * @version V1.0.0
 * @date    3-Sept-2018
 * @brief   UART driver used for AT parser
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

static int _timeout = 100;
static uint8_t at_buffer[MX_SERIAL_RX_BUF_SIZE];
static struct ringbuffer at_rx;
static pthread_mutex_t at_mtx;

/******************************************************************************
 *                               Type Definitions
 ******************************************************************************/

static uart_dev_t uart_dev = {
    AT_UART_PORT,
    {
        AT_UART_BAUDRATE,
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

static void* uart_recv_task(void* arg)
{
    int ret = 0;
    uint8_t RXData;
    uint32_t recv_len = 0;

    while (1) {
        ret = mx_hal_uart_recv(&uart_dev, &RXData, 1, &recv_len, _timeout);
        if ((ret == 0) && (recv_len == 1)) {
            // printf("%c\r\n", RXData);
            pthread_mutex_lock(&at_mtx);
            ringbuffer_put(&at_rx, RXData);
            pthread_mutex_unlock(&at_mtx);
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

    pthread_mutex_lock(&at_mtx);
    num = ringbuffer_num(&at_rx);
    pthread_mutex_unlock(&at_mtx);

    while ((was_read < num) && (was_read < length)) {
        ringbuffer_get(&at_rx, &buf[was_read++]);
    }

    return (int32_t)was_read;
}

void mx_hal_serial_init(int timeout)
{
    mx_status err;
    _timeout = timeout;
    pthread_t ntid;

    ringbuffer_init(&at_rx, at_buffer, MX_SERIAL_RX_BUF_SIZE);

    err = mx_hal_uart_init(&uart_dev);
    require_noerr(err, exit);

    pthread_mutex_init(&at_mtx, NULL);

    err = pthread_create(&ntid, NULL, uart_recv_task, NULL);
    require_noerr(err, exit);

exit:
    return;
}

void mx_hal_serial_set_timeout(int timeout)
{
    _timeout = timeout;
}

int mx_hal_serial_putc(char c)
{
    return mx_hal_uart_send(&uart_dev, &c, 1, _timeout);
}

int mx_hal_serial_getc(void)
{
    uint32_t current = mx_hal_ms_ticker_read();
    uint8_t ch;

    do {
        if (at_async_read(&ch, 1) == 1)
            return ch;
    } while ((mx_hal_ms_ticker_read() - current) < _timeout);

    return -1;
}

bool mx_hal_serial_readable(void)
{
    if (ringbuffer_num(&at_rx))
        return true;
    return false;
}

void mx_hal_serial_flush(void)
{
    uint32_t num;
    uint8_t tmp;

    pthread_mutex_lock(&at_mtx);
    for (num = ringbuffer_num(&at_rx); num > 0; num--) {
        ringbuffer_get(&at_rx, &tmp);
    }
    pthread_mutex_unlock(&at_mtx);
}
