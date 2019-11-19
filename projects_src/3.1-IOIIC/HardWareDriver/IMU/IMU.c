#include "IMU.h"
#define DATA_SIZE 200
volatile float exInt, eyInt, ezInt;  
volatile float integralFBx, integralFBy, integralFBz;
volatile float q0, q1, q2, q3; 
volatile float qa0, qa1, qa2, qa3;
volatile float integralFBhand, handdiff;
volatile double halftime ;
volatile uint32_t lastUpdate, now; 
volatile uint16_t sysytem_time_ms = 0;
volatile float IMU_Pitch, IMU_Roll, IMU_Yaw, ACC_Pitch, ACC_Roll;
volatile float  IMU_GYROx, IMU_GYROy, IMU_GYROz;
volatile unsigned char IMU_inited = 0;
volatile uint16_t imu_clce = 0;
volatile float acc_vector = 0;  //  M/S^2

// Fast inverse square-root

float invSqrt(float x)
{
	volatile float halfx = 0.5f * x;
	volatile float y = x;
	long i = *(long *)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float *)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

/*
Get values from sensors
MPU6050 returns 6 values.
0, 1, 2 are accelemeter data
3 4 5 are gyro data.
*/
#define new_weight 0.4f
#define old_weight 0.6f


