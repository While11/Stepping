#include	"stm32f4xx.h"
#include  "sys.h"
#include  "led.h"
#include  "key.h"
#include  "delay.h"
#include  "X_Step_Motor.h"
#include "usart.h"
#include "adc.h"
#include "command.h"
#include "oled.h"
float angle,angle_last=0.0,angle_first=0.0;
u16 adcx;
float temp;
char COSTT_end=0;
char peace=0;
u32 X_CosTTNum = 0;										// X 匀速阶段的脉冲个数
char flag_angle=1;
int main(void)
{
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2

	delay_init(168);    //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	LED_Init();
	OLED_Init();
	
	
	X_GPIO_Init();
	Adc_Init();         //初始化ADC
	TIM4_Config();      //摆角定时采样
	X_Calculate_SpeedList();//生成加减速阶段的速度表
	X_PWM_S_Output_Right();//逆时针启动：控制电机的TIM3初始化
	while(1);

}




