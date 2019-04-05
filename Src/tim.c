/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* Variables -----------------------------------------------------------------*/
volatile uint16_t StepCW = 0,StepCCW = 0;
extern volatile float PD;
extern float V_LOAD, I_LOAD;

/* Typedefs -----------------------------------------------------------------*/
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim8;
TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim11;

/* Timer interrupt call back */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM7)
	{
		Perturb_Observe(V_LOAD, I_LOAD);
	}
	else if(htim->Instance == TIM10)                    //Stepper motor interrupt
	{
		if(PD > 0)                                      //If PD>0 spin clockwise
			{
				Step_Clockwise();
			}
		else if(PD < 0)                                 //If PD<0 spin counter clockwise
			{
				Step_Counter_Clockwise();
			}
	}
	
}

/* TIM7 init function */
void MX_TIM7_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim7.Instance = TIM7;
  htim7.Init.Prescaler = TIM7_PRESCALER - 1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = (TIMER_CLK_FREQ/TIM7_PRESCALER)/MPPT_CONTROL_LOOP_FREQ-1;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}
/* TIM8 init function */
void MX_TIM8_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim8.Instance = TIM8;
  htim8.Init.Prescaler = TIM8_PRESCALER - 1;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = (TIMER_CLK_FREQ/TIM8_PRESCALER)/ADC_FREQ-1;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim8) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

}
/* TIM10 init function */
void MX_TIM10_Init(void)
{

  htim10.Instance = TIM10;
  htim10.Init.Prescaler = TIM10_PRESCALER-1;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = (TIMER_CLK_FREQ/TIM10_PRESCALER)/STEPPER_FREQ-1;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }

}
/* TIM11 init function */
void MX_TIM11_Init(void)
{
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim11.Instance = TIM11;
  htim11.Init.Prescaler = TIM11_PRESCALER - 1;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 2*(TIMER_CLK_FREQ/TIM11_PRESCALER)/PWM_FREQ - 1;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = (TIMER_CLK_FREQ/TIM11_PRESCALER)/PWM_FREQ;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim11, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_TIM_MspPostInit(&htim11);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{
  if(tim_baseHandle->Instance==TIM7)
  {
	/* TIM7 clock enable */
	__HAL_RCC_TIM7_CLK_ENABLE();

    /* TIM7 interrupt Init */
    HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM7_IRQn);
  }
  else if(tim_baseHandle->Instance==TIM8)
  {
    /* TIM8 clock enable */
    __HAL_RCC_TIM8_CLK_ENABLE();
  }
  else if(tim_baseHandle->Instance==TIM10)
  {
    /* TIM10 clock enable */
    __HAL_RCC_TIM10_CLK_ENABLE();

    /* TIM10 interrupt Init */
    HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
  }
  else if(tim_baseHandle->Instance==TIM11)
  {
    /* TIM11 clock enable */
    __HAL_RCC_TIM11_CLK_ENABLE();
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM11)
  {
    __HAL_RCC_GPIOF_CLK_ENABLE();
    /**TIM11 GPIO Configuration    
    PF7     ------> TIM11_CH1 
    */
    GPIO_InitStruct.Pin = ARD_D6_PWM_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_TIM11;
    HAL_GPIO_Init(ARD_D6_PWM_GPIO_Port, &GPIO_InitStruct);
  }

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{
  if(tim_baseHandle->Instance==TIM7)
  {
	/* Peripheral clock disable */
    __HAL_RCC_TIM7_CLK_DISABLE();

     /* TIM7 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM7_IRQn);
  }
  else if(tim_baseHandle->Instance==TIM8)
  {
    /* Peripheral clock disable */
    __HAL_RCC_TIM8_CLK_DISABLE();
  }
  else if(tim_baseHandle->Instance==TIM10)
  {
    /* Peripheral clock disable */
    __HAL_RCC_TIM10_CLK_DISABLE();

    /* TIM10 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM1_UP_TIM10_IRQn);
  }
  else if(tim_baseHandle->Instance==TIM11)
  {
    /* Peripheral clock disable */
    __HAL_RCC_TIM11_CLK_DISABLE();
  }
} 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
