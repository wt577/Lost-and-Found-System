#include "stm32f10x.h"                  // Device header
#include "KEY.h"
#include "OLED.h"
#include "MC.h"
#include "Motor.h"
#include "ADC.h"
void MC_Start(uint8_t MC )
{
	if(MC)
	{
	AC=0;
	uint8_t KEY;
	KEY=Key_GetNum();
	OLED_ShowChinese (49,16,"手动模式");
	if(KEY ==1){
		Motor_Start (1);}
	if(KEY==2){
		Motor_Stop();}
		if(AD3<20)
		{
			AC=1;
			MC=0;
		}	
	}
}
