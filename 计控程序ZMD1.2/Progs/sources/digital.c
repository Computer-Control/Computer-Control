//��·�����������������
//���ߣ�������
#include "digital.h"
void digitalout(u8 outdata)
{
	u16 i;
	WR_L;
	DigitalOutPinAd_L;
	i=GPIO_ReadOutputData(PrinterPort);
	GPIO_Write(PrinterPort, (u16) (outdata|i) );//��Ӱ���8λ״̬
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
