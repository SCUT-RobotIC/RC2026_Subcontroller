#ifndef __BSP_DM_CTRL_H__
#define __BSP_DM_CTRL_H__

#include <stdint.h>
#include "fdcan.h"

// by RobotIC laboratory, South China University of Technology

#define MIT_MODE  0x000
#define POS_MODE  0x100
#define SPD_MODE  0x200
#define PSI_MODE  0x300

#define KP_MIN    0.0f
#define KP_MAX    500.0f
#define KD_MIN    0.0f
#define KD_MAX    5.0f

typedef enum
{
  CAN_ID_DM_M0  = 0x30,
  CAN_ID_DM_M1  = 0x31,
  CAN_ID_DM_M2  = 0x32,
  CAN_ID_DM_M3  = 0x33,
  CAN_ID_DM_M4  = 0x34,
  CAN_ID_DM_M5  = 0x35,
  CAN_ID_DM_M6  = 0x36,
  CAN_ID_DM_M7  = 0x37,

  MASTER_ID_DM_M0  = 0x40,
  MASTER_ID_DM_M1  = 0x41,
  MASTER_ID_DM_M2  = 0x42,
  MASTER_ID_DM_M3  = 0x43,
  MASTER_ID_DM_M4  = 0x44,
  MASTER_ID_DM_M5  = 0x45,
  MASTER_ID_DM_M6  = 0x46,
  MASTER_ID_DM_M7  = 0x47,
} can_id_dm_e;

typedef enum
{
	mit_mode = 1,
	pos_mode = 2,
	spd_mode = 3,
	psi_mode = 4
} mode_dm_e;

// 电机回传信息结构体
typedef struct
{
	int   id;
	int   state;
	int   p_int;
	int   v_int;
	int   t_int;
	int   kp_int;
	int   kd_int;
	float pos;
	float vel;
	float tor;
	float Kp;
	float Kd;
	float Tmos;
	float Tcoil;
} motor_fbpara_t;

// 电机参数设置结构体
typedef struct
{
	uint8_t mode;
	float   pos_set;
	float   vel_set;
	float   tor_set;
	float   cur_set;
	float   kp_set;
	float   kd_set;
} motor_ctrl_t;

// 电机参数
typedef struct
{
	uint8_t  read_flag;
	uint8_t  write_flag;
	uint8_t  save_flag;

	float    UV_Value;    // 低压保护值
	float    KT_Value;    // 扭矩系数
	float    OT_Value;    // 过温保护值
	float    OC_Value;    // 过流保护值
	float    ACC;         // 加速度
	float    DEC;         // 减速度
	float    MAX_SPD;     // 最大速度
	uint32_t MST_ID;      // 反馈ID
	uint32_t ESC_ID;      // 接收ID
	uint32_t TIMEOUT;     // 超时警报时间
	uint32_t cmode;       // 控制模式
	float    Damp;        // 电机粘滞系数
	float    Inertia;     // 电机转动惯量
	uint32_t hw_ver;      // 保留
	uint32_t sw_ver;      // 软件版本号
	uint32_t SN;          // 保留
	uint32_t NPP;         // 电机极对数
	float    Rs;          // 电阻
	float    Ls;          // 电感
	float    Flux;        // 磁链
	float    Gr;          // 齿轮减速比
	float    PMAX;        // 位置映射范围
	float    VMAX;        // 速度映射范围
	float    TMAX;        // 扭矩映射范围
	float    I_BW;        // 电流环控制带宽
	float    KP_ASR;      // 速度环Kp
	float    KI_ASR;      // 速度环Ki
	float    KP_APR;      // 位置环Kp
	float    KI_APR;      // 位置环Ki
	float    OV_Value;    // 过压保护值
	float    GREF;        // 齿轮力矩效率
	float    Deta;        // 速度环阻尼系数
	float    V_BW;        // 速度环滤波带宽
	float    IQ_cl;       // 电流环增强系数
	float    VL_cl;       // 速度环增强系数
	uint32_t can_br;      // CAN波特率代码
	uint32_t sub_ver;     // 子版本号
	float    u_off;       // u相偏置
	float    v_off;       // v相偏置
	float    k1;          // 补偿因子1
	float    k2;          // 补偿因子2
	float    m_off;       // 角度偏移
	float    dir;         // 方向
	float    p_m;         // 电机位置
	float    x_out;       // 输出轴位置
} esc_inf_t;

typedef struct
{
	uint16_t id;
	uint16_t mst_id;
	motor_fbpara_t para;
	motor_ctrl_t ctrl;
	esc_inf_t tmp;
} motor_dm_t;

typedef union
{
	float f_val;
	uint32_t u_val;
	uint8_t b_val[4];
} float_type_u;

void dm_motor_init(motor_dm_t *motor, uint8_t can_id, uint8_t mode, float kp, float kd, float pmax, float vmax, float tmax);
void dm_motor_setmode(FDCAN_HandleTypeDef *hfdcan, motor_dm_t *motor, uint8_t mode);
void dm_motor_enable(FDCAN_HandleTypeDef *hfdcan, motor_dm_t *motor);
void dm_motor_disable(FDCAN_HandleTypeDef *hfdcan, motor_dm_t *motor);
void dm_motor_ctrl_send(FDCAN_HandleTypeDef *hfdcan, motor_dm_t *motor);
void fdcan_send_dm(FDCAN_HandleTypeDef *hfdcan, uint16_t id, uint8_t *data, uint32_t len);
void motor_update_dm(motor_dm_t *motor, uint8_t *data);
void motor_allmsg_update_dm(motor_dm_t *motor, uint8_t *data);

#endif
