#ifndef __X_STEP_MOTOR_H
#define __X_STEP_MOTOR_H

#include "sys.h"
#include "math.h"				// exp()
#include <stdio.h>
#include "delay.h"

// Motor Parameter
typedef struct{
	float X_Step;
	float X_Fre_Min;
	float X_Fre_Max;
	float X_Jerk;
}X_SpeedList_TypeDef;
#define		X_TIM3_FREQ							(84000000 / X_TIM3_Prescaler)		// 2MHz
#define		X_TIM3_Pulse						(X_TIM3_FREQ / 500)
#define		X_Accel_Step						50.0f
#define		X_SpeedList_LEN					((u8)X_Accel_Step)
#define		X_FREQ_MIN							500.0f
#define		X_FREQ_MAX							6400.0f
#define		X_JERK									4.0f

// Motor State
#define		X_ACCEL									1						// acceleration
#define		X_COSTT									2						// constant
#define   X_DECEL                 3						// deceleration
#define		X_UNIFM									4						// uniform
#define		X_STOP									0						// stop

#define		TRUE										1
#define		FALSE										0

// X - TIM3: CH1 - PA6, DIR - PA7
#define		X_TIM3_Prescaler				42
#define		X_TIM3_Period						0xFFFF
#define		X_TIM3_IRQHandler				TIM3_IRQHandler

// Calculate
void X_Calculate_SpeedList(void);

// X
void X_GPIO_Init(void);			//GPIO
void X_TIM3_Config(void);		// TIM
void X_PWM_S_Output_Left(void);
void X_PWM_S_Output_Right(void);
void X_COSTT_Output_Inverted(u32 X_PulseNum);
void X_COSTT_Output_clockwise(u32 X_PulseNum);
void X_Stop(void);
void X_TIM3_IRQHandler(void);

#endif	/* __X_STEP_MOTOR_H */

/****************************END OF FILE****************************/
