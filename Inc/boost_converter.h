/**
  ******************************************************************************
  * File Name          : boost_converter.h
  * Description        : This file provides code to control the boost converter
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOOST_CONVERTER_H
#define __BOOST_CONVERTER_H

/* Includes ------------------------------------------------------------------*/
#include "adc.h"
#include "main.h"
#include "tim.h"
#include "lcd.h"

/* Defines -------------------------------------------------------------------*/
#define SHUNT_RESISTOR 				0.22f   //Current Sensor Shunt Resistor
#define V_MIN						10.0f
#define V_MAX						15.0f
#define SC_VOLT_DIFF				0.01f	//Short circuit voltage difference
#define MAX_PWM_COUNT 				2*(TIMER_CLK_FREQ/TIM11_PRESCALER)/PWM_FREQ
#define DELTA_DUTY_CYCLE			DUTY_CYCLE_2_5_PERCENT //Value to change duty cycle by (MPPT)
#define DUTY_CYCLE_1_PERECENT		MAX_PWM_COUNT/100
#define DUTY_CYCLE_2_5_PERCENT		MAX_PWM_COUNT/40
#define DUTY_CYCLE_5_PERCENT		MAX_PWM_COUNT/20
#define DUTY_CYCLE_10_PERCENT 		MAX_PWM_COUNT/10
#define DUTY_CYCLE_15_PERCENT		MAX_PWM_COUNT/7
#define DUTY_CYCLE_30_PERCENT 		MAX_PWM_COUNT/3
#define BOOST_KP					MAX_PWM_COUNT/100	//Proportional error constant

/* Prototypes ----------------------------------------------------------------*/
void Perturb_Observe(float V_LOAD, float I_LOAD);

#endif /* __BOOST_CONVERTER_H */
