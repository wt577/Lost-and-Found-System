#include "stm32f10x.h"                  // Device header
#include "stdio.h" 
#include "stdarg.h"
#include "string.h"
#include "MC.h"
#include "AC.h"
#include "AT24.h"
#include "OLED.h"
#include "WKUP.h"
uint8_t USART3_RxData;		//定义串口接收的数据变量
uint8_t USART3_RxFlag;		//定义串口接收的标志位变量
uint8_t Rx3Data;
uint16_t USART3_RX_STA=0;
//串口接收发送缓存区		
u8 USART3_RX_BUF[600]; 			//USART3_MAX_RECV_LEN
u8 USART3_TX_BUF[600]; //USART3_MAX_SEND_LEN

void Usart3_Init(uint32_t bound)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE );
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init (GPIOB ,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init (GPIOB ,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate =bound;
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;//硬件流控制，无需
	USART_InitStructure.USART_Mode =USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity =USART_Parity_No;//奇偶校验位
	USART_InitStructure.USART_StopBits =USART_StopBits_1;
	USART_InitStructure.USART_WordLength =USART_WordLength_8b;
	USART_Init (USART3 ,&USART_InitStructure);
	
	USART_ITConfig (USART3,USART_IT_RXNE ,ENABLE );
	
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2 );
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel =USART3_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
	NVIC_Init (&NVIC_InitStructure);
	
	USART_Cmd(USART3 ,ENABLE );
}

void Usart3_SendByte(uint8_t Byte)//串口发送一个字节
{
	USART_SendData (USART3 ,Byte);
	while(USART_GetFlagStatus (USART3 ,USART_FLAG_TXE)==RESET);
}

void Usart3_SendArray(uint8_t *Array,uint16_t Length)//串口发送数组
{
	uint16_t i;
	for(i=0;i<Length;i++)
	{
		Usart3_SendByte (Array[i]);
	}
}

void Usart3_SendString(char *Usart)
{
	uint8_t i;
	for (i = 0; Usart[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
	{
		Usart3_SendByte(Usart[i]);		//依次调用SendByte发送每个字节数据
	}
}

uint8_t USART3_GetRxFlag(void)//获取标志位
{
	if(USART3_RxFlag==1)
	{
		USART3_RxFlag=0;
		return 1;
	}
	return 0;
}

uint8_t USART3_GetRxData(void)
{
	return USART3_RxData;	
}

//串口3,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void u3_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF);		//此次发送数据的长度
	for(j=0;j<i;j++)							//循环发送数据
	{
	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
		USART_SendData(USART3,USART3_TX_BUF[j]); 
	} 
}

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus (USART3 ,USART_IT_RXNE )==SET)
	{
		uint8_t Res;
		Res=USART_ReceiveData (USART3 );
		if((USART3_RX_STA&0x8000)==0)//接收完成
		{
			if(USART3_RX_STA &0x4000)
			{
				if(Res!=0x0a)USART3_RX_STA =0;//接受错误，重开
				else USART3_RX_STA |=0x8000;
			}
			else
			{
				if(Res==0x0d)USART3_RX_STA |=0X4000;
				else 
				{
					USART3_RX_BUF[USART3_RX_STA &0X3FFF]=Res;
					USART3_RX_STA++;
					if(USART3_RX_STA>199)USART3_RX_STA =0;
			  }				
			}
		}
	}
if(USART3_RX_STA&0x8000)
	{					   
		if(USART3_RX_BUF[0]=='I'&&USART3_RX_BUF[1]=='W'&&USART3_RX_BUF[2]=='S')
		{
			if(USART3_RX_BUF[4]=='A')
			{
				if(USART3_RX_BUF[6]=='1')
				{
					AC=1;
					AT24_Write_Byte(10,AC);
				}
				else if(USART3_RX_BUF[6]=='0')
				{
					AC=0;
					AT24_Write_Byte(10,AC);
				}

			}
			if(USART3_RX_BUF[4]=='M')
			{
				if(USART3_RX_BUF[6]=='1')
				{
					MC=1;
					AT24_Write_Byte(11, MC);
				}
				else if(USART3_RX_BUF[6]=='0')
				{
					MC=0;
					AT24_Write_Byte(11, MC);
				}
			}
			if(USART3_RX_BUF[4]=='S'&&USART3_RX_BUF[5]=='T'&&USART3_RX_BUF[6]=='B'&&USART3_RX_BUF[7]=='Y')
			{
				if(USART3_RX_BUF[9]=='1'&&USART3_RX_BUF[11]=='1')
				{
					OLED_ShowChinese (49,17,"待机模式");
					WKUP_Init();
				}
			}		
		}
		printf("%s",USART3_RX_BUF);
		USART3_RX_STA=0;
	}
}
