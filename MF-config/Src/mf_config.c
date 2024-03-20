/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : mf_config.c
  * @brief          : MCU FUNCTION CONFIG
  ******************************************************************************
  * @attention
  *
  * Copyright (c) [2019] [Fudan Microelectronics]
  * THIS SOFTWARE is licensed under the Mulan PSL v1.
  * can use this software according to the terms and conditions of the Mulan PSL v1.
  * You may obtain a copy of Mulan PSL v1 at:
  * http://license.coscl.org.cn/MulanPSL
  * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
  * PURPOSE.
  * See the Mulan PSL v1 for more details.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "mf_config.h"
#include "user_init.h"


/* Private function prototypes -----------------------------------------------*/

 
/**
  * @brief  UART0 Initialization function
  * @param  void
  * @retval None
  */
void MF_UART0_Init(void)
{

    FL_GPIO_InitTypeDef    GPIO_InitStruct;

    FL_UART_InitTypeDef    defaultInitStruct;

    GPIO_InitStruct.pin = FL_GPIO_PIN_2;
    GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;

    FL_GPIO_Init( GPIOA, &GPIO_InitStruct );

    GPIO_InitStruct.pin = FL_GPIO_PIN_3;
    GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;

    FL_GPIO_Init( GPIOA, &GPIO_InitStruct );

    defaultInitStruct.clockSrc = FL_RCC_UART0_CLK_SOURCE_APB1CLK;
    defaultInitStruct.baudRate = 115200;
    defaultInitStruct.dataWidth = FL_UART_DATA_WIDTH_8B;
    defaultInitStruct.stopBits = FL_UART_STOP_BIT_WIDTH_1B;
    defaultInitStruct.parity = FL_UART_PARITY_EVEN;
    defaultInitStruct.transferDirection = FL_UART_DIRECTION_TX_RX;

    FL_UART_Init(UART0,&defaultInitStruct );
    
}
 
/**
  * @brief  NVIC Initialization function
  * @param  void
  * @retval None
  */
void MF_NVIC_Init(void)
{


    NVIC_ConfigTypeDef    InterruptConfigStruct;

    InterruptConfigStruct.preemptPriority = 2;
    NVIC_Init(&InterruptConfigStruct,UART0_IRQn );
	
    InterruptConfigStruct.preemptPriority = 3;
    NVIC_Init(&InterruptConfigStruct,ATIM_IRQn );

    
}

void ClockInit(uint32_t clock)
{
    switch (clock)
    {
        case FL_RCC_RCHF_FREQUENCY_8MHZ:
            FL_RCC_RCHF_WriteTrimValue(RCHF8M_TRIM);
            break;
        
        case FL_RCC_RCHF_FREQUENCY_16MHZ:
            FL_RCC_RCHF_WriteTrimValue(RCHF16M_TRIM);
            break;
        
        case FL_RCC_RCHF_FREQUENCY_24MHZ:
            FL_RCC_RCHF_WriteTrimValue(RCHF24M_TRIM);
            break;
        
        default:
            FL_RCC_RCHF_WriteTrimValue(RCHF8M_TRIM);
            break;
    }
    
    FL_RCC_RCHF_SetFrequency(clock);
    FL_RCC_SetSystemClockSource(FL_RCC_SYSTEM_CLK_SOURCE_RCHF);
}
/**
  * @brief  The application entry point.
  * @retval int
  */
void MF_Clock_Init(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  /* System interrupt init*/

  /* Initialize all configured peripherals */
 
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void MF_SystemClock_Config(void)
{
     ClockInit(FL_RCC_RCHF_FREQUENCY_24MHZ);
}

void MF_ATIM_TimerBase_Init(void)
{

    /*IO CONFIG*/
    FL_ATIM_InitTypeDef    TimerBaseInitStruct;

    TimerBaseInitStruct.clockSource = FL_RCC_ATIM_CLK_SOURCE_APB2CLK;
    TimerBaseInitStruct.prescaler = 0;
    TimerBaseInitStruct.counterMode = FL_ATIM_COUNTER_DIR_UP;
    TimerBaseInitStruct.autoReload = 0xFFFFU;
    TimerBaseInitStruct.autoReloadState = FL_DISABLE;
    TimerBaseInitStruct.clockDivision = FL_ATIM_CLK_DIVISION_DIV1;
    TimerBaseInitStruct.repetitionCounter = 0;

    FL_ATIM_Init(ATIM,&TimerBaseInitStruct );


    FL_ATIM_EnableIT_Update(ATIM );

    
}
 


void MF_Config_Init(void)
{
	/*FUNCTION CALL*/
	//MF_UART0_Init();
	MF_DMA_Channel2_Init();
	MF_DMA_Channel3_Init();

	MF_ATIM_TimerBase_Init();
	MF_NVIC_Init();
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

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
  /* USER CODE BEGIN Assert_Failed */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END Assert_Failed */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT FMSH *****END OF FILE****/
