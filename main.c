#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "includes.h"
#include "OLED.h"
#include "adc_8.h"
#include "dac_8.h"


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
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;              
  GPIO_Init(GPIOA , &GPIO_InitStructure);   
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //�������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
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
  ADC1_Init();
	LCD_Init();
		
	delay_ms(500);	
	
	printf("\r\n **��ʼadת��**\r\n");
	
	DAC_init();
	
  while(1)
  {
		GPIO_SetBits(GPIOB , GPIO_Pin_9); 
    printf("\r\n *********************\r\n");	
		AD_Start(); 
    USART2_Tx_Puts();
 

		printf("\r\n *****����һ�β���*****\r\n"); 
		delay_ms(1500);
		LCD_Print(0, 0, "co2Ũ�ȣ�",TYPE16X16,TYPE8X16);
		LCD_Print(0, 16, "�¶�Ϊ��",TYPE16X16,TYPE16X16);
		LCD_Print(96, 16, "��",TYPE16X16,TYPE16X16);
		LCD_Print(0, 32, "ʪ��Ϊ��",TYPE16X16,TYPE6X8);
		LCD_Print(96, 32, "%RH",TYPE16X16,TYPE8X16);
		LCD_Print(0, 48, "����ǿ�ȣ�",TYPE16X16,TYPE6X8);
		
	} 		
}
