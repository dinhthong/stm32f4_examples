#include <stdio.h>
#include <rt_misc.h>
#include <stdint.h>
#include "stm32f4xx.h"

#pragma import(__use_no_semihosting_swi)

struct __FILE { int handle; };

FILE __stdout;
FILE __stdin;

volatile int32_t ITM_RxBuffer; 

int fputc(int ch, FILE *f) {
    ITM_SendChar(ch);
    return(ch);
}

void _sys_exit(int return_code) {
    return ;
}