
#include "stm32f4xx.h"

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
TIM_ICInitTypeDef  TIM_ICInitStructure;

void TIM_IC_Config(void);
void GPIO_TIM_PWM_Config(void);
void TIM_PWM_Config(void);

int Period_on = 100;

int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */    
  TIM_IC_Config();
	GPIO_TIM_PWM_Config();
	TIM_PWM_Config();
	TIM4->CCR1 = Period_on    ; 	//CCR1 controls channel _ 2/2
	TIM4->CCR2 = Period_on/2  ;		//CCR2 controls channel 1 _ 1/2
	TIM4->CCR3 = Period_on/4  ;		//CCR3 controls channel 1 _ 0.5/2
  TIM4->CCR4 = Period_on/8  ;		//CCR4 controls channel 1 _ 0.25/2=0.125
  while (1);
}

void TIM_IC_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* GPIOB clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  /* TIM4 chennel2 configuration : PA.07 */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* TIM4 configuration: PWM Input mode ------------------------
     The external signal is connected to TIM4 CH2 pin (PB.07), 
     The Rising edge is used as active edge,
     The TIM4 CCR2 is used to compute the frequency value 
     The TIM4 CCR1 is used to compute the duty cycle value
  ------------------------------------------------------------ */

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;

  TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);

  /* Select the TIM3 Input Trigger: TI2FP2 */
  TIM_SelectInputTrigger(TIM3, TIM_TS_TI2FP2);

  /* Select the slave Mode: Reset Mode */
  TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
  TIM_SelectMasterSlaveMode(TIM3,TIM_MasterSlaveMode_Enable);

  /* TIM enable counter */
  TIM_Cmd(TIM3, ENABLE);

  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);

  /* Connect TIM pin to AF2 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);

  /* Enable the TIM4 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}
void GPIO_TIM_PWM_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	/* Enable TIM4 Clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
}

void TIM_PWM_Config(void)
{
	
	// f(TIM4) = f(ABP1) = 84 000 000 Hz
	// Xac dinh f(PWM) mong muon. Ex: 0.5 Hz -> T=2s
		// f(PWM) = 10 Hz ->   T=100ms |  Period = 10000-1 | Prescaler = 840-1;
		// f(PWM) = 1000 Hz -> T=1ms   |  Period = 1000-1  | Prescaler = 84-1;
  /* Time base TIM 4 configuration */
  TIM_TimeBaseStructure.TIM_Period = 100-1;  //TIM4->ARR -> Auto-Reload Register
  TIM_TimeBaseStructure.TIM_Prescaler = 83; //TIM4->PSC
		
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;//TIMx->CR1[9:8]->CKD
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIMx->CR1[4]->DIR
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	//
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//TIMx_CCMR1_CC1S=0 and TIMx->CCMR1[6:4]->CC1M=0x0060
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
  /* PWM1 Mode configuration: Channel 1 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;/*TIMx_CCER_CC1E : OC1 signal is 
	output on the corresponding output pin*/
  TIM_OCInitStructure.TIM_Pulse = 0;//TIMx->CCR1 is zero to be load later
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
  /* PWM1 Mode configuration: Channel 2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
  /* PWM1 Mode configuration: Channel 3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
  /* PWM1 Mode configuration: Channel 4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
  TIM_ARRPreloadConfig(TIM4, ENABLE);   //TIM_CR1_ARPE: Auto-reload-preload enable
	  
  TIM_Cmd(TIM4, ENABLE);/* TIM4 enable counter */
}
