#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "includes.h"
#include "OLED.h"
#include "adc_8.h"
#include "dac_8.h"


/**************************************************************************************
 * 描  述 : GPIO/USART1初始化配置
 * 入  参 : 无
 * 返回值 : 无
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
	GPIO_DeInit(GPIOB);	 //将外设GPIOB寄存器重设为缺省值
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOD3,6	
 
	
	GPIO_DeInit(GPIOA);	 //将外设GPIOA寄存器重设为缺省值
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;              
  GPIO_Init(GPIOA , &GPIO_InitStructure);   
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

/**************************************************************************************
 * 描  述 : MAIN函数
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
int main(void)
{
	SystemInit();			//设置系统时钟72MHZ
	GPIO_Configuration();
	USART1_Init();    //初始化配置TIM 
  USART2_Init();	
  ADC1_Init();
	LCD_Init();
		
	delay_ms(500);	
	
	printf("\r\n **开始ad转换**\r\n");
	
	DAC_init();
	
  while(1)
  {
		GPIO_SetBits(GPIOB , GPIO_Pin_9); 
    printf("\r\n *********************\r\n");	
		AD_Start(); 
    USART2_Tx_Puts();
 

		printf("\r\n *****结束一次采样*****\r\n"); 
		delay_ms(1500);
		LCD_Print(0, 0, "co2浓度：",TYPE16X16,TYPE8X16);
		LCD_Print(0, 16, "温度为：",TYPE16X16,TYPE16X16);
		LCD_Print(96, 16, "℃",TYPE16X16,TYPE16X16);
		LCD_Print(0, 32, "湿度为：",TYPE16X16,TYPE6X8);
		LCD_Print(96, 32, "%RH",TYPE16X16,TYPE8X16);
		LCD_Print(0, 48, "光照强度：",TYPE16X16,TYPE6X8);
		
	} 		
}
