//打印机接口函数
//作者：周明德
#ifndef _PRINTER_H
#define _PRINTER_H
#include "stm32f10x_gpio.h" 
#include "digital.h"
#define PinPrinterBSY	GPIO_Pin_9
#define PinPrinterSTB GPIO_Pin_8
#define PrinterPinAddress	GPIO_Pin_11
#define PinPrinterWR  GPIO_Pin_14

#define PrinterPort GPIOC
#define BSY			GPIO_ReadOutputDataBit(PrinterPort,PinPrinterBSY)
#define STB_H		GPIO_SetBits(PrinterPort,PinPrinterSTB)
#define STB_L		GPIO_ResetBits(PrinterPort,PinPrinterSTB)
#define WR_L		GPIO_ResetBits(PrinterPort,PinPrinterWR)
#define WR_H		GPIO_SetBits(PrinterPort,PinPrinterWR)
#define APrinter_L	GPIO_ResetBits(PrinterPort,PrinterPinAddress)
#define APrinter_H	GPIO_SetBits(PrinterPort,PrinterPinAddress)

void print_one(u8 ch);
void printerInit(void);
#endif


