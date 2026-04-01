#ifndef __BSP_MATLAB_CTRL_H__
#define __BSP_MATLAB_CTRL_H__

#include "fdcan.h"
#include <stdint.h>

#define VEL 1
#define ANG 2

void matlab_motor_para_init(FDCAN_HandleTypeDef *hfdcan, uint16_t can_id, uint8_t mode,
                            float spd_kp, float spd_ki, float spd_kd, float spd_deadband,
                            float ang_a_kp, float ang_a_ki, float ang_a_kd, float ang_a_deadband,
                            float ang_s_kp, float ang_s_ki, float ang_s_kd, float ang_s_deadband);
void matlab_motor_inport_update(void);
void matlab_motor_outport_update(void);
void matlab_motor_set_reset(void);

#endif
