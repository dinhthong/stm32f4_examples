#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
void delay_us(uint32_t period);
void delay_01ms(uint16_t period);
uint16_t time=0;
double distance=0;
int main(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  /* Configure PD12, PD13 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
  while (1)
  {
		//delay 1.5 second every read.
		delay_us(1500000);
    GPIO_SetBits(GPIOD,GPIO_Pin_12);
		delay_us(11); 
		GPIO_ResetBits(GPIOD,GPIO_Pin_12);
    while(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13)==(uint8_t)Bit_RESET) ;
		while (GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13)==(uint8_t)Bit_SET)
		{
			delay_us(10);
			time++;
		}
    distance=(double)time*34/100/2;//time*10us*34000/2(cm/s)
		time=0;
  }
}

int i;
void delay_us(uint32_t period) {

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
void delay_01ms(uint16_t period) {
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