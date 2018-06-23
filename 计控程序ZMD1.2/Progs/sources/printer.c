/*打印机程序
作者：周明德*/
#include "printer.h"
#include "stdio.h"
/**************************************************************************************
 * 描  述 : 单字节打印程序
 * 入  参 : 
 * 返回值 : 无
 **************************************************************************************/
void print_one(u8 ch)
{
	u16 i;
	WR_L;//输出写使能
	APrinter_L;
	while(BSY);
	STB_H;
	i=GPIO_ReadOutputData(PrinterPort);
	GPIO_Write(PrinterPort, (u16) (ch|i) );//不影响高8位状态
	STB_L;
	__nop();
	__nop();
	STB_H;
	WR_H;
	APrinter_H;
}
/**************************************************************************************
 * 描  述 : 重定向c库函数printf到打印机
 * 入  参 : 
 * 返回值 : 无
 **************************************************************************************/
int fputc(int ch, FILE *f)
{
		print_one((uint8_t) ch);	
		return (ch);
}
/**************************************************************************************
 * 描  述 : 打印机GPIO初始化程序
 * 入  参 : 
 * 返回值 : 无
 **************************************************************************************/
void printerInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|PinPrinterSTB;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(PrinterPort, &GPIO_InitStructure);	//初始化数据引脚和STB引脚
  
	GPIO_InitStructure.GPIO_Pin = PinPrinterBSY;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //浮空输入
 	GPIO_Init(PrinterPort, &GPIO_InitStructure);	  //初始化BSY引脚
}


