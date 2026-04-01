#ifndef __BSP_DJI_H__
#define __BSP_DJI_H__

#include "bsp_fdcan.h"
#include <stdint.h>

// by RobotIC laboratory, South China University of Technology

#define FRONT 0
#define LAST 1

typedef enum{
  CAN_ID_DJI_FRONT = 0x200,
  CAN_ID_DJI_LAST = 0x1FF,

  CAN_ID_DJI_M0 = 0x201,
  CAN_ID_DJI_M1 = 0x202,
  CAN_ID_DJI_M2 = 0x203,
  CAN_ID_DJI_M3 = 0x204,
  CAN_ID_DJI_M4 = 0x205,
  CAN_ID_DJI_M5 = 0x206,
  CAN_ID_DJI_M6 = 0x207,
  CAN_ID_DJI_M7 = 0x208,

  CAN_ID_DJI_6020_FRONT  = 0x1FF,
  CAN_ID_DJI_6020_LAST = 0x2FF,
  
  CAN_ID_DJI_6020_M0 = 0x205,
  CAN_ID_DJI_6020_M1 = 0x206,
  CAN_ID_DJI_6020_M2 = 0x207,
  CAN_ID_DJI_6020_M3 = 0x208,
  CAN_ID_DJI_6020_M4 = 0x209,
  CAN_ID_DJI_6020_M5 = 0x20A,
  CAN_ID_DJI_6020_M6 = 0x20B,
} can_id_dji_e;

typedef struct
{
  uint8_t activate;
  uint16_t ecd;
  int16_t speed_rpm;
  int16_t given_current;
  uint8_t temperate;
  int16_t last_ecd;
  int circle;

  int16_t current_set;
  float target;
  
} motor_dji_t;

void fdcan_send_dji(FDCAN_HandleTypeDef *hfdcan, int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4, uint8_t motor_id);
void motor_update_dji(motor_dji_t *ptr, uint8_t *data);
void circle_cc_dji(motor_dji_t *ptr);

#endif
