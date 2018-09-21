/**
 ******************************************************************************
 * @file    mx_uart.c
 * @author  QQ Ding
 * @version V1.0.0
 * @date    3-Sept-2018
 * @brief   UART driver function
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

#include <assert.h>
#include <errno.h>
#include <execinfo.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include <pthread.h>

#include "mx_common.h"
#include "mx_hal.h"

/******************************************************************************
 *                              Variable Definitions
 ******************************************************************************/

#define LINUX_UART_LINUX_DEV "/dev/ttyUSB1"

static char linux_dev[15] = { 0 };
static int at_uart_fd = -1;

/******************************************************************************
 *                              Function Definitions
 ******************************************************************************/

static int read_and_discard_all_data(const int fd)
{
    int was_msg_already_printed = 0;

    while (1) {
        char buffer[1024];
        const ssize_t read_count = read(fd, buffer, sizeof(buffer));

        if (read_count == 0) {
            /* "EOF" or "connection closed at the other end"*/
            return 0;
        }

        if (read_count > 0) {
            if (!was_msg_already_printed) {
                printf("Some stale data was discarded.\r\n");
                was_msg_already_printed = 1;
            }

            continue;
        }

        assert(read_count == -1); /* According to the specification. */

        const int errno_code = errno;

        if (errno_code == EINTR)
            continue;

        if (errno_code == EAGAIN || errno_code == EWOULDBLOCK) {
            /**
             * We know that the file descriptor has been opened with
             * O_NONBLOCK or O_NDELAY, and these codes mean that there
             * is no data to read at present.
             */
            return 0;
        }

        /* Some other error has occurred. */
        return -1;
    }
}

void mx_hal_uart_dev_set(char* dev)
{
    strncpy(linux_dev, dev, 15);
}

int32_t mx_hal_uart_init(uart_dev_t* uart)
{
    int fd;
    struct termios t_opt;
    speed_t baud;

    if (uart->port != AT_UART_PORT)
        return 0;

    if ((at_uart_fd = open(linux_dev,
             O_RDWR | O_NOCTTY | O_NDELAY))
        == -1) {
        printf("open at uart failed\r\n");
        return -1;
    }

    switch (uart->config.baud_rate) {
    case 115200:
        baud = B115200;
        break;
    default:
        baud = B115200;
        break;
    }

    fd = at_uart_fd;
    /* set the serial port parameters */
    fcntl(fd, F_SETFL, 0);
    if (0 != tcgetattr(fd, &t_opt))
        return -1;

    if (0 != cfsetispeed(&t_opt, baud))
        return -1;

    if (0 != cfsetospeed(&t_opt, baud))
        return -1;

    // 8N1, flow control, etc.
    t_opt.c_cflag |= (CLOCAL | CREAD);
    if (uart->config.parity == NO_PARITY)
        t_opt.c_cflag &= ~PARENB;
    if (uart->config.stop_bits == STOP_BITS_1)
        t_opt.c_cflag &= ~CSTOPB;
    else
        t_opt.c_cflag |= CSTOPB;
    t_opt.c_cflag &= ~CSIZE;
    switch (uart->config.data_width) {
    case DATA_WIDTH_5BIT:
        t_opt.c_cflag |= CS5;
        break;
    case DATA_WIDTH_6BIT:
        t_opt.c_cflag |= CS6;
        break;
    case DATA_WIDTH_7BIT:
        t_opt.c_cflag |= CS7;
        break;
    case DATA_WIDTH_8BIT:
        t_opt.c_cflag |= CS8;
        break;
    default:
        t_opt.c_cflag |= CS8;
        break;
    }
    t_opt.c_lflag &= ~(ECHO | ECHOE | ISIG | ICANON);
    if (uart->config.flow_control == FLOW_CONTROL_DISABLED)
        t_opt.c_cflag &= ~CRTSCTS;

    /**
     * AT is going to use a binary protocol, so make sure to
     * turn off any CR/LF translation and the like.
     */
    t_opt.c_iflag &= ~(IXON | IXOFF | IXANY | INLCR | ICRNL);

    t_opt.c_oflag &= ~OPOST;
    t_opt.c_cc[VMIN] = 0;
    t_opt.c_cc[VTIME] = 5;

    if (0 != tcsetattr(fd, TCSANOW, &t_opt)) {
        return -1;
    }

    printf("open at uart succeed\r\n");

    // clear uart buffer
    read_and_discard_all_data(fd);

    return 0;
}

int32_t mx_hal_uart_send(uart_dev_t* uart, const void* data,
    uint32_t size, uint32_t timeout)
{
    uint32_t ret, rmd = size;

    if (uart->port == AT_UART_PORT) {
        while (rmd > 0) {
            ret = write(at_uart_fd, data + size - rmd, rmd);
            if (ret == -1) {
                printf("write uart fd failed on error: %d.\r\n", errno);
                return -1;
            }
            rmd -= ret;
        }
    } else
        ret = write(0, data, size);
    return 0;
}

int32_t mx_hal_uart_recv(uart_dev_t* uart, void* data, uint32_t expect_size,
    uint32_t* recv_size, uint32_t timeout)
{
    int fd, n;

    if (uart->port == AT_UART_PORT)
        fd = at_uart_fd;
    else
        fd = 0;

    if ((n = read(fd, data, expect_size)) == -1) {
        // printf("read failed\r\n");
        return -1;
    }

    if (uart->port != AT_UART_PORT && *(char*)data == '\n')
        *(char*)data = '\r';
    if (recv_size)
        *recv_size = n;

    return 0;
}

int32_t mx_hal_uart_finalize(uart_dev_t* uart)
{
    if (uart->port == AT_UART_PORT)
        close(at_uart_fd);
    return 0;
}

