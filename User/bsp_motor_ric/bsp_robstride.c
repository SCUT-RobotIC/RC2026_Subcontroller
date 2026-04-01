#include "bsp_robstride.h"
#include <string.h>

FDCAN_TxHeaderTypeDef fdcan_tx_header_robstride;
uint8_t fdcan_tx_data_robstride[8];
motor_robstride_t motor_robstride[2][16] = {0};//3 CAN buses,16 motors per bus

/****************************************************************     
  @功能     		: 延时微秒函数
****************************************************************/
void delay_us(uint32_t us) 
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 8000000 * us); 
    while (delay--); 
}

/*************************************************************************************************
    以下均为 RobStride 电机通信函数
  @通信类型1    运控模式电机控制指令
  @通信类型3    使能电机
  @通信类型4    失能电机
  @通信类型6    设置机械零位
  @通信类型17   单个参数读取
  @通信类型18   单个参数设置
  @通信类型22   参数保存
  @通信类型24   电机主动上报
*************************************************************************************************/

/****************************************************************
  @通信类型1    
  @功能     		: 运控模式电机控制指令
  @参数         : 角度设定值，速度设定值，kp设定值，kd设定值
  @应答         : 应答电机反馈帧
****************************************************************/
void robstride_move_control_send(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor,uint16_t ang_tar,uint16_t spd_tar,
                           uint16_t kp_tar,uint16_t kd_tar,uint16_t tor_tar)
{
  //while(motor_robstride->getResponse == 1) delay_us(5);
  fdcan_tx_header_robstride.Identifier = 0x1 << 24 | tor_tar << 8 | motor->id;
  fdcan_tx_header_robstride.IdType = FDCAN_EXTENDED_ID;
  fdcan_tx_header_robstride.TxFrameType = FDCAN_DATA_FRAME;
  fdcan_tx_header_robstride.DataLength = FDCAN_DLC_BYTES_8;

  fdcan_tx_data_robstride[0] = (ang_tar >> 8) & 0xFF;
  fdcan_tx_data_robstride[1] = ang_tar & 0xFF;
  fdcan_tx_data_robstride[2] = (spd_tar >> 8) & 0xFF;
  fdcan_tx_data_robstride[3] = spd_tar & 0xFF;
  fdcan_tx_data_robstride[4] = (kp_tar >> 8) & 0xFF;
  fdcan_tx_data_robstride[5] = kp_tar & 0xFF;
  fdcan_tx_data_robstride[6] = (kd_tar >> 8) & 0xFF;
  fdcan_tx_data_robstride[7] = kd_tar & 0xFF;

  HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &fdcan_tx_header_robstride, fdcan_tx_data_robstride);
	motor->getResponse = 1;
}
/****************************************************************
  @通信类型3     
  @功能     		: 使能电机
  @参数         : None
  @应答         : 应答电机反馈帧
****************************************************************/
void robstride_enable(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride)
{
  //while(motor_robstride->getResponse == 1) delay_us(5);
  fdcan_tx_header_robstride.Identifier = 0x3 << 24 | motor_robstride->master_id << 8 | motor_robstride->id;
  fdcan_tx_header_robstride.IdType = FDCAN_EXTENDED_ID;
  fdcan_tx_header_robstride.TxFrameType = FDCAN_DATA_FRAME;
  fdcan_tx_header_robstride.DataLength = FDCAN_DLC_BYTES_8;

  memset(fdcan_tx_data_robstride, 0, 8);

  HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &fdcan_tx_header_robstride, fdcan_tx_data_robstride);
  
  motor_robstride->getResponse = 1;	
}

/****************************************************************
  @通信类型4     
  @功能     		: 失能电机
  @参数         : None
  @应答         : 应答电机反馈帧
****************************************************************/
void robstride_disable(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride)
{
  //while(motor_robstride->getResponse == 1) delay_us(5);
  fdcan_tx_header_robstride.Identifier = 0x4 << 24 | motor_robstride->master_id << 8 | motor_robstride->id;
  fdcan_tx_header_robstride.IdType = FDCAN_EXTENDED_ID;
  fdcan_tx_header_robstride.TxFrameType = FDCAN_DATA_FRAME;
  fdcan_tx_header_robstride.DataLength = FDCAN_DLC_BYTES_8;

  memset(fdcan_tx_data_robstride, 0, 8);

  HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &fdcan_tx_header_robstride, fdcan_tx_data_robstride);

  motor_robstride->getResponse = 1;
}

/****************************************************************
  @通信类型6     
  @功能     		: 设置电机机械零位
  @参数         : None
  @应答         : 应答电机反馈帧
****************************************************************/
void robstride_set_zero_position(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride)
{
  //while(motor_robstride->getResponse == 1) delay_us(5);
  fdcan_tx_header_robstride.Identifier = 0x6 << 24 | motor_robstride->master_id << 8 | motor_robstride->id;
  fdcan_tx_header_robstride.IdType = FDCAN_EXTENDED_ID;
  fdcan_tx_header_robstride.TxFrameType = FDCAN_DATA_FRAME;
  fdcan_tx_header_robstride.DataLength = FDCAN_DLC_BYTES_8;

  fdcan_tx_data_robstride[0] = 01;
  fdcan_tx_data_robstride[1] = 00;
  fdcan_tx_data_robstride[2] = 00;
  fdcan_tx_data_robstride[3] = 00;
  fdcan_tx_data_robstride[4] = 00;
  fdcan_tx_data_robstride[5] = 00;
  fdcan_tx_data_robstride[6] = 00;
  fdcan_tx_data_robstride[7] = 00;

  HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &fdcan_tx_header_robstride, fdcan_tx_data_robstride);

  motor_robstride->getResponse = 1;
}

/****************************************************************
  @通信类型17     
  @功能     		: 单个参数读取
  @参数         : 参数地址
  @应答         : 应答电机反馈帧
****************************************************************/
void robstride_get_motor_parameter(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride,uint16_t Index){
	//while(motor_robstride->getResponse == 1) delay_us(5);
  fdcan_tx_header_robstride.Identifier = 0x11 << 24 | motor_robstride->master_id << 8 | motor_robstride->id;
  fdcan_tx_header_robstride.IdType = FDCAN_EXTENDED_ID;
  fdcan_tx_header_robstride.TxFrameType = FDCAN_DATA_FRAME;
  fdcan_tx_header_robstride.DataLength = FDCAN_DLC_BYTES_8;
  fdcan_tx_data_robstride[0] = Index & 0xFF;
	fdcan_tx_data_robstride[1] = Index>>8 & 0xFF;
	fdcan_tx_data_robstride[2] = 0x00;
	fdcan_tx_data_robstride[3] = 0x00;	

	HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &fdcan_tx_header_robstride, fdcan_tx_data_robstride);	

  motor_robstride->getResponse = 1;
}

/****************************************************************
  @通信类型18     
  @功能     		: 设置电机参数
  @参数1        : 参数地址
  @参数2        : 参数数值
  @应答         : 应答电机反馈帧
****************************************************************/
void robstride_set_motor_parameter(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride,uint16_t Index, float Value, char Value_mode){
	//while(motor_robstride->getResponse == 1) delay_us(5);
  fdcan_tx_header_robstride.Identifier = 0x12 << 24 | motor_robstride->master_id << 8 | motor_robstride->id;
  fdcan_tx_header_robstride.IdType = FDCAN_EXTENDED_ID;
  fdcan_tx_header_robstride.TxFrameType = FDCAN_DATA_FRAME;
  fdcan_tx_header_robstride.DataLength = FDCAN_DLC_BYTES_8;
  fdcan_tx_data_robstride[0] = Index & 0xFF;
	fdcan_tx_data_robstride[1] = Index>>8 & 0xFF;
	fdcan_tx_data_robstride[2] = 0x00;
	fdcan_tx_data_robstride[3] = 0x00;	
	if (Value_mode == Set_parameter)
	{
		memcpy(&fdcan_tx_data_robstride[4],&Value,4);
	}
	else if (Value_mode == Set_mode)
	{
		fdcan_tx_data_robstride[4] = (uint8_t)Value;
		fdcan_tx_data_robstride[5] = 0x00;	
		fdcan_tx_data_robstride[6] = 0x00;	
		fdcan_tx_data_robstride[7] = 0x00;	
	}
	HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &fdcan_tx_header_robstride, fdcan_tx_data_robstride);	

  motor_robstride->getResponse = 1;
}

/****************************************************************
  @通信类型22  
  @功能     		: 电机数据保存
  @参数         : None
  @应答         ：应答电机反馈帧  
  @注意         : 仅可保存上位机模块内参数表功能码 0x20 开头参数
****************************************************************/
void robstride_data_save(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride){
	//while(motor_robstride->getResponse == 1) delay_us(5);
  fdcan_tx_header_robstride.Identifier = 0x16 << 24 | motor_robstride->master_id << 8 | motor_robstride->id;
  fdcan_tx_header_robstride.IdType = FDCAN_EXTENDED_ID;
  fdcan_tx_header_robstride.TxFrameType = FDCAN_DATA_FRAME;
  fdcan_tx_header_robstride.DataLength = FDCAN_DLC_BYTES_8;
  fdcan_tx_data_robstride[0] = 1;
	fdcan_tx_data_robstride[1] = 2;
	fdcan_tx_data_robstride[2] = 3;
	fdcan_tx_data_robstride[3] = 4;	
	fdcan_tx_data_robstride[4] = 5;
	fdcan_tx_data_robstride[5] = 6;	
	fdcan_tx_data_robstride[6] = 7;	
	fdcan_tx_data_robstride[7] = 8;	
	
	HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &fdcan_tx_header_robstride, fdcan_tx_data_robstride);

  motor_robstride->getResponse = 1;
}

/****************************************************************
  @通信类型24     
  @功能     		: 电机主动上报帧
  @参数         : None
  @应答         : 应答电机反馈帧
****************************************************************/
void robstride_autofeedback(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride)
{
  //while(motor_robstride->getResponse == 1) delay_us(5);
  fdcan_tx_header_robstride.Identifier = 0x18 << 24 | motor_robstride->master_id << 8 | motor_robstride->id;
  fdcan_tx_header_robstride.IdType = FDCAN_EXTENDED_ID;
  fdcan_tx_header_robstride.TxFrameType = FDCAN_DATA_FRAME;
  fdcan_tx_header_robstride.DataLength = FDCAN_DLC_BYTES_8;

  fdcan_tx_data_robstride[0] = 01;
  fdcan_tx_data_robstride[1] = 02;
  fdcan_tx_data_robstride[2] = 03;
  fdcan_tx_data_robstride[3] = 04;
  fdcan_tx_data_robstride[4] = 05;
  fdcan_tx_data_robstride[5] = 06;
  fdcan_tx_data_robstride[6] = 01;
  fdcan_tx_data_robstride[7] = 00;

  HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &fdcan_tx_header_robstride, fdcan_tx_data_robstride);

  motor_robstride->getResponse = 1;
}

/*************************************************************************************************
  以下为参数修改，均调用通信类型
*************************************************************************************************/
/****************************************************************
  @ 功能     		: 设置电机运行模式
  @ 参数        : 电机运行模式
                  move_control_mode   0	//运控模式
                  pos_control_mode    1	//插补位置模式
                  speed_control_mode  2 //速度模式
                  elect_control_mode  3 //电流模式
                  csp_control_mode    5	//位置模式
****************************************************************/
void robstride_set_mode(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride, uint8_t mode){
	robstride_set_motor_parameter(hfdcan, motor_robstride,0x7005, mode, Set_mode);
}

/****************************************************************
  @ 功能     		 : 设置运控模式参数
****************************************************************/
void robstride_move_control(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor)
{
  robstride_move_control_send(hfdcan, motor, motor->ang_tar, motor->spd_tar, motor->kp_tar, motor->kd_tar, motor->tor_tar);
}

/****************************************************************
  @ 功能     		 : 设置插补位置模式参数（PP）
  @ 函数1        : 目标位置
  @ 函数2        : 目标速度
  @ 函数3        : 目标加速度
****************************************************************/
void robstride_pos_set_position(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride,float Value){
	robstride_set_motor_parameter(hfdcan, motor_robstride,loc_ref, Value, Set_parameter);
}
void robstride_pos_set_velocity(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride,float Value){
  robstride_set_motor_parameter(hfdcan, motor_robstride,vel_max, Value, Set_parameter);
}
void robstride_pos_set_acceleration(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride,float Value){
  robstride_set_motor_parameter(hfdcan, motor_robstride,acc_set, Value, Set_parameter);
}

/****************************************************************
  @ 功能     		 : 设置速度模式参数
  @ 函数1        : 目标速度
  @ 函数2        : 目标加速度
****************************************************************/
void robstride_speed_set_velocity(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride,float Value){
	robstride_set_motor_parameter(hfdcan, motor_robstride,spd_ref, Value, Set_parameter);
}
void robstride_speed_set_acceleration(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride,float Value){
	robstride_set_motor_parameter(hfdcan, motor_robstride,acc_rad, Value, Set_parameter);
}
/****************************************************************
  @ 功能     		 : 设置电流模式参数
  @ 函数1        : 目标电流
****************************************************************/
void robstride_current_set_current(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride,float Value){
	robstride_set_motor_parameter(hfdcan, motor_robstride,iq_ref, Value, Set_parameter);
}

/****************************************************************
  @ 功能     		 : 设置位置模式参数（CSP）
  @ 函数1        : 目标位置
  @ 函数2        : 目标速度
****************************************************************/
void robstride_csp_set_position(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride,float Value){
	robstride_set_motor_parameter(hfdcan, motor_robstride,loc_ref, Value, Set_parameter);
}
void robstride_csp_set_velocity(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride,float Value){
  robstride_set_motor_parameter(hfdcan, motor_robstride,limit_spd, Value, Set_parameter);
}



/*************************************************************************************************
  以下为参数更新函数，在时钟中断回调中循环调用，周期10ms  (实测周期1ms卡应答)
************************************************************************************************/
/****************************************************************
  @ 功能     		 : 参数更新
  @ 参数         : motor_robstride_t结构体指针
****************************************************************/
void robstride_parameter_update(motor_robstride_t *motor)
{
  motor->last_ang_tar = motor->ang_tar;
  motor->last_spd_tar = motor->spd_tar;
  motor->last_tor_tar = motor->tor_tar;
  motor->last_kp_tar  = motor->kp_tar;
  motor->last_kd_tar  = motor->kd_tar;	

  motor->ang_tar = (uint16_t)((motor->ang_tar_f + motor->ang_max) / (2*motor->ang_max) * 65535.0f);
  motor->spd_tar = (uint16_t)((motor->spd_tar_f + motor->spd_max) / (2*motor->spd_max) * 65535.0f);
  motor->tor_tar = (uint16_t)((motor->tor_tar_f + motor->tor_max) / (2*motor->tor_max) * 65535.0f);
  motor->kp_tar  = (uint16_t)((motor->kp_tar_f / motor->kp_max) * 65535.0f);
  motor->kd_tar  = (uint16_t)((motor->kd_tar_f / motor->kd_max) * 65535.0f);
}

/****************************************************************
  @ 功能     		 : 命令更新
  @ 参数         : FDCAN指针 & motor_robstride_t结构体指针
****************************************************************/
void robstride_command_update(FDCAN_HandleTypeDef *hfdcan,motor_robstride_t *motor)
{
  if(motor->error != 0){
    robstride_disable(hfdcan, motor);
    motor->enabled = 0;
    return;
  }
  switch(motor->mode){
    case move_control_mode:{
      robstride_move_control(hfdcan, motor);
      break;
    }
    case pos_control_mode:{
      robstride_pos_set_position(hfdcan, motor, motor->ang_tar_f);
			if(motor->last_ang_tar != motor->ang_tar)
				robstride_pos_set_velocity(hfdcan, motor, motor->spd_tar_f);
				robstride_pos_set_acceleration(hfdcan, motor, motor->acc_tar_f);
      break;
    }
    case speed_control_mode:{
      robstride_speed_set_velocity(hfdcan, motor, motor->spd_tar_f);
      if(motor->last_spd_tar != motor->spd_tar)
				robstride_speed_set_acceleration(hfdcan, motor, motor->acc_tar_f);
      break;
    }
    case elect_control_mode:{
      robstride_current_set_current(hfdcan, motor, motor->tor_tar_f);
      break;
    }
    case csp_control_mode:{
      robstride_csp_set_position(hfdcan, motor, motor->ang_tar_f);
      if(motor->last_spd_tar != motor->spd_tar)
				robstride_csp_set_velocity(hfdcan, motor, motor->spd_tar_f);
      break;
    }
    default:{
      break;
    }
  }
  delay_us(50);
}

/****************************************************************
  @ 功能     		 : 所有电机命令更新
****************************************************************/
void robstride_command_update_all(void)
{
  for (int i = 0; i < 16; i++)
  {
    if (motor_robstride[0][i].enabled)
    {
      robstride_parameter_update(&motor_robstride[0][i]);
      robstride_command_update(&hfdcan1, &motor_robstride[0][i]);
    }
    if (motor_robstride[1][i].enabled)
    {
      robstride_parameter_update(&motor_robstride[1][i]);
      robstride_command_update(&hfdcan2, &motor_robstride[1][i]);
    }
  }
}


/****************************************************************
  @ 功能     		 : 电机信息更新
  @ 参数         : 数据数组指针
****************************************************************/
void robstride_info_update(motor_robstride_t *motor, uint32_t id, uint8_t *data)
{
  switch(id>>24){
    case 0x2:{}
		case 0x18:
      motor->error  = id>>16&0xF;
      motor->angle  = (float)((data[0] << 8) | data[1]) / 65535.0f * 2 *motor->ang_max - motor->ang_max;
      motor->speed  = (float)((data[2] << 8) | data[3]) / 65535.0f * 2 *motor->spd_max - motor->spd_max;
      motor->torque = (float)((data[4] << 8) | data[5]) / 65535.0f * 2 *motor->tor_max - motor->tor_max;
      motor->temperature = (float)((data[6] << 8) | data[7]) / 10.0f;
      break;
    case 0x11:{
      if((id>>16&0xFF)==0x00){
        motor->info_index= (data[1] << 8) | data[0];
        memcpy(&motor->info_value,&data[4],4);      
      }
      break;
    }
    case 0x15:{
      motor->error&=0b01000000;//byte6置1
      break;
    }
    default:
      break;
  }
  motor->getResponse = 0;
}

/****************************************************************
  @ 功能     		 : 电机初始化
  @ 参数         : CAN口号(1-3)，电机ID(1-16)，电机系列，电机模式
****************************************************************/
void robstride_init(uint8_t canport_1_to_2, uint8_t motorID_1_to_16, uint8_t series, uint8_t mode)
{
  if(   canport_1_to_2 < 1  || canport_1_to_2 > 3   || 
        motorID_1_to_16 < 1 || motorID_1_to_16 > 16 ||
        series < 0          || series > 6           ||
        mode < 0            || mode > 5                 ) 
    return;
  motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].id           = CAN_ID_ROBSTRIDE_M + motorID_1_to_16;
  motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].master_id    = MASTER_ID_ROBSTRIDE_M + motorID_1_to_16;
  motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].mode         = mode;
  motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].series       = series;
  motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].enabled      = 1;
  motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].getResponse  = 0;
  motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].error        = 0;
  motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].ang_tar_f    = 0.0f;
  motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].spd_tar_f    = 0.0f;
  motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].tor_tar_f    = 0.0f;
  motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].acc_tar_f    = 0.0f;
  motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].kp_tar_f     = 0.0f;
  motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].kd_tar_f     = 0.0f;
  switch(series)
  {
    case rs00:{
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].ang_max = rs00_max_angle;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].spd_max = rs00_max_speed;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].tor_max = rs00_max_torque;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].kp_max  = rs00_max_kp;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].kd_max  = rs00_max_kd;
      break;}
    case rs01:{
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].ang_max = rs01_max_angle;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].spd_max = rs01_max_speed;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].tor_max = rs01_max_torque;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].kp_max  = rs01_max_kp;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].kd_max  = rs01_max_kd;
      break;}
    case rs02:{
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].ang_max = rs02_max_angle;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].spd_max = rs02_max_speed;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].tor_max = rs02_max_torque;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].kp_max  = rs02_max_kp;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].kd_max  = rs02_max_kd;
      break;}
    case rs03:{
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].ang_max = rs03_max_angle;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].spd_max = rs03_max_speed;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].tor_max = rs03_max_torque;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].kp_max  = rs03_max_kp;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].kd_max  = rs03_max_kd;
      break;}
    case rs04:{
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].ang_max = rs04_max_angle;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].spd_max = rs04_max_speed;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].tor_max = rs04_max_torque;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].kp_max  = rs04_max_kp;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].kd_max  = rs04_max_kd;
      break;}
    case rs05:{
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].ang_max = rs05_max_angle;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].spd_max = rs05_max_speed;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].tor_max = rs05_max_torque;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].kp_max  = rs05_max_kp;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].kd_max  = rs05_max_kd;
      break;}
    case rs06:{
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].ang_max = rs06_max_angle;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].spd_max = rs06_max_speed;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].tor_max = rs06_max_torque;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].kp_max  = rs06_max_kp;
      motor_robstride[canport_1_to_2-1][motorID_1_to_16-1].kd_max  = rs06_max_kd;
      break;}
    default:{
      break;}
  }
	switch(canport_1_to_2){
		case 1:{
			delay_us(500);
			robstride_set_mode(&hfdcan1,&motor_robstride[canport_1_to_2-1][motorID_1_to_16-1],mode);
			delay_us(500);
			robstride_enable(&hfdcan1,&motor_robstride[canport_1_to_2-1][motorID_1_to_16-1]);
			break;
		}
		case 2:{
			delay_us(500);
			robstride_set_mode(&hfdcan2,&motor_robstride[canport_1_to_2-1][motorID_1_to_16-1],mode);
			delay_us(500);
			robstride_enable(&hfdcan2,&motor_robstride[canport_1_to_2-1][motorID_1_to_16-1]);
			break;
		}
		default:break;
	}
}
