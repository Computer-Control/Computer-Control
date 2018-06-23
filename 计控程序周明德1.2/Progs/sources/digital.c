//八路数字量输入输出程序
//作者：周明德
#include "digital.h"
void digitalout(u8 outdata)
{
	u16 i;
	WR_L;
	DigitalOutPinAd_L;
	i=GPIO_ReadOutputData(PrinterPort);
	GPIO_Write(PrinterPort, (u16) (outdata|i) );//不影响高8位状态
	DigitalOutPinAd_H;
	WR_H;
}
u8 digitalin(void)
{
	u16 gpioreads;
	RD_L;
	DigitalinPinAd_L;
	gpioreads=GPIO_ReadInputData(PORTDATA);
	DigitalinPinAd_H;
	RD_H;
	gpioreads=gpioreads&0xff;
	return (u8)gpioreads;
}
