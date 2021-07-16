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
u32 X_CosTTNum = 0;										// X ���ٽ׶ε��������
int main(void)
{
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	LED_Init();
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	
	X_GPIO_Init();
	OLED_Init();
	Adc_Init();         //��ʼ��ADC
	TIM4_Config();
	X_Calculate_SpeedList();//���ɼӼ��ٽ׶ε��ٶȱ�
	X_PWM_S_Output_Right();//��ʱ������
	while(1)
	{ 
//		printf("%d\r\n",1);
	//	printf("%d",0x01);
		
//		X_COSTT_Output_Right(3200-151);
		//	while(COSTT_end!=1);//�ȴ�endΪ1��endΪ1��ֹͣ
	//	COSTT_end=0;//���end������0��ֹͣ
	////	X_PWM_S_Output_Left();
	//	X_COSTT_Output_Left(3200-151);
//		peace=1;//����ֹͣ
//		adcx=Get_Adc_Average(ADC_Channel_5,20);//��ȡͨ��5��ת��ֵ��20��ȡƽ��
//		printf("%d\r\n",adcx);    //��ʾADCC�������ԭʼֵ
//		temp=(float)adcx*(3.3/4096);          //��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111  //12λADC
//		printf("%f\r\n",temp);
//		delay_ms(250);	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	}
}




