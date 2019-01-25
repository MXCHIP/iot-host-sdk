
## QLINK指令

|序号|指令|描述|
|---|---|---|
|1|AT+QLINKPRODUCT|设置/读取QLINK产品参数|
|2|AT+QLINKVERSION|设置/查询QLINK版本参数|
|3|AT+QLINKDM|设置/查询DM参数|
|4|AT+QLINKRAWSTART|启动QLINK一键配网|
|5|AT+QLINKUSERSTART|启动QLINK用户配网|
|6|AT+QLINKSTART|启动QLINK服务|
|7|AT+QLINKSTOP|结束QLINK服务|
|8|AT+QLINKSTATUS|查询QLINK服务状态|
|9|AT+QLINKSEND|设备发送数据|
|10|AT+QLINKPARAM|设备收到平台设置指令后返回给WIFI模块|

### AT+QLINKPRODUCT
功能：查询/设置QLINK product info

|查询指令：|`AT+QLINKPRODUCT？`|
|---|:---|
|响应：|`+QLINKPRODUCT:<device_id>,<product_token>,<andlink_token>`|
||`OK`|

|设置指令：|`AT+QLINKPRODUCT=<device_id>，,<product_token>,<andlink_token>`|
|---|:---|
|响应：|`OK`|
|参数：|`product_id`：产品ID|
||`product_token`：product token|
||`andlink_token`：andlink token|


### AT+QLINKVERSION
功能：查询/设置QLINK product version info

|查询指令：|`AT+QLINKVERSION？`|
|---|:---|
|响应：|`+QLINKVERSION:<firmware_version>,<software_version>`|
||`OK`|

|设置指令：|`AT+QLINKDEV=<firmware_version>,<software_version>`|
|---|:---|
|响应：|`OK`|
|参数：|`firmware_version`：WIFI固件版本信息|
||`software_version`：系统软件版本信息|

### AT+QLINKDM
功能：查询/设置QLINK DM info

|查询指令：|`AT+QLINKDM？`|
|---|:---|
|响应：|`+QLINKDM:<enable>,<envir>,<cmei>,<version>,<appkey>,<secret>`|
||`OK`|

|设置指令：|`AT+QLINKDEV=<enable>,<envir>,<cmei>,<version>,<appkey>,<secret>`|
|---|:---|
|响应：|`OK`|
|参数：|`enable`：1:开启DM       0:关闭DM|
||`envir`：1:测试环境      0正式环境|
||`cmei`： 从移动终端公司获取|
||`version`：从移动终端公司获取|
||`appkey`：从移动终端公司获取|
||`secret`：从移动终端公司获取|

### AT+AT+QLINKRAWSTART
功能：开始QLINK一键配网，如果一键配网超时，自动进入用户配网模式

|执行指令：|`AT+QLINKRAWSTART`|
|---|:---|
|响应：|`OK`|

### AT+AT+QLINKUSERSTART
功能：开始QLINK用户配网，绑定成功前，系统默认进入此模式

|执行指令：|`AT+QLINKRAWSTART`|
|---|:---|
|响应：|`OK`|

### AT+QLINKSTART
功能：启动QLINK服务

|执行指令：|`AT+QLINKSTART`|
|---|:---|
|响应：|`OK`|

### AT+QLINKSTOP
功能：停止QLINK服务

|执行指令：|`AT+QLINKSTOP`|
|---|:---|
|响应：|`OK`|

### AT+QLINKSTATUS
功能：查询QLINK服务状态

|查询指令：|`AT+QLINKSTATUS?`|
|---|:---|
|响应：|`+QLINKSTATUS:<status>`|
||`OK`|
|参数：|`status`：NONE:未初始化；INIT:启动QLINK,开始连接服务器；CONNECT:连接到服务器；DISCONNECT:断开服务器|

### AT+QLINKSEND
功能：向服务器发送数据

|设置指令：|`AT+QLINKSEND=<msg_type>，<data_len>`|
|---|:---|
|响应|`>`|
|响应：|`OK`|
|参数：|`msg_type`：Inform,File,ParamChange,Data|
||`data_len`：数据长度|

### AT+QLINKPARAM
功能：设备收到平台设置指令后返回给WIFI模块

|设置指令：|`AT+QLINKSEND=<type>`|
|---|:---|
|响应|`>`|
|响应：|`OK`|
|参数：|`type`：1:当前字段仅有配置功能，0:当前字段既有配置功能又有控制功能，每次收到服务器下发字段都需要发送给WIFI模块|


## QLINK事件通知
|序号|指令|指令说明|
|---|:---|:---|
|1|`+QLINKEVENT=QLINK,<status>`|连接服务器状态。status：CONNECT:连接；DISCONNECT:断开|
|2|`+QLINKEVENT=GET`|服务器请求设备状态|
|3|`+QLINKEVENT=SET,<data_len>,<data>`|data_len:数据长度；data:透传数据|
|4|`+QLINKEVENT=SET,6,UNBIND`|解绑设备|
|5|`+QLINKEVENT=SET,6,REBOOT`|重启设备|
|6|`+QLINKEVENT=SET,7,FACTORY`|恢复出厂设置|

# QLINK AT demo


```
/*说明：
 *该测试例程是阿里智能厂测包，配网入口V3热点配网,数据格式是JSON
 */

/*设置QLINK 产品信息*/
AT+QLINKPRODUCT=QLINK_product,QLINKTEST_LIVING_LIGHT_QLINK_TEST,5gPFl8G4GyFZ1fPWk20m,ngthgTlZ65bX5LpViKIWNsDPhOf2As9ChnoL9gQb,JSON\r

如果使用透传：
AT+QLINKPRODUCT=QLINK_TEST_LUA,QLINKTEST_LIVING_LIGHT_QLINK_TEST_LUA,PpvWt4yEJhWW9f987meF,c2IGOfujoX7JrbixF1yHNf4tcD8J2rAOIv7AszV8,RAW\r

/*设置QLINK 设备信息*/
AT+QLINKDEV=LIGHT,LIVING,QLINKTEST\r

/*查询QLINK 产品信息*/
AT+QLINKPRODUCT?

/*查询QLINK 设备信息*/
AT+QLINKDEV？

/*开启本地网关一键配网模式*/
AT+QLINKRAWSTART

/*开启云网关网关配网模式，默认优先云网关配网*/
AT+QLINKUSERSTART

/*当模块连接连上路由器后，模块主动下发*/
+WEVENT:WIFI,CONNECT

/*当连上路由器后，启动 QLINK 服务*/
AT+QLINKSTART\r

/*当 QLINK 连接服务器成功后会主动下发*/
+EVENT:QLINK,CONNECT

/*当 QLINK 与服务器断开 会主动下发*/
+EVENT:QLINK,DISCONNECT

/*查询 QLINK 连接状态*/
AT+QLINKSTATUS?

/*当APP调用请求接口后，会主动请求设备状态，此时模块主动下发*/
+QLINKEVENT:GET

/*发送所有数据*/
AT+QLINKSEND=Inform,59\r
{"params":[{"paramCode":"permitJoining","paramValue":"1"}]}

/*当APP调用控制接口后，会主动下发指令控制设备状态，此时模块主动下发，MCU需要返回AT+QLINKPARAM指令*/
+QLINKEVENT:SET,<len>,<data>

/*当APP调用重启接口，此时模块主动下发*/
+QLINKEVENT:SET,6,REBOOT

/*当APP调用复位接口，此时模块主动下发，用户需要清除保存的参数，回复出厂设置*/
+QLINKEVENT:SET,7,FACTORY

/*当APP调用解绑接口，此时模块主动下发，通知用户设备已经解绑*/
+QLINKEVENT:SET,6,UNBIND

```

