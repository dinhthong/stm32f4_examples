#include "stm32f4xx.h"
void GPIO_Config(void);
void delay_01ms(uint16_t period);
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
__IO uint16_t CCR1_Val = 54618;
uint16_t PrescalerValue;
GPIO_InitTypeDef           GPIO_InitStructure;
uint16_t duty=0,fade=4;
int main(void)
{ 
  NVIC_InitTypeDef NVIC_InitStructure;
  /* Enable the TIM4 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	GPIO_Config();
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	
  //PrescalerValue = (uint16_ts) ((SystemCoreClock / 2) / 500000) - 1;
	
  TIM4->ARR = 400 ;
  TIM4->PSC = 21;
//		TIM4->CCR1 = 400;
  TIM4->EGR = TIM_PSCReloadMode_Immediate;
  TIM4->CCER = 0x0001;
  TIM4->CCMR1 = TIM_OCMode_PWM1 ;
	TIM4->CCMR1|=0x0008;
// TIM4->DIER =0x0002; //note thay doi cai nay bam xung lien tuc duoc. can giai thich
  TIM4->CR1 |= TIM_CR1_CEN;//  TIM_Cmd(TIM3, ENABLE);

	while(1){
    TIM4->CCR1 = duty;
		duty+=fade;
		if (duty == 400 || duty == 0) {
		    fade = -fade;
		}
		delay_01ms(500);
	}
}
void GPIO_Config(void) {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOD, &GPIO_InitStructure); 
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4); 
}
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
void TIM4_IRQHandler(void)
{

}