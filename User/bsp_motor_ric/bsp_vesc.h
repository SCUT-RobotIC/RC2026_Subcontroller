#ifndef __BSP_VESC_H__
#define __BSP_VESC_H__

#include "bsp_fdcan.h"
#include <stdint.h>

// by Poria Cocos, RobotIC, South China University of Technology
// https://github.com/Poria-Cocos

typedef struct {
	uint8_t cmd;
	uint8_t id;
	int32_t data;

	int32_t rpm;
	int16_t current;
	int16_t duty;
	int16_t position;
	int32_t amp_hours;
	int32_t amp_hours_charged;
	int32_t watt_hours;
	int32_t watt_hours_charged;
	int16_t temp_fet;
	int16_t temp_motor;
	int16_t current_in;
	int16_t pid_pos_now;
} motor_vesc_t;

typedef enum
{
	CAN_ID_VESC_M0  = 0x10,
	CAN_ID_VESC_M1  = 0x11,
	CAN_ID_VESC_M2  = 0x12,
	CAN_ID_VESC_M3  = 0x13,
	CAN_ID_VESC_M4  = 0x14,
	CAN_ID_VESC_M5  = 0x15,
	CAN_ID_VESC_M6  = 0x16,
	CAN_ID_VESC_M7  = 0x17,
} can_id_vesc_e;

void vesc_init(FDCAN_HandleTypeDef *hfdcan);
void fdcan_filter_init_vesc(FDCAN_HandleTypeDef *hfdcan);
void VescStop(void);
void VescSet(uint8_t fdcan_ch, uint8_t id, uint8_t cmd, int32_t data);
void fdcan_send_vesc(FDCAN_HandleTypeDef *hfdcan, motor_vesc_t *motor_vesc);
void motor_update_vesc(motor_vesc_t *motor, uint8_t status, uint8_t *data);
void CAN_bus_off_check_reset(FDCAN_HandleTypeDef *hfdcan);
// void HAL_FDCAN_ErrorStatusCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs);

#endif // __BSP_VESC_H__
