/* 
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
/* Functions protocol ---------------------------------------------------------*/
//chua du lieu cua 3 kenh adc
volatile uint16_t ADCValue;
void ADC_Config(void);

void DAC_Setup(void);

/* MAIN -----------------------------------------------------------------------*/
int main(void)
{
    DAC_Setup();
    ADC_Config();
    while (1)
    {
        DAC_SetChannel1Data(DAC_Align_12b_R, 145); // Mask 12 bits
        ADC_SoftwareStartConv(ADC1);//Start the conversion
        while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)) {}
        ADCValue = ADC_GetConversionValue(ADC1);
    }
}

/* Functions -------------------------------------------------------------------*/

void DAC_Setup(void)
/*  DAC channel1 with DAC_OUT1 (PA4) as output
  DAC channel2 with DAC_OUT2 (PA5) as output	*/
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
void ADC_Config(void)
{

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//cho phep clock ADC1
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//Cho phep clock PortC

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;//Cho phep chan 0,1,2 hoat dong
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//Che do Analog
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//Khong dung dien tro keo
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* ADC Common Init **********************************************************/
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//55:chon mode ADC
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//56: Chon bo chia 2
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//58: thoi gian tre giua 2 lan lay mau
    ADC_CommonInit(&ADC_CommonInitStructure);

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//61: do phan giai 12 bit
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//Khong su dung che do Convert bang tin hieu ngoai
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//65:Canh le phai
    ADC_InitStructure.ADC_NbrOfConversion = 1;//So kenh chuyen doi
    ADC_Init(ADC1, &ADC_InitStructure);

    //Config ADC1_Channel_10 -> PC0
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_3Cycles);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    /* Start ADC1 Software Conversion */
    ADC_SoftwareStartConv(ADC1);
}