#ifndef __ADC_H
#define __ADC_H
void Adc_DMA_Init(void );
uint16_t AD_GetValue(uint8_t ADC_Channel);
uint16_t DHT11_Get(void);
uint16_t Light_Get(void);
void Light_LED(void);
extern uint16_t AD3,AD4;	//定义AD值变量
extern uint8_t T,DHT;
extern uint16_t AD_Value[2];
#endif

