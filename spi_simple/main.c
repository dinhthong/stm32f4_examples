#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
/*
Sample delay functions.
*/
#define SPI_EN GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SPI_DIS  GPIO_SetBits(GPIOA, GPIO_Pin_4)
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
volatile uint8_t SPI_data_send = 0, SPI_data_get;
GPIO_InitTypeDef  GPIO_InitStructure;
SPI_InitTypeDef   SPI_InitStructure;
NVIC_InitTypeDef  NVIC_InitStructure;
void SPI_Configuration(void);

int main(void)
{
	SysTick_Config(SystemCoreClock/1000);
  SPI_Configuration();
	
  while (1)
  {
   SPI_EN;
	 while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}
	 SPI_I2S_SendData(SPI1, SPI_data_send);
	 delay_01ms(1000);
	 SPI_data_send++;
}
}
void SPI_Configuration(void)
{
  /* SPI_MASTER configuration ------------------------------------------------*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1); // SCK
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1); // MISO
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1); // MOSI
  
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
//  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);
    /* Enable SPI_MASTER */
  SPI_Cmd(SPI1, ENABLE);
	
  /* SPI_SLAVE configuration ------------------------------------------------*/ 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //see peripheral mapping table to connect between the two mcu
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_SPI2);   // SPI2_NSS
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);   // SPI2_SCK
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2);   // SPI2_MISO
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);   // SPI2_MOSI
  
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; // dummy value, because it's decided by the master
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
 // SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);
	  SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
    /* Enable SPI_SLAVE */
  SPI_Cmd(SPI2, ENABLE);
	
  NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


