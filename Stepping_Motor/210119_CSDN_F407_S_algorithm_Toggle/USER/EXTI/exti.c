/**
******************************************************************************
* @file    exti.c
* @author  SieYuan
* @version V1.0
* @date    2021-01-06
* @brief   设置外部中断，对紧急情况下，使电机复位或急停。 - F407
*
******************************************************************************
*/ 
#include "exti.h"

void EXTIx_Init(void)
{
	EXTI_InitTypeDef	EXTI_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	KEY_Init();		// 按键初始化
	
	// F407 中断与IO口映射  通过SYSCFG时钟
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SYSCFG, ENABLE); //F407
	// KEY0 - GPIOE.4 中断线以及中断初始化配置，下降沿触发。
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);		//PE4 连接到中断线4
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	// KEY1 - GPIOE.3 中断线以及中断初始化配置，下降沿触发。
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);		//PE3 连接到中断线3
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	// KEY2 - GPIOE.2 中断线以及中断初始化配置，下降沿触发。
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);		//PE2 连接到中断线2
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	// 中断配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

// KEY0 中断服务程序
void EXTI4_IRQHandler(void)
{
	delay_ms(20);	//消抖
	if (KEY0 == 0)
	{
		X_Uniform_Output_Left(2000);		// 中断，回到原点
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}

// KEY1 中断服务程序
void EXTI3_IRQHandler(void)
{
	delay_ms(20);	//消抖
	if (KEY1 == 0)
	{
		X_Uniform_Output_Right(2000);				// 急停
	}
	EXTI_ClearITPendingBit(EXTI_Line3);
}

// KEY2 中断服务程序
void EXTI2_IRQHandler(void)
{
	delay_ms(20);	//消抖
	if (KEY2 == 0)
	{
		X_Stop();
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}

/****************************END OF FILE****************************/
