#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "ADC.h"
#include "KEY.h"
#include "OLED.h"
#include "AC.h"
#include "MC.h"
#include "USART.h"
#include "DHT11.h"
#include "ESP8266.h"
uint8_t KEY;
uint32_t t=0;
void TIM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimBaseStructure;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);		//选择TIM2为内部时钟，若不调用此函数，TIM默认也为内部时钟	
	
	TIM_TimBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;
	TIM_TimBaseStructure.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimBaseStructure.TIM_Period =100-1;
	TIM_TimBaseStructure.TIM_Prescaler =720-1;
	TIM_TimBaseStructure.TIM_RepetitionCounter =0;
	TIM_TimeBaseInit (TIM2 ,&TIM_TimBaseStructure);
														
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit (&TIM_OCInitStructure);
	

	TIM_OCInitStructure.TIM_OCMode =TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState =TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse =0;//初始CCR值
	TIM_OC2Init (TIM2 ,&TIM_OCInitStructure);
	
//	TIM_OCInitStructure.TIM_OCMode =TIM_OCMode_PWM1;
//	TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;
//	TIM_OCInitStructure.TIM_OutputState =TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_Pulse =0;//初始CCR值
//	TIM_OC3Init (TIM2 ,&TIM_OCInitStructure);	
	
	TIM_Cmd (TIM2,ENABLE);
}

void PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2 (TIM2 ,Compare );
}

void PWM_SetCompare3(uint16_t Compare)
{
//	TIM_SetCompare3 (TIM2,Compare);
}

void TIM3_4_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	TIM_InternalClockConfig(TIM3);		//选择TIM2为内部时钟，若不调用此函数，TIM默认也为内部时钟	
	
	TIM_TimBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;
	TIM_TimBaseStructure.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimBaseStructure.TIM_Period =1000-1;
	TIM_TimBaseStructure.TIM_Prescaler =7200-1;
	TIM_TimBaseStructure.TIM_RepetitionCounter =0;
	TIM_TimeBaseInit (TIM3 ,&TIM_TimBaseStructure);
	
	TIM_TimBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;
	TIM_TimBaseStructure.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimBaseStructure.TIM_Period =1000-1;
	TIM_TimBaseStructure.TIM_Prescaler =7200-1;
	TIM_TimBaseStructure.TIM_RepetitionCounter =0;
	TIM_TimeBaseInit (TIM4 ,&TIM_TimBaseStructure);
	
	TIM_ITConfig (TIM3,TIM_IT_Update,ENABLE);//使能定时器中断，允许更新中断
	TIM_ITConfig (TIM4,TIM_IT_Update,ENABLE);//使能定时器中断，允许更新中断
	
	NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;//响应优先级
	NVIC_Init (&NVIC_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel =TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;//响应优先级
	NVIC_Init (&NVIC_InitStructure);	
	
	TIM_Cmd (TIM3,ENABLE);
	TIM_Cmd (TIM4,ENABLE);
}
	

void TIM4_IRQHandler(void )
{
	if(TIM_GetITStatus (TIM4,TIM_IT_Update)!=RESET )
	{	
		TIM_ClearFlag (TIM4,TIM_IT_Update);
			AC_Start (AC);
			MC_Start (MC);

	}
}

void TIM3_IRQHandler(void )
{
	if(TIM_GetITStatus (TIM3,TIM_IT_Update)!=RESET )
	{
		TIM_ClearFlag (TIM3,TIM_IT_Update);
		Light_LED();
		OLED_ShowNum(65, 33, DHT,4, OLED_8X16);
	  OLED_ShowNum(65, 49, AD3,4, OLED_8X16);
		OLED_Update();	
		t++;
		if(t>=15)
		{
		Data_UP();//上传数据至云端
		t=0;
		}
	}
}
