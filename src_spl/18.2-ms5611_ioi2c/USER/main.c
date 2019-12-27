/*
*/
#include "main.h"
void get_Baro(void);

float LPF(float x, float pre_value, float CUTOFF,float dt);
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

// LPF
float cut_off=10;
/*Baro */
double referencePressure;
long realPressure;
float fil_comple_alt, fil_lpf_alt, EstAlt;
float absoluteAltitude, ms5611_temperature;
double dt;
uint32_t loop_var;

int main(void) {
    delay_init(168);
    delay_ms(200);
    board_leds_config();
    Initial_System_Timer();  
	
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    // I2C
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_Init(GPIOD, &GPIO_InitStructure);

	  delay_ms(20);
	       begin();
			   delay_ms(100);
        referencePressure = readPressure(0);
	      delay_ms(100);
        loop_var = micros();
        while(1) {
            GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
            get_Baro();
            while ((micros() - loop_var )< 4000) {
		        };
			 
            dt=(micros()-loop_var)*0.000001;
            loop_var = micros();
    }
}

void get_Baro(void){
	  
		update_baro(&ms5611_temperature, &realPressure, &absoluteAltitude);
    fil_lpf_alt = LPF(absoluteAltitude, fil_lpf_alt, cut_off, dt);
    fil_comple_alt = fil_comple_alt*0.977f + fil_lpf_alt*0.023f;
	  EstAlt = fil_comple_alt*10;
}

float LPF(float x, float pre_value, float CUTOFF,float dt)
{
    float RC, alpha, y;
    RC = 1.0f/(CUTOFF*2*3.1416f);
    alpha = dt/(RC+dt);
    y = pre_value + alpha * ( x - pre_value );
    return y;
}
