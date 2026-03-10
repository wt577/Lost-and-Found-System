#ifndef __TIM_H
#define __TIM_H

void TIM_Init(void);
void PWM_SetCompare2(uint16_t Compare);
void PWM_SetCompare3(uint16_t Compare);
void TIM2_IRQHandler(void );
void TIM3_4_Init(void);
void TIM3_IRQHandler(void );
void TIM4_IRQHandler(void );
#endif
