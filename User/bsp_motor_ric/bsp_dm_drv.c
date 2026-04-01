#include "bsp_dm_drv.h"
#include "bsp_dm_ctrl.h"
#include <stdint.h>

/**
************************************************************************
* @brief:      	enable_motor_mode: Function to enable motor mode
* @param[in]:   hcan:     Pointer to the CAN_HandleTypeDef structure
* @param[in]:   motor_id: Motor ID specifying the target motor
* @param[in]:   mode_id:  Mode ID specifying the mode to enable
* @retval:     	void
* @details:     Sends a command via the CAN bus to enable a specific mode on the targeted motor
************************************************************************
**/
void enable_motor_mode(hcan_t* hcan, uint16_t motor_id, uint16_t mode_id)
{
	uint8_t data[8];
	uint16_t id = motor_id + mode_id;
	
	data[0] = 0xFF;
	data[1] = 0xFF;
	data[2] = 0xFF;
	data[3] = 0xFF;
	data[4] = 0xFF;
	data[5] = 0xFF;
	data[6] = 0xFF;
	data[7] = 0xFC;
	
	fdcan_send_dm(hcan, id, data, 8);
}

/**
************************************************************************
* @brief:      	disable_motor_mode: Function to disable motor mode
* @param[in]:   hcan:     Pointer to the CAN_HandleTypeDef structure
* @param[in]:   motor_id: Motor ID specifying the target motor
* @param[in]:   mode_id:  Mode ID specifying the mode to disable
* @retval:     	void
* @details:     Sends a command via the CAN bus to disable a specific mode on the targeted motor
************************************************************************
**/
void disable_motor_mode(hcan_t* hcan, uint16_t motor_id, uint16_t mode_id)
{
	uint8_t data[8];
	uint16_t id = motor_id + mode_id;
	
	data[0] = 0xFF;
	data[1] = 0xFF;
	data[2] = 0xFF;
	data[3] = 0xFF;
	data[4] = 0xFF;
	data[5] = 0xFF;
	data[6] = 0xFF;
	data[7] = 0xFD;
	
	fdcan_send_dm(hcan, id, data, 8);
}

/**
************************************************************************
* @brief:      	dm_motor_clear_para
* @param[in]:   hcan:    fdcan struct
* @param[in]:   motor:   motor typedef struct
* @details:    	clear paraments
************************************************************************
**/
void dm_motor_clear_para(motor_dm_t *motor)
{
	motor->ctrl.kd_set 	= 0;
	motor->ctrl.kp_set	= 0;
	motor->ctrl.pos_set = 0;
	motor->ctrl.vel_set = 0;
	motor->ctrl.tor_set = 0;
	motor->ctrl.cur_set = 0;
}

/**
************************************************************************
* @brief:      	mit_ctrl: MIT mode motor control function
* @param[in]:   hcan:      Pointer to the CAN_HandleTypeDef structure
* @param[in]:   motor_id:  Motor ID specifying the target motor
* @param[in]:   pos:       Position setpoint
* @param[in]:   vel:       Velocity setpoint
* @param[in]:   kp:        Position proportional gain
* @param[in]:   kd:        Position derivative gain
* @param[in]:   tor:       Torque setpoint
* @retval:     	void
* @details:     Sends a control frame in MIT mode to the motor via the CAN bus
************************************************************************
**/
void mit_ctrl(hcan_t* hcan, motor_dm_t *motor, uint16_t motor_id, float pos, float vel,float kp, float kd, float tor)
{
	uint8_t data[8];
	uint16_t pos_tmp,vel_tmp,kp_tmp,kd_tmp,tor_tmp;
	uint16_t id = motor_id + MIT_MODE;

	pos_tmp = float_to_uint(pos, -motor->tmp.PMAX, motor->tmp.PMAX, 16);
	vel_tmp = float_to_uint(vel, -motor->tmp.VMAX, motor->tmp.VMAX, 12);
	tor_tmp = float_to_uint(tor, -motor->tmp.TMAX, motor->tmp.TMAX, 12);
	kp_tmp  = float_to_uint(kp,  KP_MIN, KP_MAX, 12);
	kd_tmp  = float_to_uint(kd,  KD_MIN, KD_MAX, 12);

	data[0] = (pos_tmp >> 8);
	data[1] = pos_tmp;
	data[2] = (vel_tmp >> 4);
	data[3] = ((vel_tmp&0xF)<<4)|(kp_tmp>>8);
	data[4] = kp_tmp;
	data[5] = (kd_tmp >> 4);
	data[6] = ((kd_tmp&0xF)<<4)|(tor_tmp>>8);
	data[7] = tor_tmp;
	
	fdcan_send_dm(hcan, id, data, 8);
}

/**
************************************************************************
* @brief:      	pos_ctrl: Position and speed control function
* @param[in]:   hcan:      Pointer to the CAN_HandleTypeDef structure
* @param[in]:   motor_id:  Motor ID specifying the target motor
* @param[in]:   pos:       Position setpoint
* @param[in]:   vel:       Velocity setpoint
* @retval:     	void
* @details:     Sends a position and speed control command to the motor via the CAN bus
************************************************************************
**/
void pos_ctrl(hcan_t* hcan,uint16_t motor_id, float pos, float vel)
{
	uint16_t id;
	uint8_t *pbuf, *vbuf;
	uint8_t data[8];
	
	id = motor_id + POS_MODE;
	pbuf=(uint8_t*)&pos;
	vbuf=(uint8_t*)&vel;
	
	data[0] = *pbuf;
	data[1] = *(pbuf+1);
	data[2] = *(pbuf+2);
	data[3] = *(pbuf+3);

	data[4] = *vbuf;
	data[5] = *(vbuf+1);
	data[6] = *(vbuf+2);
	data[7] = *(vbuf+3);
	
	fdcan_send_dm(hcan, id, data, 8);
}

/**
************************************************************************
* @brief:      	spd_ctrl: Speed control function
* @param[in]:   hcan:      Pointer to the CAN_HandleTypeDef structure
* @param[in]:   motor_id:  Motor ID specifying the target motor
* @param[in]:   vel:       Velocity setpoint
* @retval:     	void
* @details:     Sends a speed control command to the motor via the CAN bus
************************************************************************
**/
void spd_ctrl(hcan_t* hcan, uint16_t motor_id, float vel)
{
	uint16_t id;
	uint8_t *vbuf;
	uint8_t data[4];
	
	id = motor_id + SPD_MODE;
	vbuf=(uint8_t*)&vel;
	
	data[0] = *vbuf;
	data[1] = *(vbuf+1);
	data[2] = *(vbuf+2);
	data[3] = *(vbuf+3);
	
	fdcan_send_dm(hcan, id, data, 4);
}

/**
************************************************************************
* @brief:      	psi_ctrl: Mixed control mode
* @param[in]:   hcan:      Pointer to the CAN_HandleTypeDef structure
* @param[in]:   motor_id:  Motor ID specifying the target motor
* @param[in]:   pos:       Position setpoint
* @param[in]:   vel:       Velocity setpoint
* @param[in]:   cur:       Current setpoint
* @retval:     	void
* @details:     Sends a mixed control command to the motor via the CAN bus
************************************************************************
**/
void psi_ctrl(hcan_t* hcan, uint16_t motor_id, float pos, float vel,float cur)
{
	uint16_t id;
	uint8_t *pbuf, *vbuf, *cbuf;
	uint8_t data[12];
	
	id = motor_id + PSI_MODE;
	pbuf=(uint8_t*)&pos;
	vbuf=(uint8_t*)&vel;
	cbuf=(uint8_t*)&cur;
	
	data[0] = *pbuf;
	data[1] = *(pbuf+1);
	data[2] = *(pbuf+2);
	data[3] = *(pbuf+3);

	data[4] = *vbuf;
	data[5] = *(vbuf+1);
	data[6] = *(vbuf+2);
	data[7] = *(vbuf+3);

	data[8] = *cbuf;
	data[9] = *(cbuf+1);
	data[10] = *(cbuf+2);
	data[11] = *(cbuf+3);

	fdcan_send_dm(hcan, id, data, 12);
}

/**
************************************************************************
* @brief:      	dm_motor_clear_err
* @param[in]:   hcan:    fdcan struct
* @param[in]:   motor:   motor typedef struct
* @retval:     	void
* @details:    	clear motor error
************************************************************************
**/
void dm_motor_clear_err(hcan_t* hcan, motor_dm_t *motor)
{
	switch(motor->ctrl.mode)
	{
		case mit_mode:
			clear_err(hcan, motor->id, MIT_MODE);
			break;
		case pos_mode:
			clear_err(hcan, motor->id, POS_MODE);
			break;
		case spd_mode:
			clear_err(hcan, motor->id, SPD_MODE);
			break;
		case psi_mode:
			clear_err(hcan, motor->id, PSI_MODE);
			break;
	}	
}

/**
************************************************************************
* @brief:      	clear_err: Function to clear motor error
* @param[in]:   hcan:     Pointer to the CAN_HandleTypeDef structure
* @param[in]:   motor_id: Motor ID specifying the target motor
* @param[in]:   mode_id:  Mode ID specifying the mode to clear errors
* @retval:     	void
* @details:     Sends a command via the CAN bus to clear errors on a specific motor
************************************************************************
**/
void clear_err(hcan_t* hcan, uint16_t motor_id, uint16_t mode_id)
{
	uint8_t data[8];
	uint16_t id = motor_id + mode_id;
	
	data[0] = 0xFF;
	data[1] = 0xFF;
	data[2] = 0xFF;
	data[3] = 0xFF;
	data[4] = 0xFF;
	data[5] = 0xFF;
	data[6] = 0xFF;
	data[7] = 0xFB;
	
	fdcan_send_dm(hcan, id, data, 8);
}

/**
************************************************************************
* @brief:      	float_to_uint: Function to convert a float to an unsigned integer
* @param[in]:   x_float:	Float value to be converted
* @param[in]:   x_min:		Minimum range value
* @param[in]:   x_max:		Maximum range value
* @param[in]:   bits: 		Bit width of the target unsigned integer
* @retval:     	Unsigned integer result
* @details:    	Maps the given float x linearly within the specified range [x_min, x_max] to an unsigned integer of the specified bit width.
************************************************************************
**/
int float_to_uint(float x_float, float x_min, float x_max, int bits)
{
	/* Converts a float to an unsigned int, given range and number of bits */
	float span = x_max - x_min;
	float offset = x_min;
	return (int) ((x_float-offset)*((float)((1<<bits)-1))/span);
}
/**
************************************************************************
* @brief:      	uint_to_float: Function to convert an unsigned integer to a float
* @param[in]:   x_int: Unsigned integer to be converted
* @param[in]:   x_min: Minimum range value
* @param[in]:   x_max: Maximum range value
* @param[in]:   bits:  Bit width of the unsigned integer
* @retval:     	Float result
* @details:    	Maps the given unsigned integer x_int linearly within the specified range [x_min, x_max] to a float.
************************************************************************
**/
float uint_to_float(int x_int, float x_min, float x_max, int bits)
{
	/* converts unsigned int to float, given range and number of bits */
	float span = x_max - x_min;
	float offset = x_min;
	return ((float)x_int)*span/((float)((1<<bits)-1)) + offset;
}

/**
************************************************************************
* @brief:      	save_pos_zero: Function to save position zero point
* @param[in]:   hcan:     Pointer to the CAN_HandleTypeDef structure
* @param[in]:   motor_id: Motor ID specifying the target motor
* @param[in]:   mode_id:  Mode ID specifying the mode to save position zero point
* @retval:     	void
* @details:     Sends a command via the CAN bus to save the position zero point for a specific motor
************************************************************************
**/
void save_pos_zero(hcan_t* hcan, uint16_t motor_id, uint16_t mode_id)
{
	uint8_t data[8];
	uint16_t id = motor_id + mode_id;
	
	data[0] = 0xFF;
	data[1] = 0xFF;
	data[2] = 0xFF;
	data[3] = 0xFF;
	data[4] = 0xFF;
	data[5] = 0xFF;
	data[6] = 0xFF;
	data[7] = 0xFE;
	
	fdcan_send_dm(hcan, id, data, 8);
}

// 从此处开始，下面的函数基本都用不到了

/**
************************************************************************
* @brief:      	read_all_motor_data: 读取电机的所有寄存器的数据信息
* @param:      	motor_t：电机参数结构体
* @retval:     	void
* @details:    	逐次发送读取命令
************************************************************************
**/
void read_all_motor_data(hcan_t* hcan, motor_dm_t *motor)
{
  switch (motor->tmp.read_flag)
  {
		case 1:  read_motor_data(hcan, motor->id, RID_UV_VALUE);  break; // UV_Value
		case 2:  read_motor_data(hcan, motor->id, RID_KT_VALUE);  break; // KT_Value
		case 3:  read_motor_data(hcan, motor->id, RID_OT_VALUE);  break; // OT_Value
		case 4:  read_motor_data(hcan, motor->id, RID_OC_VALUE);  break; // OC_Value
		case 5:  read_motor_data(hcan, motor->id, RID_ACC);       break; // ACC
		case 6:  read_motor_data(hcan, motor->id, RID_DEC);       break; // DEC
		case 7:  read_motor_data(hcan, motor->id, RID_MAX_SPD);   break; // MAX_SPD
		case 8:  read_motor_data(hcan, motor->id, RID_MST_ID);    break; // MST_ID 
		case 9:  read_motor_data(hcan, motor->id, RID_ESC_ID);    break; // ESC_ID
		case 10: read_motor_data(hcan, motor->id, RID_TIMEOUT);   break; // TIMEOUT 
		case 11: read_motor_data(hcan, motor->id, RID_CMODE);     break; // CTRL_MODE 
		case 12: read_motor_data(hcan, motor->id, RID_DAMP);      break; // Damp 
		case 13: read_motor_data(hcan, motor->id, RID_INERTIA);   break; // Inertia
		case 14: read_motor_data(hcan, motor->id, RID_HW_VER);    break; // Rsv1 
		case 15: read_motor_data(hcan, motor->id, RID_SW_VER);    break; // sw_ver 
		case 16: read_motor_data(hcan, motor->id, RID_SN);        break; // Rsv2 
		case 17: read_motor_data(hcan, motor->id, RID_NPP);       break; // NPP 
		case 18: read_motor_data(hcan, motor->id, RID_RS);        break; // Rs 
		case 19: read_motor_data(hcan, motor->id, RID_LS);        break; // Ls 
		case 20: read_motor_data(hcan, motor->id, RID_FLUX);      break; // Flux 
		case 21: read_motor_data(hcan, motor->id, RID_GR);        break; // Gr 
		case 22: read_motor_data(hcan, motor->id, RID_PMAX);      break; // PMAX 
		case 23: read_motor_data(hcan, motor->id, RID_VMAX);      break; // VMAX 
		case 24: read_motor_data(hcan, motor->id, RID_TMAX);      break; // TMAX 
		case 25: read_motor_data(hcan, motor->id, RID_I_BW);      break; // I_BW 
		case 26: read_motor_data(hcan, motor->id, RID_KP_ASR);    break; // KP_ASR 
		case 27: read_motor_data(hcan, motor->id, RID_KI_ASR);    break; // KI_ASR 
		case 28: read_motor_data(hcan, motor->id, RID_KP_APR);    break; // KP_APR 
		case 29: read_motor_data(hcan, motor->id, RID_KI_APR);    break; // KI_APR 
		case 30: read_motor_data(hcan, motor->id, RID_OV_VALUE);  break; // OV_Value 
		case 31: read_motor_data(hcan, motor->id, RID_GREF);      break; // GREF 
		case 32: read_motor_data(hcan, motor->id, RID_DETA);      break; // Deta 
		case 33: read_motor_data(hcan, motor->id, RID_V_BW);      break; // V_BW 
		case 34: read_motor_data(hcan, motor->id, RID_IQ_CL);     break; // IQ_c1 
		case 35: read_motor_data(hcan, motor->id, RID_VL_CL);     break; // VL_c1 
		case 36: read_motor_data(hcan, motor->id, RID_CAN_BR);    break; // can_br 
		case 37: read_motor_data(hcan, motor->id, RID_SUB_VER);   break; // sub_ver 
		case 38: read_motor_data(hcan, motor->id, RID_U_OFF);     break; // u_off 
		case 39: read_motor_data(hcan, motor->id, RID_V_OFF);     break; // v_off 
		case 40: read_motor_data(hcan, motor->id, RID_K1);        break; // k1 
		case 41: read_motor_data(hcan, motor->id, RID_K2);        break; // k2 
		case 42: read_motor_data(hcan, motor->id, RID_M_OFF);     break; // m_off 
		case 43: read_motor_data(hcan, motor->id, RID_DIR);       break; // dir 
		case 44: read_motor_data(hcan, motor->id, RID_P_M);       break; // pm 
		case 45: read_motor_data(hcan, motor->id, RID_X_OUT);     break; // xout 
  }
}

/**
************************************************************************
* @brief:      	read_motor_data: Sends a command to read a register
* @param[in]:   id:    Motor CAN ID
* @param[in]:   rid:   Register address
* @retval:     	void
* @details:     Reads motor parameters by sending a CAN command to the motor
************************************************************************
**/
void read_motor_data(hcan_t* hcan, uint16_t id, uint8_t rid) 
{
	uint8_t can_id_l = id & 0x0F;
	uint8_t can_id_h = (id >> 4) & 0x0F;
	
	uint8_t data[4] = {can_id_l, can_id_h, 0x33, rid};
	fdcan_send_dm(hcan, 0x7FF, data, 4);
}

/**
************************************************************************
* @brief:      	read_motor_ctrl_fbdata(uint16_t id, uint8_t rid) 
* @param[in]:   id:    Motor CAN ID
* @retval:     	void
* @details:     Read the control data feedback from the motor
************************************************************************
**/
void read_motor_ctrl_fbdata(hcan_t* hcan, uint16_t id) 
{
	uint8_t can_id_l = id & 0x0F;
	uint8_t can_id_h = (id >> 4) & 0x0F;
	
	uint8_t data[4] = {can_id_l, can_id_h, 0xCC, 0x00};
	fdcan_send_dm(hcan, 0x7FF, data, 4);
}

/**
************************************************************************
* @brief:      	write_motor_data: Sends a command to write to a register
* @param[in]:   id:    Motor CAN ID
* @param[in]:   rid:   Register address
* @param[in]:   d0-d3: Data to write to the register
* @retval:     	void
* @details:     Writes data to a motor register by sending a CAN command
************************************************************************
**/
void write_motor_data(hcan_t* hcan, uint16_t id, uint8_t rid, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
	uint8_t can_id_l = id & 0x0F;
	uint8_t can_id_h = (id >> 4) & 0x0F;
	
	uint8_t data[8] = {can_id_l, can_id_h, 0x55, rid, d0, d1, d2, d3};
	fdcan_send_dm(hcan, 0x7FF, data, 8);
}

/**
************************************************************************
* @brief:      	save_motor_data: Sends a command to save motor data
* @param[in]:   id:    Motor CAN ID
* @param[in]:   rid:   Register address
* @retval:     	void
* @details:     Saves the written motor parameters by sending a CAN command
************************************************************************
**/
void save_motor_data(hcan_t* hcan, uint16_t id, uint8_t rid) 
{
	uint8_t can_id_l = id & 0x0F;
	uint8_t can_id_h = (id >> 4) & 0x0F;
	
	uint8_t data[4] = {can_id_l, can_id_h, 0xAA, 0x01};
	fdcan_send_dm(hcan, 0x7FF, data, 4);
}
