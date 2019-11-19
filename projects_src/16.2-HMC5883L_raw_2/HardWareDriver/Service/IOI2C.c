/******************** (C) COPYRIGHT 2015 DUT ********************************
  *1. I2C 接口 PB6  PB7 引脚
********************************************************************************
 */
#include "IOI2C.h"
#include "delay.h"


int16_t  I2C_Erorr_Count;
int16_t  I2C2_Erorr_Count;
int16_t  I2C3_Erorr_Count;
/**************************实现函数********************************************

*******************************************************************************/
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	//配置PB6 PB7 为开漏输出  刷新频率为50Mhz
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//应用配置到GPIOB
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**************************实现函数********************************************

*******************************************************************************/
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA = 1;
	IIC_SCL = 1;
	delay_us(2);
	IIC_SDA = 0; //START:when CLK is high,DATA change form high to low
	delay_us(2);
	IIC_SCL = 0; //钳住I2C总线，准备发送或接收数据
}

/**************************实现函数********************************************

*******************************************************************************/
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL = 0;
	IIC_SDA = 0; //STOP:when CLK is high DATA change form low to high
	delay_us(2);
	IIC_SCL = 1;
	IIC_SDA = 1; //发送I2C总线结束信号
	delay_us(2);
}


u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA_IN();      //SDA设置为输入
	IIC_SDA = 1;
	delay_us(1);
	IIC_SCL = 1;
	delay_us(1);
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime > 50)
		{
			IIC_Stop();
			I2C_Erorr_Count ++;	 //I2C 错误
			return 1;
		}
		delay_us(1);
	}
	IIC_SCL = 0; //时钟输出0
	return 0;
}


void IIC_Ack(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 0;
	delay_us(1);
	IIC_SCL = 1;
	delay_us(1);
	IIC_SCL = 0;
}


void IIC_NAck(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 1;
	delay_us(1);
	IIC_SCL = 1;
	delay_us(1);
	IIC_SCL = 0;
}


void IIC_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	IIC_SCL = 0; //拉低时钟开始数据传输
	for(t = 0; t < 8; t++)
	{
		IIC_SDA = (txd & 0x80) >> 7;
		txd <<= 1;
		delay_us(1);
		IIC_SCL = 1;
		delay_us(1);
		IIC_SCL = 0;
		delay_us(1);
	}
}

/**************************实现函数********************************************

*******************************************************************************/
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	SDA_IN();//SDA设置为输入
	for(i = 0; i < 8; i++ )
	{
		IIC_SCL = 0;
		delay_us(1);
		IIC_SCL = 1;
		receive <<= 1;
		if(READ_SDA)receive++;
		delay_us(1);
	}
	if (ack)
		IIC_Ack(); //发送ACK
	else
		IIC_NAck();//发送nACK
	return receive;
}


/**************************实现函数********************************************

*******************************************************************************/
unsigned char I2C_ReadOneByte(unsigned char I2C_Addr, unsigned char addr)
{
	unsigned char res = 0;

	IIC_Start();
	IIC_Send_Byte(I2C_Addr);	   //发送写命令
	res++;
	IIC_Wait_Ack();
	IIC_Send_Byte(addr);
	res++;  //发送地址
	IIC_Wait_Ack();
	//IIC_Stop();//产生一个停止条件
	IIC_Start();
	IIC_Send_Byte(I2C_Addr + 1);
	res++;          //进入接收模式
	IIC_Wait_Ack();
	res = IIC_Read_Byte(0);
	IIC_Stop();//产生一个停止条件

	return res;
}


/**************************实现函数********************************************

*******************************************************************************/
u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data)
{
	u8 count = 0;

	IIC_Start();
	IIC_Send_Byte(dev);	   // Send address with Transmitter mode
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(dev + 1); //稴end address with Receiver mode
	IIC_Wait_Ack();

	for(count = 0; count < length; count++)
	{

		if(count != length - 1)data[count] = IIC_Read_Byte(1); //磖ead byte with ACK
		else  data[count] = IIC_Read_Byte(0);	 //最后一个字节NACK
	}
	IIC_Stop();//产生一个停止条件
	return count;
}

/**************************实现函数********************************************

*******************************************************************************/
u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8 *data)
{

	u8 count = 0;
	IIC_Start();
	IIC_Send_Byte(dev);	   //发送写命令
	IIC_Wait_Ack();
	//for MS5611
	if (reg!=0)
	{
	IIC_Send_Byte(reg);   //发送地址
	IIC_Wait_Ack();
	}
	for(count = 0; count < length; count++)
	{
		IIC_Send_Byte(data[count]);
		IIC_Wait_Ack();
	}
	IIC_Stop();//产生一个停止条件

	return 1; //status == 0;
}

/**************************实现函数********************************************

*******************************************************************************/
u8 IICreadByte(u8 dev, u8 reg, u8 *data)
{
	*data = I2C_ReadOneByte(dev, reg);
	return 1;
}

/**************************实现函数********************************************

*******************************************************************************/
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data)
{
	return IICwriteBytes(dev, reg, 1, &data);
}

u8 IICwriteBits(u8 dev, u8 reg, u8 bitStart, u8 length, u8 data)
{

	u8 b;
	if (IICreadByte(dev, reg, &b) != 0)
	{
		u8 mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
		data <<= (8 - length);
		data >>= (7 - bitStart);
		b &= mask;
		b |= data;
		return IICwriteByte(dev, reg, b);
	}
	else
	{
		return 0;
	}
}

/********************************************************************

*******************************************************************************/
u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
{
	u8 b;
	IICreadByte(dev, reg, &b);
	b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
	return IICwriteByte(dev, reg, b);
}

//------------------End of File----------------------------



void IIC2_Start(void)
{
	SDA2_OUT();     //sda线输出
	IIC2_SDA = 1;
	IIC2_SCL = 1;
	delay_us(1);
	IIC2_SDA = 0; //START:when CLK is high,DATA change form high to low
	delay_us(1);
	IIC2_SCL = 0; //钳住I2C总线，准备发送或接收数据
}

/**************************实现函数********************************************

*******************************************************************************/
void IIC2_Stop(void)
{
	SDA2_OUT();//sda线输出
	IIC2_SCL = 0;
	IIC2_SDA = 0; //STOP:when CLK is high DATA change form low to high
	delay_us(1);
	IIC2_SCL = 1;
	delay_us(1);
	IIC2_SDA = 1; //发送I2C总线结束信号
	delay_us(1);
}


u8 IIC2_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA2_IN();      //SDA设置为输入
	IIC2_SDA = 1;
	delay_us(1);
	IIC2_SCL = 1;
	delay_us(1);
	while(READ2_SDA)
	{
		ucErrTime++;
		if(ucErrTime > 50)
		{
			IIC2_Stop();
			I2C2_Erorr_Count ++;	 //I2C 错误
			return 1;
		}
		delay_us(2);
	}
	IIC2_SCL = 0; //时钟输出0
	return 0;
}


void IIC2_Ack(void)
{
	IIC2_SCL = 0;
	SDA2_OUT();
	IIC2_SDA = 0;
	delay_us(1);
	IIC2_SCL = 1;
	delay_us(1);
	IIC2_SCL = 0;
}


void IIC2_NAck(void)
{
	IIC2_SCL = 0;
	SDA2_OUT();
	IIC2_SDA = 1;
	delay_us(1);
	IIC2_SCL = 1;
	delay_us(1);
	IIC2_SCL = 0;
}


void IIC2_Send_Byte(u8 txd)
{
	u8 t;
	SDA2_OUT();
	IIC2_SCL = 0; //拉低时钟开始数据传输
	for(t = 0; t < 8; t++)
	{
		IIC2_SDA = (txd & 0x80) >> 7;
		txd <<= 1;
		delay_us(1);
		IIC2_SCL = 1;
		delay_us(1);
		IIC2_SCL = 0;
		delay_us(1);
	}
}

/**************************实现函数********************************************

*******************************************************************************/
u8 IIC2_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	SDA2_IN();//SDA设置为输入
	for(i = 0; i < 8; i++ )
	{
		IIC2_SCL = 0;
		delay_us(2);
		IIC2_SCL = 1;
		receive <<= 1;
		if(READ2_SDA)receive++;
		delay_us(1);
	}
	if (ack)
		IIC2_Ack(); //发送ACK
	else
		IIC2_NAck();//发送nACK
	return receive;
}


/**************************实现函数********************************************

*******************************************************************************/
unsigned char I2C2_ReadOneByte(unsigned char I2C_Addr, unsigned char addr)
{
	unsigned char res = 0;

	IIC2_Start();
	IIC2_Send_Byte(I2C_Addr);	   //发送写命令
	res++;
	IIC2_Wait_Ack();
	IIC2_Send_Byte(addr);
	res++;  //发送地址
	IIC2_Wait_Ack();
	//IIC_Stop();//产生一个停止条件
	IIC2_Start();
	IIC2_Send_Byte(I2C_Addr + 1);
	res++;          //进入接收模式
	IIC2_Wait_Ack();
	res = IIC_Read_Byte(0);
	IIC2_Stop();//产生一个停止条件

	return res;
}


/**************************实现函数********************************************

*******************************************************************************/
u8 IIC2readBytes(u8 dev, u8 reg, u8 length, u8 *data)
{
	u8 count = 0;

	IIC2_Start();
	IIC2_Send_Byte(dev);	   // Send address with Transmitter mode
	IIC2_Wait_Ack();
//	delay_us(5);
	IIC2_Send_Byte(reg); 
  IIC2_Wait_Ack();
	IIC2_Stop();
	delay_us(5);  
	IIC2_Start();
	IIC2_Send_Byte(dev + 1); //稴end address with Receiver mode
	IIC2_Wait_Ack();
  delay_us(6);
	for(count = 0; count < length; count++)
	{

		if(count != length - 1) {
			data[count] = IIC2_Read_Byte(1); //磖ead byte with ACK
			delay_us(3);
		}
		else  data[count] = IIC2_Read_Byte(0);	 //最后一个字节NACK
	}
	IIC2_Stop();//产生一个停止条件
	return count;
}

/**************************实现函数********************************************

*******************************************************************************/
u8 IIC2writeBytes(u8 dev, u8 reg, u8 length, u8 *data)
{

	u8 count = 0;
	IIC2_Start();
	IIC2_Send_Byte(dev);	   //发送写命令
	IIC2_Wait_Ack();
	//for MS5611
	if (reg!=0)
	{
	IIC2_Send_Byte(reg);   //发送地址
	IIC2_Wait_Ack();
	}
	for(count = 0; count < length; count++)
	{
		IIC2_Send_Byte(data[count]);
		IIC2_Wait_Ack();
	}
	IIC2_Stop();//产生一个停止条件

	return 1; //status == 0;
}

/**************************实现函数********************************************

*******************************************************************************/
u8 IIC2readByte(u8 dev, u8 reg, u8 *data)
{
	*data = I2C2_ReadOneByte(dev, reg);
	return 1;
}

/**************************实现函数********************************************

*******************************************************************************/
unsigned char IIC2writeByte(unsigned char dev, unsigned char reg, unsigned char data)
{
	return IIC2writeBytes(dev, reg, 1, &data);
}

u8 IIC2writeBits(u8 dev, u8 reg, u8 bitStart, u8 length, u8 data)
{

	u8 b;
	if (IIC2readByte(dev, reg, &b) != 0)
	{
		u8 mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
		data <<= (8 - length);
		data >>= (7 - bitStart);
		b &= mask;
		b |= data;
		return IIC2writeByte(dev, reg, b);
	}
	else
	{
		return 0;
	}
}

/********************************************************************

*******************************************************************************/
u8 IIC2writeBit(u8 dev, u8 reg, u8 bitNum, u8 data)
{
	u8 b;
	IIC2readByte(dev, reg, &b);
	b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
	return IIC2writeByte(dev, reg, b);
}

/*
Third I2C
*/

void IIC3_Start(void)
{
	SDA3_OUT();     //sda线输出
	IIC3_SDA = 1;
	IIC3_SCL = 1;
	delay_us(2);
	IIC3_SDA = 0; //START:when CLK is high,DATA change form high to low
	delay_us(2);
	IIC3_SCL = 0; //钳住I2C总线，准备发送或接收数据
}

/**************************实现函数********************************************

*******************************************************************************/
void IIC3_Stop(void)
{
	SDA3_OUT();//sda线输出
	IIC3_SCL = 0;
	IIC3_SDA = 0; //STOP:when CLK is high DATA change form low to high
	delay_us(2);
	IIC3_SCL = 1;
//	delay_us(1);
	IIC3_SDA = 1; //发送I2C总线结束信号
	delay_us(2);
}


u8 IIC3_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA3_IN();      //SDA设置为输入
	IIC3_SDA = 1;
	delay_us(1);
	IIC3_SCL = 1;
	delay_us(1);
	while(READ3_SDA)
	{
		ucErrTime++;
		if(ucErrTime > 50)
		{
			IIC3_Stop();
			I2C3_Erorr_Count ++;	 //I2C 错误
			return 1;
		}
		delay_us(1);
	}
	IIC3_SCL = 0; //时钟输出0
	return 0;
}


void IIC3_Ack(void)
{
	IIC3_SCL = 0;
	SDA3_OUT();
	IIC3_SDA = 0;
	delay_us(1);
	IIC3_SCL = 1;
	delay_us(1);
	IIC3_SCL = 0;
}


void IIC3_NAck(void)
{
	IIC3_SCL = 0;
	SDA3_OUT();
	IIC3_SDA = 1;
	delay_us(1);
	IIC3_SCL = 1;
	delay_us(1);
	IIC3_SCL = 0;
}


void IIC3_Send_Byte(u8 txd)
{
	u8 t;
	SDA3_OUT();
	IIC3_SCL = 0; //拉低时钟开始数据传输
	for(t = 0; t < 8; t++)
	{
		IIC3_SDA = (txd & 0x80) >> 7;
		txd <<= 1;
		delay_us(1);
		IIC3_SCL = 1;
		delay_us(1);
		IIC3_SCL = 0;
		delay_us(1);
	}
}

/**************************实现函数********************************************

*******************************************************************************/
u8 IIC3_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	SDA3_IN();//SDA设置为输入
	for(i = 0; i < 8; i++ )
	{
		IIC3_SCL = 0;
		delay_us(1);
		IIC3_SCL = 1;
		receive <<= 1;
		if(READ3_SDA)receive++;
		delay_us(1);
	}
	if (ack)
		IIC3_Ack(); //发送ACK
	else
		IIC3_NAck();//发送nACK
	return receive;
}


/**************************实现函数********************************************

*******************************************************************************/
unsigned char I2C3_ReadOneByte(unsigned char I2C_Addr, unsigned char addr)
{
	unsigned char res = 0;

	IIC3_Start();
	IIC3_Send_Byte(I2C_Addr);	   //发送写命令
	res++;
	IIC3_Wait_Ack();
	IIC3_Send_Byte(addr);
	res++;  //发送地址
	IIC3_Wait_Ack();
	//IIC_Stop();//产生一个停止条件
	IIC3_Start();
	IIC3_Send_Byte(I2C_Addr + 1);
	res++;          //进入接收模式
	IIC3_Wait_Ack();
	res = IIC_Read_Byte(0);
	IIC3_Stop();//产生一个停止条件

	return res;
}


/**************************实现函数********************************************

*******************************************************************************/
u8 IIC3readBytes(u8 dev, u8 reg, u8 length, u8 *data)
{
	u8 count = 0;

	IIC3_Start();
	IIC3_Send_Byte(dev);	   // Send address with Transmitter mode
	IIC3_Wait_Ack();
//	delay_us(5);
	IIC3_Send_Byte(reg); 
  IIC3_Wait_Ack();
	//IIC3_Stop();
	//delay_us(5);  
	IIC3_Start();
	IIC3_Send_Byte(dev + 1); //稴end address with Receiver mode
	IIC3_Wait_Ack();
 // delay_us(6);
	for(count = 0; count < length; count++)
	{

		if(count != length - 1) {
			data[count] = IIC3_Read_Byte(1); //磖ead byte with ACK
		//	delay_us(3);
		}
		else  data[count] = IIC3_Read_Byte(0);	 //最后一个字节NACK
	}
	IIC3_Stop();//产生一个停止条件
	return count;
}

/**************************实现函数********************************************

*******************************************************************************/
u8 IIC3writeBytes(u8 dev, u8 reg, u8 length, u8 *data)
{

	u8 count = 0;
	IIC3_Start();
	IIC3_Send_Byte(dev);	   //发送写命令
	IIC3_Wait_Ack();
	//for MS5611
	if (reg!=0)
	{
	IIC3_Send_Byte(reg);   //发送地址
	IIC3_Wait_Ack();
	}
	for(count = 0; count < length; count++)
	{
		IIC3_Send_Byte(data[count]);
		IIC3_Wait_Ack();
	}
	IIC3_Stop();//产生一个停止条件

	return 1; //status == 0;
}

/**************************实现函数********************************************

*******************************************************************************/
u8 IIC3readByte(u8 dev, u8 reg, u8 *data)
{
	*data = I2C3_ReadOneByte(dev, reg);
	return 1;
}

/**************************实现函数********************************************

*******************************************************************************/
unsigned char IIC3writeByte(unsigned char dev, unsigned char reg, unsigned char data)
{
	return IIC3writeBytes(dev, reg, 1, &data);
}

u8 IIC3writeBits(u8 dev, u8 reg, u8 bitStart, u8 length, u8 data)
{

	u8 b;
	if (IIC3readByte(dev, reg, &b) != 0)
	{
		u8 mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
		data <<= (8 - length);
		data >>= (7 - bitStart);
		b &= mask;
		b |= data;
		return IIC3writeByte(dev, reg, b);
	}
	else
	{
		return 0;
	}
}

/********************************************************************

*******************************************************************************/
u8 IIC3writeBit(u8 dev, u8 reg, u8 bitNum, u8 data)
{
	u8 b;
	IIC3readByte(dev, reg, &b);
	b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
	return IIC3writeByte(dev, reg, b);
}
