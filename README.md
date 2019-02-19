# How to use iot-sdk


## 1. 简介
本 sdk 提供一个基于 Linux和STM32F411RE，通过与 Wi-Fi 模块进行 AT 指令串口通信，实现与阿里云 ILOP 和 sds 以及中国移动杭研平台 直连的 IoT 物联网典型开发应用示例源代码,master分支是基于linux平台，NUCLEOF411RE分支是基于IAR和GCC平台。

## 2. 目录结构

```c
|---example
|   |---ilopicaapp  /*ilop平台 ica标准数据格式的demo*/
|   |---iloprawapp  /*ilop平台 自定义数据格式的demo*/
|   |---sdsapp      /*sds平台的demo*/
|   |---qlinkapp    /*杭研平台的demo*/
|   |---test        /*测试demo*/
|
|---platform
|   |---linux       /*linux平台相关的接口实现*/
|   |---mx_hal.h
|
|---src 
    |---alicloud_ilop   /*ilop平台的实现*/
    |---alicloud_sds    /*sds平台的实现*/
    |---helper          /*at指令解析内核*/
```

## 3. Linux 平台开发

### 3.1 准备

- 串口接入PC，并查询设备，命令：`ls -l /dev/ttyUSB*`

```
parallels@parallels-vm:~/iot-sdk$ ls -l /dev/ttyUSB*
crw-rw---- 1 root dialout 188, 0 Sep  3 16:15 /dev/ttyUSB0
```

### 3.2 编译
iot-sdk 提供四个demo，编译demo的命令格式 `make <demo>`

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

### 4.3 杭研 qlink 平台的demo

#### 4.3.1 使用流程

##### 4.3.1.1 前期准备

* (1) 获取客户在杭研申请账号密码
* (2) 同时选择最终上线方式，是使用本地网关+云网关方式，还是仅仅使用云网关方式上线

```
本地网关+云网关：
    概念：
        所谓本地网关是指杭研提供的智能家庭网关，安装移动宽带后会免费赠送杭研智能家庭网关，必须使用杭研网关，不能使用其他路由器；
        云网关可以理解为远程服务器，杭研平台根据设备的上线地址，负载仲裁后给出一个地址的服务器给到设备，设备和此服务器进行数据交互，杭研后台做处理在显示到APP。
    优点：
        杭研目前在推这种方式，目前移动各个营业展厅，是采用杭研智能家庭网关进行产品演示功能，吸引用户购买。
    缺点：
        这种方式要进行两轮测试流程，可能耗时比较长，一般是本地网关现在开发环境进行开发，用户搭建虚拟开发环境，自测OK后，需要到杭研现场验证，进入门户环境，门户环境验证OK后会进入测试环，测试可以和APP联调，APP目前可以由厂商自行开发/或者杭研开发/杭研公版APP，测试环境结合APP快验OK后，进入生产环境，生产环境验证OK后，需要将设备寄到北京终端公司进行DM验证；
    总结：
        如上流程走完后基本完成了本地网关的入库，但如果产品上线的话，还需要继续完成云网关的认证，云网关认证需要首先提交配置资料，杭研进行配置参数信息，便可以基于云网关进行调试，目前云网关和本地网关接口基本一致，只需要修改下配网接口即可，验证流程类似本地网关，需要先在开发环境进行验证，验证OK后，杭研快验流程没问题后，需要在测试环境进行测试，测试环境验证OK后，即可进入生产环境验证，生产环境验收OK后，整个项目方可以上线。

云网关：
    优点：
        目前验证流程简化很多，加速产品上线
    缺点：
        目前移动展厅均是展示基于本地网关产品，尚不清楚是否会展示基于云网关的产品。

```
* (3) 确定好开发方式之后，接下来需要在杭研后台建立新产品，并提交参数审核表给到杭研邵慧华经理审核，在提交审核表的同事，WIFI这边可以同步开发，但是必须待杭研审核表完成之后，才可以提交设备到杭研测试部门，如果是本地网关还需要开发人员现场验证


##### 4.3.1.2 本地网关开发联调

* (1) 参数审核表提交后，可以进入开发阶段，搭建虚拟网关开发环境（详见lib_qlink_simplify中materials文件夹）

* (2) 进行杭研智能网关环境下验证(使用虚拟网关开发调试)

* (3) 在虚拟网关下开发OK后，需要到杭研现场验收，此时在门户环境下验收，
* (4) 验收通过后设备会进入测试环境，测试环境便可以配置APP相关参数信息，和APP联调了；配置参数信息是指：厂家提交配网相关资料，杭研配置成XML文件集成到H5开发的和家亲APP(此步奏或许可以提前，需要厂商和杭研沟通)
* (5) 完成门户环境验收即可进入测试环境，测试环境会首先配置好APP信息，然后进行快验（杭研李宇飞），快验OK后，会进入测试环境下验收，测试部验收通过后，便可进入本地网关正式环境；
* (6) 本地网关开发OK后，还不能够上线，需要在云网关下进行验证

* (7) 使用lib_qlink/materials下的和家亲APP测试(开发环境以.199结尾)，
* (8) 杭研云网关验证
 
##### 4.3.1.3 云网关开发联调

* (1) 如果之前进行了本地网关验证，进行云网关验证时候需要首先修改配网接口，默认是调用本地网关接口，如果2分钟内没有进行本地网关配网，会自动进入云网关配网模式，建议使用按键区分，调用不同接口进入不同方式
* (2) 厂家提交配网相关资料，杭研配置成XML文件集成到H5开发的和家亲APP
* (3) 提交到庞军那进行开发环境快验(类似本地网关的门户环境验收)，开发过去或者寄送设备过去
* (4) 验证通过后会进入测试环境，此时需要杭研测试组进行测试环境下验证
* (5) 杭研测试组在测试环境验收的同事，可以进行DM部分验证，这次验证需要寄送设备到北京-中国移动终端公司
 
#### 4.3.2 使用方法

(1) 庆科wifi模块烧录杭研qlink最新[AT固件](http://developer.mxchip.com/downloads/22/)

    
固件烧录具体方法可以参考resource/common/烧录路径下相关文档。

(2) 修改产品信息
      

新建产品后，可以获得product_token,andlink_token,product_id,豪恩报警盒子产品信息如下：
<img src="./resource/qlink/image/产品资料.png" width="500" hegiht="313" align=center />
```
初始化产品参数
const emh_qlink_config_t config = {
    .product_info = {
        .product_id   = "30531",
        .product_token = "PXmsE6kQmxEgc0e3",
        .andlink_token = "ZxMo0zJvvZMWP3Az",
        .format = EMH_ARG_QLINK_FORMAT_JSON,
    },
    .version_info = {
        .firmware_version = "1.4.3",
        .software_version = "1.0.1",
    }
};

```

(3) 编译运行demo


* make clean
```
wumingju@ubuntu:/mnt/hgfs/Work/project/AT_demo/at_host_qlink$ make clean
cleaning...
done
```

* make qlinkapp
 
```
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
compiling src/helper/emhost/emh_qlink.c
generating build/iot-qlinkapp.elf
   text    data     bss     dec     hex filename
  22826    1168    3696   27690    6c2a build/iot-qlinkapp.elf
```

* 运行 sudo ./build/iot-qlinkapp.elf

```
dev[/dev/ttyUSB0]
[APP: main.c: 150] open cli function
open at uart succeed
[QLINK: cloud_qlink.c: 119] FW version: basic_AT_v2.1.2
[APP: main.c:  30] WiFi config in AP mode......
[APP: main.c:  30] WiFi config in AP mode......
[APP: main.c:  34] Wi-Fi connected
[APP: main.c:  42] Cloud connected
```

(4) 基于虚拟网关使用说明


* 按照4.3.1使用流程中说明搭建好本地虚拟网关开发环境
 
* 测试本地环境是否OK

  如果本地环境OK，点击配网andlinkdemo1.0.apk 开始连接按钮，会有如下页面：
<img src="./resource/qlink/image/环境OK.png" width="500" hegiht="313" align=center />
  
* 配网操作 
  
  通过cli串口发送 raw start命令 或者 按键触发调用qlink_raw_start()接口使设备进入本地网关一键配网模式，

  同时手机APP点击开始连接按钮，绑定成功后APP提示添加成功，后台虚拟网关下面会显示出设备，如下：
<img src="./resource/qlink/image/bind_ok.png" width="500" hegiht="313" align=center />
* 数据下发
    
  通过后台获取所有字段，操作如下所示：

<img src="./resource/qlink/image/request_all_data.png" width="500" hegiht="313" align=center />

  通过后台获取部分字段，操作如下所示：
  
<img src="./resource/qlink/image/request_some_data.png" width="500" hegiht="313" align=center />
    
  通过后台下发命令，操作如下所示：
  
<img src="./resource/qlink/image/control_data.png" width="500" hegiht="313" align=center />
  
  设备收到云端获取字段指令后，需要调用mx_status emh_qlink_send_json_to_cloud( char *type, uint8_t *data, uint32_t len ) 函数将数据发送到云端
  
* 数据上传
    
  mx_status qlink_send_data(char *type, uint8_t *data, uint32_t len)，通过此函数 即可完成发送数据的功能

```
type: 为发送数据的类型，注意主动上报和响应要选择不同类型
	
 		  设备主动上报：        type = Inform
  		  人为触发的数据上报：  type = ParamChange
    	  云端请求上报：        type = Data
  		  文件操作：            type = File
data:上传具体内容

```
  也可通过cli串口输入send type <data> 发送数据

(5) 基于云网关使用说明
 
* 提交XML配网资料 
  
  完成本地网关验证后，可以提交配网引导界面相关资料，杭研配置XML到手机APP，之后方可进行云网关验证操作，
  提交配网资料可以提交本地网关+云网关方式资料或者仅通过云网关配网资料，建议提供只用云网关配网资料，
  如此一来，上线验证只需要验证云网关部分即可，减少验证流程，提高上线时间
  
* 配网操作 
  
  通过cli串口发送 user start命令 或者 按键触发调用qlink_user_start()接口使设备进入云网关用户配网模式，

  同时手机APP(resource/qlink/2.云网关/app路径)进入添加设备页面，扫描到如下页面，说明设备进入了配网页面：
  
<img src="./resource/qlink/image/user_config.png" width="500" hegiht="313" align=center />
  
  点击弹出来的添加按钮即可进行配网操作
  

#### 4.3.3 注意事项
    
(1) 云端查询数据


  回调函数emh_ev_qlink_get_local_attrs()收到数据时候，用户需要根据收到的内容选择发送给云端的数据，组成json包后再调用qlink_send_data()接口将数据发送给远端，注意类型应该是"ParamChange"类型。

(2) 云端下发命令数据
  
  云端下发命令，回调函数emh_ev_qlink_set_local_attrs()会收到数据，用户需要根据字段的类型进行处理，如果字段即下发又要求上报，则return 1,否则return 0。 

(3) 其他


联系人：庆科李博

邮箱：  libo@mxchip.com

## 5. 移植SDK

- 只需要实现platform/mx_hal.h文件中的函数即可
