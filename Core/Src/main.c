/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fdcan.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_fdcan.h"
#include "bsp_dji.h"
#include "bsp_dm_ctrl.h"
#include "bsp_dm_drv.h"
#include "bsp_vesc.h"
#include "bsp_matlab_ctrl.h"
#include "bsp_robstride.h"
#include "PID_MODEL_SYSTEM.h"
#include "bsp_subcontrol.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define VEL 1
#define ANG 2
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern motor_dji_t motor_dji[2][8];
extern motor_dm_t motor_dm[2][8];
extern motor_vesc_t motor_vesc[2][8];
extern motor_robstride_t motor_robstride[2][16];
extern data_subcontrol_t subcontroller;
uint8_t uart_index[2]={0};
uint8_t buffer[10]={0};
uint8_t USART1_RX_BUF[10]={0};
uint8_t USART2_RX_BUF[10]={0};

uint8_t cnt[10]={0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */
void Set_servo(TIM_HandleTypeDef * htim,uint32_t Channel,uint8_t angle){
	if(angle<=180)
		__HAL_TIM_SET_COMPARE(htim, Channel,0.5*1000+angle*100/9);
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FDCAN1_Init();
  MX_FDCAN2_Init();
  MX_FDCAN3_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  MX_I2C1_Init();
  MX_SPI4_Init();
  MX_USART3_UART_Init();
  MX_USART10_UART_Init();
  /* USER CODE BEGIN 2 */
  fdcan_all_init_start();	
	subcontrol_init();
  dm_motor_init(&motor_dm[0][0],CAN_ID_DM_M0,mit_mode,0.0f,2.0f,12.5f,30.0f,10.0f);	
  for(int i=0;i<1;i++){
		dm_motor_enable(&hfdcan1, &motor_dm[0][0]);
		robstride_init(1,1,rs03,move_control_mode);
		HAL_Delay(10);
	}

	PID_MODEL_SYSTEM_initialize();
  matlab_motor_para_init(&hfdcan1,CAN_ID_DJI_M0,VEL,10,3,0.01,20,0,0,0,10,0,0,0,20);
	HAL_UART_Receive_IT(&huart1, buffer, 1);                       
	HAL_UART_Receive_IT(&huart1, buffer+1, 1);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_ALL);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_ALL);
  Set_servo(&htim2,TIM_CHANNEL_1,0);
  Set_servo(&htim2,TIM_CHANNEL_2,45);
  Set_servo(&htim3,TIM_CHANNEL_1,90);
  Set_servo(&htim3,TIM_CHANNEL_2,180);
	HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = 64;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 30;
  RCC_OscInitStruct.PLL.PLLP = 1;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
    {
//		if(uart_index[0] == 0 && buffer[0]!=包头){
//			HAL_UART_Receive_IT(&huart1, buffer, 1);
//		}
//		else{
//			USART1_RX_BUF[uart_index[0]++] = buffer[0];
//			if (uart_index[0] == 包长)
//			{
//				uart_index[0] = 0;
//				if (USART1_RX_BUF[0] == 包头 && USART1_RX_BUF[] == 包尾) 
//				{                        
							
//				}
//			}
//			HAL_UART_Receive_IT(&huart1, buffer, 1);
//		}
    }
	if (huart->Instance == USART2)
    {
//		if(uart_index[1] == 0 && buffer[1]!=包头){
//			HAL_UART_Receive_IT(&huart2, buffer+1, 1);
//		}
//		else{
//			USART1_RX_BUF[uart_index[1]++] = buffer[1];
//			if (uart_index[0] == 包长)
//			{
//				uart_index[1] = 0;
//				if (USART2_RX_BUF[0] == 包头 && USART2_RX_BUF[] == 包尾) 
//				{                        
							
//				}
//			}
//			HAL_UART_Receive_IT(&huart2, buffer+1, 1);
//		}
    }
}

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
	if (htim->Instance == TIM6){
//    motor_dji[0][0].target = 1000;
		if(subcontroller.heartBeatCnt==59999) subcontroller.heartBeatCnt=0;	
		else subcontroller.heartBeatCnt++;
			
    matlab_motor_inport_update();
    PID_MODEL_SYSTEM_step();
    matlab_motor_outport_update();
    fdcan_send_dji(&hfdcan1, motor_dji[0][0].current_set, 0, 0, 0, FRONT);
    dm_motor_ctrl_send(&hfdcan1, &motor_dm[0][0]);
		fdcan_send_vesc(&hfdcan1, &motor_vesc[0][0]);
				
	}
	if (htim->Instance == TIM7){
		cnt[0]++;
		if(cnt[0]==100){
			HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_4);
			HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_5);
			cnt[0]=0;
		}
		robstride_command_update_all();
    subcontrol_transmit();	
    if(subcontroller.received_data[0]==0x2B){
      Set_servo(&htim2,TIM_CHANNEL_1,180);
    }
	}
  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
