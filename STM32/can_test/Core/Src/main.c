/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "can.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static uint8_t buffer[8];

void can_receive()
{
    CAN_FilterTypeDef canFilterSt;
    canFilterSt.FilterActivation = ENABLE;
    canFilterSt.FilterMode = CAN_FILTERMODE_IDMASK;
    canFilterSt.FilterScale = CAN_FILTERSCALE_32BIT;
    canFilterSt.FilterIdHigh = 0x0000;
    canFilterSt.FilterIdLow = 0x0000;
    canFilterSt.FilterMaskIdHigh = 0x0000;
    canFilterSt.FilterMaskIdLow = 0x0000;
    canFilterSt.FilterBank = 0;
    canFilterSt.FilterFIFOAssignment = CAN_RX_FIFO1;
    HAL_CAN_ConfigFilter(&hcan, &canFilterSt);
    HAL_CAN_Start(&hcan);
    HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO1_MSG_PENDING);
}

CAN_TxHeaderTypeDef tx_msg = {
    .StdId = 0x50,
    .IDE = CAN_ID_STD,
    .RTR = CAN_RTR_DATA,
    .DLC = 0x08,
};

uint32_t send_box = CAN_TX_MAILBOX0;

uint8_t uart_send_buffer[32];

#define SEND 0


#if SEND
//输出接收到的数据
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *can)
{
    CAN_RxHeaderTypeDef rx_header;
    HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO1, &rx_header, buffer);
    tx_msg.DLC = rx_header.DLC;
    sprintf((char *) uart_send_buffer, "rx: %s\r\n", buffer);
    HAL_UART_Transmit(&huart1, uart_send_buffer, strlen((char *) uart_send_buffer), 100);
    memset(uart_send_buffer, 0, 32);
}
#else
// 接收数据转发
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *can)
{
    CAN_RxHeaderTypeDef rx_header;
    HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO1, &rx_header, buffer);
    tx_msg.DLC = rx_header.DLC;
    HAL_CAN_AddTxMessage(&hcan, &tx_msg, buffer, &send_box);
    sprintf((char *) uart_send_buffer,
            "rx: %d %d %d\r\n",
            *(uint16_t *) buffer,
            *(uint16_t *) (buffer + 2),
            *(uint16_t *) (buffer + 4));
    HAL_UART_Transmit(&huart1, uart_send_buffer, strlen((char *) uart_send_buffer), 100);
    memset(uart_send_buffer, 0, 32);
}
#endif
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

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
    MX_USART1_UART_Init();
    MX_CAN_Init();
    /* USER CODE BEGIN 2 */
    can_receive();

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    int cnt = 0;
    uint8_t test_buffer[8];
    while (1) {
        HAL_Delay(500);
#if SEND
        tx_msg.DLC = 3;
        cnt++;
        if (cnt >= 10000) {
            cnt = 0;
        }
        sprintf((char *) test_buffer, "%d", cnt);
        HAL_CAN_AddTxMessage(&hcan, &tx_msg, test_buffer, &send_box);
#endif
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

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
        | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
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
