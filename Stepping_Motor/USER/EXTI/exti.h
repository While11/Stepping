#ifndef		__EXTI__H
#define   __EXTI__H

#include "sys.h"
#include "key.h"
#include "led.h"
#include "delay.h"
#include "X_Step_Motor.h"

void EXTIx_Init(void);
void EXTI4_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI2_IRQHandler(void);

#endif	/* __EXTI_H */

/****************************END OF FILE****************************/
