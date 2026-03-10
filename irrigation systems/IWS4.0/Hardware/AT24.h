#ifndef __AT24_H
#define __AT24_H

void AT24_Init(void);
void AT24_Write_Byte(uint8_t WAddr,uint8_t Data);
uint8_t AT24_Recive_Byte(uint8_t RAddr);
uint8_t AT24_Check(void);

#endif
