#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h" 
#include "OLED.h" 
void Sys_Standby(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE );
	PWR_WakeUpPinCmd (ENABLE );
	PWR_EnterSTANDBYMode ();
}

void Sys_Enter_Standby(void)
{
	RCC_APB2PeriphResetCmd (0x01FC,DISABLE );
	Sys_Standby();
}

uint8_t Check_WKUP(void)
{
	uint8_t t=0;
	while(1)
	{
	if(GPIO_ReadInputDataBit (GPIOA ,GPIO_Pin_0 )==1)
	{
//    LED3_ON();
		t++;
		Delay_ms(30);
		if(t>100)//待机开启
		{
			LED2_OFF();
	    LED3_OFF();
			LED4_OFF();
			LED5_OFF();
			return 1;
		}
   }else //待机失败
		{
			LED2_ON();
	    LED3_ON();
			LED4_ON();
			LED5_ON();
			return 0;
		}
	}
}

void EXTI_IRQHandler(void)
{
	EXTI_ClearITPendingBit (EXTI_Line0 );
	if(Check_WKUP() )//待机？
	{
		Sys_Enter_Standby();
	}
}

//PA0 WKUP唤醒初始化
void WKUP_Init(void)
{	
  GPIO_InitTypeDef  GPIO_InitStructure;  		  
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);//使能GPIOA和复用功能时钟

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;	 //PA.0
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化IO
//使用外部中断方式
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);	//中断线0连接GPIOA.0

  EXTI_InitStructure.EXTI_Line = EXTI_Line0;	//设置按键所有的外部线路
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//设外外部中断模式:EXTI线路为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //上升沿触发
 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	// 初始化外部中断

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	if(Check_WKUP()==0) Sys_Standby();//不是开机,进入待机模式 
}


