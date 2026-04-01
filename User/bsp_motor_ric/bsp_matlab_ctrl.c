#include "bsp_matlab_ctrl.h"
#include "PID_MODEL_SYSTEM.h"
#include "bsp_dji.h"
#include <stdint.h>

extern motor_dji_t motor_dji[2][8];

void matlab_motor_para_init(FDCAN_HandleTypeDef *hfdcan, uint16_t can_id, uint8_t mode,
                            float spd_kp, float spd_ki, float spd_kd, float spd_deadband,
                            float ang_a_kp, float ang_a_ki, float ang_a_kd, float ang_a_deadband,
                            float ang_s_kp, float ang_s_ki, float ang_s_kd, float ang_s_deadband)
{
  if(can_id < CAN_ID_DJI_M0 || can_id > CAN_ID_DJI_M7) return;
  uint8_t channel = hfdcan==&hfdcan1 ? 0 : (hfdcan==&hfdcan2 ? 1 : 2);
  uint8_t idx = channel * 8 + (can_id-CAN_ID_DJI_M0);
  rtU.motor_in_c[idx].mode = mode;
  motor_para.spd_kp[idx] = spd_kp;
  motor_para.spd_ki[idx] = spd_ki;
  motor_para.spd_kd[idx] = spd_kd;
  motor_para.spd_deadband[idx] = spd_deadband;
  motor_para.ang_a_kp[idx] = ang_a_kp;
  motor_para.ang_a_ki[idx] = ang_a_ki;
  motor_para.ang_a_kd[idx] = ang_a_kd;
  motor_para.ang_a_deadband[idx] = ang_a_deadband;
  motor_para.ang_s_kp[idx] = ang_s_kp;
  motor_para.ang_s_ki[idx] = ang_s_ki;
  motor_para.ang_s_kd[idx] = ang_s_kd;
  motor_para.ang_s_deadband[idx] = ang_s_deadband;
}

void matlab_motor_inport_update(void)
{
  for(uint8_t i=0; i<24; i++){
    rtU.motor_in_c[i].speed_in = motor_dji[i/8][i%8].speed_rpm;
    rtU.motor_in_c[i].circle_in = motor_dji[i/8][i%8].circle;
    rtU.motor_in_c[i].ecd =motor_dji[i/8][i%8].ecd;
    rtU.motor_in_c[i].target = motor_dji[i/8][i%8].target;
  }
}

void matlab_motor_outport_update(void)
{
  for(uint8_t i=0; i<16; i++){
    if(rtU.motor_in_c[i].mode == VEL){
      motor_dji[i/8][i%8].current_set = (int16_t)(rtY.motor_out_c[i].speed_out);
    }
    else if(rtU.motor_in_c[i].mode == ANG){
      motor_dji[i/8][i%8].current_set = (int16_t)(rtY.motor_out_c[i].angle_out);
    }
  }
}

void matlab_motor_set_reset(void)
{
  for(uint8_t i=0; i<16; i++){
    if(rtU.motor_in_c[i].mode == ANG && (fabs(rtU.motor_in_c[i].target - motor_dji[i/8][i%8].circle*8191 - motor_dji[i/8][i%8].ecd) < 20)){
      rtU.motor_in_c[i].reset = 1-rtU.motor_in_c[i].reset;
    }
  }
}
