#include "command.h"
#include "X_Step_Motor.h"
#include "adc.h"
#include "led.h"
#include "usart.h"
extern u16 adcx;
extern float temp;
extern float angle,angle_last,angle_first;
float delt,delt_last=0;
u32 Pulse=0;
extern char flag_angle;
extern u32 X_CosTTNum;
float k=0.0,k1=0.0;

//TIM4 CH3 PB8
void TIM4_Config(void)
{


	  TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	  NVIC_InitTypeDef	        NVIC_InitStructure;

	  RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE);
		TIM_TimeBaseStructure.TIM_Prescaler = TIM4_Prescaler - 1;
		TIM_TimeBaseStructure.TIM_Period = TIM4_Period;
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit( TIM4, &TIM_TimeBaseStructure);
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init( &NVIC_InitStructure);	
		
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	// 开启定时器更新中断
	  TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	// 使能定时器
		TIM_Cmd(TIM4, ENABLE);

}


void  TIM4_IRQHandler (void)
{
   static float count=0;
	if ( TIM_GetITStatus( TIM4, TIM_IT_Update) != RESET ) 
	{	
    TIM_ClearITPendingBit(TIM4 , TIM_IT_Update);  
		count++;
    adcx=Get_Adc_Average(ADC_Channel_5,20);//获取通道5的转换值，20次取平均
		angle=(float)adcx*(360.0/4096)-angle0;          //获取计算后的带小数的实际电压值，比如3.1111  //12位ADC
//    if(angle>=89&&angle<=91)
//			cnt++;
		//		printf("%f\r\n",angle);
		if(count==1)
		{
				angle_first=90.0-angle;
			  angle_last=angle-2;
			k=180.0/(2*angle_first);
			k1=k;
//			printf("%f\r\n",angle);
//			printf("%f\r\n",angle_first);
		}
		if((angle-angle_last)>=0)
		{
			Pulse=((180.0/(2*angle_first))*(angle-angle_last))/0.056;
		}
		else
		{
		  Pulse=((180.0/(2*angle_first))*(angle_last-angle))/0.056;
		}
		
//		printf("%f\r\n",angle-angle_last);
//		printf("%d\r\n",Pulse);
		angle_last=angle;
		X_COSTT_Output_Inverted(Pulse);//逆时针转
		
		
		
	}		 	
}