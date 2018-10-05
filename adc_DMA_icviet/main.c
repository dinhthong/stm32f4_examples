#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
/*
Sample delay functions.
*/
int i;

void delay_us(uint32_t period){
	
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  	TIM2->PSC = 83;		// clk = SystemCoreClock /2/(PSC+1) = 1MHz
  	TIM2->ARR = period-1;
  	TIM2->CNT = 0;
  	TIM2->EGR = 1;		// update registers;
  	TIM2->SR  = 0;		// clear overflow flag
  	TIM2->CR1 = 1;		// enable Timer6
  	while (!TIM2->SR);
  	TIM6->CR2 = 0;		// stop Timer6
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
}
//delay 0.1ms
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


DMA_InitTypeDef DMA_InitStructure;
ADC_InitTypeDef ADC_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
 
volatile uint16_t ADCValue[3]={0};
 
void ADC_Config(void);
 
int main(void)
{
ADC_Config();
while (1)
{
 
}
}
 
 
void ADC_Config(void)
{
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
 
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
GPIO_Init(GPIOC, &GPIO_InitStructure);
 
DMA_InitStructure.DMA_Channel = DMA_Channel_0; 
DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADCValue;
DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));
DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
DMA_InitStructure.DMA_BufferSize = 3;
DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
DMA_InitStructure.DMA_Priority = DMA_Priority_High;
DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable; 
DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
DMA_Init(DMA2_Stream0, &DMA_InitStructure);
 
/* DMA2_Stream0 enable */
DMA_Cmd(DMA2_Stream0, ENABLE);
 
 
/* ADC Common Init **********************************************************/
ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
ADC_CommonInit(&ADC_CommonInitStructure);
 
ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
ADC_InitStructure.ADC_ScanConvMode = ENABLE;
ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
ADC_InitStructure.ADC_NbrOfConversion = 3;
ADC_Init(ADC1, &ADC_InitStructure);
 
/* ADC1 regular channels configuration */ 
ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_3Cycles);
ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_3Cycles);
ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_3Cycles);
 
/* Enable ADC1 DMA */
ADC_DMACmd(ADC1, ENABLE);
 
/* Enable DMA request after last transfer (Single-ADC mode) */
ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
 
/* Enable ADC1 */
ADC_Cmd(ADC1, ENABLE);
 
/* Start ADC1 Software Conversion */
ADC_SoftwareStartConv(ADC1);
}
 
 
void Delay(__IO uint32_t nCount)
{
while(nCount--)
{
}
}
 
 
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
/* User can add his own implementation to report the file name and line number,
ex: printf("Wrong parameters value: file %s on line %drn", file, line) */
 
while (1)
{}
}
#endif
