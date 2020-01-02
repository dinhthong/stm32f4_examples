/**
  ******************************************************************************
  * @file    TIM/TIM_6Steps/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example shows how to use TIM1 peripheral to generate 6 Steps.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup TIM_6Steps
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
 
/* Step Index */
 __IO uint32_t uwStep = 0;

/* Timer handler declaration */
TIM_HandleTypeDef TimHandle;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef sConfig;

/* Timer Break Configuration Structure declaration */
TIM_BreakDeadTimeConfigTypeDef sConfigBK;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
void pwm_set_sample_trigger_perc(const float duty);
void pwm_6step_init(void);
//#define PWM_FREQUENCY_HZ 20000
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
uint32_t m_pwm_period = 0;

float pwm_gate_c = 50.0f;
int main(void)
{
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();
  
  /* Configure the system clock to 168 MHz */
  SystemClock_Config();
  
  /* Configure LED3 */
 // BSP_LED_Init(LED3);
  pwm_6step_init();
 
  /*--------------------------------------------------------------------------*/
  hall_init();
	adc_init();
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Commutation event callback in non blocking mode 
  * @param  htim : hadc handle
  * @retval None
  */

/* Sets the time when the sample trigger is triggered.
 * Edge-aligned:      0 - 100, where 0 is positive edge.
 * Center-aligned: -100 - 100, where 0 is center of positive
 *                             waveform, and +/-100 is center of low. */
void pwm_set_sample_trigger_perc(const float duty)
{
  const uint32_t full_period = m_pwm_period - 1;
 // const uint32_t ccr = (uint32_t) (0.01f * fabsf(duty) * (float)full_period);

 // TimHandle.Instance->CCR4 = MIN(MAX(ccr, 1u), full_period);
TimHandle.Instance->CCR4 = 1;
  uint32_t polarity;
  if (duty > 0.0f) {
    polarity = TIM_OCMODE_PWM2;
  } else {
    polarity = TIM_OCMODE_PWM1;
  }
  set_oc_mode_ch4(TimHandle.Instance, polarity);

}

void HAL_TIMEx_CommutationCallback(TIM_HandleTypeDef *htim)
{
  /* Entry state */
  if (uwStep == 0)
  {
    uwStep = 1;
  }
  
  if (uwStep == 1)
  {
		pwm_set_duty_gate_c_perc(15.0f);
		pwm_ch3_pwm_afw();
		pwm_ch2_off();
		pwm_ch1_sink();
    uwStep++;
  }
  
  else if (uwStep == 2)
  {
		pwm_ch3_pwm_afw();
		pwm_ch2_sink();
		pwm_ch1_off();
    
    uwStep++;
  }
  
  else if (uwStep == 3)
  {
    pwm_set_duty_gate_a_perc(15.0f);
		pwm_ch3_off();
		pwm_ch2_sink();
		pwm_ch1_pwm_afw();
    
    uwStep++;
  }
  else if (uwStep == 4)
  {
		pwm_ch3_sink();
		pwm_ch2_off();
		pwm_ch1_pwm_afw();

    uwStep++;
  }
  
  else if (uwStep == 5)
  {
		pwm_set_duty_gate_b_perc(15.0f);
		pwm_ch3_sink();
		pwm_ch2_pwm_afw();
		pwm_ch1_off();
    uwStep++;
  }
  
  else
  {

		pwm_ch3_off();
		pwm_ch2_pwm_afw();
		pwm_ch1_sink();
    
    uwStep = 1;
  }
}  
  
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
    /* Turn LED3 on */
//    BSP_LED_On(LED3);
    while(1)
    {
    }
}

void pwm_6step_init(void) {
	 /*##-1- Configure the TIM peripheral #######################################*/
  /*----------------------------------------------------------------------------
  The STM32F4xx TIM1 peripheral offers the possibility to program in advance the 
  configuration for the next TIM1 outputs behaviour (step) and change the configuration
  of all the channels at the same time. This operation is possible when the COM 
  (commutation) event is used.
  The COM event can be generated by software by setting the COM bit in the TIM1_EGR
  register or by hardware (on TRC rising edge).
  In this example, a software COM event is generated each 1 ms: using the SysTick 
  interrupt.
  The TIM1 is configured in Timing Mode, each time a COM event occurs, a new TIM1 
  configuration will be set in advance.
  ----------------------------------------------------------------------------*/
  
  /* Initialize TIMx peripheral as follow:
       + Prescaler = 0
       + Period = 4095
       + ClockDivision = 0
       + Counter direction = Up
  */
  TimHandle.Instance = TIM1;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_CENTERALIGNED3;
  m_pwm_period = HAL_RCC_GetPCLK2Freq()/PWM_FREQUENCY_HZ - 1; /* TIM1 clock source 2*APB2 (2*90 MHz), since APB2 presc != 1 */
  TimHandle.Init.Period            = m_pwm_period;
  TimHandle.Init.Prescaler         = 0;
  TimHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  //TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;
  //TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  
  HAL_TIM_PWM_Init(&TimHandle);

  
  /*##-2- Configure the output channels ######################################*/ 
  /* Common configuration for all channels */
  sConfig.OCMode       = TIM_OCMODE_PWM1;
  sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
  sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
	sConfig.Pulse = 0;
  /* Set the pulse value for channel 1 */
  //sConfig.Pulse = 2047;  
  HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1);
  
  /* Set the pulse value for channel 2 */
 // sConfig.Pulse = 1023;
  HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_2);
  
  /* Set the pulse value for channel 3 */
 // sConfig.Pulse = 511;
  HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_3);

  HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_4); // TIM1_CH4 is used to trigger adc
	pwm_set_sample_trigger_perc(0.0f);
  /*##-3- Configure the Break stage ##########################################*/
  sConfigBK.OffStateRunMode  = TIM_OSSR_DISABLE;
  sConfigBK.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sConfigBK.LockLevel        = TIM_LOCKLEVEL_1;
  sConfigBK.BreakState       = TIM_BREAK_ENABLE;
  sConfigBK.BreakPolarity    = TIM_BREAKPOLARITY_HIGH;
  sConfigBK.AutomaticOutput  = TIM_AUTOMATICOUTPUT_DISABLE;
  sConfigBK.DeadTime         = 0;
  
  HAL_TIMEx_ConfigBreakDeadTime(&TimHandle, &sConfigBK);
  /* Enable interrupt at a break event */
  __HAL_TIM_ENABLE_IT(&TimHandle, TIM_IT_BREAK);
  NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, 0);
  NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
  
  /*##-4- Configure the commutation event: software event ####################*/
  HAL_TIMEx_ConfigCommutationEvent_IT(&TimHandle, TIM_TS_NONE, TIM_COMMUTATION_SOFTWARE);
  
  /*##-5- Start signals generation ###########################################*/ 
  /*--------------------------------------------------------------------------*/
  /* Start channel 1 */
  HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1);
 
  /* Start channel 1N */
	HAL_TIMEx_OCN_Start(&TimHandle, TIM_CHANNEL_1);
 
  /*--------------------------------------------------------------------------*/
  
  
  /*--------------------------------------------------------------------------*/
  /* Start channel 2 */
  HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_2);
  /* Start channel 2N */
  HAL_TIMEx_OCN_Start(&TimHandle, TIM_CHANNEL_2);

  /*--------------------------------------------------------------------------*/
  
  
  /*--------------------------------------------------------------------------*/
  /* Start channel 3 */
  HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_3);
  /* Start channel 3N */
  HAL_TIMEx_OCN_Start(&TimHandle, TIM_CHANNEL_3);
	
	HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_4);
	pwm_ch1_off();
	pwm_ch2_off();
	pwm_ch3_off();
}
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
  if (HAL_GetREVID() == 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
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
