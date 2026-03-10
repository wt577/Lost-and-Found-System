#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "USART3.h" 
#include "stdio.h"  
#include "ADC.h"
#include "MC.h"
#include "AC.h"
void WIFI_Connect(void)
{
	Usart3_SendString("AT\r\n");
	Delay_ms (1000);
	Usart3_SendString("AT+CWMODE=1\r\n");
	Delay_ms (1000);
	u3_printf("AT+CWJAP=\"%s\",\"%s\"\r\n","Xiaomi 13","88888889");
	Delay_ms (10000);
	u3_printf("AT+ATKCLDSTA=\"%s\",\"%s\"\r\n","16951906983209681796","12345678");
	Delay_ms (2000);	
}

void Data_UP(void)
{
	u3_printf("Light:%d\r\n",AD3);
	u3_printf("DHT:%d\r\n",DHT);
	if(AC==1){
		u3_printf("自动模式\r\n");	
	}else if(MC==1)
 {
		u3_printf("手动模式\r\n");	
	}	
}


