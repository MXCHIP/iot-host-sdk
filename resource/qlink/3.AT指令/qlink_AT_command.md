
## QLINK指令

|序号|指令|描述|
|---|---|---|
|1|AT+QLINKPRODUCT|设置/读取QLINK产品参数|
|2|AT+QLINKVERSION|设置/查询QLINK版本参数|
|3|AT+QLINKRAWSTART|启动QLINK一键配网|
|4|AT+QLINKUSERSTART|启动QLINK用户配网|
|5|AT+QLINKSTART|启动QLINK服务|
|6|AT+QLINKSTOP|结束QLINK服务|
|7|AT+QLINKSTATUS|查询QLINK服务状态|
|8|AT+QLINKSEND|设备发送数据|

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
AT+QLINKSEND=Inform,123\r
{"ErrorCode":{"value":"0"},"Hue":{"value":"50"},"Luminance":{"value":"50"},"Switch":{"value":"1"},"WorkMode":{"value":"0"}}

/*当APP调用控制接口后，会主动下发指令控制设备状态，此时模块主动下发*/
+QLINKEVENT:SET,<len>,<data>

/*当APP调用重启接口，此时模块主动下发*/
+QLINKEVENT:SET,6,REBOOT

/*当APP调用复位接口，此时模块主动下发，用户需要清除保存的参数，回复出厂设置*/
+QLINKEVENT:SET,7,FACTORY

/*当APP调用解绑接口，此时模块主动下发，通知用户设备已经解绑*/
+QLINKEVENT:SET,6,UNBIND

```

