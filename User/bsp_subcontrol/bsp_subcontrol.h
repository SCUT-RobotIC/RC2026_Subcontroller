#ifndef __BSP_SUBCONTROL_H__
#define __BSP_SUBCONTROL_H__
#include "bsp_fdcan.h"
#include "bsp_dji.h"
#include "bsp_dm_ctrl.h"
#include "bsp_dm_drv.h"
#include "bsp_vesc.h"
#include "bsp_matlab_ctrl.h"
#include "bsp_robstride.h"
#include "PID_MODEL_SYSTEM.h"

#define SUBCTRL_ID  0x70	//0x70-0x77

#define SUBCONTROL_FRONT       0
#define SUBCONTROL_LAST        1


typedef struct{
    uint8_t id;//0-7
    uint32_t heartBeatCnt;//1000hz
    uint8_t errorState;
		uint8_t received_data[20];
		uint8_t transmit_data[16];
}data_subcontrol_t;

void subcontrol_init(void);
void subcontrol_receive(uint32_t id, uint8_t *data);
void subcontrol_transmit(void);
#endif  
