/******************** (C) COPYRIGHT 2015 DUT ********************************
 * 作者    ：胡文博
 * 文件名  ：IOI2C.h
 * 描述    ：模拟IIC驱动头文件
 * 日期    ：2015/11/30 12:43:38
 * 联系方式：1461318172（qq）
**********************************************************************************/

#ifndef __IOI2C_H
#define __IOI2C_H
#include "stm32f4xx.h"
#include "sys.h"

//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

//IO口地址映射
//#define GPIOB_ODR_Addr    (GPIOB_BASE+0x14) //0x40010C14 
//#define GPIOC_ODR_Addr    (GPIOC_BASE+0x14) //0x40011014 

//#define GPIOB_IDR_Addr    (GPIOB_BASE+0x10) //0x40010C10 
//#define GPIOC_IDR_Addr    (GPIOC_BASE+0x10) //0x40011010 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

//IO方向设置
#define SDA_IN()  {GPIOC->MODER&=0xFFFF3FFF;GPIOC->MODER|=0x00000000;}
#define SDA_OUT() {GPIOC->MODER&=0xFFFF3FFF;GPIOC->MODER|=0x00004000;}


//IO操作函数
#define IIC_SCL    PCout(6) //SCL
#define IIC_SDA    PCout(7) //SDA	 
#define READ_SDA   PCin(7)  //输入SDA 

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号


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


// Second I2C.

#define DELAY_IIC 2
//IO方向设置
#define SDA2_IN()  {GPIOB->MODER&=0xFF3FFFFF;GPIOB->MODER|=0x00000000;}
#define SDA2_OUT() {GPIOB->MODER&=0xFF3FFFFF;GPIOB->MODER|=0x00400000;}


//IO操作函数
#define IIC2_SCL    PBout(10) //SCL
#define IIC2_SDA    PBout(11) //SDA	 
#define READ2_SDA   PBin(11)  //输入SDA 

//IIC2所有操作函数
//void IIC2_Init(void);                //初始化IIC的IO口
void IIC2_Start(void);				//发送IIC开始信号
void IIC2_Stop(void);	  			//发送IIC停止信号
void IIC2_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC2_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC2_Wait_Ack(void); 				//IIC2等待ACK信号
void IIC2_Ack(void);					//IIC2发送ACK信号
void IIC2_NAck(void);				//IIC2不发送ACK信号

extern int16_t  I2C2_Erorr_Count;

void IIC2_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 IIC2_Read_One_Byte(u8 daddr, u8 addr);
unsigned char I2C_Readkey(unsigned char I2C_Addr);

unsigned char I2C2_ReadOneByte(unsigned char I2C_Addr, unsigned char addr);
unsigned char IIC2writeByte(unsigned char dev, unsigned char reg, unsigned char data);
u8 IIC2writeBytes(u8 dev, u8 reg, u8 length, u8 *data);
u8 IIC2writeBits(u8 dev, u8 reg, u8 bitStart, u8 length, u8 data);
u8 IIC2writeBit(u8 dev, u8 reg, u8 bitNum, u8 data);
u8 IIC2readBytes(u8 dev, u8 reg, u8 length, u8 *data);

// Third I2C.

//#define DELAY_IIC 2
// PA5,PA6
// this need correction
#define SDA3_IN()  {GPIOC->MODER&=0xFFFFFF3F;GPIOC->MODER|=0x00000000;}
#define SDA3_OUT() {GPIOC->MODER&=0xFFFFFF3F;GPIOC->MODER|=0x00000040;}


//IO操作函数
#define IIC3_SCL    PCout(1) //SCL
#define IIC3_SDA    PCout(3) //SDA	 
#define READ3_SDA   PCin(3)  //输入SDA 

//IIC2所有操作函数
//void IIC2_Init(void);                //初始化IIC的IO口
void IIC3_Start(void);				//发送IIC开始信号
void IIC3_Stop(void);	  			//发送IIC停止信号
void IIC3_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC3_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC3_Wait_Ack(void); 				//IIC2等待ACK信号
void IIC3_Ack(void);					//IIC2发送ACK信号
void IIC3_NAck(void);				//IIC2不发送ACK信号

extern int16_t  I2C3_Erorr_Count;

void IIC3_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 IIC3_Read_One_Byte(u8 daddr, u8 addr);
//unsigned char I2C_Readkey(unsigned char I2C_Addr);

unsigned char I2C3_ReadOneByte(unsigned char I2C_Addr, unsigned char addr);
unsigned char IIC3writeByte(unsigned char dev, unsigned char reg, unsigned char data);
u8 IIC3writeBytes(u8 dev, u8 reg, u8 length, u8 *data);
u8 IIC3writeBits(u8 dev, u8 reg, u8 bitStart, u8 length, u8 data);
u8 IIC3writeBit(u8 dev, u8 reg, u8 bitNum, u8 data);
u8 IIC3readBytes(u8 dev, u8 reg, u8 length, u8 *data);

#endif

//------------------End of File----------------------------
