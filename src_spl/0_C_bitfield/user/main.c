/*
     https://www.tutorialspoint.com/cprogramming/c_bit_fields.htm
		 Have you ever wondered how do some environment allow us access the fifth bit of Port 0 (8-bit register) using syntax: P0.7
		 And how do they do it with the best data structure (minimal redudant memory)
		 
		 It's actually just an another interesting data structure technique. That heavily requires the knowledge of bit-field.
		 In case of Renesas RL78 (refer to "iodefine.h")
		 
		 In this example I just only do an simple example, for complete implementation of "that interesting data structure" you need
		 to read the Renesas's example code more :D 
*/
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#define PWM_period 20000 // micro seconds

typedef struct {
	unsigned char no0;
	unsigned char no1;
	unsigned char no2;
	unsigned char no3;
	unsigned char no4;
	unsigned char no5;
	unsigned char no6;
	unsigned char no7;
} BITS8_t;

typedef struct {
	unsigned char no0 :1;
	unsigned char no1 :1;
	unsigned char no2 :1;
	unsigned char no3 :1;
	unsigned char no4 :1;
	unsigned char no5 :1;
	unsigned char no6 :1;
	unsigned char no7 :1;
} __BITS8;

typedef struct {
	unsigned short no0 :1;
	unsigned short no1 :1;
	unsigned short no2 :1;
	unsigned short no3 :1;
	unsigned short no4 :1;
	unsigned short no5 :1;
	unsigned short no6 :1;
	unsigned short no7 :1;
	unsigned short no8 :1;
	unsigned short no9 :1;
	unsigned short no10 :1;
	unsigned short no11 :1;
	unsigned short no12 :1;
	unsigned short no13 :1;
	unsigned short no14 :1;
	unsigned short no15 :1;
} __BITS16;

// delay 0.1ms
void delay_01ms(uint16_t period) {

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    TIM6->PSC = 8399; // clk = SystemCoreClock /2 /(PSC+1) = 10KHz
    TIM6->ARR = period - 1;
    TIM6->CNT = 0;
    TIM6->EGR = 1; // update registers;
    TIM6->SR = 0;  // clear overflow flag
    TIM6->CR1 = 1; // enable Timer6
    while (!TIM6->SR)
        ;
    TIM6->CR1 = 0; // stop Timer6
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, DISABLE);
}
BITS8_t portM;
/*
		We use typedef struct __BITS8 to access each bit of an 8-bit register using register.[bit number n]
		This register can be LED turn on register
*/
__BITS8* bit8_ptr;
__BITS8 portA;
char demo_8bit_reg;
GPIO_InitTypeDef GPIO_InitStructure;
#define N 10
void init_leds_gpio(void);

int main(void) {
		demo_8bit_reg = 120;
		bit8_ptr = (__BITS8 *)&demo_8bit_reg;
		init_leds_gpio();
    printf("Hello this is the start of trace program\r\n");

    printf("sizeof(portA) = %d\n\r", sizeof(portA));
		printf("sizeof(portM) = %d\n\r", sizeof(portM));
		portA.no0 = 0;
		portA.no1 = 1;
		portA.no2 = 1;
		printf("portA.no1 = %d\n\r", (char)portA.no1);
		printf("Value of demo_8bit_reg: %d, in hex: 0x%x\n\r", demo_8bit_reg, demo_8bit_reg);
		
		bit8_ptr->no4 = 0;
		bit8_ptr->no3 = 0;
		printf("Value of demo_8bit_reg: %d, in hex: 0x%x", demo_8bit_reg, demo_8bit_reg);
    while (1) {
        delay_01ms(5000);
        GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
    }
}

void init_leds_gpio(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC->AHB1ENR |= RCC_AHB1Periph_GPIOD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}
