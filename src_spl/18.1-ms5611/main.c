#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "MPU6050.h"
#include <Math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ms5611.h"
#define UPDATE_INTERVAL 25000
#define I2C_SPEED 100000
#define I2C_DUTYCYCLE I2C_DutyCycle_2
void Delay(__IO uint32_t nCount);
void I2C_ms5611(void);

void _delay_us(uint32_t us);
void _delay_ms(uint32_t ms);

static float low_pass_filter(float vNew,float vPrev,float factor);
float LPF(float x,float pre_value, float CUTOFF);


int8_t start=0;
uint32_t mili(void);
uint32_t micros(void);
int16_t dem_calib;
float cut_off=10;
int dem_timer,dem_time;
uint32_t loop_time,timer_land, loop_4ms;
double dt;
double referencePressure;
float ms5611_temperature;
long realPressure;
float ms5611_alt_temp,fil_comple_alt,fil_lpf_alt ;
float ms5611_alt_filter,abs_Alt_baro,Alt_baro_cal,ms5611_absolute_alt;
GPIO_InitTypeDef  GPIO_InitStructure;
int main(void)
{
    SysTick_Config(168);
    I2C_ms5611();
    ms5611_begin();
    referencePressure = readPressure(0);
    _delay_us(500000);
		/* GPIOD Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    /* Configure PD12, PD13 in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    for (dem_calib = 0; dem_calib < 1000; dem_calib++)
    {
        ms5611_getupdate(&ms5611_temperature,&realPressure,&ms5611_alt_temp);
        fil_lpf_alt =LPF(ms5611_alt_temp,fil_lpf_alt,cut_off);
        fil_comple_alt =fil_comple_alt*(0.977)+fil_lpf_alt*(0.023);
        ms5611_alt_filter =fil_comple_alt*100; //final result
			if (dem_calib>=200) {
			  Alt_baro_cal+=ms5611_alt_filter;
			}
		GPIO_ToggleBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
        while((tick_count - loop_4ms )< 4000) {};
        dt=(tick_count-loop_4ms)*0.000001;
        loop_4ms= tick_count;
    }
    
		Alt_baro_cal=Alt_baro_cal/800.0;
    while (1)
    {
			
	      ms5611_getupdate(&ms5611_temperature,&realPressure,&ms5611_alt_temp);
        fil_lpf_alt =LPF(ms5611_alt_temp,fil_lpf_alt,cut_off);
        fil_comple_alt =fil_comple_alt*(0.977)+fil_lpf_alt*(0.023);
        ms5611_alt_filter =fil_comple_alt*100; //final result
        ms5611_absolute_alt= ms5611_alt_filter-Alt_baro_cal;
        while((tick_count - loop_4ms )< 4000) {};
        dt=(tick_count-loop_4ms)*0.000001;
        loop_4ms= tick_count;
    }
}

void I2C_ms5611(void)
{
    I2C_InitTypeDef   I2C_InitStructure;

		GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  // enable pull up resistors
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_I2C1);   // only connect to
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_I2C1);   // only connect toz

    I2C_DeInit(I2C1);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_ClockSpeed = 100000;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &I2C_InitStructure);
    /* I2C ENABLE */
    I2C_Cmd(I2C1, ENABLE);
}

void _delay_us(uint32_t us)
{
    uint32_t now = micros();
    while (micros() - now < us);
}

uint32_t mili(void)
{
    float ms;
    ms = tick_count*0.001;
    return ms;
}

void _delay_ms(uint32_t ms)
{
    uint32_t now = mili();
    while (mili() - now < ms);
}
uint32_t micros(void)
{
    float us;
    us = tick_count;
    return us;
}
void Delay(__IO uint32_t nCount)
{
    while(nCount--)
    {
    }
}
float low_pass_filter(float vNew,float vPrev,float factor)
{
    return (vPrev*factor + vNew) / ( 1 + factor);
}

float LPF(float x,float pre_value, float CUTOFF)
{
    float RC, alpha, y;
    RC = 1.0/(CUTOFF*2*3.1416);
    //dt =SAMPLE_RATE;
    alpha = dt/(RC+dt);
    y = pre_value + alpha * ( x - pre_value );
    return y;
}
