
typedef unsigned long uint32_t;
//#include "stm32f4xx.h"
#define FLASH_BASE 0x08000000
#define SRAM_BASE 0x20000000
#define PERIPH_BASE 0x40000000

/* Work out end of RAM address as initial stack pointer
9 * (specific of a given STM32 MCU */
#define SRAM_SIZE 128*1024 // STM32F401RE has 96 KB of RAM
#define SRAM_END (SRAM_BASE + SRAM_SIZE)

/* RCC peripheral addresses applicable to GPIOA
14 * (specific of a given STM32 MCU */
//#define RCC_BASE (PERIPH_BASE + 0x23800)


#define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region                                */

#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800)

#define RCC_AHB1Periph_GPIOD             ((uint32_t)0x00000008)

//#define RCC_APB1ENR ((uint32_t*)(RCC_BASE + 0x30))
/* GPIOA peripheral addresses
19 * (specific of a given STM32 MCU */
#define GPIOD_BASE (PERIPH_BASE + 0x20000)
#define GPIOD_MODER ((uint32_t*)(GPIOD_BASE + 0x00))
#define GPIOD_ODR ((uint32_t*)(GPIOD_BASE + 0x14))
/*
__IO uint32_t AHB1ENR; offset = 0x30
*/
#define RCC_AHB1ENR ((uint32_t*)(RCC_BASE + 0x30))
/* GPIOD */

/** 
  * @brief General Purpose I/O
  */

// typedef struct
// {
//   __IO uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
//   __IO uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
//   __IO uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
//   __IO uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
//   __IO uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
//   __IO uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
//   __IO uint16_t BSRRL;    /*!< GPIO port bit set/reset low register,  Address offset: 0x18      */
//   __IO uint16_t BSRRH;    /*!< GPIO port bit set/reset high register, Address offset: 0x1A      */
//   __IO uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
//   __IO uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
// } GPIO_TypeDef;

#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)
#define GPIOD_BASE            (AHB1PERIPH_BASE + 0x0C00)
#define GPIOD_MODER 			((uint32_t*)GPIOD_BASE+0x00)
#define GPIOD_ODR 			((uint32_t*)GPIOD_BASE+0x14)
/* User functions */
int main(void);
void delay(uint32_t count);

/* Minimal vector table */
uint32_t *vector_table[] __attribute__((section(".isr_vector"))) = {
	(uint32_t *)SRAM_END, // initial stack pointer
	(uint32_t *)main // main as Reset_Handler
};

/** @defgroup GPIO_pins_define 
  * @{
  */ 
#define GPIO_Pin_0                 (0x0001)  /* Pin 0 selected */
#define GPIO_Pin_1                 (0x0002)  /* Pin 1 selected */
#define GPIO_Pin_2                 (0x0004)  /* Pin 2 selected */
#define GPIO_Pin_3                 (0x0008)  /* Pin 3 selected */
#define GPIO_Pin_4                 (0x0010)  /* Pin 4 selected */
#define GPIO_Pin_5                 (0x0020)  /* Pin 5 selected */
#define GPIO_Pin_6                 (0x0040)  /* Pin 6 selected */
#define GPIO_Pin_7                 (0x0080)  /* Pin 7 selected */
#define GPIO_Pin_8                 (0x0100)  /* Pin 8 selected */
#define GPIO_Pin_9                 (0x0200)  /* Pin 9 selected */
#define GPIO_Pin_10                (0x0400)  /* Pin 10 selected */
#define GPIO_Pin_11                (0x0800)  /* Pin 11 selected */
#define GPIO_Pin_12                (0x1000)  /* Pin 12 selected */
#define GPIO_Pin_13                (0x2000)  /* Pin 13 selected */
#define GPIO_Pin_14                (0x4000)  /* Pin 14 selected */
#define GPIO_Pin_15                (0x8000)  /* Pin 15 selected */
#define GPIO_Pin_All               (0xFFFF)  /* All pins selected */

#define DELAY_INTERVAL 100000
int main() {
	/* Enable clock on GPIOA peripheral */
	//*RCC_APB1ENR = 0x1;
	*RCC_AHB1ENR |= RCC_AHB1Periph_GPIOD;
	/* Configure the PA5 as output pull-up */
	*GPIOD_MODER |= 0x55000000; //0x400; // Sets MODER[11:10] = 0x1

while(1) {
	*(uint32_t *)0x40020C18 = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	delay(DELAY_INTERVAL);
	*(uint32_t *)0x40020C18 = (GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15)<<16;
	delay(DELAY_INTERVAL);
}
}

void delay(uint32_t count) {
	while(count--);
}
