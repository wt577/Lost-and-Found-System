#include "stm32f10x.h"                  // Device header
#include "stdio.h" 
#include "MC.h"
#include "WKUP.h"
#include "AT24.h"
#include "OLED.h"

uint8_t USART_RxData;		//定义串口接收的数据变量
uint8_t USART_RxFlag;		//定义串口接收的标志位变量
uint8_t RxData;
uint16_t USART_RX_STA=0;
uint8_t USART_RX_BUF[200];//接收缓冲
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (uint8_t) ch;      
	return ch;
}

void Usart_Init(uint32_t bound)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE );
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init (GPIOA ,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init (GPIOA ,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate =bound;
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;//硬件流控制，无需
	USART_InitStructure.USART_Mode =USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity =USART_Parity_No;//奇偶校验位
	USART_InitStructure.USART_StopBits =USART_StopBits_1;
	USART_InitStructure.USART_WordLength =USART_WordLength_8b;
	USART_Init (USART1 ,&USART_InitStructure);
	
	USART_ITConfig (USART1 ,USART_IT_RXNE ,ENABLE );
	
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2 );
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel =USART1_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
	NVIC_Init (&NVIC_InitStructure);
	
	USART_Cmd(USART1 ,ENABLE );
}

void Usart_SendByte(uint8_t Byte)//串口发送一个字节
{
	USART_SendData (USART1 ,Byte);
	while(USART_GetFlagStatus (USART1 ,USART_FLAG_TXE)==RESET);
}

void Usart_SendArray(uint8_t *Array,uint16_t Length)//串口发送数组
{
	uint16_t i;
	for(i=0;i<Length;i++)
	{
		Usart_SendByte (Array[i]);
	}
}

void Usart_SendString(char *Usart)
{
	uint8_t i;
	for (i = 0; Usart[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
	{
		Usart_SendByte(Usart[i]);		//依次调用SendByte发送每个字节数据
	}
}

uint8_t USART_GetRxFlag(void)//获取标志位
{
	if(USART_RxFlag==1)
	{
		USART_RxFlag=0;
		return 1;
	}
	return 0;
}

uint8_t USART_GetRxData(void)
{
	return USART_RxData;	
}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus (USART1 ,USART_IT_RXNE )==SET)
	{
		uint8_t Res;
		Res=USART_ReceiveData (USART1 );
		if((USART_RX_STA&0x8000)==0)//接收完成
		{
			if(USART_RX_STA &0x4000)
			{
				if(Res!=0x0a)USART_RX_STA =0;//接受错误，重开
				else USART_RX_STA |=0x8000;
			}
			else
			{
				if(Res==0x0d)USART_RX_STA |=0X4000;
				else 
				{
					USART_RX_BUF[USART_RX_STA &0X3FFF]=Res;
					USART_RX_STA++;
					if(USART_RX_STA>199)USART_RX_STA =0;
			  }				
			}
		}
	if(USART_RX_STA&0x8000)
	{					   
		if(USART_RX_BUF[0]=='I'&&USART_RX_BUF[1]=='W'&&USART_RX_BUF[2]=='S')
		{
			if(USART_RX_BUF[4]=='A')
			{
				if(USART_RX_BUF[6]=='1')
				{
					AC=1;
					AT24_Write_Byte(10,AC);
				}
				else if(USART_RX_BUF[6]=='0')
				{
					AC=0;
					AT24_Write_Byte(10,AC);
				}

			}
			if(USART_RX_BUF[4]=='M')
			{
				if(USART_RX_BUF[6]=='1')
				{
					MC=1;
					AT24_Write_Byte(11, MC);
				}
				else if(USART_RX_BUF[6]=='0')
				{
					MC=0;
					AT24_Write_Byte(11, MC);
				}
			}
			if(USART_RX_BUF[4]=='S'&&USART_RX_BUF[5]=='T'&&USART_RX_BUF[6]=='B'&&USART_RX_BUF[7]=='Y')
			{
				if(USART_RX_BUF[9]=='1'&&USART_RX_BUF[11]=='1')
				{
					OLED_ShowChinese (49,17,"待机模式");
					WKUP_Init();
				}
			}
			
		}
		
		USART_RX_STA=0;
	}
//		USART_RxData =USART_ReceiveData (USART1 );
//		USART_RxFlag=1;
//		USART_ClearITPendingBit (USART1 ,USART_IT_RXNE );
//		RxData=USART_GetRxData();
//		Usart_SendByte (RxData );
//		Usart_SendString ("IWS");
	}
	
}

