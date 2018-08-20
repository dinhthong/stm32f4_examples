#include "stm32f4xx.h"
void GPIO_Config(void);
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
__IO uint16_t CCR1_Val = 54618;
uint16_t PrescalerValue;
GPIO_InitTypeDef           GPIO_InitStructure;
int main(void)
{ 
	GPIO_Config();
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	
  PrescalerValue = (uint16_t)((SystemCoreClock / 2) / 500000) - 1;
  TIM4->ARR = 65535 ;
  TIM4->PSC = PrescalerValue;
  TIM4->EGR = TIM_PSCReloadMode_Immediate;
  /* Set the Capture Compare Register value */
  TIM4->CCR1 = CCR1_Val;
  TIM4->CCER = 0x0001;
  TIM4->CCMR1 = TIM_OCMode_PWM1 ;
  TIM4->DIER =  0x0002;
	TIM4->CR1 |= 0x0060;
	TIM4->CR1 |= TIM_CR1_CEN;
}
void GPIO_Config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4); 
}