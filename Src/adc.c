/**
  ******************************************************************************
  * File Name          : ADC.c
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/**
  * @brief  This function calculates the value of the analog voltage supply .
  * @param  Filtered voltage reference calibration data
  * @retval Analog voltage supply value: VDDA
  */
float Calc_VDDA(float VREFINT_DATA)
{
	float VDDA = 3.3f * (*VREFIN_CAL_ADDR)/VREFINT_DATA;
	return VDDA;
}

/**
  * @brief  This function calculates the internal temperature of the microcontroller .
  * @param  Temperature sensor data
  * @retval Internal temperature of the microcontroller in degree C
  */
float Calc_Temperature(uint16_t Temp_Sensor_Voltage)
{
	float temperature;
	temperature = (float)(TS_CAL2_TEMP - TS_CAL1_TEMP )/(*TS_CAL2_ADDR - *TS_CAL1_ADDR)
		*(Temp_Sensor_Voltage - *TS_CAL1_ADDR)
		+ TS_CAL1_TEMP;
	return temperature;
}

/**
  * @brief  This function converts raw ADC data to voltages.
  * @param  Filtered ADC data, VDDA
  * @retval Voltage from 0.0V -> VDDA
  */
float Convert_ADC_Voltage(float ADC_Data, float Vdd)
{
	float voltage = (Vdd/ADC_12_BIT_FULL_SCALE)*ADC_Data;
	return voltage;
}

/**
  * @brief  This function converts filtered filtered ADC data to voltage.
  * @param  Filtered ADC data
  * @retval Voltage at the load: V_LOAD
  */
float Get_V_LOAD(float Filtered_ADC_Data, float Vdd)
{
	float Voltage = Convert_ADC_Voltage(Filtered_ADC_Data, Vdd);
	return (VOLTMETER_LINEAR_COEFF*Voltage + VOLTMETER_CONSTANT_COEFF) * Voltage;
}

/**
  * @brief  This function converts filtered filtered ADC data to current.
  * @param  Filtered ADC data
  * @retval Current at the load: I_LOAD
  */
float Get_I_LOAD(float Filtered_ADC_Data, float Vdd)
{
	float Current = Convert_ADC_Voltage(Filtered_ADC_Data, Vdd);
	return (AMMETER_LINEAR_COEFF*Current + AMMETER_CONSTANT_COEFF)*Current;
}

/**
  * @brief  This function converts filtered filtered ADC data to position.
  * @param  Filtered ADC data
  * @retval Position of the wind direction sensor
  */
float Get_Position(float Filtered_ADC_Data)
{
	return Filtered_ADC_Data * (DEGREES_PER_ROTATION/ADC_12_BIT_FULL_SCALE);
}

/**
  * @brief  Filters ADC readings by computing the mean of the previous ADC_FILTER_SIZE readings (oversampling filter)
  * @param  ChannelCount Number of ADC channels used
  * @param  FilterSize Oversampling filter size (number of readings)
  * @param  ADC_Readings Pointer to array containing the unfiltered ADC readings
  * @param  Filtered_ADC_Readings Pointer to array in which the filtered ADC readings will be stored
  * @retval None
 */
void Filter_ADC_Readings(uint32_t ChannelCount, uint32_t FilterSize, volatile uint16_t *ADC_Readings, float *Filtered_ADC_Readings)
{
	int i;
	int j;

	// Reset filtered ADC readings array to 0
	for(i = 0; i < ChannelCount; i++)
	{
		Filtered_ADC_Readings[i] = 0;
	}

	// Calculate sum of previous ADC_FILTER_SIZE readings
	for(i = 0; i < FilterSize; i++)
	{
		for(j = 0; j < ChannelCount; j++)
		{
			Filtered_ADC_Readings[j] += ADC_Readings[(ChannelCount * i) + j];
		}
	}

	// Calculate average of previous ADC_FILTER_SIZE readings
	for(i = 0; i < ChannelCount; i++)
	{
		Filtered_ADC_Readings[i] = Filtered_ADC_Readings[i] / FilterSize;
	}
}

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T8_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 4;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_VREFINT;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();
  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration    
    PC2     ------> ADC1_IN12
    PA4     ------> ADC1_IN4
    PA6     ------> ADC1_IN6 
    */
    GPIO_InitStruct.Pin = ARD_A2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ARD_A2_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ARD_A1_Pin|ARD_A0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ADC1 DMA Init */
    /* ADC1 Init */
    hdma_adc1.Instance = DMA2_Stream4;
    hdma_adc1.Init.Channel = DMA_CHANNEL_0;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
    hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc1);

  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();
  
    /**ADC1 GPIO Configuration    
    PC2     ------> ADC1_IN12
    PA4     ------> ADC1_IN4
    PA6     ------> ADC1_IN6 
    */
    HAL_GPIO_DeInit(ARD_A2_GPIO_Port, ARD_A2_Pin);

    HAL_GPIO_DeInit(GPIOA, ARD_A1_Pin|ARD_A0_Pin);

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);
  }
} 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
