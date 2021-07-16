/**
**************************************************************************************
* @file    main.c
* @author  SieYuan
* @version V1.0
* @date    2021-01-19
* @brief   实现	S 形加减速算法
**************************************************************************************
*/
#include	"stm32f4xx.h"
#include  "sys.h"
#include  "led.h"
#include  "key.h"
#include  "exti.h"
#include  "delay.h"
#include  "X_Step_Motor.h"
#include "usart.h"
char COSTT_end=0;
char peace=0;
u32 X_CosTTNum = 0;										// X 匀速阶段的脉冲个数
int main(void)
{
	
	LED_Init();
//	KEY_Init();
	EXTIx_Init();
	X_GPIO_Init();
	delay_init(168);
	uart_init(115200);

	X_Calculate_SpeedList();//生成加减速阶段的速度表
//	printf("%d",0x01);
	X_PWM_S_Output_Right();//逆时针启动
	X_COSTT_Output_Right(3200-151);
	//	while(COSTT_end!=1);//等待end为1，end为1后停止
//	COSTT_end=0;//如果end不等于0则停止
////	X_PWM_S_Output_Left();
//	X_COSTT_Output_Left(3200-151);
	peace=1;//减速停止
//	delay_ms(650);
//	X_PWM_S_Output_Right();//逆时针启动
//	X_COSTT_Output_Right(202-101);
//	X_Uniform_Output_Left(1600);
//	X_Uniform_Output_Right(1600);
//	X_Stop();
	
	while(1);
}
/****************************END OF FILE****************************/
