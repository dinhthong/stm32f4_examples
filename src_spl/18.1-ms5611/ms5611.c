/*
https://github.com/jarzebski/Arduino-MS5611

*/
#include "ms5611.h"
#include "math.h"
	uint16_t fc[6];
	uint8_t ct;
	uint8_t uosr;
	int32_t TEMP2,timer_D1,timer_D2;
	uint32_t D1,D2;
	int64_t OFF2, SENS2;
	int sw=0;
  int ut,up;
void ms5611_begin()
{
    reset();
    setOversampling(MS5611_ULTRA_HIGH_RES); // do phan giai
    _delay_ms(100);
    readPROM(); // read Calibration coefficients.
}
/*
  Implementation of reading MS5611 without having to use delay functions.
  In order to make use of free time for other programs to run!.
*/
void ms5611_getupdate(float *temperature, long *pressure, float *alti) {	
 switch (sw){
    case 0:
	  timer_D2 = mili();
    ms5611WriteByte(MS5611_CMD_CONV_D2 + uosr);// convert command. D2 + uosr   
    sw = 1;		
    break;
    case 1:
      if (mili() - timer_D2 >= ct) {
	     D2 = readRawTemperature();
       *temperature = readTemperature(0);  // calculate temperature.
       ms5611WriteByte(MS5611_CMD_CONV_D1 + uosr);
				// get mili() to start timer_D1 for next conversion
       timer_D1 = mili();				
       sw = 2;		
      }
    break;    
    case 2:			  
      if (mili() - timer_D1 >= ct) {	          
				D1=readRawPressure();
        *pressure = readPressure(1);
				// 1 atm = 101325 N = 1013.25 mbar
        *alti = getAltitude(*pressure,101325);
				sw = 0;
      }
    break;
	default:
	break;
  }
}
/* Set oversampling value. aka OSR
to calculate delay values for D1,D2 conversions after sending covert commands.
*/
void setOversampling(int osr)
{
    switch (osr)
    {
	case MS5611_ULTRA_LOW_POWER:
	    ct = 1;
	    break;
	case MS5611_LOW_POWER:
	    ct = 2;
	    break;
	case MS5611_STANDARD:
	    ct = 3;
	    break;
	case MS5611_HIGH_RES:
	    ct = 5;
	    break;
	case MS5611_ULTRA_HIGH_RES:
	    ct = 10;
	    break;
    }

    uosr = osr;
}

// Get oversampling value
ms5611_osr_t getOversampling(void)
{
    return (ms5611_osr_t)uosr;
}

void reset(void)
{
    ms5611WriteByte(MS5611_CMD_RESET);

}
/*
The read command for PROM shall be executed once after reset by the user to read the content of the calibration
PROM and to calculate the calibration coefficients
*/
void readPROM(void)
{  uint8_t offset;
    for (offset = 0; offset < 6; offset++)
    {
	fc[offset] = readRegister16(MS5611_CMD_READ_PROM + (offset * 2));
    }
}


uint32_t readRawTemperature(void)
{
  return readRegister24(MS5611_CMD_ADC_READ);
}

uint32_t readRawPressure(void)
{
    return readRegister24(MS5611_CMD_ADC_READ);
}
/*
Read the MS5611 datasheet to understand more about pressure calculation. 
return P: pressure in [mbar]
*/
int32_t readPressure(int compensation)
{   
		uint32_t P;
    int32_t dT = D2 - (uint32_t)fc[4] * 256;

    int64_t OFF = (int64_t)fc[1] * 65536 + (int64_t)fc[3] * dT / 128;
    int64_t SENS = (int64_t)fc[0] * 32768 + (int64_t)fc[2] * dT / 256;

    if (compensation==1)
    {
	int32_t TEMP = 2000 + ((int64_t) dT * fc[5]) / 8388608;

	OFF2 = 0;
	SENS2 = 0;

	if (TEMP < 2000)
	{
	    OFF2 = 5 * ((TEMP - 2000) * (TEMP - 2000)) / 2;
	    SENS2 = 5 * ((TEMP - 2000) * (TEMP - 2000)) / 4;
	}

	if (TEMP < -1500)
	{
	    OFF2 = OFF2 + 7 * ((TEMP + 1500) * (TEMP + 1500));
	    SENS2 = SENS2 + 11 * ((TEMP + 1500) * (TEMP + 1500)) / 2;
	}

	OFF = OFF - OFF2;
	SENS = SENS - SENS2;
    }

     P = (D1 * SENS / 2097152 - OFF) / 32768;

    return P;
}
/*
return: temperature in [Celcius]*/
double readTemperature(int compensation)
{
   // uint32_t D2 = ut;
    int32_t dT = D2 - (uint32_t)fc[4] * 256;

    int32_t TEMP = 2000 + ((int64_t) dT * fc[5]) / 8388608;

    TEMP2 = 0;

    if (compensation==1)
    {
	if (TEMP < 2000)
	{
	    TEMP2 = (dT * dT) / (2 << 30);
	}
    }

    TEMP = TEMP - TEMP2;

    return ((double)TEMP/100);
}

/* Calculate altitude from Pressure & Sea level pressure
in [meters]
*/
double getAltitude(int32_t pressure, int32_t referencePressure)
{
    return (44330.0f * (1.0f - pow((double)pressure / (double)referencePressure, 0.1902949f)));
}
/*
https://os.mbed.com/teams/Aerodyne/code/MS5611/file/847720b736ea/MS5611Base.h/
https://os.mbed.com/teams/Aerodyne/code/MS5611Example/
this fomular doesn't work well
*/
double toAltitude(int32_t pressure,int32_t referencePressure) {
        // Ref. 29124-AltimeterAppNote1.pdf
        const float R = 287.052; // specific gas constant R*/M0
        const float g = 9.80665; // standard gravity 
        const float t_grad = 0.0065; // gradient of temperature
        const float t0 = 273.15 + 30; // temperature at 0 altitude
        const float p0 = referencePressure; // pressure at 0 altitude
 
        return t0 / t_grad * (1 - exp((t_grad * R / g) * log(pressure / p0)));
    }
// Calculate sea level from Pressure given on specific altitude
double getSeaLevel(double pressure, double altitude)
{
    return ((double)pressure / pow(1.0f - ((double)altitude / 44330.0f), 5.255f));
}

// Read 16-bit from register (oops MSB, LSB)
uint16_t readRegister16(uint8_t reg)
{
  uint16_t value;
  unsigned short msb=0;
  unsigned short lsb=0;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
  I2C_AcknowledgeConfig(I2C1,ENABLE);
  I2C_GenerateSTART(I2C1,ENABLE);

  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
  I2C_Send7bitAddress(I2C1, MS5611_ADDRESS, I2C_Direction_Transmitter);

  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  I2C_SendData(I2C1,reg);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  I2C_GenerateSTART(I2C1,ENABLE);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  I2C_Send7bitAddress(I2C1, MS5611_ADDRESS, I2C_Direction_Receiver);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
  msb = I2C_ReceiveData(I2C1);

  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
  lsb = I2C_ReceiveData(I2C1);

  I2C_GenerateSTOP(I2C1,ENABLE);
  I2C_AcknowledgeConfig(I2C1,DISABLE);

  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
  I2C_ReceiveData(I2C1);

  value= (msb << 8) | lsb;

    return value;
}

// Read 24-bit from register (oops XSB, MSB, LSB)
uint32_t readRegister24(uint8_t reg)
{
    uint32_t value;
   unsigned long msb=0;
  unsigned long lsb=0;
  unsigned long xsb=0;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
  I2C_AcknowledgeConfig(I2C1,ENABLE);
  I2C_GenerateSTART(I2C1,ENABLE);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  I2C_Send7bitAddress(I2C1, MS5611_ADDRESS, I2C_Direction_Transmitter);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  I2C_SendData(I2C1,reg);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  I2C_GenerateSTART(I2C1,ENABLE);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  I2C_Send7bitAddress(I2C1, MS5611_ADDRESS, I2C_Direction_Receiver);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
  msb = I2C_ReceiveData(I2C1);

  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
  lsb = I2C_ReceiveData(I2C1);

  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
  xsb = I2C_ReceiveData(I2C1);

  I2C_GenerateSTOP(I2C1,ENABLE);
  I2C_AcknowledgeConfig(I2C1,DISABLE);

  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
  I2C_ReceiveData(I2C1);

  value = (msb << 16) | (lsb << 8) | xsb;

    return value;
}
void ms5611WriteByte(unsigned char data)
{

  I2C_GenerateSTART(I2C1,ENABLE);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  I2C_Send7bitAddress(I2C1, MS5611_ADDRESS, I2C_Direction_Transmitter);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

//  I2C_SendData(I2C1,address);
//  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  I2C_SendData(I2C1,data);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  I2C_GenerateSTOP(I2C1,ENABLE);

 // while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

