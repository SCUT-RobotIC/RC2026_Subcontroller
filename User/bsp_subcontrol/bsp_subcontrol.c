#include "bsp_subcontrol.h"
#include "string.h"
data_subcontrol_t subcontroller;

FDCAN_TxHeaderTypeDef fdcan_tx_header_subcontrol;
uint8_t fdcan_tx_data_subcontrol[8]={0};

uint8_t try[8]={0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF};
void subcontrol_init()
{
	subcontroller.errorState=0;
	subcontroller.heartBeatCnt=0;
	subcontroller.id=SUBCTRL_ID;
	memset(subcontroller.received_data,0,8);
	memset(subcontroller.transmit_data,0,8);
}

/****************************************************************
  @功能     	  : CAN接收处理
  @参数         : 标准id，数据区20字节
****************************************************************/
void subcontrol_receive(uint8_t *data)
{
  memcpy(subcontroller.received_data,data,20);
}
	
/****************************************************************
  @功能     	  : CAN发送处理
  @参数         : 标准id，数据区20字节
****************************************************************/
void subcontrol_transmit(void)
{
	fdcan_tx_header_subcontrol.Identifier = subcontroller.id+0x08;
	fdcan_tx_header_subcontrol.IdType = FDCAN_STANDARD_ID;
	fdcan_tx_header_subcontrol.TxFrameType = FDCAN_DATA_FRAME;
	fdcan_tx_header_subcontrol.DataLength = FDCAN_DLC_BYTES_8;
  memcpy(fdcan_tx_data_subcontrol, subcontroller.transmit_data, 8);
  HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan3, &fdcan_tx_header_subcontrol, fdcan_tx_data_subcontrol);
}
