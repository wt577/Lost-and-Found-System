#ifndef __USART3_H
#define __USART3_H
void Usart3_Init(uint32_t bound);
void Usart3_SendByte(uint8_t Byte);
void Usart3_SendArray(uint8_t *Array,uint16_t Length);
void Usart3_SendString(char *Usart);
uint8_t USART3_GetRxFlag(void);
uint8_t USART3_GetRxData(void);
void USART3_IRQHandler(void);
extern uint8_t Rx3Data;
void u3_printf(char* fmt,...) ;
#endif
