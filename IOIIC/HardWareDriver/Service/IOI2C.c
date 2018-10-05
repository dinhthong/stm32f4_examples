/*
I2C comminucation protocol using Software.
29 August 2018.

 */
#include "IOI2C.h"
#include "delay.h"

void configure_pin_output(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);
void IIC_Start(IOIIC_Pin_t* IOIIC);
void IIC_Stop(IOIIC_Pin_t* IOIIC);
void IIC_Ack(IOIIC_Pin_t* IOIIC);					//IIC ACK
void IIC_NAck(IOIIC_Pin_t* IOIIC);				//IIC not ACK
u8 IIC_Wait_Ack(IOIIC_Pin_t* IOIIC);
int16_t  I2C_Erorr_Count = 0;


// set bits GPIOB->MODER7[1:0] = GPIOB->MODER[14:15] = 0b01


/* configure PORT,PIN MODER bit as output
GPIO_Mode_OUT  = 0x01
*/
void configure_pin_output(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin) 
{
	    // reset corresponding 2 MODER bits 
			GPIOx->MODER  &= ~(0x11 << (GPIO_Pin * 2));
	    // set to 0b01 = 1
			GPIOx->MODER |= (((uint32_t)GPIO_Mode_OUT) << (GPIO_Pin * 2));
}

void configure_pin_input(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin) 
{
	    // reset corresponding 2 MODER bits 
			GPIOx->MODER  &= ~(0x11 << (GPIO_Pin * 2));
	    // set to 0b01 = 1
			GPIOx->MODER |= (((uint32_t)GPIO_Mode_IN) << (GPIO_Pin * 2));
}
u8 read_sda_input(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{
	return GPIOx->IDR & (1<<GPIO_Pin);
}
/*******************************************************************

*******************************************************************************/
//void IIC_Start(IOIIC_Pin_t* IOIIC)
void IIC_Start(IOIIC_Pin_t* IOIIC)
{
	// IOIIC
//	SDA_OUT();     // SDA line as output
	configure_pin_output(IOIIC->SDA_Port, IOIIC->SDA_Pin); 
	// SDA OUT = 1.
	IOIIC->SDA_Port->BSRRL = 1<<(IOIIC->SDA_Pin);
//	GPIOC->BSRRL = GPIO_Pin_7;
	IOIIC->SDA_Port->BSRRL = 1<<(IOIIC->SDA_Pin);
	
	IOIIC->SCL_Port->BSRRL = 1<<(IOIIC->SCL_Pin);
//	IIC_SCL = 1;
	delay_us(2);
//	IIC_SDA = 0; //START: when CLK is high,DATA change form high to low
	IOIIC->SDA_Port->BSRRH = 1<<(IOIIC->SDA_Pin);
	delay_us(2);
//	IIC_SCL = 0; //穚repare to send or receive data
	IOIIC->SCL_Port->BSRRH = 1<<(IOIIC->SCL_Pin);
	
}

void IIC_Stop(IOIIC_Pin_t* IOIIC)
{
	configure_pin_output(IOIIC->SDA_Port, IOIIC->SDA_Pin); 
	//IIC_SCL = 0;
	IOIIC->SCL_Port->BSRRH = 1<<(IOIIC->SCL_Pin);
	//IIC_SDA = 0; //STOP: when CLK is high DATA change form low to high
	IOIIC->SDA_Port->BSRRH = 1<<(IOIIC->SDA_Pin);
	delay_us(2);
//	IIC_SCL = 1;
	IOIIC->SCL_Port->BSRRL = 1<<(IOIIC->SCL_Pin);
//	IIC_SDA = 1;
	IOIIC->SDA_Port->BSRRL = 1<<(IOIIC->SDA_Pin);
	delay_us(2);
}


u8 IIC_Wait_Ack(IOIIC_Pin_t* IOIIC)
{
	u8 ucErrTime = 0;
//	SDA_IN();      //SDA as input pin
	configure_pin_input(IOIIC->SDA_Port, IOIIC->SDA_Pin); 
	IOIIC->SDA_Port->BSRRL = 1<<(IOIIC->SDA_Pin);
	delay_us(1);
	IOIIC->SCL_Port->BSRRL = 1<<(IOIIC->SCL_Pin);
	delay_us(1);
	while(read_sda_input(IOIIC->SDA_Port, IOIIC->SDA_Pin))
	{
		ucErrTime++;
		if(ucErrTime > 50)
		{
			IIC_Stop(IOIIC);
			I2C_Erorr_Count ++;	 //I2C
			return 1;
		}
		delay_us(1);
	}
	IOIIC->SCL_Port->BSRRH = 1<<(IOIIC->SCL_Pin); 
	return 0;
}


void IIC_Ack(IOIIC_Pin_t* IOIIC)
{
	IOIIC->SCL_Port->BSRRH = 1<<(IOIIC->SCL_Pin);
	
	configure_pin_output(IOIIC->SDA_Port, IOIIC->SDA_Pin); 
//	SDA_OUT();
	IOIIC->SDA_Port->BSRRH = 1<<(IOIIC->SDA_Pin);
	
	delay_us(1);
	IOIIC->SCL_Port->BSRRL = 1<<(IOIIC->SCL_Pin);
	delay_us(1);
	IOIIC->SCL_Port->BSRRH = 1<<(IOIIC->SCL_Pin);
}


void IIC_NAck(IOIIC_Pin_t* IOIIC)
{
	IOIIC->SCL_Port->BSRRH = 1<<(IOIIC->SCL_Pin);
	configure_pin_output(IOIIC->SDA_Port, IOIIC->SDA_Pin); 
	//SDA_OUT();
	IOIIC->SDA_Port->BSRRL = 1<<(IOIIC->SDA_Pin);
	delay_us(1);
	IOIIC->SCL_Port->BSRRL = 1<<(IOIIC->SCL_Pin);
	delay_us(1);
	IOIIC->SCL_Port->BSRRH = 1<<(IOIIC->SCL_Pin);
}


void IIC_Send_Byte(u8 txd, IOIIC_Pin_t* IIC)
{
	u8 t;
	configure_pin_output(IIC->SDA_Port, IIC->SDA_Pin); 
//	SDA_OUT();
	IIC->SCL_Port->BSRRH = 1<<(IIC->SCL_Pin); // Pull down the clock line to start data transmission
	for(t = 0; t < 8; t++)
	{
	//	IIC_SDA = (txd & 0x80) >> 7;
		((txd & 0x80) >> 7)?(IIC->SDA_Port->BSRRL = 1<<(IIC->SDA_Pin)):(IIC->SDA_Port->BSRRH = 1<<(IIC->SDA_Pin));
		txd <<= 1;
		delay_us(1);
		IIC->SCL_Port->BSRRL = 1<<(IIC->SCL_Pin);
		delay_us(1);
		IIC->SCL_Port->BSRRH = 1<<(IIC->SCL_Pin);
		delay_us(1);
	}
}

u8 IIC_Read_Byte(unsigned char ack, IOIIC_Pin_t* IIC)
{
	unsigned char i, receive = 0;
	//SDA_IN();
	configure_pin_input(IIC->SDA_Port, IIC->SDA_Pin);
	for(i = 0; i < 8; i++ )
	{
	//	IIC_SCL = 0;
		IIC->SCL_Port->BSRRH = 1<<(IIC->SCL_Pin);
		delay_us(1);
	//	IIC_SCL = 1;
		IIC->SCL_Port->BSRRL = 1<<(IIC->SCL_Pin);
		receive <<= 1;
		if(read_sda_input(IIC->SDA_Port, IIC->SDA_Pin))receive++;
		delay_us(1);
	}
	if (ack)
		IIC_Ack(IIC); //
	else
		IIC_NAck(IIC);//
	return receive;
}

unsigned char I2C_ReadOneByte(unsigned char I2C_Addr, unsigned char addr, IOIIC_Pin_t* IIC)
{
	unsigned char res = 0;

	IIC_Start(IIC);
	IIC_Send_Byte(I2C_Addr, IIC);
	res++;
	IIC_Wait_Ack(IIC);
	IIC_Send_Byte(addr, IIC);
	res++;
	IIC_Wait_Ack(IIC);
	//IIC_Stop();//产生一个停止条件
	IIC_Start(IIC);
	IIC_Send_Byte(I2C_Addr + 1, IIC);
	res++;
	IIC_Wait_Ack(IIC);
	res = IIC_Read_Byte(0, IIC);
	IIC_Stop(IIC);

	return res;
}

u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data, IOIIC_Pin_t* IIC)
{
	u8 count = 0;

	IIC_Start(IIC);
	IIC_Send_Byte(dev, IIC);	   // Send address with Transmitter mode
	IIC_Wait_Ack(IIC);
	IIC_Send_Byte(reg, IIC);   
	IIC_Wait_Ack(IIC);
	IIC_Start(IIC);
	IIC_Send_Byte(dev + 1, IIC); //稴end address with Receiver mode
	IIC_Wait_Ack(IIC);

	for(count = 0; count < length; count++)
	{

		if(count != length - 1)data[count] = IIC_Read_Byte(1, IIC); //磖ead byte with ACK
		else  data[count] = IIC_Read_Byte(0, IIC);	 
	}
	IIC_Stop(IIC);
	return count;
}

u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8 *data, IOIIC_Pin_t* IIC)
{

	u8 count = 0;
	IIC_Start(IIC);
	IIC_Send_Byte(dev, IIC);	   
	IIC_Wait_Ack(IIC);
	//for MS5611
	if (reg!=0)
	{
	IIC_Send_Byte(reg, IIC);
	IIC_Wait_Ack(IIC);
	}
	for(count = 0; count < length; count++)
	{
		IIC_Send_Byte(data[count], IIC);
		IIC_Wait_Ack(IIC);
	}
	IIC_Stop(IIC);

	return 1; //status == 0;
}

u8 IICreadByte(u8 dev, u8 reg, u8 *data, IOIIC_Pin_t* IIC)
{
	*data = I2C_ReadOneByte(dev, reg, IIC);
	return 1;
}


unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data, IOIIC_Pin_t* IIC)
{
	return IICwriteBytes(dev, reg, 1, &data, IIC);
}

u8 IICwriteBits(u8 dev, u8 reg, u8 bitStart, u8 length, u8 data, IOIIC_Pin_t* IIC)
{

	u8 b;
	if (IICreadByte(dev, reg, &b, IIC) != 0)
	{
		u8 mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
		data <<= (8 - length);
		data >>= (7 - bitStart);
		b &= mask;
		b |= data;
		return IICwriteByte(dev, reg, b, IIC);
	}
	else
	{
		return 0;
	}
}

u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data, IOIIC_Pin_t* IIC)
{
	u8 b;
	IICreadByte(dev, reg, &b, IIC);
	b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
	return IICwriteByte(dev, reg, b, IIC);
}
