#include "main.h"
#include <stdio.h> 
#include <stdlib.h> 
#define PWM_period 20000 //micro seconds
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
void test_address_stack();
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
GPIO_InitTypeDef GPIO_InitStructure;

int* ptr;
uint16_t while_cnt;
int main(void)
{
	   
    int i, sum = 0;
		//int a[10];
		char a[] = "helloworld";
		char b[10] = "monday";
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC->AHB1ENR |= RCC_AHB1Periph_GPIOD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
	  PWM_Config();
    //"on the fly" update CCR registers. -> use for quadcopter
    //This is sent to initialize the motors
    printf("Hello this is the start of trace program\r\n");

    // Dynamically allocate memory using malloc() 
    ptr = (int*)malloc(5 * sizeof(int)); 
  
    // Check if the memory has been successfully 
    // allocated by malloc or not 
		printf("Size of ptr: %d\n", sizeof(ptr)); 
		printf("size a = %d, size b = %d, \n", sizeof(a), sizeof(b));
    if (ptr == NULL) { 
        printf("Memory not allocated.\n"); 
        return -1;
    } 
    else { 
        // Memory has been successfully allocated 
        printf("Memory successfully allocated using malloc.\n"); 
        // Get the elements of the array 
        for (i = 0; i < 5; ++i) { 
            ptr[i] = i + 100; 
        } 
        // Print the elements of the array 
        printf("The elements of the array are: "); 
        for (i = 0; i < 5; ++i) { 
            printf("address: 0x%x, value: %d\n\r, ",ptr+i, ptr[i]); 
        } 
				printf("\n\rEnd of malloc program\n\r");
    }
		test_address_stack();

    while(1) {
			while_cnt++;
			delay_01ms(5000);
			printf("Hello this is tracing %d\r\n", while_cnt);
			GPIO_ToggleBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	}
}

void test_address_stack() {
		char stack_var[10];
	uint8_t i;
		for (i=0; i<10; i++) {
			printf("address stack var: i = %d, address = 0x%x, value: %d\n\r, ",i, &stack_var[i]); 
		}
}
 void PWM_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_9 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);
 
    TIM_TimeBaseStructure.TIM_Prescaler = 168-1; // 1 tick = 1us
    TIM_TimeBaseStructure.TIM_Period = PWM_period-1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0;

    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_OC4Init(TIM1, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM1, ENABLE);

    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
