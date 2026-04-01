#include "bsp_dm_ctrl.h"
#include "bsp_dm_drv.h"
#include <stdint.h>

FDCAN_TxHeaderTypeDef fdcan_tx_header_dm;

motor_dm_t motor_dm[2][8] = {0};

void dm_motor_init(motor_dm_t *motor, uint8_t can_id, uint8_t mode, float kp, float kd, float pmax, float vmax, float tmax)
{
  if(can_id < CAN_ID_DM_M0 || can_id > CAN_ID_DM_M7) return;
  motor->id = can_id;
  motor->mst_id = MASTER_ID_DM_M1 + (can_id - CAN_ID_DM_M0);
  motor->tmp.read_flag = 1;
  motor->ctrl.mode = mode;
  motor->ctrl.vel_set = 0.0f;
  motor->ctrl.pos_set = 0.0f;
  motor->ctrl.cur_set = 0.0f;
  motor->ctrl.kp_set = kp;
  motor->ctrl.kd_set = kd;
  motor->tmp.PMAX = pmax;
  motor->tmp.VMAX = vmax;
  motor->tmp.TMAX = tmax;
}

void dm_motor_setmode(FDCAN_HandleTypeDef *hfdcan, motor_dm_t *motor, uint8_t mode)
{
  if(mode < mit_mode || mode > psi_mode) return;
  dm_motor_disable(hfdcan, motor);
  dm_motor_clear_para(motor);
  write_motor_data(hfdcan, motor->id, 0x0A, mode, 0, 0, 0);
  motor->ctrl.mode = mode;
}

void dm_motor_enable(FDCAN_HandleTypeDef *hfdcan, motor_dm_t *motor)
{
	switch(motor->ctrl.mode)
	{
		case mit_mode:
			enable_motor_mode(hfdcan, motor->id, MIT_MODE);
			break;
		case pos_mode:
			enable_motor_mode(hfdcan, motor->id, POS_MODE);
			break;
		case spd_mode:
			enable_motor_mode(hfdcan, motor->id, SPD_MODE);
			break;
		case psi_mode:
			enable_motor_mode(hfdcan, motor->id, PSI_MODE);
			break;
	}	
}

void dm_motor_disable(FDCAN_HandleTypeDef *hfdcan, motor_dm_t *motor)
{
	switch(motor->ctrl.mode)
	{
		case mit_mode:
			disable_motor_mode(hfdcan, motor->id, MIT_MODE);
			break;
		case pos_mode:
			disable_motor_mode(hfdcan, motor->id, POS_MODE);
			break;
		case spd_mode:
			disable_motor_mode(hfdcan, motor->id, SPD_MODE);
			break;
		case psi_mode:
			disable_motor_mode(hfdcan, motor->id, PSI_MODE);
			break;
	}	
	dm_motor_clear_para(motor);
}

void dm_motor_ctrl_send(FDCAN_HandleTypeDef *hfdcan, motor_dm_t *motor)
{
	switch(motor->ctrl.mode)
	{
		case mit_mode:
			mit_ctrl(hfdcan, motor, motor->id, motor->ctrl.pos_set, motor->ctrl.vel_set, motor->ctrl.kp_set, motor->ctrl.kd_set, motor->ctrl.tor_set);
			break;
		case pos_mode:
			pos_ctrl(hfdcan, motor->id, motor->ctrl.pos_set, motor->ctrl.vel_set);
			break;
		case spd_mode:
			spd_ctrl(hfdcan, motor->id, motor->ctrl.vel_set);
			break;
		case psi_mode:
			psi_ctrl(hfdcan, motor->id,motor->ctrl.pos_set, motor->ctrl.vel_set, motor->ctrl.cur_set);
			break;
	}	
}

void fdcan_send_dm(FDCAN_HandleTypeDef *hfdcan, uint16_t id, uint8_t *data, uint32_t len)
{
  fdcan_tx_header_dm.Identifier = id;
  fdcan_tx_header_dm.IdType = FDCAN_STANDARD_ID;
  fdcan_tx_header_dm.TxFrameType = FDCAN_DATA_FRAME;
	
	if(len<=8)
		fdcan_tx_header_dm.DataLength = FDCAN_DLC_BYTES_8;
	else if(len==12)
		fdcan_tx_header_dm.DataLength = FDCAN_DLC_BYTES_12;
	else if(len==16)
		fdcan_tx_header_dm.DataLength = FDCAN_DLC_BYTES_16;
	else if(len==20)
		fdcan_tx_header_dm.DataLength = FDCAN_DLC_BYTES_20;
	else if(len==24)
		fdcan_tx_header_dm.DataLength = FDCAN_DLC_BYTES_24;
	else if(len==32)
		fdcan_tx_header_dm.DataLength = FDCAN_DLC_BYTES_32;
	else if(len==48)
		fdcan_tx_header_dm.DataLength = FDCAN_DLC_BYTES_48;
	else if(len==64)
		fdcan_tx_header_dm.DataLength = FDCAN_DLC_BYTES_64;
	
  fdcan_tx_header_dm.ErrorStateIndicator=FDCAN_ESI_ACTIVE;
  fdcan_tx_header_dm.BitRateSwitch=FDCAN_BRS_ON;
  fdcan_tx_header_dm.FDFormat=FDCAN_FD_CAN;
  fdcan_tx_header_dm.TxEventFifoControl=FDCAN_NO_TX_EVENTS;
  fdcan_tx_header_dm.MessageMarker=0;

  HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &fdcan_tx_header_dm, data);
}

void motor_update_dm(motor_dm_t *motor, uint8_t *data)
{
  motor->para.id     = (data[0]) & 0x0F;
  motor->para.state  = (data[0]) >> 4;
  motor->para.p_int  = (data[1] << 8) | data[2];
  motor->para.v_int  = (data[3] << 4) | (data[4] >> 4);
  motor->para.t_int  = ((data[4] & 0xF) << 8) | data[5];
  motor->para.pos    = uint_to_float(motor->para.p_int, -motor->tmp.PMAX, motor->tmp.PMAX, 16); // (-12.5,12.5)
  motor->para.vel    = uint_to_float(motor->para.v_int, -motor->tmp.VMAX, motor->tmp.VMAX, 12); // (-45.0,45.0)
  motor->para.tor    = uint_to_float(motor->para.t_int, -motor->tmp.TMAX, motor->tmp.TMAX, 12); // (-18.0,18.0)
  motor->para.Tmos   = (float)(data[6]);
  motor->para.Tcoil  = (float)(data[7]);
}

void motor_allmsg_update_dm(motor_dm_t *motor, uint8_t *data)
{
  if (motor->tmp.read_flag == 0)
    return;

  float_type_u y;

  if (data[2] == 0x33)
  {
    y.b_val[0] = data[4];
    y.b_val[1] = data[5];
    y.b_val[2] = data[6];
    y.b_val[3] = data[7];

    switch (data[3])
    {
      case  0: motor->tmp.UV_Value   = y.f_val; motor->tmp.read_flag =  2; break;
      case  1: motor->tmp.KT_Value   = y.f_val; motor->tmp.read_flag =  3; break;
      case  2: motor->tmp.OT_Value   = y.f_val; motor->tmp.read_flag =  4; break;
      case  3: motor->tmp.OC_Value   = y.f_val; motor->tmp.read_flag =  5; break;
      case  4: motor->tmp.ACC        = y.f_val; motor->tmp.read_flag =  6; break;
      case  5: motor->tmp.DEC        = y.f_val; motor->tmp.read_flag =  7; break;
      case  6: motor->tmp.MAX_SPD    = y.f_val; motor->tmp.read_flag =  8; break;
      case  7: motor->tmp.MST_ID     = y.u_val; motor->tmp.read_flag =  9; break;
      case  8: motor->tmp.ESC_ID     = y.u_val; motor->tmp.read_flag = 10; break;
      case  9: motor->tmp.TIMEOUT    = y.u_val; motor->tmp.read_flag = 11; break;
      case 10: motor->tmp.cmode      = y.u_val; motor->tmp.read_flag = 12; break;
      case 11: motor->tmp.Damp       = y.f_val; motor->tmp.read_flag = 13; break;
      case 12: motor->tmp.Inertia    = y.f_val; motor->tmp.read_flag = 14; break;
      case 13: motor->tmp.hw_ver     = y.u_val; motor->tmp.read_flag = 15; break;
      case 14: motor->tmp.sw_ver     = y.u_val; motor->tmp.read_flag = 16; break;
      case 15: motor->tmp.SN         = y.u_val; motor->tmp.read_flag = 17; break;
      case 16: motor->tmp.NPP        = y.u_val; motor->tmp.read_flag = 18; break;
      case 17: motor->tmp.Rs         = y.f_val; motor->tmp.read_flag = 19; break;
      case 18: motor->tmp.Ls         = y.f_val; motor->tmp.read_flag = 20; break;
      case 19: motor->tmp.Flux       = y.f_val; motor->tmp.read_flag = 21; break;
      case 20: motor->tmp.Gr         = y.f_val; motor->tmp.read_flag = 22; break;
      case 21: motor->tmp.PMAX       = y.f_val; motor->tmp.read_flag = 23; break;
      case 22: motor->tmp.VMAX       = y.f_val; motor->tmp.read_flag = 24; break;
      case 23: motor->tmp.TMAX       = y.f_val; motor->tmp.read_flag = 25; break;
      case 24: motor->tmp.I_BW       = y.f_val; motor->tmp.read_flag = 26; break;
      case 25: motor->tmp.KP_ASR     = y.f_val; motor->tmp.read_flag = 27; break;
      case 26: motor->tmp.KI_ASR     = y.f_val; motor->tmp.read_flag = 28; break;
      case 27: motor->tmp.KP_APR     = y.f_val; motor->tmp.read_flag = 29; break;
      case 28: motor->tmp.KI_APR     = y.f_val; motor->tmp.read_flag = 30; break;
      case 29: motor->tmp.OV_Value   = y.f_val; motor->tmp.read_flag = 31; break;
      case 30: motor->tmp.GREF       = y.f_val; motor->tmp.read_flag = 32; break;
      case 31: motor->tmp.Deta       = y.f_val; motor->tmp.read_flag = 33; break;
      case 32: motor->tmp.V_BW       = y.f_val; motor->tmp.read_flag = 34; break;
      case 33: motor->tmp.IQ_cl      = y.f_val; motor->tmp.read_flag = 35; break;
      case 34: motor->tmp.VL_cl      = y.f_val; motor->tmp.read_flag = 36; break;
      case 35: motor->tmp.can_br     = y.u_val; motor->tmp.read_flag = 37; break;
      case 36: motor->tmp.sub_ver    = y.u_val; motor->tmp.read_flag = 38; break;
      case 50: motor->tmp.u_off      = y.f_val; motor->tmp.read_flag = 39; break;
      case 51: motor->tmp.v_off      = y.f_val; motor->tmp.read_flag = 40; break;
      case 52: motor->tmp.k1         = y.f_val; motor->tmp.read_flag = 41; break;
      case 53: motor->tmp.k2         = y.f_val; motor->tmp.read_flag = 42; break;
      case 54: motor->tmp.m_off      = y.f_val; motor->tmp.read_flag = 43; break;
      case 55: motor->tmp.dir        = y.f_val; motor->tmp.read_flag = 44; break;
      case 80: motor->tmp.p_m        = y.f_val; motor->tmp.read_flag = 45; break;
      case 81: motor->tmp.x_out      = y.f_val; motor->tmp.read_flag = 0 ; break;
    }
  }
}
