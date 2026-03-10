#include "stm32f10x.h"                  // Device header
#include "TIM.h"
void PWM_Init(void)
{
//	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
//	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1|GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);

//	
//  GPIO_ResetBits (GPIOA,GPIO_Pin_1 |GPIO_Pin_2);	
}

void Motor_Start(uint8_t setnum)
{
	switch (setnum)
	{
		case 1:
				PWM_SetCompare2(100);
				PWM_SetCompare3(100);break ;
    case 2:
			PWM_SetCompare3(100);
		  PWM_SetCompare2(100);	break;		
	}		
}

void Motor_Stop(void)
{
	  PWM_SetCompare2(0);
	  PWM_SetCompare3(0);
}
