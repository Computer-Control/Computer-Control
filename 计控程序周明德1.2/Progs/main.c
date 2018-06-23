/*主函数
作者：周明德*/
#include "main.h"
void Service_Init(void)
{
	/* 优先级分组设置为4 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE| RCC_APB2Periph_AFIO , ENABLE); 						 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	GPIO_DeInit(GPIOA);	
	GPIO_DeInit(GPIOB);	
	GPIO_DeInit(GPIOC);	
	GPIO_DeInit(GPIOD);	
	GPIO_DeInit(GPIOE);	
	bsp_InitUart(); 	/* 初始化串口 */
	bsp_InitTimer();	/* 初始化系统滴答定时器 (此函数会开中断) */
	SPI_Fram_Init();	/*	spi初始化*/
	printerInit();
	keyInit();
}
int main(void)
{
		u16 keyboard;
		u8	gpioin;
		Service_Init();
		while(1)
		{
			keyscan(&keyboard);
			gpioin=digitalin();
			digitalout(0x08);
			printf("%d,%d",keyboard,gpioin);
			MODH_Poll();
		}
}


