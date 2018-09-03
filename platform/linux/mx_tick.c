/**
 ******************************************************************************
 * @file    mx_tick.c
 * @author  QQ Ding
 * @version V1.0.0
 * @date    3-Sept-2018
 * @brief   Tick for delay and timeout
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

#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "mx_common.h"

/******************************************************************************
 *                              Variable Definitions
 ******************************************************************************/

static struct timeval sys_start_time;

/******************************************************************************
 *                              Function Definitions
 ******************************************************************************/

void mx_hal_ms_ticker_init(void)
{
    return;
}

uint32_t mx_hal_ms_ticker_read(void)
{
    struct timeval tv;
    uint32_t ms;
    gettimeofday(&tv, NULL);
    timersub(&tv, &sys_start_time, &tv);
    ms = (uint32_t)(tv.tv_sec * 1000LL) + tv.tv_usec / 1000;
    return ms;
}

void mx_hal_delay_ms(volatile uint32_t delay)
{
    usleep(delay * 1000);
}
