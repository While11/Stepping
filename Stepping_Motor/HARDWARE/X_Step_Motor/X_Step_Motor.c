/**
******************************************************************************
*反转电平输出脉冲，输出固定脉冲数的PWM波
84MHz！  高级定时器 168MHz
******************************************************************************
*/
#include "X_Step_Motor.h"
#include "usart.h"
#include  "led.h"
#include "adc.h"
int cnt=0;
extern float angle,angle_last;
extern float angle,angle_last,angle_first;
extern u16 adcx;
extern u32 Pulse;
extern char flag_angle;
X_SpeedList_TypeDef	X_Speed;
extern char COSTT_end;
extern char peace;
char Accel_flag=0;
char flag_pulse=0;

u32 X_Step_Position = 0;			// 当前位置
u8	X_Motion_Status = 0;			// 0:停止，1:加速，2:匀速，3:减速
float X_Fre_List[X_SpeedList_LEN];			// 频率列表
u16 X_Toggle_Pulse[X_SpeedList_LEN];		// 频率对应的脉冲个数
extern u32 X_CosTTNum;										// X 匀速阶段的脉冲个数

/******************** X - GPIO *********************/
void X_GPIO_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOA, ENABLE);
	// GPIOA A6
	GPIO_PinAFConfig( GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;		// TIM3_CH1 - PA6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			// 复用
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		// 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			// 上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// GPIOA A7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			// 输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		// 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			// 上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/******************** X - TIM3 *********************/
void X_TIM3_Config(void)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	// 时钟频率设置
	TIM_TimeBaseStructure.TIM_Prescaler = X_TIM3_Prescaler - 1;
	TIM_TimeBaseStructure.TIM_Period = X_TIM3_Period;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIM3, &TIM_TimeBaseStructure);
	
	TIM_OCInitTypeDef		TIM_OCInitStructure;
	// 设置工作模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;			// 比较输出模式，反转输出
	TIM_OCInitStructure.TIM_Pulse = X_TIM3_Pulse / 2;									// 让第一个脉冲是500Hz
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	// 使能比较输出					
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;			// 输出为正逻辑
	TIM_OC1Init( TIM3, &TIM_OCInitStructure);								// 初始化
	TIM_OC1PreloadConfig( TIM3, TIM_OCPreload_Disable);		// CH1预装载失能，修改	
	
	NVIC_InitTypeDef	NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure);	
}
/**
	* 函数功能：速度表计算函数
	* 输入参数：fre_List[], 各个步的频率数组
	*						toggle_pulse[], 对应频率的Period
	*						step[],	变速过程中步数要求， 实际加减速阶段总步数 = 1 + 2 * step = 101
	*   				fre_min - 初始频率，Hz / step/s
  *           float X_FREQ_MAX  最高工作频率
	*						jerk, S型的平滑程度，越小越平滑
	*	说    明：根据速度曲线和加速时间，将数据密集化，即计算每一步的速度值，并存放在内存中。
  *           这里采用的数学模型是匀变速直线运动，加加速-减加速-匀速-加减速-减减速。
**/
void X_Calculate_SpeedList(void)
{
	u8 i_x = 0;
	X_Speed.X_Step = X_Accel_Step;
	X_Speed.X_Fre_Min = X_FREQ_MIN;
	X_Speed.X_Fre_Max = X_FREQ_MAX;
	X_Speed.X_Jerk = X_JERK;
	float num_x;										// 其实是整数
	float molecule_x;				// 公式分子
	float denominator_x;		// 公式分母
	num_x = X_Speed.X_Step / 2;
//	step = step + 1;			// 有一个起始频率，现在用的就是50步。
	molecule_x = X_Speed.X_Fre_Max - X_Speed.X_Fre_Min;
//	X_CosTTNum = X_PulseNum - (2 * X_Speed.X_Step + 1);
	
	for (i_x = 0; i_x < (u8)X_Speed.X_Step; i_x++)
	{
		denominator_x = 1.0f + (float)exp(-X_Speed.X_Jerk * (i_x - num_x) / num_x);
		X_Fre_List[i_x] = X_Speed.X_Fre_Min + molecule_x / denominator_x;
		X_Toggle_Pulse[i_x] = (u16)(X_TIM3_FREQ / X_Fre_List[i_x]);
//		printf("%d",1);
//		printf("%d step: frequency: %.2f, pulse:%d.\r\n", i_x, X_Fre_List[i_x], X_Toggle_Pulse[i_x]);
	}
	}//i_X范围为0-X_Speed.X_Step-1

void X_PWM_S_Output_Right(void)//逆时针
{
	X_Step_Position = 0;
	X_Motion_Status = X_ACCEL;
	
//	GPIO_SetBits(GPIOA, GPIO_Pin_7);
//	delay_us(100);
	GPIO_ResetBits(GPIOA, GPIO_Pin_7);
	delay_us(125);   //  > 125us
	
	X_TIM3_Config();
	TIM_ClearITPendingBit( TIM3, TIM_IT_CC1);
	TIM_ITConfig( TIM3, TIM_IT_CC1, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

void X_PWM_S_Output_Left(void)//顺时针
{
	X_Step_Position = 0;
	X_Motion_Status = X_ACCEL;
	
//	GPIO_ResetBits(GPIOA, GPIO_Pin_7);
//	delay_us(100);   
	GPIO_SetBits(GPIOA, GPIO_Pin_7);
	delay_us(125);		//  > 125us
	
	X_TIM3_Config();
	TIM_ClearITPendingBit( TIM3, TIM_IT_CC1);
	TIM_ITConfig( TIM3, TIM_IT_CC1, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

void X_COSTT_Output_Inverted(u32 X_PulseNum)//最大速度匀速运行
{
	cnt++;
	if(X_PulseNum>60)
	{
		flag_pulse=0;
	X_Step_Position = 0;
	X_Motion_Status = X_COSTT;
	X_CosTTNum = X_PulseNum;
	GPIO_ResetBits(GPIOA, GPIO_Pin_7);
	delay_us(125);   //  > 125us
		printf("%d\r\n",cnt);
	}
	else 
	{
	 flag_pulse++;
	 if(flag_pulse==3)
	 X_Motion_Status = X_DECEL;
	}
	
}

void X_COSTT_Output_clockwise(u32 X_PulseNum)//顺时针
{
	X_Step_Position = 0;
	X_Motion_Status = X_COSTT;
	X_CosTTNum = X_PulseNum;
	

	GPIO_SetBits(GPIOA, GPIO_Pin_7);
	delay_us(125);		//  > 125us
}


void X_Stop(void)
{
	TIM_Cmd(TIM3, DISABLE);		// 关闭定时器
	TIM_ITConfig( TIM3, TIM_IT_CC1, DISABLE);
	X_Step_Position = 0;
	X_Motion_Status = X_STOP;
	
}



/******************** X - IRQ *********************/
void X_TIM3_IRQHandler(void)
{
	u16 X_TIM_Count = 0;
	static u8 j_x = 0;
	volatile static float X_TIM_Pulse = (X_TIM3_Pulse / 2);			// 第一个脉冲 500Hz
	
	if (TIM_GetITStatus( TIM3, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit( TIM3, TIM_IT_CC1);		// 清楚中断标志位
		X_TIM_Count = TIM_GetCapture1(TIM3);
		j_x++;
		if (j_x == 2)
		{
			j_x = 0;
			if (X_Motion_Status == X_ACCEL)
			{
				X_Step_Position++;
				if (X_Step_Position < X_SpeedList_LEN)
				{
					X_TIM_Pulse = X_Toggle_Pulse[X_Step_Position - 1] / 2;   // -1
				}
				else
				{
					X_TIM_Pulse = X_Toggle_Pulse[X_Step_Position - 1] / 2;		// 加速阶段第50步，配置好下一个状态
				  X_Motion_Status = X_COSTT;	
					X_Step_Position = 0;
					
				}
			}
			else if (X_Motion_Status == X_COSTT)
			{
				X_Step_Position++;			// 当前将要执行的步数 Step_Position - 1 是已经执行的步数
				//前4周一个频率,中间4周一个频率，后四周一个频率
				if(cnt<=250)
				{X_TIM_Pulse = X_Toggle_Pulse[X_SpeedList_LEN - 1] / 2;}
				else if(cnt>250&&cnt<=370)
				{
				X_TIM_Pulse = X_Toggle_Pulse[X_SpeedList_LEN - 2] / 2;
				}
				else if(cnt>370)
				{
				X_TIM_Pulse = X_Toggle_Pulse[X_SpeedList_LEN - 3] / 2;
				}
        
			}
			else if (X_Motion_Status == X_DECEL)
			{
				X_Step_Position++;
				if (X_Step_Position < (X_SpeedList_LEN + 1))
				{
					X_TIM_Pulse = X_Toggle_Pulse[X_SpeedList_LEN - X_Step_Position] / 2;
				}
				else
				{
					TIM_ITConfig( TIM3, TIM_IT_CC1, DISABLE);
					TIM_Cmd(TIM3, DISABLE);		// 关闭定时器
					X_Step_Position = 0;
					X_Motion_Status = X_STOP;
				}
			}
			else if (X_Motion_Status == X_UNIFM)
			{
				X_Step_Position++;
				if (X_Step_Position < X_CosTTNum)
				{
					X_TIM_Pulse = X_TIM3_Pulse / 2;
				}
				else
				{
					TIM_ITConfig( TIM3, TIM_IT_CC1, DISABLE);
					TIM_Cmd(TIM3, DISABLE);		// 关闭定时器
					X_Step_Position = 0;
					X_Motion_Status = X_STOP;
				}
			}
		}
	}
	TIM_SetCompare1(TIM3, (u16)(X_TIM_Count + X_TIM_Pulse));
}

/****************************END OF FILE****************************/
