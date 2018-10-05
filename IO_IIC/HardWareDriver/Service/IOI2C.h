/*

*/
#ifndef __IOI2C_H
#define __IOI2C_H
#include "stm32f4xx.h"
#include "sys.h"

//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n) 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n) 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)

/*
Set IIC GPIO mode. Input or Output GPIO.
*/
// reset bits GPIOB->MODER7[1:0] = GPIOB->MODER[14:15]
#define SDA_IN()  {GPIOC->MODER&=0xFFFF3FFF;GPIOC->MODER|=0x00000000;}
// set bits GPIOB->MODER7[1:0] = GPIOB->MODER[14:15] = 0b01
#define SDA_OUT() {GPIOC->MODER&=0xFFFF3FFF;GPIOC->MODER|=0x00004000;}

#define IIC_SCL    PCout(6) //SCL
#define IIC_SDA    PCout(7) //SDA	 
#define READ_SDA   PCin(7)  //read SDA 

//IIC所有操作函数
void IIC_gpio_config(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(u8 txd);			
u8 IIC_Read_Byte(unsigned char ack);
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);					//IIC ACK
void IIC_NAck(void);				//IIC not ACK

extern int16_t  I2C_Erorr_Count;

void IIC_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 IIC_Read_One_Byte(u8 daddr, u8 addr);
unsigned char I2C_Readkey(unsigned char I2C_Addr);

unsigned char I2C_ReadOneByte(unsigned char I2C_Addr, unsigned char addr);
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data);
u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8 *data);
u8 IICwriteBits(u8 dev, u8 reg, u8 bitStart, u8 length, u8 data);
u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data);
u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data);

#endif