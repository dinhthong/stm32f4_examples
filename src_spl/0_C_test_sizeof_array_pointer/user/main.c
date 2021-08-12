/*
                Compare between pointer to actual array and pointer to an array
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
#define N 10
void init_leds_gpio(void);
int main(void) {

    int int_arr[N];
    int *ptr_arr, *ptr;
    int i, sum = 0;

    init_leds_gpio();

    printf("Hello this is the start of trace program\r\n");

    /*
        Fill buffer with values
    */
    for (i = 0; i < N; ++i) {
        int_arr[i] = i + 10;
    }
    ptr_arr = int_arr; // ptr_arr - &a[0];
    /*
        Array element can be accessed by both ways
    */
    for (i = 0; i < N; ++i) {
        printf("By *(ptr_arr+i): i = %d, int_arr[i] = %d\n", i, *(ptr_arr + i));
    }
    for (i = 0; i < N; ++i) {
        printf("By int_arr[i]: i = %d, int_arr[i] = %d\n", i, int_arr[i]);
    }

    /*
        But if i exceeds the array value, this compiler supports it
    */
    for (i = 0; i < N + 1; ++i) {
        printf("By *(ptr_arr+i): i = %d, int_arr[i] = %d\n", i, *(ptr_arr + i));
    }
    for (i = 0; i < N + 1; ++i) {
        printf("By int_arr[i]: i = %d, int_arr[i] = %d\n", i, int_arr[i]);
    }
    /*
    The output: 40, 4.
    */

    // Dynamically allocate memory using malloc()
    ptr = (int *)malloc(5 * sizeof(int));

    // Check if the memory has been successfully
    // allocated by malloc or not
    if (ptr == NULL) {
        printf("Memory not allocated.\n");
        return -1;
    } else {
        // Memory has been successfully allocated
        printf("Memory successfully allocated using malloc.\n");
        // Get the elements of the array
        for (i = 0; i < 5; ++i) {
            ptr[i] = i + 100;
        }
        // Print the elements of the array
        printf("The elements of the array are: ");
        for (i = 0; i < 5; ++i) {
            printf("address: %x, value: %d\n\r, ", ptr + i, ptr[i]);
        }
        printf("\n\rEnd of malloc program\n\r");
    }

    printf("sizeof(int_arr) = %d, sizeof(ptr_arr) = %d, sizeof(ptr) = %d\n", sizeof(int_arr),
           sizeof(ptr_arr), sizeof(ptr));
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
