//��չio�Ķ���������������
//���ߣ�������
#include "keyboard.h"
#include "digital.h"
void keyscan(u16 * results)
{
	u16 gpioreads;
	RD_L;
	A0_L;
	A1_H;
	gpioreads=GPIO_ReadInputData(PORTDATA);
	gpioreads=gpioreads&0xff;
	*results=gpioreads;
	__nop();
	__nop();
	A0_H;
	A1_L;
	gpioreads=GPIO_ReadInputData(PORTDATA);
	gpioreads=gpioreads&0xff;
	gpioreads<<=0x08;
	*results|=gpioreads;
	RD_H;
}
void keyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //��������
 	GPIO_Init(PORTDATA, &GPIO_InitStructure);	//��ʼ����������
  
	GPIO_InitStructure.GPIO_Pin = PinA1|PinA0|PinRD|DigitalinPinAddress;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz 
 	GPIO_Init(PORTCONTROL, &GPIO_InitStructure);	  //��ʼ���������ַ����
}
