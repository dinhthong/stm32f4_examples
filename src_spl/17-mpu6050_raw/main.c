#include "stm32f4xx.h"
#include "MPU6050.h"
//#include "HMC5883L.h"
/*	      MASTER                    
	 PB6 --- I2C1_SCL              
	 PB7 --- I2C1_SDA              
*/
int16_t MPU6050data[7]; 
//int16_t HMC5883Ldata[3]; 
void I2C_Configuration(void);
void Delay(__IO uint32_t nCount);

int main(void)
{
  I2C_Configuration();  
  MPU6050_Initialize();
  while (1)
  {
    MPU6050_GetRawAccelTempGyro(MPU6050data);
		Delay(240);
  }
}

void I2C_Configuration(void)
{
#ifdef FAST_I2C_MODE
 #define I2C_SPEED 400000
 #define I2C_DUTYCYCLE I2C_DutyCycle_16_9  
#else /* STANDARD_I2C_MODE*/
 #define I2C_SPEED 100000
 #define I2C_DUTYCYCLE I2C_DutyCycle_2
#endif /* FAST_I2C_MODE*/
	
  GPIO_InitTypeDef  GPIO_InitStructure;
  I2C_InitTypeDef   I2C_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);	

  /* I2C De-initialize */
  I2C_DeInit(I2C1);
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DUTYCYCLE;
  I2C_InitStructure.I2C_OwnAddress1 = 0;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_Init(I2C1, &I2C_InitStructure);
 /* I2C ENABLE */
  I2C_Cmd(I2C1, ENABLE); 
  /* Enable Interrupt */
}

void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}



