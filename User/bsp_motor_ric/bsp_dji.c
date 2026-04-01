#include "bsp_dji.h"
#include <stdint.h>

FDCAN_TxHeaderTypeDef fdcan_tx_header_dji;
static uint8_t fdcan_tx_data_dji[8];

motor_dji_t motor_dji[3][8] = {0};

void fdcan_send_dji(FDCAN_HandleTypeDef *hfdcan, int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4, uint8_t motor_id)
{
	if(motor_id == FRONT)
		fdcan_tx_header_dji.Identifier = CAN_ID_DJI_FRONT;
	else if (motor_id == LAST)
		fdcan_tx_header_dji.Identifier = CAN_ID_DJI_LAST;
  
  fdcan_tx_header_dji.IdType = FDCAN_STANDARD_ID;
  fdcan_tx_header_dji.TxFrameType = FDCAN_DATA_FRAME;
  fdcan_tx_header_dji.DataLength = FDCAN_DLC_BYTES_8;
  
  fdcan_tx_data_dji[0] = motor1 >> 8;
  fdcan_tx_data_dji[1] = motor1;
  fdcan_tx_data_dji[2] = motor2 >> 8;
  fdcan_tx_data_dji[3] = motor2;
  fdcan_tx_data_dji[4] = motor3 >> 8;
  fdcan_tx_data_dji[5] = motor3;
  fdcan_tx_data_dji[6] = motor4 >> 8;
  fdcan_tx_data_dji[7] = motor4;

  HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &fdcan_tx_header_dji, fdcan_tx_data_dji);
}

void motor_update_dji(motor_dji_t *ptr, uint8_t *data)
{
  (ptr)->activate=1;
  (ptr)->last_ecd = (ptr)->ecd;
  (ptr)->ecd = (uint16_t)(data[0] << 8 | data[1]);
  (ptr)->speed_rpm = (uint16_t)(data[2] << 8 | data[3]);
  (ptr)->given_current = (uint16_t)(data[4] << 8 | data[5]);
  (ptr)->temperate = data[6];
}


void circle_cc_dji(motor_dji_t *ptr)
{
  if (((ptr)->ecd - (ptr)->last_ecd) > 5000)
  {
    ptr->circle -= 1;
  }
  else if (((ptr)->ecd - (ptr)->last_ecd) < -5000)
  {
    ptr->circle += 1;
  }
}
