#include "copter_config.h"

RCC_ClocksTypeDef RCC_ClocksStruct;
GPIO_InitTypeDef GPIO_InitStruct;
I2C_InitTypeDef   I2C_InitStruct;
USART_InitTypeDef USART_InitStruct;
NVIC_InitTypeDef  NVIC_InitStruct;
EXTI_InitTypeDef  EXTI_InitStruct;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef          TIM_OCInitStruct;

void board_leds_config(void)
{
    // GPIO LED
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    /* Configure PD12, PD13 in output pushpull mode */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15 ;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

}