#include "stm32f10x.h"                  // Device header
#include "Delay.h"  
#include "DHT11.h" 
void DHT11_Rest(void)
{
	DHT11_Mode(OUT);
	DHT11_Low;
	Delay_ms(20);
	DHT11_Hight;
	Delay_us(20);
}

//等待回应
uint8_t DHT11_Check(void)
{
	uint8_t retry=0;
	DHT11_Mode(IN);
	while(GPIO_ReadInputDataBit (GPIOA ,GPIO_Pin_4)&&retry<100)
	{
		retry++;
		Delay_us (1);
	}
	if(retry>=100){
		return 1;}else{retry= 0;} 
	while(!GPIO_ReadInputDataBit (GPIOA ,GPIO_Pin_4)&&retry<100)
	{
		retry++;
		Delay_us (1);
	}
	if(retry>=100){
		return 1;}else{
		retry=0;
		return 0;} 	
}

uint8_t DHT11_Read_Bit(void)
{
	uint8_t t=0;
	while(GPIO_ReadInputDataBit(GPIOA ,GPIO_Pin_4)&&t<100)//等待变为低电平
	{
		t++;
		Delay_us(1);
	}
	t=0;
	while(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)&&t<100)//等待变高电平
	{
		t++;
		Delay_us(1);
	}
	Delay_us(40);//等待40us
	if(GPIO_ReadInputDataBit(GPIOA ,GPIO_Pin_4))return 1;
	else return 0;		
}

uint8_t DHT11_Read_Byte(void)
{
	uint16_t i,dat;
	for(i=0;i<8;i++)
	{
		dat<<=1;
		dat|=DHT11_Read_Bit ();
	}
		return dat;
}

uint8_t DHT11_Read_Data(uint8_t *T,uint8_t *DHT)
{
	uint8_t buf[5];
	uint8_t i;
	DHT11_Rest();
	if(DHT11_Check ()==0)
	{
		for(i=0;i<5;i++)
		{
			buf[i]=DHT11_Read_Byte ();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*DHT=buf[0];
			*T=buf[2];
		}
	}else return 1;
	return 0;
}

void  DHT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructur;
	
	GPIO_InitStructur.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStructur.GPIO_Pin =GPIO_Pin_4;
	GPIO_InitStructur.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA ,&GPIO_InitStructur);
	
	DHT11_Rest ();
}

void DHT11_Mode (uint8_t Mode)
{
	GPIO_InitTypeDef GPIO_InitStructur;
	if(Mode)
	{
	GPIO_InitStructur.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStructur.GPIO_Pin =GPIO_Pin_4;
	GPIO_InitStructur.GPIO_Speed =GPIO_Speed_50MHz;
	}else
	{
	GPIO_InitStructur.GPIO_Mode =GPIO_Mode_IN_FLOATING;
	GPIO_InitStructur.GPIO_Pin =GPIO_Pin_4;		
	}
	GPIO_Init(GPIOA ,&GPIO_InitStructur);
}
