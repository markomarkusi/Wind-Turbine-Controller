/**
  ******************************************************************************
  * @file    Wind_Turbine_Controller/Src/main.c
  * @author  Marko Markusi
  * @brief   Wind turbine controller responsible for maximum power point
  *			 tracking (MPPT) and wind direction tracking.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Variables -----------------------------------------------------------------*/
uint16_t Unfiltered_ADC_Readings[ADC_CHANNEL_COUNT * ADC_FILTER_SIZE];
float Filtered_ADC_Readings[ADC_CHANNEL_COUNT];
float VDDA,V_LOAD,I_LOAD, Position;
char LCD_String_Buffer[50];
extern volatile uint16_t Duty_Cycle_Previous;
extern volatile float P_IN_Previous;

/**
  * @brief  Main program.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the LCD */
  LCD_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM11_Init();
  MX_TIM10_Init();
  MX_TIM8_Init();
  MX_TIM7_Init();

  /* Start initialized peripherals */
  HAL_TIM_Base_Start_IT(&htim8);
  HAL_ADC_Start_DMA(&hadc1,(uint32_t*)Unfiltered_ADC_Readings,ADC_CHANNEL_COUNT * ADC_FILTER_SIZE);
  HAL_TIM_PWM_Start(&htim11,TIM_CHANNEL_1);
  HAL_TIM_Base_Start_IT(&htim10);
  HAL_TIM_Base_Start_IT(&htim7);

  /* Infinite loop */
  while (1)
  {
	  Display_Readings(V_LOAD,I_LOAD,P_IN_Previous,Duty_Cycle_Previous,Position);
  }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	  RCC_OscInitTypeDef RCC_OscInitStruct;
	  HAL_StatusTypeDef ret = HAL_OK;

	  /* Enable HSE Oscillator and activate PLL with HSE as source */
	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	  RCC_OscInitStruct.PLL.PLLM = 25;
	  RCC_OscInitStruct.PLL.PLLN = 400;
	  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	  RCC_OscInitStruct.PLL.PLLQ = 9;
	  RCC_OscInitStruct.PLL.PLLR = 7;

	  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
	  if(ret != HAL_OK)
	  {
	    while(1) { ; }
	  }

	  /* Activate the OverDrive to reach the 216 MHz Frequency */
	  ret = HAL_PWREx_EnableOverDrive();
	  if(ret != HAL_OK)
	  {
	    while(1) { ; }
	  }

	  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
	  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
	  if(ret != HAL_OK)
	  {
	    while(1) { ; }
	  }
}

/**
  * @brief Hardware triggered callback once ADC buffer array
  * is filled with 16 samples of each channel.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	Filter_ADC_Readings(ADC_CHANNEL_COUNT, ADC_FILTER_SIZE, Unfiltered_ADC_Readings, Filtered_ADC_Readings);

	/* Calculate value of analog voltage supply */
	VDDA = Calc_VDDA(Filtered_ADC_Readings[3]);

	V_LOAD = Get_V_LOAD(Filtered_ADC_Readings[0],VDDA);
	I_LOAD = Get_I_LOAD(Filtered_ADC_Readings[1],VDDA);

	Position = Get_Position(Filtered_ADC_Readings[2]);

	Stepper_Control_Loop(Position);
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
