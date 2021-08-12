#include <stdio.h>
#include <rt_misc.h>
#include <stdint.h>
#include "stm32f4xx.h"

#pragma import(__use_no_semihosting_swi)

  
struct __FILE { int handle; };

FILE __stdout;
FILE __stdin;
FILE __stderr;
 // Define _sys_exit () to avoid using semi-host mode    
//_sys_exit(int x) 
//{ 
//    x = x; 
//}


//__use_no_semihosting was requested, but _ttywrch was 
_ttywrch(int ch)
{
    ch = ch;
}
volatile int32_t ITM_RxBuffer; 

int fputc(int ch, FILE *f) {
    ITM_SendChar(ch);
    return(ch);
}

void _sys_exit(int return_code) {
    return ;
}