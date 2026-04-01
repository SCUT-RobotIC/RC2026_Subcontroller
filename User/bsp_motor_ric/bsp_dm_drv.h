#ifndef __BSP_DM_DRV_H__
#define __BSP_DM_DRV_H__

#include <stdint.h>
#include "bsp_dm_ctrl.h"
#include "fdcan.h"

// by RobotIC laboratory, South China University of Technology

#define hcan_t FDCAN_HandleTypeDef

typedef enum {
  RID_UV_VALUE=0,    // 低压保护值
  RID_KT_VALUE=1,    // 扭矩系数
  RID_OT_VALUE=2,    // 过温保护值
  RID_OC_VALUE=3,    // 过流保护值
  RID_ACC		=4,    // 加速度
  RID_DEC		=5,    // 减速度
  RID_MAX_SPD	=6,    // 最大速度
  RID_MST_ID	=7,    // 反馈ID
  RID_ESC_ID	=8,    // 接收ID
  RID_TIMEOUT	=9,    // 超时警报时间
  RID_CMODE	=10,   // 控制模式
  RID_DAMP	=11,   // 电机粘滞系数
  RID_INERTIA =12,   // 电机转动惯量
  RID_HW_VER	=13,   // 保留
  RID_SW_VER	=14,   // 软件版本号
  RID_SN		=15,   // 保留
  RID_NPP		=16,   // 电机极对数
  RID_RS		=17,   // 电阻
  RID_LS		=18,   // 电感
  RID_FLUX	=19,   // 磁链
  RID_GR		=20,   // 齿轮减速比
  RID_PMAX	=21,   // 位置映射范围
  RID_VMAX	=22,   // 速度映射范围
  RID_TMAX	=23,   // 扭矩映射范围
  RID_I_BW	=24,   // 电流环控制带宽
  RID_KP_ASR	=25,   // 速度环Kp
  RID_KI_ASR	=26,   // 速度环Ki
  RID_KP_APR	=27,   // 位置环Kp
  RID_KI_APR	=28,   // 位置环Ki
  RID_OV_VALUE=29,   // 过压保护值
  RID_GREF	=30,   // 齿轮力矩效率
  RID_DETA	=31,   // 速度环阻尼系数
  RID_V_BW	=32,   // 速度环滤波带宽
  RID_IQ_CL	=33,   // 电流环增强系数
  RID_VL_CL	=34,   // 速度环增强系数
  RID_CAN_BR	=35,   // CAN波特率代码
  RID_SUB_VER	=36,   // 子版本号
  RID_U_OFF	=50,   // u相偏置
  RID_V_OFF	=51,   // v相偏置
  RID_K1		=52,   // 补偿因子1
  RID_K2		=53,   // 补偿因子2
  RID_M_OFF	=54,   // 角度偏移
  RID_DIR		=55,   // 方向
  RID_P_M		=80,   // 电机位置
  RID_X_OUT	=81    // 输出轴位置
} rid_e;

void enable_motor_mode(hcan_t* hcan, uint16_t motor_id, uint16_t mode_id);
void disable_motor_mode(hcan_t* hcan, uint16_t motor_id, uint16_t mode_id);
void dm_motor_clear_para(motor_dm_t *motor);
void mit_ctrl(hcan_t* hcan, motor_dm_t *motor, uint16_t motor_id, float pos, float vel, float kp, float kd, float tor);
void pos_ctrl(hcan_t* hcan, uint16_t motor_id, float pos, float vel);
void spd_ctrl(hcan_t* hcan, uint16_t motor_id, float vel);
void psi_ctrl(hcan_t* hcan, uint16_t motor_id, float pos, float vel, float cur);
void dm_motor_clear_err(hcan_t* hcan, motor_dm_t *motor);
void clear_err(hcan_t* hcan, uint16_t motor_id, uint16_t mode_id);
int float_to_uint(float x_float, float x_min, float x_max, int bits);
float uint_to_float(int x_int, float x_min, float x_max, int bits);
void save_pos_zero(hcan_t* hcan, uint16_t motor_id, uint16_t mode_id);

void read_all_motor_data(hcan_t* hcan, motor_dm_t *motor);
void read_motor_data(hcan_t* hcan, uint16_t id, uint8_t rid);
void read_motor_ctrl_fbdata(hcan_t* hcan, uint16_t id);
void write_motor_data(hcan_t* hcan, uint16_t id, uint8_t rid, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
void save_motor_data(hcan_t* hcan, uint16_t id, uint8_t rid);

#endif
