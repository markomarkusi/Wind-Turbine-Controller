/**
  ******************************************************************************
  * File Name          : lcd.h
  * Description        : This file provides code to initialize the LCD and
  * 					 display readings on the LCD
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_H
#define __LCD_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Typedefs ----------------------------------------------------------------*/
/**
 *  @brief Turbine_StatusTypeDef
 *  Defines operation of generator. If status is 'START'
 *  electric break is applied to keep blades from spinning
 */
typedef enum
{
  START                = 0x00,
  STOP             = 0x01,
} Turbine_StatusTypeDef;

/**
 *  @brief Turbine_DirectionTypeDef
 *  Defines direction motor will spin depending on touch input
 */
typedef enum
{
  CLOCKWISE    		            = 0x00,
  COUNTER_CLOCKWISE             = 0x01,
  PD_CONTROLLER					= 0x02,
} Turbine_DirectionTypeDef;

/* Prototypes ----------------------------------------------------------------*/
void LCD_Config(void);
void Get_Touch(void);
void Display_Readings(float V_LOAD, float I_LOAD, float P_IN_Previous, uint16_t Duty_Cycle_Previous, float Position);

#endif /* __LCD_H */
