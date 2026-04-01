#include "bsp_subcontrol.h"
#include "string.h"
data_subcontrol_t subcontroller;

FDCAN_TxHeaderTypeDef fdcan_tx_header_subcontrol;
uint8_t fdcan_tx_data_subcontrol[8]={0};

uint8_t try[16]={0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF,0xFE,0xDC,0xBA,0x98,0x76,0x54,0x32,0x10};
void subcontrol_init()
{
	subcontroller.errorState=0;
	subcontroller.heartBeatCnt=0;
	subcontroller.id=SUBCTRL_ID;
	memset(subcontroller.received_data,0,20);
	memcpy(subcontroller.transmit_data,try,16);
	
}

/****************************************************************
  @功能     	  : CAN接收处理
  @参数         : 29位扩展id，数据区8字节
****************************************************************/
void subcontrol_receive(uint32_t id, uint8_t *data)
{
  switch(id>>25){
    case 0x01:{
			switch(id>>24 & 0x1){
			case 0:{ // dataPacket [0]-[9]
					subcontroller.received_data[0] = (id >> 8) & 0xFF;    
					subcontroller.received_data[1] = (id >> 16) & 0xFF; 
					memcpy(subcontroller.received_data+2, data, 8); // 将接收到的数据复制到全局变量
					break;
			} 
			case 1:{ // dataPacket [10]-[19]
					subcontroller.received_data[10] = (id >> 8) & 0xFF;    
					subcontroller.received_data[11] = (id >> 16) & 0xFF;
					memcpy(subcontroller.received_data + 12, data, 8); // 将接收到的数据复制到全局变量
					break;
			}
			default:break;
			}
		}
    default:break;
  }
}
	
/****************************************************************
  @功能     	  : CAN发送处理
  @参数         : id，，数据区8字节
****************************************************************/
void subcontrol_transmit(void)
{
  fdcan_tx_header_subcontrol.Identifier = 0x2 << 25 | SUBCONTROL_FRONT<<24 |subcontroller.heartBeatCnt << 8 | (subcontroller.id+0x08);
  fdcan_tx_header_subcontrol.IdType = FDCAN_EXTENDED_ID;
  fdcan_tx_header_subcontrol.TxFrameType = FDCAN_DATA_FRAME;
  fdcan_tx_header_subcontrol.DataLength = FDCAN_DLC_BYTES_8;

  memcpy(fdcan_tx_data_subcontrol, subcontroller.transmit_data, 8); // 将要发送的数据复制到全局变量
  HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan3, &fdcan_tx_header_subcontrol, fdcan_tx_data_subcontrol);
	
	fdcan_tx_header_subcontrol.Identifier = 0x2 << 25 | SUBCONTROL_LAST<<24 |subcontroller.heartBeatCnt << 8 | (subcontroller.id+0x08);

  memcpy(fdcan_tx_data_subcontrol, subcontroller.transmit_data+8, 8); // 将要发送的数据复制到全局变量
  HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan3, &fdcan_tx_header_subcontrol, fdcan_tx_data_subcontrol);
}
