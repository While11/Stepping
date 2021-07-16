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
u16 adcx;
float temp;
char COSTT_end=0;
char peace=0;
u32 X_CosTTNum = 0;										// X 匀速阶段的脉冲个数
int main(void)
{
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	LED_Init();
	delay_init(168);    //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	
	X_GPIO_Init();
	OLED_Init();
	Adc_Init();         //初始化ADC
	TIM4_Config();
	X_Calculate_SpeedList();//生成加减速阶段的速度表
	X_PWM_S_Output_Right();//逆时针启动
	while(1)
	{ 
//		printf("%d\r\n",1);
	//	printf("%d",0x01);
		
//		X_COSTT_Output_Right(3200-151);
		//	while(COSTT_end!=1);//等待end为1，end为1后停止
	//	COSTT_end=0;//如果end不等于0则停止
	////	X_PWM_S_Output_Left();
	//	X_COSTT_Output_Left(3200-151);
//		peace=1;//减速停止
//		adcx=Get_Adc_Average(ADC_Channel_5,20);//获取通道5的转换值，20次取平均
//		printf("%d\r\n",adcx);    //显示ADCC采样后的原始值
//		temp=(float)adcx*(3.3/4096);          //获取计算后的带小数的实际电压值，比如3.1111  //12位ADC
//		printf("%f\r\n",temp);
//		delay_ms(250);	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	}
}




