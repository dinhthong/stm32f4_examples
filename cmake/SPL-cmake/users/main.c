#include "stm32f4xx.h"
//#include "stm32f4xx_it.h"
//#define NORMAL

/*
Sample delay functions.
*/
int i;
void Delay(int t){
	int i;
	for (i=0; i<t; i++);
}
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


int main(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;	
 // RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  RCC->AHB1ENR |= RCC_AHB1Periph_GPIOD;
  /* Configure PD12, PD13 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  while (1)
  {
	  #ifdef NORMAL
		delay_01ms(10000);
        // GPIO_SetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
		GPIOD->BSRRL = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
		delay_01ms(10000);
		//GPIO_ResetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
		GPIOD->BSRRH = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	#else
			delay_01ms(5000);
		// address for GPIOD.BSRRL = x + GPIOD_BASE  = BSRR_offset + AHB1PERIPH_BASE + 0x0C00 = BSRR_offset + 0x4000 0000 + 0x0002 0000 + 0x0C00
		// with BSRR_offset = 0x18
		// Low bits <-> Port x set bit y.
//			*(uint32_t *)Address = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
		*(uint32_t *)0x40020C18 = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
		delay_01ms(5000);
		uint32_t Address = 0x40020C00 + 0x18;
		//GPIO_ResetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
		*(uint32_t *)Address = (GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15)<<16;
	#endif
  }
}