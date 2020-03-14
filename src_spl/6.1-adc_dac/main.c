/* 
Based on Embedded System Control Design class

PC0 -> ADC reader.
PA4 -> Digital to Analog
*/
#include "stm32f4xx.h"

/* Private typedef -----------------------------------------------------------*/
DAC_InitTypeDef DAC_InitStructure;
GPIO_InitTypeDef GPIO_InitStruct;
ADC_InitTypeDef ADC_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStructure;
GPIO_InitTypeDef GPIO_InitStructure;

// 0.1ms delay
void delay_01ms(uint16_t period){
	
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
  	TIM6->PSC = 8399;		// clk = SystemCoreClock /2 /(PSC+1) = 10KHz
  	TIM6->ARR = period-1;
  	TIM6->CNT = 0;
  	TIM6->EGR = 1;		// update registers;
  	TIM6->SR  = 0;		// clear overflow flag
  	TIM6->CR1 = 1;		// enable Timer6
  	while (!TIM6->SR);
  	TIM6->CR1 = 0;		// stop Timer6
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, DISABLE);
}

/* Functions protocol ---------------------------------------------------------*/
void ADC_Config(void);
void DAC_Setup(void);

uint16_t dac_val, adc_value;
/* MAIN -----------------------------------------------------------------------*/
int main(void)
{
    DAC_Setup();
    ADC_Config();
    while (1)
    {
			dac_val++;
			if (dac_val==4095) dac_val = 0;
      DAC_SetChannel1Data(DAC_Align_12b_R, dac_val); // Mask 12 bits
      ADC_SoftwareStartConv(ADC1); //Start the conversion
      while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)) {}
      adc_value = ADC_GetConversionValue(ADC1);
			delay_01ms(10);
    }
}
/*
	DAC channel1 with DAC_OUT1 (PA4) as output
	DAC channel2 with DAC_OUT2 (PA5) as output	
*/

void DAC_Setup(void)

{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* GPIOA clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);		/* DAC Periph clock enable */

    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_4;			 	// Select DAC channel 1
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AN;			// Select MODE Analog
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;	// No push pull
    GPIO_Init(GPIOA, &GPIO_InitStruct);							// Init

    DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;								// No trigger
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None; // No WaveGeneration
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;		// Enable buffer
    DAC_Init(DAC_Channel_1, &DAC_InitStructure); 										// Init

    DAC_Cmd(DAC_Channel_1, ENABLE);		/* Enable DAC Channel1 */
}
/*
	PC0
*/
void ADC_Config(void)
{

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* ADC Common Init **********************************************************/
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//select ADC mode
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//56: Chon bo chia 2
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//58: thoi gian tre giua 2 lan lay mau
    ADC_CommonInit(&ADC_CommonInitStructure);

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//No Convert Triggering using external signal
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;// Data right alignment
    ADC_InitStructure.ADC_NbrOfConversion = 1;// Number of conversions
    ADC_Init(ADC1, &ADC_InitStructure);

    //Config ADC1_Channel_10 -> PC0
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_3Cycles);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    /* Start ADC1 Software Conversion */
    ADC_SoftwareStartConv(ADC1);
}