#include "stm32f10x.h"
#include "delay.h"
#include "OLED.h"
#include "DS12C887.h"
#include "pidadd.h" 


/**************************************************************************************
 * 描  述 : 数组定义
 **************************************************************************************/
int a[8];
/**************************************************************************************
* 描述 : lcd动态显示数字
**************************************************************************************/
void lcd(u8 x,u8 y,int z)
{
	switch(z)
	{
		case 0: LCD_Print(x, y, "0",TYPE16X16,TYPE8X16);
		break;
		case 1: LCD_Print(x, y, "1",TYPE16X16,TYPE8X16);
		break;
		case 2: LCD_Print(x, y, "2",TYPE16X16,TYPE8X16);
		break;
		case 3: LCD_Print(x, y, "3",TYPE16X16,TYPE8X16);
		break;
		case 4: LCD_Print(x, y, "4",TYPE16X16,TYPE8X16);
		break;
		case 5: LCD_Print(x, y, "5",TYPE16X16,TYPE8X16);
		break;
		case 6: LCD_Print(x, y, "6",TYPE16X16,TYPE8X16);
		break;
		case 7: LCD_Print(x, y, "7",TYPE16X16,TYPE8X16);
		break;
		case 8: LCD_Print(x, y, "8",TYPE16X16,TYPE8X16);
		break;
		case 9: LCD_Print(y, y, "9",TYPE16X16,TYPE8X16);
		break;
	}
}
/**************************************************************************************
 * 描  述 : DS12C887初始化配置
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
int DS12C887_Init(void)  
{  
    FSMC_NORSRAMInitTypeDef fsmc;  
    FSMC_NORSRAMTimingInitTypeDef fsmc_timing;  
    GPIO_InitTypeDef gpio;  
      
    // PD2为RST复位引脚(默认输出低电平)  
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;  
    gpio.GPIO_Pin = GPIO_Pin_2;  
    gpio.GPIO_Speed = GPIO_Speed_2MHz;  
    GPIO_Init(GPIOD, &gpio);  
      
    // PD3为NADV, 取反后送到AS引脚上, 该引脚不可用地址线代替  
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;  
    gpio.GPIO_Pin = GPIO_Pin_3;  
    gpio.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOD, &gpio);  
      
    // PD0~1为AD2~3, PD4为NOE接DS引脚, PD5为NWE接RW引脚, PD7为NE1片选引脚接CS, PD14~15为AD0~1  
    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_14 | GPIO_Pin_15;  
    GPIO_Init(GPIOD, &gpio);  
      
    // PE7~10为AD4~7  
    gpio.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;  
    GPIO_Init(GPIOE, &gpio);  
      
    // FSMC的Bank1, Subbank1设为8位NOR Flash地址/数据线复用模式, 关闭NWAIT引脚  
    fsmc.FSMC_ReadWriteTimingStruct = &fsmc_timing;  
    fsmc.FSMC_WriteTimingStruct = &fsmc_timing;  
    FSMC_NORSRAMStructInit(&fsmc);  
    fsmc.FSMC_MemoryType = FSMC_MemoryType_NOR; // 存储器类型为NOR Flash  
    fsmc.FSMC_WaitSignal = FSMC_WaitSignal_Disable; // 不使用NWAIT引脚  
    fsmc_timing.FSMC_AddressHoldTime = 1;  
    fsmc_timing.FSMC_AddressSetupTime = 0;  
    fsmc_timing.FSMC_BusTurnAroundDuration = 0;  
    fsmc_timing.FSMC_DataSetupTime = 2; // HCLK=72MHz时, DATAST的最小值为2, 即3xHCLK clock cycles  
    FSMC_NORSRAMInit(&fsmc);  
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE); // 虽然Subbank1默认是启用的, 但执行FSMC_NORSRAMInit函数时会被关闭, 所以需要再次开启  
      
    delay_ms(200);  
    GPIO_WriteBit(GPIOD, GPIO_Pin_2, Bit_SET); // RESET=1, 撤销复位信号  
		
		return 0;
}  
/**************************************************************************************
 * 描  述 : GPIO初始化配置
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable the GPIO_LED Clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE); 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE| RCC_APB2Periph_AFIO , ENABLE); 						 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB , ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	//OLED 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOD3,6	
 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


/**************************************************************************************
 * 描  述 : DS12C887函数
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
int DS12C887(void)
{
	  // 写入时间  
  if (RTC2->CR2 & RTC2_CR2_SET)  
  {  
     RTC2->CR2 |= RTC2_CR2_DM | RTC2_CR2_24_12;  
     RTC2->CENTURY = 20;  
     RTC2->YEAR = 17;  
     RTC2->MONTH = 9;  
     RTC2->DATE = 27;  
     RTC2->HOUR = 21;  
     RTC2->MIN = 6;  
     RTC2->SEC = 30;  
     RTC2->DAY = 3; // 星期必须手动写入, 无法自动计算  
     RTC2->CR1 = RTC2_CR1_DV1; // 打开晶振  
     RTC2->CR2 &= ~RTC2_CR2_SET; // 时钟开始走时  
    } 
	return 0;
}

/**************************************************************************************
 * 描  述 : oled display函数
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
int oled(void)
{
		int i,j,p;
		LCD_Print(0, 0, "a[0]:",TYPE16X16,TYPE8X16);
		LCD_Print(64, 0, "a[1]:",TYPE16X16,TYPE8X16);
		LCD_Print(0, 16, "a[2]:",TYPE16X16,TYPE8X16);
		LCD_Print(64, 16, "a[3]:",TYPE16X16,TYPE8X16);
		LCD_Print(0, 32, "a[4]:",TYPE16X16,TYPE8X16);
		LCD_Print(64, 32, "a[5]:",TYPE16X16,TYPE8X16);
		LCD_Print(0, 48, "a[6]:",TYPE16X16,TYPE8X16);
		LCD_Print(64, 48, "a[7]:",TYPE16X16,TYPE8X16);
		

		p=0;
		for(i=0;i<4;i++)
		{
			for(j=0;j<2;j++)
			{
				p++;
				lcd(64*j,16*i,a[p]);
			}
		}
	return 0;
}
/**************************************************************************************
 * 描  述 : 数字量函数
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
int digital_in_out(void)
 {
	 	a[0]=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_0);
		a[1]=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_1);
		a[2]=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_2);
		a[3]=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_3);
		a[4]=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4);
		a[5]=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5);
		a[6]=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_6);
		a[7]=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_7);
		GPIO_ResetBits(GPIOC , GPIO_Pin_0);
		GPIO_ResetBits(GPIOC , GPIO_Pin_1);
		GPIO_ResetBits(GPIOC , GPIO_Pin_2);
		GPIO_ResetBits(GPIOC , GPIO_Pin_3);
		GPIO_ResetBits(GPIOC , GPIO_Pin_4);
		GPIO_ResetBits(GPIOC , GPIO_Pin_5);
		GPIO_ResetBits(GPIOC , GPIO_Pin_6);
		GPIO_ResetBits(GPIOC , GPIO_Pin_7);
	 
	 	return 0;
 }

 /**************************************************************************************
 * 描  述 : DS12C887_printf函数
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
int DS12C887_printf(void)
 {
    // 第一次启动后需要等较长的时间时钟才能开始走时   
    printf("%02d%02d-%02d-%02d", RTC2->CENTURY, RTC2->YEAR, RTC2->MONTH, RTC2->DATE);  
    if ((RTC2->CR4 & RTC2_CR4_VRT) == 0)  
			printf("!"); // 如果电池已耗尽, 则显示感叹号  
          
    printf(" %02d:%02d:%02d ", RTC2->HOUR, RTC2->MIN, RTC2->SEC);  
    printf("%c", "?MTWTFSS"[RTC2->DAY]); // 星期的第一个字母  
    printf("%c", "?ouehrau"[RTC2->DAY]); // 星期的第二个字母  
    printf("%c\n", "?neduitn"[RTC2->DAY]); // 星期的第三个字母  
          
    // 等待时间更新完毕  
    while ((RTC2->CR1 & RTC2_CR1_UIP) == 0);  
    while (RTC2->CR1 & RTC2_CR1_UIP);  
	 
	 	return 0;
 }

/**************************************************************************************
 * 描  述 : MAIN函数
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
int main2(void)
{
	GPIO();
	DS12C887_Init();
	LCD_Init();
		
	delay_ms(500);	
	
	printf("DS12C887 RTC\n");  
	
  DS12C887();
	
	
  while(1)
  {
		pid_add();
		
		digital_in_out();
		printf("\r\n *********************\r\n");	

 

		printf("\r\n *****结束一次数字量*****\r\n"); 
		delay_ms(1500);
		oled();
			
 
      
		DS12C887_printf();
	}		
}
