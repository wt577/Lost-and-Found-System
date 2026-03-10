#include "stm32f10x.h"                  // Device header

void LED_Init(void )
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB,ENABLE );
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5 |GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	
	GPIO_Init (GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8;
	GPIO_Init (GPIOB,&GPIO_InitStructure);
}

void LED2_OFF(void )
{
	GPIO_SetBits (GPIOA ,GPIO_Pin_5 );
}
void LED2_ON(void )
{
	GPIO_ResetBits (GPIOA ,GPIO_Pin_5 );
}

void LED3_OFF(void )
{
	GPIO_SetBits (GPIOA ,GPIO_Pin_6 );
}
void LED3_ON(void )
{
	GPIO_ResetBits (GPIOA ,GPIO_Pin_6 );
}

void LED4_OFF(void )
{
	GPIO_SetBits (GPIOA ,GPIO_Pin_7 );
}
void LED4_ON(void )
{
	GPIO_ResetBits (GPIOA ,GPIO_Pin_7 );
}

void LED5_OFF(void )
{
	GPIO_SetBits (GPIOB ,GPIO_Pin_8 );
}
void LED5_ON(void )
{
	GPIO_ResetBits (GPIOB ,GPIO_Pin_8 );
}
