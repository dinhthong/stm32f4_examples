#ifndef __IMU_H
#define __IMU_H

#include "common.h"
#include <math.h>

#define M_PI  (float)3.1415926535
#define micros() TIM5->CNT

extern volatile float IMU_Pitch, IMU_Roll, IMU_Yaw, ACC_Pitch, ACC_Roll;	 //µ¥Î» ¶È

extern volatile float IMU_GYROx, IMU_GYROy, IMU_GYROz;	
extern volatile float acc_vector;  
//Mini IMU AHRS 
void IMU_getValues(float *values);
void IMU_init(void);
#endif

