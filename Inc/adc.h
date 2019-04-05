/**
  ******************************************************************************
  * File Name          : ADC.h
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __adc_H
#define __adc_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Defines -------------------------------------------------------------------*/
#define VREFIN_CAL_ADDR 			((uint16_t*)(uint32_t)0x1FF0F44A)
#define TS_CAL1_ADDR 				((uint16_t*)(uint32_t)0x1FF0F44C)
#define TS_CAL2_ADDR 				((uint16_t*)(uint32_t)0x1FF0F44E)
#define TS_CAL1_TEMP				30
#define TS_CAL2_TEMP				110
#define	ADC_12_BIT_FULL_SCALE		4095
#define ADC_CHANNEL_COUNT	   		4
#define ADC_FILTER_SIZE		 		16	// Oversampling filter size (number of readings)
#define DEGREES_PER_ROTATION		300.0f	//0° -> 300° rotation angle
/* Ammeter and Voltmeter both have non-constant linear gain */
/* Gain function coefficients */
#define AMMETER_CONSTANT_COEFF		0.1752f
#define AMMETER_LINEAR_COEFF		0.0076f
#define VOLTMETER_CONSTANT_COEFF	5.9342f
#define VOLTMETER_LINEAR_COEFF		0.0202f
/* Typedefs ------------------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;

/* Prototypes ----------------------------------------------------------------*/
void MX_ADC1_Init(void);
float Calc_VDDA(float VREFINT_DATA);
float Calc_Temperature(uint16_t Temp_Sensor_Voltage);
float Convert_ADC_Voltage(float ADC_Data, float Vdd);
void Filter_ADC_Readings(uint32_t ChannelCount, uint32_t FilterSize, volatile uint16_t *ADC_Readings, float *Filtered_ADC_Readings);
float Get_V_LOAD(float Filtered_ADC_Data, float Vdd);
float Get_I_LOAD(float Filtered_ADC_Data, float Vdd);
float Get_Position(float Filtered_ADC_Data);
#ifdef __cplusplus
}
#endif
#endif /*__ adc_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
