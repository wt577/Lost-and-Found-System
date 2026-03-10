#ifndef __USART_H
#define __USART_H
void Usart_Init(uint32_t bound);
void Usart_SendByte(uint8_t Byte);
void Usart_SendArray(uint8_t *Array,uint16_t Length);
void Usart_SendString(char *Usart);
uint8_t USART_GetRxFlag(void);
uint8_t USART_GetRxData(void);
void USART1_IRQHandler(void);
extern uint8_t RxData;
#endif
