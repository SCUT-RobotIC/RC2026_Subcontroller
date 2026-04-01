## 分控板驱动协议及使用说明
### 基本参数
* can_id    : 0x70~0x77
* master_id : 0x78~0x7F
* 波特率    : 1Mbps

<br>

### 通信类型1 - 数据包接收
##### 方向 主控->分控
##### fdcan_tx_header_dji.DataLength = FDCAN_DLC_BYTES_8
|数据域|bit[28:25]|bit[24]|bit[23:8]|bit[7:0]|数据区<br>Byte[0:7]|
|:---:|:---:|:---:|:---:|:---:|:---:|
|举个栗子|0x01|0x1|0x00 0x11|0x70|0x22 33 44 55 66 77 88 99|
|描述|通信类型<br>0b0001|0b1 <br>0~传输数据包[0]-[9]; <br>1~传输数据包[10]-[19]|dataPacket [0]&[1] / [10]&[11]|target can_id|dataPacket [2]-[9] / [12]-[19]|

###### 数据包格式为 uint8_t [10]

<br>

### 通信类型2 - 数据包发送
##### 方向 分控->主控
##### fdcan_tx_header_dji.DataLength = FDCAN_DLC_BYTES_8
|数据域|bit[28:25]|bit[24]|bit[23:8]|bit[7:0]|数据区<br>Byte[0:7]|
|:---:|:---:|:---:|:---:|:---:|:---:|
|举个栗子|0x01|0x0|0x00 0x11(0-65535)|0x70|0x22 33 44 55 66 77 88 99|
|描述|通信类型<br>0b0001|0b1 <br>0~传输数据包[0]-[7]; <br>1~传输数据包[8]-[15]|心跳计数 <br>1000Hz,到达60000后置零|target can_id|dataPacket [0]-[7] / [8]-[15]|

###### 数据包格式为 uint8_t [10]

### 通信类型3 - 大疆电机参数调整
##### 方向 主控->分控
##### fdcan_tx_header_dji.DataLength = FDCAN_DLC_BYTES_4
|数据域|bit[28:25]|bit[24]|bit[23:20]|bit[15:8]|bit[7:0]|Byte[3:0]|
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|举个栗子|0x2|0x1|0x1|0x1|0x70|0x01 23 45 67|
|描述|通信类型<br>0b0010|canport (1~2) -1|motor id (1~8) -1|待修改参数id|target can_id|目标值<br>float/int32_t|
#### DJI电机参数id对应表
|target code|parameter|type|discription|
|:---:|:---:|:---:|:---:|
|0x1|mode|int32_t|VEL:1<br>ANG:2|
|0x2|target|float|/|
|0x3|spd_kp|float|/|
|0x4|spd_ki|float|/|
|0x5|spd_kd|float|/|
|0x6|spd_deadband|float|/|
|0x7|ang_a_kp|float|/|
|0x8|ang_a_ki|float|/|
|0x9|ang_a_kd|float|/|
|0xA|ang_a_deadband|float|/|
|0xB|ang_s_kp|float|/|
|0xC|ang_s_ki|float|/|
|0xD|ang_s_kd|float|/|
|0xE|ang_s_deadband|float|/|

<br>

### 通信类型4 - 达妙电机参数调整
##### 方向 主控->分控
##### fdcan_tx_header_dji.DataLength = FDCAN_DLC_BYTES_4
|数据域|bit[28:25]|bit[24]|bit[23:20]|bit[15:8]|bit[7:0]|Byte[3:0]|
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|举个栗子|0x3|0x1|0x1|0x1|0x70|0x01 23 45 67|
|描述|通信类型<br>0b0011|canport (1~2) -1|motor id (1~8) -1|待修改参数id|target can_id|目标值<br>float/int32_t|
#### 达妙电机参数id对应表
|target code|parameter|type|discription|
|:---:|:---:|:---:|:---:|
|0x1|mode|int32_t|mit_mode:0<br>pos_mode:1<br>spd_mode:2<br>psi_mode:3|
|0x2|mit_pos|float|/|
|0x3|mit_vel|float|/|
|0x4|mit_kp|float|/|
|0x5|mit_kd|float|/|
|0x6|mit_torque|float|/|
|0x7|pos_pos|float|/|
|0x8|pos_vel|float|/|
|0x9|spd_vel|float|/|
|0xA|psi_pos|float|/|
|0xB|psi_vel|float|/|
|0xC|psi_cur|float|/|

<br>

### 通信类型5 - VESC参数调整
##### 方向 主控->分控
##### fdcan_tx_header_dji.DataLength = FDCAN_DLC_BYTES_4
|数据域|bit[28:25]|bit[24]|bit[23:20]|bit[15:8]|bit[7:0]|Byte[3:0]|
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|举个栗子|0x4|0x1|0x1|0x1|0x70|0x01 23 45 67|
|描述|通信类型<br>0b0100|canport (1~2) -1|motor id (1~8) -1|待修改参数id|target can_id|目标值<br>float/int32_t|
#### VESC参数id对应表
|target code|parameter|type|discription|
|:---:|:---:|:---:|:---:|
|0x1|mode|uint32_t|duty:0<br>current:1<br>brake current:2<br>speed:3<br>position:4|
|0x2|data|int32_t|/|

<br>

### 通信类型6 - 灵足参数调整
##### 方向 主控->分控
##### fdcan_tx_header_dji.DataLength = FDCAN_DLC_BYTES_4
|数据域|bit[28:25]|bit[24]|bit[23:20]|bit[15:8]|bit[7:0]|Byte[3:0]|
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|举个栗子|0x5|0x1|0x1|0x1|0x70|0x01 23 45 67|
|描述|通信类型<br>0b0101|canport (1~2) -1|motor id (1~8) -1|待修改参数id|target can_id|目标值<br>float/int32_t|
#### 灵足参数id对应表
|target code|parameter|type|discription|
|:---:|:---:|:---:|:---:|
|0x1|mode|uint32_t|move_control_mode:0<br>pos_control_mode:1<br>speed_control_mode:2<br>elect_control_mode:3<br>set_Zero_mode:4<br>csp_control_mode:5|
|0x2|ang_tar_f|float|/|
|0x3|spd_tar_f|float|/|
|0x4|tor_tar_f|float|/|
|0x5|acc_tar_f|float|/|
|0x6|kp_tar_f|float|/|
|0x7|kd_tar_f|float|/|

<br>