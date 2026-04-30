#include "bsp_fdcan.h"
#include "bsp_dji.h"
#include "bsp_dm_ctrl.h"
#include "bsp_vesc.h"
#include "bsp_robstride.h"
#include "bsp_subcontrol.h"
#include <stdint.h>

extern motor_dji_t motor_dji[3][8];
extern motor_dm_t motor_dm[2][8];
extern motor_vesc_t motor_vesc[2][8];
extern motor_robstride_t motor_robstride[2][16];
void fdcan_all_init_start(void)
{
  fdcan_filter_init(&hfdcan1);
  fdcan_filter_init(&hfdcan2);
  fdcan_filter_init(&hfdcan3);
  vesc_init(&hfdcan1);
  vesc_init(&hfdcan2);
  vesc_init(&hfdcan3);
  HAL_FDCAN_Start(&hfdcan1);
  HAL_FDCAN_Start(&hfdcan2);
  HAL_FDCAN_Start(&hfdcan3);
	HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_WATERMARK, 0);
  HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_WATERMARK, 0);
  HAL_FDCAN_ActivateNotification(&hfdcan3, FDCAN_IT_RX_FIFO0_WATERMARK, 0);
	HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO1_WATERMARK, 0);
	HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_BUS_OFF, 0);
	HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO1_WATERMARK, 0);
	HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_BUS_OFF, 0);
	HAL_FDCAN_ActivateNotification(&hfdcan3, FDCAN_IT_RX_FIFO1_WATERMARK, 0);
	HAL_FDCAN_ActivateNotification(&hfdcan3, FDCAN_IT_BUS_OFF, 0);
}

void fdcan_filter_init(FDCAN_HandleTypeDef *hfdcan)
{
	FDCAN_FilterTypeDef fdcan_filter;
	
	fdcan_filter.IdType = FDCAN_STANDARD_ID;
	fdcan_filter.FilterIndex = 0;
	fdcan_filter.FilterType = FDCAN_FILTER_MASK;
	fdcan_filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;// FIFO0
	fdcan_filter.FilterID1 = 0x00;
	fdcan_filter.FilterID2 = 0x00;
	HAL_FDCAN_ConfigFilter(hfdcan,&fdcan_filter);
	HAL_FDCAN_ConfigGlobalFilter(hfdcan,FDCAN_REJECT,FDCAN_REJECT,FDCAN_REJECT_REMOTE,FDCAN_REJECT_REMOTE);
	HAL_FDCAN_ConfigFifoWatermark(hfdcan, FDCAN_CFG_RX_FIFO0, 1);
}

void fdcan_fifo0_rx_callback(FDCAN_HandleTypeDef *hfdcan, uint8_t fdcan_ch)
{
  FDCAN_RxHeaderTypeDef fdcan_rx_header;
  uint8_t fdcan_rx_data[8] = {0};
  if(HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &fdcan_rx_header, fdcan_rx_data) == HAL_OK)
  {
    uint16_t id = fdcan_rx_header.Identifier;

		if(id >= CAN_ID_DJI_M0 && id <= CAN_ID_DJI_M7)
    {
      uint8_t i = id - CAN_ID_DJI_M0;
      motor_update_dji(&motor_dji[fdcan_ch][i], fdcan_rx_data);
      circle_cc_dji(&motor_dji[fdcan_ch][i]);
    }
    else if(id >= MASTER_ID_DM_M0 && id <= MASTER_ID_DM_M7)
    {
      uint8_t i = id - MASTER_ID_DM_M0;
      motor_update_dm(&motor_dm[fdcan_ch][i], fdcan_rx_data);
    }
		else if((id&0xFF)==SUBCTRL_ID){
      subcontrol_receive(fdcan_rx_data);
    }
  }
}

void fdcan_fifo1_rx_callback(FDCAN_HandleTypeDef *hfdcan, uint8_t fdcan_ch)
{
	FDCAN_RxHeaderTypeDef fdcan_rx_header;
  uint8_t fdcan_rx_data[8] = {0};
  if(HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &fdcan_rx_header, fdcan_rx_data) == HAL_OK)
  {
    uint32_t id = fdcan_rx_header.Identifier;
    if((id&0xFF)>=MASTER_ID_ROBSTRIDE_M0 && (id&0xFF)<=MASTER_ID_ROBSTRIDE_M15){
      uint8_t id_robstride = (fdcan_rx_header.Identifier >> 8) & 0xFF; // get id
      uint8_t i = id_robstride - CAN_ID_ROBSTRIDE_M0;
      robstride_info_update(&motor_robstride[fdcan_ch][i], id, fdcan_rx_data);
    }
		else if((id&0xFF)>=CAN_ID_VESC_M0 && (id&0xFF)<=CAN_ID_VESC_M7)
		{
			// uint16_t id = fdcan_rx_header.Identifier;
			uint8_t id_vesc = id & 0xFF; // get id
			uint8_t status_vesc = (id >> 8) & 0xFF; // get status

			if(id_vesc >= CAN_ID_VESC_M0 && id_vesc <= CAN_ID_VESC_M7){
				uint8_t i = id_vesc - CAN_ID_VESC_M0;
				motor_update_vesc(&motor_vesc[fdcan_ch][i], status_vesc, fdcan_rx_data);
		
			}
		}

	}
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  uint8_t fdcan_ch = 0;
  if (hfdcan == &hfdcan1)
    fdcan_ch = 0;
  else if (hfdcan == &hfdcan2)
    fdcan_ch = 1;
  else if (hfdcan == &hfdcan3)
    fdcan_ch = 2;
  else return;

  fdcan_fifo0_rx_callback(hfdcan, fdcan_ch);
}

void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  uint8_t fdcan_ch = 0;
  if (hfdcan == &hfdcan1)
    fdcan_ch = 0;
  else if (hfdcan == &hfdcan2)
    fdcan_ch = 1;
  else if (hfdcan == &hfdcan3)
    fdcan_ch = 2;
  else return;

  fdcan_fifo1_rx_callback(hfdcan, fdcan_ch);
}
