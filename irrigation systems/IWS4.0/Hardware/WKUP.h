#ifndef __WKUP_H
#define __WKUP_H
void Sys_Standby(void);
void Sys_Enter_Standby(void);
uint8_t Check_WKUP(void);
void WKUP_Init(void);
void EXTI_IRQHandler(void);
#endif
