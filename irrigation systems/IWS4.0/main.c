#include "stm32f10x.h"                  // Device header
#include "LED.h" 
#include "KEY.h" 
#include "ADC.h" 
#include "OLED.h"
#include "AC.h"
#include "TIM.h"
#include "USART.h"
#include "USART3.h"
#include "DMA.h"
#include "WKUP.h"
#include "MC.h"
#include "Delay.h"
#include "Motor.h"
#include "AT24.h"
#include "DHT11.h"
#include "ESP8266.h"
uint16_t AD3,AD4;	//定义AD值变量
uint8_t T,DHT;
uint8_t AC=0,MC=0;

int main()
{
	LED_Init();
	KEY_Init();
	Usart_Init(115200);
	Usart3_Init(115200);//必须为115200，WIFI模块需115200
	WIFI_Connect();//等待WIFI连接
	Adc_DMA_Init();
	OLED_Init();
	PWM_Init();
  TIM_Init ();
	TIM3_4_Init();
	AT24_Init();
	DHT_Init ();
//	WKUP_Init();
	if(AT24_Recive_Byte (10)==1)
	{
		AC=1;
	}
	if(AT24_Recive_Byte (11==1))
	{
		MC=1;
	}
	OLED_ShowChinese(0,17,"模式");
	OLED_ShowChinese(0,33,"湿度");
	OLED_ShowChinese(0,49,"光照强度");
	while(1)
	{
   	DHT11_Read_Data (&T,&DHT);
  }
	
}
