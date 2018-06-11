#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "includes.h"
#include "OLED.h"


/**************************************************************************************
 * ��  �� : ���鶨��
 **************************************************************************************/
int a[8];
/**************************************************************************************
 * ��  �� : GPIO/USART1��ʼ������
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable the GPIO_LED Clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO , ENABLE); 						 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB , ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	//OLED 
	GPIO_DeInit(GPIOB);	 //������GPIOB�Ĵ�������Ϊȱʡֵ
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6	
 
	
	GPIO_DeInit(GPIOA);	 //������GPIOA�Ĵ�������Ϊȱʡֵ
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //�������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //�������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_DeInit(GPIOC);	 //������GPIOC�Ĵ�������Ϊȱʡֵ
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //�������
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_DeInit(GPIOD);	 //������GPIOD�Ĵ�������Ϊȱʡֵ
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //��������
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/**************************************************************************************
 * ��  �� : MAIN����
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
int main(void)
{ 
	SystemInit();			//����ϵͳʱ��72MHZ
	GPIO_Configuration();
	USART1_Init();    //��ʼ������TIM 
	USART2_Init();	
	LCD_Init();
		
	delay_ms(500);	
	
	printf("\r\n **��ʼadת��**\r\n");
	
  while(1)
  {
		a[0]=GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_0);
		a[1]=GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_1);
		a[2]=GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_2);
		a[3]=GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_3);
		a[4]=GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_4);
		a[5]=GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_5);
		a[6]=GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_6);
		a[7]=GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_7);
		GPIO_ResetBits(GPIOC , GPIO_Pin_0);
		GPIO_ResetBits(GPIOC , GPIO_Pin_1);
		GPIO_ResetBits(GPIOC , GPIO_Pin_2);
		GPIO_ResetBits(GPIOC , GPIO_Pin_3);
		GPIO_ResetBits(GPIOC , GPIO_Pin_4);
		GPIO_ResetBits(GPIOC , GPIO_Pin_5);
		GPIO_ResetBits(GPIOC , GPIO_Pin_6);
		GPIO_ResetBits(GPIOC , GPIO_Pin_7);
		printf("\r\n *********************\r\n");	
		USART2_Tx_Puts();
 

		printf("\r\n *****����һ�β���*****\r\n"); 
		delay_ms(1500);
		LCD_Print(0, 0, "a[0]:",TYPE16X16,TYPE8X16);
		LCD_Print(64, 0, "a[1]:",TYPE16X16,TYPE8X16);
		LCD_Print(0, 16, "a[2]:",TYPE16X16,TYPE8X16);
		LCD_Print(64, 16, "a[3]:",TYPE16X16,TYPE8X16);
		LCD_Print(0, 32, "a[4]:",TYPE16X16,TYPE8X16);
		LCD_Print(64, 32, "a[5]:",TYPE16X16,TYPE8X16);
		LCD_Print(0, 48, "a[6]:",TYPE16X16,TYPE8X16);
		LCD_Print(64, 48, "a[7]:",TYPE16X16,TYPE8X16);
		
	} 		
}
