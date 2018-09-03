/**
 ******************************************************************************
 * @file    mx_hal.h
 * @author  QQ Ding
 * @version V1.0.0
 * @date    3-Sept-2018
 * @brief   MCU peripheral driver PI header file
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

#ifndef _MX_HAL_H_
#define _MX_HAL_H_

#include "mx_common.h"
#include "mx_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *                                   Macros
 ******************************************************************************/

#define AT_UART_PORT (0)
#define AT_UART_BAUDRATE (115200)

#define STDIO_UART_PORT (1)
#define STDIO_UART_BAUDRATE (115200)

/******************************************************************************
 *                                UART Function
 ******************************************************************************/

/*
 * UART data width
 */
typedef enum {
    DATA_WIDTH_5BIT,
    DATA_WIDTH_6BIT,
    DATA_WIDTH_7BIT,
    DATA_WIDTH_8BIT,
    DATA_WIDTH_9BIT
} mx_hal_uart_data_width_t;

/*
 * UART stop bits
 */
typedef enum {
    STOP_BITS_1,
    STOP_BITS_2
} mx_hal_uart_stop_bits_t;

/*
 * UART flow control
 */
typedef enum {
    FLOW_CONTROL_DISABLED,
    FLOW_CONTROL_CTS,
    FLOW_CONTROL_RTS,
    FLOW_CONTROL_CTS_RTS
} mx_hal_uart_flow_control_t;

/*
 * UART parity
 */
typedef enum {
    NO_PARITY,
    ODD_PARITY,
    EVEN_PARITY
} mx_hal_uart_parity_t;

/*
 * UART mode
 */
typedef enum {
    MODE_TX,
    MODE_RX,
    MODE_TX_RX
} mx_hal_uart_mode_t;

/*
 * UART configuration
 */
typedef struct {
    uint32_t baud_rate;
    mx_hal_uart_data_width_t data_width;
    mx_hal_uart_parity_t parity;
    mx_hal_uart_stop_bits_t stop_bits;
    mx_hal_uart_flow_control_t flow_control;
    mx_hal_uart_mode_t mode;
} uart_config_t;

typedef struct {
    uint8_t port; /* uart port */
    uart_config_t config; /* uart config */
} uart_dev_t;

/**
 * 
 * @brief 	    Set up serial port device description
 * @note        only linux platform
 * 
 * param[in]    dev: device description, like '/dev/ttyUSB0'
 * 
 * @return	    none
 *
 */
void mx_hal_uart_dev_set(char* dev);

/**
 * 
 * @brief 	    Initialises a UART interface
 * 
 * param[in]    uart: the interface which should be initialised
 * 
 * @return	    status
 *
 */
int32_t mx_hal_uart_init(uart_dev_t* uart);

/**
 * 
 * @brief 	    Deinitialises a UART interface
 * 
 * param[in]    uart: the interface which should be deinitialised
 * 
 * @return	    status
 *
 */
int32_t mx_hal_uart_finalize(uart_dev_t* uart);

/**
 * 
 * @brief 	    Transmit data on a UART interface
 * 
 * param[in]    uart    : the UART interface
 * param[in]    data    : pointer to the start of data
 * param[in]    size    : number of bytes to transmit
 * param[in]    timeout : timeout in milisecond
 * 
 * @return	    status
 *
 */
int32_t mx_hal_uart_send(uart_dev_t* uart, const void* data, uint32_t size, uint32_t timeout);

/**
 * 
 * @brief 	    Receive data on a UART interface
 * 
 * param[in]    uart        : the UART interface
 * param[in]    data        : pointer to the buffer which will store incoming data
 * param[in]    expect_size : number of bytes to Expected data received
 * param[out]   recv_size   : number of bytes to Actual data received
 * param[in]    timeout     : timeout in milisecond
 * 
 * @return	    status
 *
 */
int32_t mx_hal_uart_recv(uart_dev_t* uart, void* data, uint32_t expect_size, uint32_t* recv_size, uint32_t timeout);

/******************************************************************************
 *                                Serial Function
 *                      Serial port driver used for AT parser
 ******************************************************************************/

/**
 * 
 * @brief 	    Initialises AT instruction port
 * 
 * param[in]    timeout: timeout in milisecond
 * 
 * @return	    status
 *
 */
void mx_hal_serial_init(int timeout);

/**
 * 
 * @brief 	    AT instruction port receiving timeout time
 * 
 * param[in]    timeout : timeout in milisecond
 * 
 * @return	    status
 *
 */
void mx_hal_serial_set_timeout(int timeout);

/**
 * 
 * @brief 	    AT instruction port output
 * 
 * param[in]    c : Output one byte
 * 
 * @return	    status
 *
 */
int mx_hal_serial_putc(char c);

/**
 * 
 * @brief 	    AT instruction port input
 * 
 * @return	    byte
 *
 */
int mx_hal_serial_getc(void);

/**
 * 
 * @brief 	    AT instruction port status
 * 
 * @return	    status
 *
 */
bool mx_hal_serial_readable(void);

/**
 * 
 * @brief 	    AT instruction port flush
 * 
 * @return	    none
 *
 */
void mx_hal_serial_flush(void);

/******************************************************************************
 *                                Stdio Function
 *                                For debug info
 ******************************************************************************/

/**
 * 
 * @brief 	    Initialises Debug port
 * 
 * @return	    none
 *
 */
void mx_hal_stdio_init(void);

/**
 * 
 * @brief 	    Command line output
 * 
 * param[in]    c : Output one byte
 * 
 * @return	    status
 *
 */
int mx_hal_cli_putstr(const char* str, uint32_t strlen);

/**
 * 
 * @brief 	    Command line intput
 * 
 * @return	    byte
 *
 */
int mx_hal_cli_getc(void);

/******************************************************************************
 *                                Ticker Function
 *                      Tick used for delay and timeout mechanism
 ******************************************************************************/

/**
 * 
 * @brief 	    System clock initialization
 * 
 * @return	    none
 *
 */
void mx_hal_ms_ticker_init(void);

/**
 * 
 * @brief 	    Get system clock
 * 
 * @return	    system time
 *
 */
uint32_t mx_hal_ms_ticker_read(void);

/**
 * 
 * @brief 	    Suspend current thread for a specific time
 * 
 * param[in]    delay : A time interval (Unit: millisecond)
 * 
 * @return	    none
 *
 */
void mx_hal_delay_ms(volatile uint32_t delay);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
