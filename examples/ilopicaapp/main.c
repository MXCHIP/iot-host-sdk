/**
 ******************************************************************************
 * @file    main.c
 * @author  QQ Ding
 * @version V1.0.0
 * @date    3-Sept-2018
 * @brief   Main function
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
