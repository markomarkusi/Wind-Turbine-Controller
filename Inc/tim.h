/**
  ******************************************************************************
  * File Name          : TIM.h
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __tim_H
#define __tim_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "stepper.h"

/* Typedefs ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim11;

/* Prototypes ----------------------------------------------------------------*/
void MX_TIM7_Init(void);
void MX_TIM8_Init(void);
void MX_TIM10_Init(void);
void MX_TIM11_Init(void);
                        
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif
#endif /*__ tim_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
