//键盘驱动接口
//作者：周明德
#ifndef _KEYBOARD_H
#define _KEYBOARD_H
#include "stm32f10x_gpio.h" 
#
#define PinRD	  		GPIO_Pin_10
#define PinA0	  		GPIO_Pin_11
#define PinA1	  		GPIO_Pin_12
#define PORTCONTROL GPIOC
#define PORTDATA		GPIOB
#define RD_L				GPIO_ResetBits(PORTCONTROL,PinRD)
#define RD_H				GPIO_SetBits(PORTCONTROL,PinRD)
#define A0_L				GPIO_ResetBits(PORTCONTROL,PinA0)
#define	A0_H				GPIO_SetBits(PORTCONTROL,PinA0)
#define A1_L				GPIO_ResetBits(PORTCONTROL,PinA1)
#define	A1_H				GPIO_SetBits(PORTCONTROL,PinA1)

void keyscan(u16 *results);
void keyInit(void);

#endif
