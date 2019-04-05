/**
  ******************************************************************************
  * File Name          : stepper.h
  * Description        : This file provides code to control the stepper motor
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STEPPER_H
#define __STEPPER_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "lcd.h"

/* Macros --------------------------------------------------------------------*/
#define ABS(X)                 ((X) > 0 ? (X) : -(X))

/* Defines -------------------------------------------------------------------*/
#define Delta_t_Readings  	0.1f
#define WIND_DIR_SS_ERROR	17.0f		//Wind direction sensor is not completely centered @ 150°
#define Desired_Position  	(150.0f + WIND_DIR_SS_ERROR)
#define FMAX STEPPER_FREQ
#define FMIN 1

/* Prototypes ----------------------------------------------------------------*/
void Stepper_Control_Loop(float Current_position);
void Step_Clockwise(void);
void Step_Counter_Clockwise(void);

#endif /* __STEPPER_H */
