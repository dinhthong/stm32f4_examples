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
GPIO_InitTypeDef GPIO_InitStructure;

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

typedef struct {
	uint32_t no0 :1;
	uint32_t no1 :1;
	uint32_t no2 :1;
	uint32_t no3 :1;
	uint32_t no4 :1;
	uint32_t no5 :1;
	uint32_t no6 :1;
	uint32_t no7 :1;
	uint32_t no8 :1;
	uint32_t no9 :1;
	uint32_t no10 :1;
	uint32_t no11 :1;
	uint32_t no12 :1;
	uint32_t no13 :1;
	uint32_t no14 :1;
	uint32_t no15 :1;
	uint32_t no16 :1;
	uint32_t no17 :1;
	uint32_t no18 :1;
	uint32_t no19 :1;
	uint32_t no20 :1;
	uint32_t no21 :1;
	uint32_t no22 :1;
	uint32_t no23 :1;
	uint32_t no24 :1;
	uint32_t no25 :1;
	uint32_t no26 :1;
	uint32_t no27 :1;
	uint32_t no28 :1;
	uint32_t no29 :1;
	uint32_t no30 :1;
	uint32_t no31 :1;
} __BITS32;

union gpio_reg {
		uint32_t reg;
		__BITS32 BIT;
};

BITS8_t portM;
/*
		We use typedef struct __BITS8 to access each bit of an 8-bit register using register.[bit number n]
		This register can be LED turn on register
*/
__BITS8 portA;
char demo_8bit_reg;

#define _8B_REG   (volatile union __BITS8 *)&demo_8bit_reg // Couldn't access using this though

/*
	Example 2:
		- We don't need to use pointer anymore, directly define the MACRO that can be used for accessing individual bits of register
*/
#define GPIOD_BSRR (*(volatile union gpio_reg *)0x40020C18).reg // access using register name (32bit access)
#define GPIOD_BSRR_bit (*(volatile union gpio_reg *)0x40020C18).BIT // access individual bit

void init_leds_gpio(void);

int main(void) {
		__BITS8* bit8_ptr = (__BITS8 *)&demo_8bit_reg;
		init_leds_gpio();
		demo_8bit_reg = 120;
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
		//*(_8B_REG).no1 = 1;
		
    while (1) {
			GPIOD_BSRR_bit.no12 = 1;
			GPIOD_BSRR_bit.no13 = 1;
      delay_01ms(2000);
			GPIOD_BSRR_bit.no28 = 1;
			GPIOD_BSRR_bit.no29 = 1;
			delay_01ms(2000);
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
