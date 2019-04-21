/**
  ******************************************************************************
  * File Name          : lcd.c
  * Description        : This file provides code to initialize the LCD and
  * 					 display readings on the LCD
  ******************************************************************************
  */

 /* Includes ------------------------------------------------------------------*/
#include "lcd.h"

/* Variables ------------------------------------------------------------------*/
static char LCD_String_Buffer[50];
//Variables for Touch state
static TS_StateTypeDef  		TS_State;
Turbine_StatusTypeDef 			Turbine_Status;
Turbine_DirectionTypeDef 		Turbine_Direction;
uint16_t x, y;

/**
  * @brief  Transmits data to LCD.
  * @param  Voltage, current, power, duty cycle
  * @retval None
  */
void Display_Readings(float V_LOAD, float I_LOAD, float P_IN_Previous, uint16_t Duty_Cycle_Previous, float Position)
{
	  sprintf(LCD_String_Buffer, "      V_LOAD =   %2.3f[V]", V_LOAD);
	  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2, (uint8_t *)LCD_String_Buffer, LEFT_MODE);
	  sprintf(LCD_String_Buffer, "      I_LOAD =   %2.3f[A]",I_LOAD);
	  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 35, (uint8_t *)LCD_String_Buffer, LEFT_MODE);
	  sprintf(LCD_String_Buffer, "      Power  =   %2.3f[W]",P_IN_Previous);
	  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 65, (uint8_t *)LCD_String_Buffer, LEFT_MODE);
	  sprintf(LCD_String_Buffer, "  Duty Cycle =   %3.2f%%  ",((float)Duty_Cycle_Previous/207.0f)*100.0f);
	  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 95, (uint8_t *)LCD_String_Buffer, LEFT_MODE);
	  sprintf(LCD_String_Buffer, "    Position = %3.2f deg",Position);
	  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 125, (uint8_t *)LCD_String_Buffer, LEFT_MODE);
	  Get_Touch();
}

/**
  * @brief  Check for touch input.
  * @param  None
  * @retval None
  */
void Get_Touch(void)
{
	BSP_TS_GetState(&TS_State);
    if(TS_State.touchDetected)
		{
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    	BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
		  /* Get X and Y position of the touch input */
		  x = TS_State.touchX[0];
		  y = TS_State.touchY[0];
		  if((x >= 180 && x<=320) && (y >= 725 && y<=800))
		    {
				  if(Turbine_Status == START)
				    {
				 	    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()-65, (uint8_t *)"START", CENTER_MODE);
				 		  Turbine_Status = STOP;
				 	  }
				  else
				 	  {
				 	    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()-65, (uint8_t *)" STOP ", CENTER_MODE);
				 		  Turbine_Status = START;
				 	  }
		    }
		  else if(y >= 660 && y<=700)
			  {
				  if(x >= 150 && x<250)
					  {
						  Turbine_Direction = COUNTER_CLOCKWISE;
					  }
				  else if(270 >= 250 && x<=345)
					  {
						  Turbine_Direction = CLOCKWISE;
					  }
			  }
		 }
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

}
/**
  * @brief  Configure the LCD for display.
  * @param  None
  * @retval None
  */
void LCD_Config(void)
{
  uint32_t  lcd_status = LCD_OK;
  uint32_t	ts_status  = TS_OK;

  /* Initialize the LCD */
  lcd_status = BSP_LCD_InitEx(LCD_ORIENTATION_PORTRAIT);;
  while(lcd_status != LCD_OK);

  BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);

  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set LCD Example description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 120);
  BSP_LCD_FillRect(BSP_LCD_GetXSize()/3,BSP_LCD_GetYSize()-80,BSP_LCD_GetXSize()/3,50);
  BSP_LCD_FillRect(BSP_LCD_GetXSize()/4,BSP_LCD_GetYSize()-145,BSP_LCD_GetXSize()/4,50);
  BSP_LCD_FillRect(BSP_LCD_GetXSize()/2,BSP_LCD_GetYSize()-145,BSP_LCD_GetXSize()/4,50);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(BSP_LCD_GetXSize()/8, BSP_LCD_GetYSize()-130, (uint8_t *)"RIGHT", CENTER_MODE);
  BSP_LCD_DisplayStringAt(BSP_LCD_GetXSize()/4, BSP_LCD_GetYSize()-130, (uint8_t *)"  LEFT", LEFT_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()-65, (uint8_t *)"STOP", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"Wind Turbine Controller", CENTER_MODE);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"ELEC 391", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 75, (uint8_t *)"Group: C1", CENTER_MODE);
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetFont(&Font24);

  /* Initialize the LCD Touch Screen */
  ts_status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
  while(ts_status != TS_OK);

}
