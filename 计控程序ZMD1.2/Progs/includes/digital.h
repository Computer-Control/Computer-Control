//八路数字量独立输入输出接口
//作者：周明德
#ifndef _DIGITAL_H_
#define _DIGITAL_H_

#include "keyboard.h"
#include "printer.h"

#define DigitalOutPinAddress	GPIO_Pin_12
#define DigitalinPinAddress	GPIO_Pin_13

#define DigitalOutPinAd_L	GPIO_ResetBits(PrinterPort,DigitalOutPinAddress)
#define DigitalOutPinAd_H	GPIO_SetBits(PrinterPort,DigitalOutPinAddress)

#define DigitalinPinAd_L	GPIO_ResetBits(PrinterPort,DigitalinPinAddress)
#define DigitalinPinAd_H	GPIO_SetBits(PrinterPort,DigitalinPinAddress)

u8 digitalin(void);
void digitalout(u8 outdata);
#endif
