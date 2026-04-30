# RC2026_Subcontroller
### 旧版本资料在\User\bsp_subcontrol\history\
## version 1 
* author 91DrSabi
* date 2026-04-01
* description 
  分控使用CAN通信协议与主控进行数据交互，支持数据包的发送和接收。分控板的CAN ID范围为0x70~0x77，主控板的CAN ID范围为0x78~0x7F，通信波特率为1Mbps。
  具体通信协议见\User\bsp_subcontrol\subcontrol.md
  分控食用步骤：
  1. 硬件连接：
     1. 分控板的强弱电与can线共三根线作为输入
     2. 分控板的输出共支持8个xt30接口
  2. 软件配置：
     1. 在\User\bsp_subcontrol\bsp_subcontrol.h中修改 20 行的SUBCTRL_ID，范围0x70-0x77，上电后有一LED会闪烁ID对应的次数(0-8)然后停止0.5s后重复。
     2. 
     3. 正常开发基于STM32H723的控制程序。
  3. 主从通信：
     1. 分控板上使用FDCAN3接口与主控进行连接，在can口下方的开关控制分控端60Ω终端电阻的接入
     2. 在程序中修改subcontroller.transmit_data，然后调用subcontrol_transmit()进行发送
     3. FDCAN3接口接收数据后会存在sucontroller.received_data中，包的具体内容自行开发喵

## version 2
* author 91DrSabi
* date 2026-04-30
* description
    根据部分用户反馈，使用扩展ID进行操作的做法较为诡谲，因此修改主从通信的方式，从扩展帧改为标准帧，心跳不作为包的一部分定时发送
    具体通信协议见\User\bsp_subcontrol\subcontrolver2.md