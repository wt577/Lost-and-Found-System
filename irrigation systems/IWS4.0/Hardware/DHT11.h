#ifndef __DHT11_H
#define __DHT11_H

#define OUT 1
#define IN  0

#define DHT11_Low  GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define DHT11_Hight  GPIO_SetBits(GPIOA,GPIO_Pin_4)


void DHT11_Rest(void);
uint8_t DHT11_Check(void);
uint8_t DHT11_Read_Bit(void);
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_Read_Data(uint8_t *T,uint8_t *DHT);
void DHT_Init(void);
void DHT11_Mode (uint8_t Mode);
#endif
