#include "stm32f10x.h"                  // Device header
#include "IIC.h" 

void AT24_Init(void)
{
	IIC_Init();
}

void AT24_Write_Byte(uint8_t WAddr,uint8_t Data)
{
	MyI2C_Start ();
	MyI2C_SendByte (0xA0);
	MyI2C_ReceiveAck ();
	MyI2C_SendByte (WAddr);
	MyI2C_ReceiveAck ();
	MyI2C_SendByte (Data);
	MyI2C_ReceiveAck ();
	MyI2C_Stop ();
}

uint8_t AT24_Recive_Byte(uint8_t RAddr)
{
	uint8_t data;
	MyI2C_Start ();
	MyI2C_SendByte (0xA0);
	MyI2C_ReceiveAck ();
	MyI2C_SendByte (RAddr);
	MyI2C_ReceiveAck ();
	MyI2C_Start ();//IICÖØ¸´¿ªÆô
	MyI2C_SendByte (0xA1);
	MyI2C_ReceiveAck ();	
	data=MyI2C_ReceiveByte ();
	MyI2C_SendAck(1);
	MyI2C_Stop();
	return data;
}
	
uint8_t AT24_Check(void)
{
	uint8_t temp;
	MyI2C_Start ();
	MyI2C_SendByte (0xA0);
	temp=MyI2C_ReceiveAck ();
	return temp;
}

