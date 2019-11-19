/*
Created by NDT. Tuesday , March 27, 2018
Use STM Studio to watch the variables. 
Working with SRAM region only. Note the following code use the SRAM_BB_BASE
References:
1. Bit-banding knowledge.
2. STM32F4xx_DSP_StdPeriph_Lib_V1.3.0\Project\STM32F4xx_StdPeriph_Examples\CortexM
*/
#include "stm32f4xx.h"
#define  Var_ResetBit_BB(VarAddr, BitNumber)    \
          (*(__IO uint32_t *) (SRAM_BB_BASE | ((VarAddr - SRAM_BASE) << 5) | ((BitNumber) << 2)) = 0)
   
#define Var_SetBit_BB(VarAddr, BitNumber)       \
          (*(__IO uint32_t *) (SRAM_BB_BASE | ((VarAddr - SRAM_BASE) << 5) | ((BitNumber) << 2)) = 1)

#define Var_GetBit_BB(VarAddr, BitNumber)       \
          (*(__IO uint32_t *) (SRAM_BB_BASE | ((VarAddr - SRAM_BASE) << 5) | ((BitNumber) << 2)))
          

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

__IO uint32_t VarAddr;
__IO uint32_t Var;
int main(void)
{
  Var = 0x0000050;
  VarAddr = (uint32_t)&Var; 
  while (1)
  {
		delay_01ms(10000);
  Var_ResetBit_BB(VarAddr, 0);  /* Var = 0x00005AA4 */
		delay_01ms(10000);
  Var_SetBit_BB(VarAddr, 0);    /* Var = 0x00005AA5 */
  	delay_01ms(10000);
  Var_ResetBit_BB(VarAddr, 11);             /* Var = 0x000052A5 */
		delay_01ms(10000);
	Var_SetBit_BB(VarAddr, 11);             /* Var = 0x000052A5 */
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
