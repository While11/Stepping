#include "command.h"
#include "X_Step_Motor.h"
#include "adc.h"
extern u16 adcx;
extern float temp;
float angle,angle_last=0,angle_first;
float delt,delt_last=0;
u32 Pulse=0;
//TIM4 CH3 PB8
void TIM4_Config(void)
{

//		GPIO_InitTypeDef	GPIO_InitStructure;
	  TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	  NVIC_InitTypeDef	        NVIC_InitStructure;
    
//	  RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOB, ENABLE);
//	  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;		// TIM4_CH3 - PB8
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			// ����
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		// ���츴�����
//		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			// ����
//		GPIO_Init(GPIOB, &GPIO_InitStructure);
	  
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
	
	// ������ʱ�������ж�
	  TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
	// ʹ�ܶ�ʱ��
		TIM_Cmd(TIM4, ENABLE);

}


void  TIM4_IRQHandler (void)
{
	static int count=0;
	if ( TIM_GetITStatus( TIM4, TIM_IT_Update) != RESET ) 
	{	
		count++;
    adcx=Get_Adc_Average(ADC_Channel_5,20);//��ȡͨ��5��ת��ֵ��20��ȡƽ��
		temp=(float)adcx*(3.3/4096);          //��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111  //12λADC
		angle=   ;
		if(count==1)
		{
				angle_first=90.0-angle;
			  angle_last=angle_first;
		}
		Pulse=(180.0/2*angle_first)*(angle-angle_last)/0.056;
		angle_last=angle;
		if(angle-angle_last>0)
		{
				X_COSTT_Output_Inverted(Pulse);//��ʱ��ת
		}
		else
		{
		    X_COSTT_Output_clockwise(Pulse);//˳ʱ��ת
		}
		TIM_ClearITPendingBit(TIM4 , TIM_IT_Update);  		 
	}		 	
}