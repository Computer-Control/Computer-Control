/*������
���ߣ�������*/
#include "main.h"
void Service_Init(void)
{
	/* ���ȼ���������Ϊ4 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE| RCC_APB2Periph_AFIO , ENABLE); 						 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	GPIO_DeInit(GPIOA);	
	GPIO_DeInit(GPIOB);	
	GPIO_DeInit(GPIOC);	
	GPIO_DeInit(GPIOD);	
	GPIO_DeInit(GPIOE);	
	bsp_InitUart(); 	/* ��ʼ������ */
	bsp_InitTimer();	/* ��ʼ��ϵͳ�δ�ʱ�� (�˺����Ὺ�ж�) */
	SPI_Fram_Init();	/*	spi��ʼ��*/
	printerInit();
	keyInit();
	
	GPIO();
	DS12C887_Init();
	LCD_Init();
	
	delay_ms(500);	
	
	printf("DS12C887 RTC\n");  
	
  DS12C887();
}
int main(void)
{
		u16 keyboard;
		u8	gpioin;
		Service_Init();
		while(1)
		{
			keyscan(&keyboard);
			gpioin=digitalin();  //����·����
			digitalout(0x08);     //��·���
			printf("%d,%d",keyboard,gpioin);
			MODH_Poll();
			
			pid_add();
			delay_ms(1500);
			oled();
			DS12C887_printf();
		}
}


