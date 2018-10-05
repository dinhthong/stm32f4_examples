/*
I2C comminucation protocol using Software.
29 August 2018.

 */
#include "IOI2C.h"
#include "delay.h"


int16_t  I2C_Erorr_Count = 0;

void IIC_gpio_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
}

/*******************************************************************

*******************************************************************************/
void IIC_Start(void)
{
	SDA_OUT();     // SDA line as output
	IIC_SDA = 1;
	IIC_SCL = 1;
	delay_us(2);
	IIC_SDA = 0; //START: when CLK is high,DATA change form high to low
	delay_us(2);
	IIC_SCL = 0; //穚repare to send or receive data
}

void IIC_Stop(void)
{
	SDA_OUT();
	IIC_SCL = 0;
	IIC_SDA = 0; //STOP: when CLK is high DATA change form low to high
	delay_us(2);
	IIC_SCL = 1;
	IIC_SDA = 1;
	delay_us(2);
}


u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA_IN();      //SDA as input pin
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
			I2C_Erorr_Count ++;	 //I2C
			return 1;
		}
		delay_us(1);
	}
	IIC_SCL = 0; 
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
	IIC_SCL = 0; // Pull down the clock line to start data transmission
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

u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	SDA_IN();
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
		IIC_Ack(); //
	else
		IIC_NAck();//
	return receive;
}

unsigned char I2C_ReadOneByte(unsigned char I2C_Addr, unsigned char addr)
{
	unsigned char res = 0;

	IIC_Start();
	IIC_Send_Byte(I2C_Addr);
	res++;
	IIC_Wait_Ack();
	IIC_Send_Byte(addr);
	res++;
	IIC_Wait_Ack();
	//IIC_Stop();//产生一个停止条件
	IIC_Start();
	IIC_Send_Byte(I2C_Addr + 1);
	res++;
	IIC_Wait_Ack();
	res = IIC_Read_Byte(0);
	IIC_Stop();

	return res;
}

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
		else  data[count] = IIC_Read_Byte(0);	 
	}
	IIC_Stop();
	return count;
}

u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8 *data)
{

	u8 count = 0;
	IIC_Start();
	IIC_Send_Byte(dev);	   
	IIC_Wait_Ack();
	//for MS5611
	if (reg!=0)
	{
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	}
	for(count = 0; count < length; count++)
	{
		IIC_Send_Byte(data[count]);
		IIC_Wait_Ack();
	}
	IIC_Stop();

	return 1; //status == 0;
}

u8 IICreadByte(u8 dev, u8 reg, u8 *data)
{
	*data = I2C_ReadOneByte(dev, reg);
	return 1;
}


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

u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
{
	u8 b;
	IICreadByte(dev, reg, &b);
	b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
	return IICwriteByte(dev, reg, b);
}
