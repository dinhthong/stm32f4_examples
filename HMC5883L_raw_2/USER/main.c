/*
PC9 <-> GY-86 DRY
PB6 <-> SCL
PB7 <-> SDA
This program read the RAW values from GY-86.
Using IOI2C driver ( I2C protocol running using GPIO ).
*/
#include "common.h"

#define reportHz (500)
#define uploadTime 1000000/reportHz

void Initial_System_Timer(void)
{
	RCC->APB1ENR |= 0x0008;	
	TIM5->CR1 = 0x0080; //TIMx_ARR buffered  upcounter
	TIM5->CR2 = 0x0000;
	TIM5->CNT = 0x0000;
	TIM5->ARR = 0xFFFFFFFF;
	TIM5->PSC = 84 - 1;	//1us
	TIM5->EGR = 0x0001;
	TIM5->CR1 |= 0x0001; // Enable
}

float IMU_values[9];
float cut_off=10;
double dt;
float HMC5883_data_2[3];
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
	delay_init(168);		
	delay_ms(500);
	GPIO_InitTypeDef GPIO_InitStructure;
	//I2C1 init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(GPIOC, &GPIO_InitStructure);
	delay_ms(50);
	// PA5,PA6 GPIO IIC3 init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	Initial_System_Timer();  
	HMC5883L_SetUp_2();
	delay_ms(50);
	uint32_t loop_4ms;

	while(1) {
		HMC58X3_mgetValues_2(HMC5883_data_2);	//
		delay_us(500);
		while((micros() - loop_4ms )< 4000) {};
        dt=(micros()-loop_4ms)*0.000001;
        loop_4ms= micros();
	}
}