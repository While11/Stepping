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
u32 X_CosTTNum = 0;										// X ���ٽ׶ε��������
char flag_angle=1;
int main(void)
{
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2

	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	LED_Init();
	OLED_Init();
	
	
	X_GPIO_Init();
	Adc_Init();         //��ʼ��ADC
	TIM4_Config();      //�ڽǶ�ʱ����
	X_Calculate_SpeedList();//���ɼӼ��ٽ׶ε��ٶȱ�
	X_PWM_S_Output_Right();//��ʱ�����������Ƶ����TIM3��ʼ��
	while(1);

}




