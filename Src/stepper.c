/**
  ******************************************************************************
  * File Name          : stepper.h
  * Description        : This file provides code to control the stepper motor
  ******************************************************************************
  */

 /* Includes ------------------------------------------------------------------*/
#include "stepper.h"

/* Variables ------------------------------------------------------------------*/
volatile float PD, Previous_Position;
const float Kp=11.11f,Kd=0.0f;
extern	Turbine_DirectionTypeDef Turbine_Direction;

/**
  * @brief  This function implements a PD controller to center the wind turbine
  * 		with the direction of the wind. 		
  * @param  Current position of wind direction sensor
  * @retval none
  */
void Stepper_Control_Loop(float Current_position)
{
	float Delta_Position,Error;

	Delta_Position = Previous_Position - Current_position;
	Error = Desired_Position - Current_position;
	PD = Kp*Error + Kd*(Delta_Position/Delta_t_Readings);

	if(ABS(PD) > FMAX)
	{
		if(PD>0)PD = FMAX;
		else PD = -FMAX;
	}
	else if(ABS(PD) < FMIN)
	{
		if(PD>0)PD = FMIN;
		else PD = -FMIN;
	}
	
	/* Stepper touch screen control */
	if(Turbine_Direction == COUNTER_CLOCKWISE)
	{
		PD = -FMAX;
		Turbine_Direction = PD_CONTROLLER;
	}
	else if(Turbine_Direction == CLOCKWISE)
	{
		PD = FMAX;
		Turbine_Direction = PD_CONTROLLER;
	}

	__HAL_TIM_SET_AUTORELOAD(&htim10,(TIMER_CLK_FREQ/TIM10_PRESCALER)/ABS(PD)-1);
	Previous_Position = Current_position;

}

/**
  * @brief  This function implements a switch statement to produce the 8 half-stepping
  * 		sequences required to produce 1 clockwise step.
  * @param  none
  * @retval none
  */
void Step_Clockwise(void)
{	
	extern volatile uint16_t StepCW;
	
			switch(StepCW)
			{
				case 0:	HAL_GPIO_WritePin(A_PLUS_GPIO_Port,A_PLUS_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(A_MINUS_GPIO_Port,A_MINUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_PLUS_GPIO_Port,B_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_MINUS_GPIO_Port,B_MINUS_Pin,GPIO_PIN_RESET);
						StepCW++;
						break;
				
				case 1:	HAL_GPIO_WritePin(A_PLUS_GPIO_Port,A_PLUS_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(A_MINUS_GPIO_Port,A_MINUS_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(B_PLUS_GPIO_Port,B_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_MINUS_GPIO_Port,B_MINUS_Pin,GPIO_PIN_RESET);
						StepCW++;
						break;
				
				case 2:	HAL_GPIO_WritePin(A_PLUS_GPIO_Port,A_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(A_MINUS_GPIO_Port,A_MINUS_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(B_PLUS_GPIO_Port,B_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_MINUS_GPIO_Port,B_MINUS_Pin,GPIO_PIN_RESET);
						StepCW++;
						break;
				
				case 3:	HAL_GPIO_WritePin(A_PLUS_GPIO_Port,A_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(A_MINUS_GPIO_Port,A_MINUS_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(B_PLUS_GPIO_Port,B_PLUS_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(B_MINUS_GPIO_Port,B_MINUS_Pin,GPIO_PIN_RESET);
						StepCW++;
						break;
				
				case 4:	HAL_GPIO_WritePin(A_PLUS_GPIO_Port,A_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(A_MINUS_GPIO_Port,A_MINUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_PLUS_GPIO_Port,B_PLUS_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(B_MINUS_GPIO_Port,B_MINUS_Pin,GPIO_PIN_RESET);
						StepCW++;
						break;
				  
				case 5:	HAL_GPIO_WritePin(A_PLUS_GPIO_Port,A_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(A_MINUS_GPIO_Port,A_MINUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_PLUS_GPIO_Port,B_PLUS_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(B_MINUS_GPIO_Port,B_MINUS_Pin,GPIO_PIN_SET);
						StepCW++;
						break;
				  
				case 6:	HAL_GPIO_WritePin(A_PLUS_GPIO_Port,A_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(A_MINUS_GPIO_Port,A_MINUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_PLUS_GPIO_Port,B_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_MINUS_GPIO_Port,B_MINUS_Pin,GPIO_PIN_SET);
						StepCW++;
						break;
				  
				case 7:	HAL_GPIO_WritePin(A_PLUS_GPIO_Port,A_PLUS_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(A_MINUS_GPIO_Port,A_MINUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_PLUS_GPIO_Port,B_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_MINUS_GPIO_Port,B_MINUS_Pin,GPIO_PIN_SET);
						StepCW++;
						break;
				  
				default:HAL_GPIO_WritePin(A_PLUS_GPIO_Port,A_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(A_MINUS_GPIO_Port,A_MINUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_PLUS_GPIO_Port,B_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_MINUS_GPIO_Port,B_MINUS_Pin,GPIO_PIN_RESET);
						StepCW = 0;
			  
		  }
	

}

/**
  * @brief  This function implements a switch statement to produce the 8 half-stepping
  * 		sequences required to produce 1 counter-clockwise step.
  * @param  none
  * @retval none
  */
void Step_Counter_Clockwise(void)										
{
	extern volatile uint16_t StepCCW;
	
			switch(StepCCW)
			{
				case 0:	HAL_GPIO_WritePin(A_PLUS_GPIO_Port,A_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(A_MINUS_GPIO_Port,A_MINUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_PLUS_GPIO_Port,B_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_MINUS_GPIO_Port,B_MINUS_Pin,GPIO_PIN_SET);
						StepCCW++;
						break;
				
				case 1:	HAL_GPIO_WritePin(A_PLUS_GPIO_Port,A_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(A_MINUS_GPIO_Port,A_MINUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_PLUS_GPIO_Port,B_PLUS_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(B_MINUS_GPIO_Port,B_MINUS_Pin,GPIO_PIN_SET);
						StepCCW++;
						break;
				
				case 2:	HAL_GPIO_WritePin(A_PLUS_GPIO_Port,A_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(A_MINUS_GPIO_Port,A_MINUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_PLUS_GPIO_Port,B_PLUS_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(B_MINUS_GPIO_Port,B_MINUS_Pin,GPIO_PIN_RESET);
						StepCCW++;
						break;
				
				case 3:	HAL_GPIO_WritePin(A_PLUS_GPIO_Port,A_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(A_MINUS_GPIO_Port,A_MINUS_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(B_PLUS_GPIO_Port,B_PLUS_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(B_MINUS_GPIO_Port,B_MINUS_Pin,GPIO_PIN_RESET);
						StepCCW++;	
						break;
				
				case 4:	HAL_GPIO_WritePin(A_PLUS_GPIO_Port,A_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(A_MINUS_GPIO_Port,A_MINUS_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(B_PLUS_GPIO_Port,B_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_MINUS_GPIO_Port,B_MINUS_Pin,GPIO_PIN_RESET);
						StepCCW++;
						break;
				  
				case 5:	HAL_GPIO_WritePin(A_PLUS_GPIO_Port,A_PLUS_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(A_MINUS_GPIO_Port,A_MINUS_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(B_PLUS_GPIO_Port,B_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_MINUS_GPIO_Port,B_MINUS_Pin,GPIO_PIN_RESET);
						StepCCW++;
						break;
				  
				case 6:	HAL_GPIO_WritePin(A_PLUS_GPIO_Port,A_PLUS_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(A_MINUS_GPIO_Port,A_MINUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_PLUS_GPIO_Port,B_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_MINUS_GPIO_Port,B_MINUS_Pin,GPIO_PIN_RESET);
						StepCCW++;
						break;
				  
				case 7:	HAL_GPIO_WritePin(A_PLUS_GPIO_Port,A_PLUS_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(A_MINUS_GPIO_Port,A_MINUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_PLUS_GPIO_Port,B_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_MINUS_GPIO_Port,B_MINUS_Pin,GPIO_PIN_SET);
						StepCCW++;
						break;
				  
				default:HAL_GPIO_WritePin(A_PLUS_GPIO_Port,A_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(A_MINUS_GPIO_Port,A_MINUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_PLUS_GPIO_Port,B_PLUS_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(B_MINUS_GPIO_Port,B_MINUS_Pin,GPIO_PIN_RESET);
						StepCCW = 0;
			  	  
		  }
}
