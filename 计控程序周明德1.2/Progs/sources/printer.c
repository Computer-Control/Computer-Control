/*��ӡ������
���ߣ�������*/
#include "printer.h"
#include "stdio.h"
/**************************************************************************************
 * ��  �� : ���ֽڴ�ӡ����
 * ��  �� : 
 * ����ֵ : ��
 **************************************************************************************/
void print_one(u8 ch)
{
	u16 i;
	WR_L;//���дʹ��
	APrinter_L;
	while(BSY);
	STB_H;
	i=GPIO_ReadOutputData(PrinterPort);
	GPIO_Write(PrinterPort, (u16) (ch|i) );//��Ӱ���8λ״̬
	STB_L;
	__nop();
	__nop();
	STB_H;
	WR_H;
	APrinter_H;
}
/**************************************************************************************
 * ��  �� : �ض���c�⺯��printf����ӡ��
 * ��  �� : 
 * ����ֵ : ��
 **************************************************************************************/
int fputc(int ch, FILE *f)
{
		print_one((uint8_t) ch);	
		return (ch);
}
/**************************************************************************************
 * ��  �� : ��ӡ��GPIO��ʼ������
 * ��  �� : 
 * ����ֵ : ��
 **************************************************************************************/
void printerInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|PinPrinterSTB;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(PrinterPort, &GPIO_InitStructure);	//��ʼ���������ź�STB����
  
	GPIO_InitStructure.GPIO_Pin = PinPrinterBSY;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //��������
 	GPIO_Init(PrinterPort, &GPIO_InitStructure);	  //��ʼ��BSY����
}


