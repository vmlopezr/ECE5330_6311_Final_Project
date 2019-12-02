/**
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim4;

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;

// Configure the Microcontroller Clock
void SystemClock_Config(void);
// Configure the Microcontroller GPIO
void MX_GPIO_Init(void);
// Configure the Microcontroller DMA to access USART2
void MX_DMA_Init(void);
// Configure Timer 1 for PWM Generation
void MX_TIM1_Init(void);
// Configure Timer 4 for PWM Generation
void MX_TIM4_Init(void);
// Configure USART2 peripheral to read data from Python application
void MX_USART2_UART_Init(void);
void Error_Handler(void);

#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
