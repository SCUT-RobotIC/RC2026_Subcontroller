#include "bsp_vesc.h"

// by Poria Cocos, RobotIC laboratory, South China University of Technology
// https://github.com/Poria-Cocos

// id:10~17
// cmd: 0x00: duty, 0x01: current, 0x02: brake current, 0x03: speed, 0x04: position
// status: 9: CAN_PACKET_STATUS, 14: CAN_PACKET_STATUS_2, 15: CAN_PACKET_STATUS_3, 16: CAN_PACKET_STATUS_4, 27: CAN_PACKET_STATUS_5

FDCAN_TxHeaderTypeDef fdcan_tx_header_vesc;
uint8_t fdcan_tx_data_vesc[4];

motor_vesc_t motor_vesc[2][8] = {0};

void vesc_init(FDCAN_HandleTypeDef *hfdcan)
{
	fdcan_filter_init_vesc(hfdcan);
	for(int j=0; j<3; j++){
		for(int i=0; i<8; i++){
			motor_vesc[j][i].id = 0x10+i;
		}
	}
	VescStop();
}

void VescStop(void)
{
	for(int j=0; j<3; j++){
		for(int i=0; i<8; i++){
			motor_vesc[j][i].cmd = 0x01;
			motor_vesc[j][i].data = 0;
		}
	}
}

void VescSet(uint8_t fdcan_ch, uint8_t id, uint8_t cmd, int32_t data)
{
	if (id>=CAN_ID_VESC_M0 && id<=CAN_ID_VESC_M7 && fdcan_ch>=1 && fdcan_ch<=3){
		for(int j=0; j<3; j++){
			for(int i=0; i<8; i++){
				if(motor_vesc[j][i].data > 50000) motor_vesc[j][i].data = 50000;
				else if(motor_vesc[j][i].data < -50000) motor_vesc[j][i].data = -50000;
			}
		}
		motor_vesc[fdcan_ch-1][id-CAN_ID_VESC_M1].cmd = cmd;
		motor_vesc[fdcan_ch-1][id-CAN_ID_VESC_M1].data = data;
	}
}

void fdcan_send_vesc(FDCAN_HandleTypeDef *hfdcan, motor_vesc_t *motor_vesc)
{
	fdcan_tx_header_vesc.Identifier = (motor_vesc->cmd << 8) + motor_vesc->id;
	fdcan_tx_header_vesc.IdType = FDCAN_EXTENDED_ID;
	fdcan_tx_header_vesc.TxFrameType = FDCAN_DATA_FRAME;
	fdcan_tx_header_vesc.DataLength = FDCAN_DLC_BYTES_4;

	fdcan_tx_data_vesc[0] = (motor_vesc->data >> 24) & 0xFF;
	fdcan_tx_data_vesc[1] = (motor_vesc->data >> 16) & 0xFF;
	fdcan_tx_data_vesc[2] = (motor_vesc->data >> 8) & 0xFF;
	fdcan_tx_data_vesc[3] = motor_vesc->data & 0xFF;

	HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &fdcan_tx_header_vesc, fdcan_tx_data_vesc);
}

void motor_update_vesc(motor_vesc_t *motor, uint8_t status, uint8_t *data)
{
	switch (status){
		case 9: // CAN_PACKET_STATUS
			motor->rpm = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
			motor->current = ((data[4] << 8) | data[5]);
			motor->duty = ((data[6] << 8) | data[7]);
			break;
		
		// If you want to get more mesage, you can uncomment the following code.
		case 14: // CAN_PACKET_STATUS_2
			motor->amp_hours = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
			motor->amp_hours_charged = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7];
			break;

		case 15: // CAN_PACKET_STATUS_3
			motor->watt_hours = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
			motor->watt_hours_charged = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7];
			break;

		case 16: // CAN_PACKET_STATUS_4
			motor->temp_fet = ((data[0] << 8) | data[1]);
			motor->temp_motor = ((data[2] << 8) | data[3]);
			motor->current_in = ((data[4] << 8) | data[5]);
			motor->pid_pos_now = ((data[6] << 8) | data[7]);
			break;

		// case 27: // CAN_PACKET_STATUS_5
		// 	motor->tachometer = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
		// 	motor->v_in = ((data[4] << 8) | data[5]);
		// 	break;

		default:
			// ignore
			break;
	}
}

void fdcan_filter_init_vesc(FDCAN_HandleTypeDef *hfdcan)
{
	// must run this function before HAL_FDCAN_Start()
	FDCAN_FilterTypeDef fdcan_filter;
	
	fdcan_filter.IdType = FDCAN_EXTENDED_ID;
	fdcan_filter.FilterIndex = 0;
	fdcan_filter.FilterType = FDCAN_FILTER_MASK;
	fdcan_filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;// FIFO1
	fdcan_filter.FilterID1 = 0x00;
	fdcan_filter.FilterID2 = 0x00;
	HAL_FDCAN_ConfigFilter(hfdcan,&fdcan_filter);
	HAL_FDCAN_ConfigGlobalFilter(hfdcan,FDCAN_REJECT,FDCAN_REJECT,FDCAN_REJECT_REMOTE,FDCAN_REJECT_REMOTE);
	HAL_FDCAN_ConfigFifoWatermark(hfdcan, FDCAN_CFG_RX_FIFO1, 1);
}

void CAN_bus_off_check_reset(FDCAN_HandleTypeDef *hfdcan)
{
	FDCAN_ProtocolStatusTypeDef protocolStatus = {};
	HAL_FDCAN_GetProtocolStatus(hfdcan, &protocolStatus);
	if (protocolStatus.BusOff) {
		CLEAR_BIT(hfdcan->Instance->CCCR, FDCAN_CCCR_INIT);
	}
}

void HAL_FDCAN_ErrorStatusCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs)
{
	if (hfdcan == &hfdcan3 || hfdcan == &hfdcan2 || hfdcan == &hfdcan1) {
		if ((ErrorStatusITs & FDCAN_IT_BUS_OFF) != RESET) {
			CAN_bus_off_check_reset(hfdcan);
		}
	}
}
