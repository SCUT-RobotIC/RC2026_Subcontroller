#ifndef __BSP_ROBSTRIDE_H__
#define __BSP_ROBSTRIDE_H__

/****************************************************************     
  last update : 2026-01-12
  author      : DrSabi
  说明：
    1.初始化：使用robstride_init() 函数进行电机初始化，初始化会完成电机信息初始化、模式设置和电机使能操作，静态功耗会从约0.4W升至1.2W
    2.命令更新：使用robstride_command_update_all() 函数进行所有电机的命令更新，建议在10ms定时中断中调用
    3.参数更新：在main.c中使用 motor->xxx_tar_f，对于不同的运动模式修改ang、spd、tor、kp、kd、acc，具体参考robstride_command_update()函数中的调用
****************************************************************/

#include "bsp_fdcan.h"
#include <stdint.h>
/****************************************************************     
  bsp里会用到的妙妙宏定义
****************************************************************/
#define Set_mode 		    0			//官方驱动中为'j'的ASCII码
#define Set_parameter   1     //官方驱动中为'p'的ASCII码
#define move_control_mode   0	//运控模式
#define pos_control_mode    1	//PP位置模式
#define speed_control_mode  2 //速度模式
#define elect_control_mode  3 //电流模式
#define set_Zero_mode       4 //零点模式
#define csp_control_mode	  5	//CSP位置模式
#define rs00            0
#define rs01            1
#define rs02            2
#define rs03            3
#define rs04            4
#define rs05            5
#define rs06            6
/****************************************************************     
  参考数据手册得到的各型号电机参数限制
****************************************************************/  
#define rs00_max_angle        12.57f
#define rs00_min_angle       -12.57f
#define rs00_max_speed        33.0f
#define rs00_min_speed       -33.0f
#define rs00_max_torque       14.0f
#define rs00_min_torque      -14.0f
#define rs00_max_kp          500.0f
#define rs00_max_kd          5.0f

#define rs01_max_angle        12.57f
#define rs01_min_angle       -12.57f
#define rs01_max_speed        44.0f
#define rs01_min_speed       -44.0f
#define rs01_max_torque       17.0f
#define rs01_min_torque      -17.0f
#define rs01_max_kp          500.0f
#define rs01_max_kd          5.0f

#define rs02_max_angle        12.57f
#define rs02_min_angle       -12.57f
#define rs02_max_speed        44.0f
#define rs02_min_speed       -44.0f
#define rs02_max_torque       17.0f
#define rs02_min_torque      -17.0f
#define rs02_max_kp          500.0f
#define rs02_max_kd          5.0f

#define rs03_max_angle        12.57f
#define rs03_min_angle       -12.57f
#define rs03_max_speed        20.0f
#define rs03_min_speed       -20.0f
#define rs03_max_torque       60.0f
#define rs03_min_torque      -60.0f
#define rs03_max_kp          5000.0f
#define rs03_max_kd          100.0f

#define rs04_max_angle        12.57f
#define rs04_min_angle       -12.57f
#define rs04_max_speed        15.0f
#define rs04_min_speed       -15.0f
#define rs04_max_torque       120.0f
#define rs04_min_torque      -120.0f
#define rs04_max_kp          5000.0f
#define rs04_max_kd          100.0f

#define rs05_max_angle        12.57f
#define rs05_min_angle       -12.57f
#define rs05_max_speed        50.0f
#define rs05_min_speed       -50.0f
#define rs05_max_torque       5.5f
#define rs05_min_torque      -5.5f
#define rs05_max_kp          500.0f
#define rs05_max_kd          5.0f

#define rs06_max_angle        12.57f
#define rs06_min_angle       -12.57f
#define rs06_max_speed        50.0f
#define rs06_min_speed       -50.0f
#define rs06_max_torque       36.0f
#define rs06_min_torque      -36.0f
#define rs06_max_kp          5000.0f
#define rs06_max_kd          100.0f

/****************************************************************     
  robstride_set_parameter() 中使用的电机参数地址定义
****************************************************************/  
#define run_mode        0x7000
#define iq_ref          0x7006
#define spd_ref         0x700A
#define limit_torque    0x700B
#define cur_kp          0x7010
#define cur_ki          0x7011
#define cur_filt_gain   0x7012
#define loc_ref         0x7016
#define limit_spd       0x7017
#define limit_cur       0x7018
#define mechPos         0x7019
#define iqf             0x701A
#define mechVel         0x701B
#define VBUS            0x701C
#define loc_kp          0x701E
#define speed_kp       	0x701F
#define speed_ki        0x7020
#define spd_filt_gain   0x7021
#define acc_rad         0x7022
#define vel_max         0x7024
#define acc_set         0x7025
#define EPScan_time     0x7026
#define canTimeout      0x7028

typedef enum
{
  CAN_ID_ROBSTRIDE_M  = 0x4F,//仅在初始化时使用，实际电机从1开始编号

	CAN_ID_ROBSTRIDE_M0  = 0x50,
	CAN_ID_ROBSTRIDE_M1  = 0x51,
	CAN_ID_ROBSTRIDE_M2  = 0x52,
	CAN_ID_ROBSTRIDE_M3  = 0x53,
	CAN_ID_ROBSTRIDE_M4  = 0x54,
	CAN_ID_ROBSTRIDE_M5  = 0x55,
	CAN_ID_ROBSTRIDE_M6  = 0x56,
	CAN_ID_ROBSTRIDE_M7  = 0x57,
  CAN_ID_ROBSTRIDE_M8  = 0x58,
  CAN_ID_ROBSTRIDE_M9  = 0x59,
  CAN_ID_ROBSTRIDE_M10 = 0x5A,
  CAN_ID_ROBSTRIDE_M11 = 0x5B,
  CAN_ID_ROBSTRIDE_M12 = 0x5C,
  CAN_ID_ROBSTRIDE_M13 = 0x5D,
  CAN_ID_ROBSTRIDE_M14 = 0x5E,
  CAN_ID_ROBSTRIDE_M15 = 0x5F,
	
} can_id_robstride_e;

typedef enum
{
  MASTER_ID_ROBSTRIDE_M  = 0x5F,//仅在初始化时使用，实际电机从1开始编号

	MASTER_ID_ROBSTRIDE_M0  = 0x60,
	MASTER_ID_ROBSTRIDE_M1  = 0x61,
	MASTER_ID_ROBSTRIDE_M2  = 0x62,
	MASTER_ID_ROBSTRIDE_M3  = 0x63,
	MASTER_ID_ROBSTRIDE_M4  = 0x64,
	MASTER_ID_ROBSTRIDE_M5  = 0x65,
	MASTER_ID_ROBSTRIDE_M6  = 0x66,
	MASTER_ID_ROBSTRIDE_M7  = 0x67,
  MASTER_ID_ROBSTRIDE_M8  = 0x68,
  MASTER_ID_ROBSTRIDE_M9 = 0x69,
  MASTER_ID_ROBSTRIDE_M10 = 0x6A,
  MASTER_ID_ROBSTRIDE_M11 = 0x6B,
  MASTER_ID_ROBSTRIDE_M12 = 0x6C,
  MASTER_ID_ROBSTRIDE_M13 = 0x6D,
  MASTER_ID_ROBSTRIDE_M14 = 0x6E,
  MASTER_ID_ROBSTRIDE_M15 = 0x6F,
} master_id_robstride_e;

typedef struct {
  /******************************     
  电机基本信息
  *******************************/  
  uint8_t enabled;      //电机使能状态，1为使能，0为失能
  uint8_t id;           //电机ID
  uint8_t master_id;    //电机主ID
  uint8_t mode;         //电机运行模式
  uint8_t series;       //电机系列号
  uint8_t error;
    //  bit[7:0]    故障信息-0为正常
    //  byte6       通信类型21-故障反馈帧
    //  byte5       未标定
    //  byte4       堵转过载故障
    //  byte3       此编码故障
    //  byte2       过温
    //  byte1       驱动故障
    //  byte0       欠压故障
  float ang_max;
  float spd_max;
  float tor_max;
  float kp_max;
  float kd_max;

  /******************************     
  电机回传值
  *******************************/
  float angle;
  float speed;
  float torque;
  float temperature;

  /******************************     
  电机控制参数，仅修改tar_f值，其余会在command_update中更新
  *******************************/
  float ang_tar_f; 
  float spd_tar_f; 
  float tor_tar_f;
  float acc_tar_f; 
  float kp_tar_f;
  float kd_tar_f; 
  uint16_t ang_tar,last_ang_tar;
  uint16_t spd_tar, last_spd_tar;
  uint16_t tor_tar, last_tor_tar;
  uint16_t kp_tar, last_kp_tar;
  uint16_t kd_tar, last_kd_tar;
  uint16_t acc_tar, last_acc_tar;

  uint8_t info_index;//用于参数读取时的反馈信息
  uint8_t info_value;//用于参数读取时的反馈信息

  uint8_t getResponse;//初始化后是否收到反馈，未收到置1，收到置0

} motor_robstride_t;

void robstride_enable(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride);
void robstride_disable(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride);
void robstride_get_motor_parameter(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride, uint16_t parameter);
void robstride_set_motor_parameter(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride,uint16_t Index, float Value, char Value_mode);
void robstride_move_control_send(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor,uint16_t ang_tar,uint16_t spd_tar,
                           uint16_t kp_tar,uint16_t kd_tar,uint16_t tor_tar);
void robstride_move_control(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor);
void robstride_pos_set_position(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride, float Value);
void robstride_pos_set_velocity(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride, float Value);
void robstride_pos_set_acceleration(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride, float Value);
void robstride_csp_set_position(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride, float Value);
void robstride_csp_set_velocity(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride, float Value);
void robstride_parameter_update(motor_robstride_t *motor_robstride);    
void robstride_command_update(FDCAN_HandleTypeDef *hfdcan, motor_robstride_t *motor_robstride);
void robstride_command_update_all(void);  
void robstride_info_update(motor_robstride_t *motor, uint32_t id, uint8_t *data);
void robstride_init(uint8_t canport_1_to_3, uint8_t motorID_1_to_16, uint8_t series, uint8_t mode);
#endif
