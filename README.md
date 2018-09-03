# How to use iot-sdk


## 1. 简介
本 sdk 提供一个基于 Linux，通过与 Wi-Fi 模块进行 AT 指令串口通信，实现与阿里云 ILOP 和 sds 直连的 IoT 物联网典型开发应用示例源代码。

## 2. 目录结构

```c
|---example
|   |---ilopicaapp  /*ilop平台 ica标准数据格式的demo*/
|   |---iloprawapp  /*ilop平台 自定义数据格式的demo*/
|   |---sdsapp      /*sds平台的demo*/
|   |---test        /*测试demo*/
|
|---platform
|   |---linux       /*linux平台相关的接口实现*/
|   |---mx_hal.h
|
|---src 
    |---alicloud_ilop   /*ilop平台的实现*/
    |---alicloud_sds    /*sds平台的实现*/
    |---helper          /*at指令解析内核*/
```

## 3. Linux 平台开发

### 3.1 准备

- 串口接入PC，并查询设备，命令：`ls -l /dev/ttyUSB*`

```
parallels@parallels-vm:~/iot-sdk$ ls -l /dev/ttyUSB*
crw-rw---- 1 root dialout 188, 0 Sep  3 16:15 /dev/ttyUSB0
```

### 3.2 编译
iot-sdk 提供四个demo，编译demo的命令格式 `make <demo>`

- 清除上次的make命令所产生的build文件，命令：`make clean`

```
parallels@parallels-vm:~/iot-sdk$ make clean
cleaning...
done
```

- 编译测试demo，命令：`make test`

```
parallels@parallels-vm:~/iot-sdk$ make test
mkdir build		
compiling platform/linux/mx_serial.c
compiling platform/linux/mx_stdio.c
compiling platform/linux/mx_tick.c
compiling platform/linux/mx_uart.c
compiling src/helper/cli/mx_cli.c
compiling src/helper/emhost/ATCmdParser/ATCmdParser.c
compiling src/helper/emhost/emh_alisds.c
compiling src/helper/emhost/emh_arg.c
compiling src/helper/emhost/emh_ilop.c
compiling src/helper/emhost/emh_module.c
compiling src/helper/emhost/emh_wlan.c
compiling src/helper/mx_utils/mx_ringbuffer.c
compiling examples/test/test.c
generating build/iot-test.elf
   text	   data	    bss	    dec	    hex	filename
  16294	    928	   3680	  20902	   51a6	build/iot-test.elf
```

- 编译完成后，在build目录下生成 `iot-test.elf` 文件

### 3.3 运行

- 运行build目录下的elf文件，命令：`./build/iot-test.elf <dev>`

```
parallels@parallels-vm:~/iot-sdk$ sudo ./build/iot-test.elf /dev/ttyUSB0
[sudo] password for parallels: 
dev[/dev/ttyUSB0]
open at uart succeed
[APP: test.c:  19] FW version: ilop_AT_v2.1.4
[APP: test.c:  20] System tick: 757
```

## 4. IoT示例

### 4.1 阿里 ilop 平台的标准数据格式demo

### 4.2 阿里 ilop 平台的透传demo

## 4. 移植SDK

- 只需要实现platform/mx_hal.h文件中的函数即可
