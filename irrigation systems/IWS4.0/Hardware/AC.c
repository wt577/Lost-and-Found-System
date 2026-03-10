#include "stm32f10x.h"                  // Device header
#include "Adc.h"  
#include "Motor.h" 
#include "OLED.h"
#include "MC.h"
void AC_Start(uint8_t AC)
{
	if(AC)
	{
	MC=0;
	OLED_ShowChinese (49,17,"自动模式");
  if(AD3<40)
	{		
		if(DHT <50){
			Motor_Start(1);
		}else if(DHT>75){
			Motor_Stop();
		}
	}else
	{
	Motor_Stop();
	}
 }
}

