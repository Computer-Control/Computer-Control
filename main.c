#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "includes.h"
#include "OLED.h"
#include "DS12C887.h"


/**************************************************************************************
 * ��  �� : ���鶨��
 **************************************************************************************/
int a[8];
/**************************************************************************************
* ���� : lcd��̬��ʾ����
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


void DS12C887_Init(void)  
{  
    FSMC_NORSRAMInitTypeDef fsmc;  
    FSMC_NORSRAMTimingInitTypeDef fsmc_timing;  
    GPIO_InitTypeDef gpio;  
      
    // PA8ΪRST��λ����(Ĭ������͵�ƽ)  
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;  
    gpio.GPIO_Pin = GPIO_Pin_8;  
    gpio.GPIO_Speed = GPIO_Speed_2MHz;  
    GPIO_Init(GPIOA, &gpio);  
      
    // PB7ΪNADV, ȡ�����͵�AS������, �����Ų����õ�ַ�ߴ���  
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;  
    gpio.GPIO_Pin = GPIO_Pin_7;  
    gpio.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOB, &gpio);  
      
    // PD0~1ΪAD2~3, PD4ΪNOE��DS����, PD5ΪNWE��RW����, PD7ΪNE1Ƭѡ���Ž�CS, PD14~15ΪAD0~1  
    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_14 | GPIO_Pin_15;  
    GPIO_Init(GPIOD, &gpio);  
      
    // PE7~10ΪAD4~7  
    gpio.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;  
    GPIO_Init(GPIOE, &gpio);  
      
    // FSMC��Bank1, Subbank1��Ϊ8λNOR Flash��ַ/�����߸���ģʽ, �ر�NWAIT����  
    fsmc.FSMC_ReadWriteTimingStruct = &fsmc_timing;  
    fsmc.FSMC_WriteTimingStruct = &fsmc_timing;  
    FSMC_NORSRAMStructInit(&fsmc);  
    fsmc.FSMC_MemoryType = FSMC_MemoryType_NOR; // �洢������ΪNOR Flash  
    fsmc.FSMC_WaitSignal = FSMC_WaitSignal_Disable; // ��ʹ��NWAIT����  
    fsmc_timing.FSMC_AddressHoldTime = 1;  
    fsmc_timing.FSMC_AddressSetupTime = 0;  
    fsmc_timing.FSMC_BusTurnAroundDuration = 0;  
    fsmc_timing.FSMC_DataSetupTime = 2; // HCLK=72MHzʱ, DATAST����СֵΪ2, ��3xHCLK clock cycles  
    FSMC_NORSRAMInit(&fsmc);  
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE); // ��ȻSubbank1Ĭ�������õ�, ��ִ��FSMC_NORSRAMInit����ʱ�ᱻ�ر�, ������Ҫ�ٴο���  
      
    delay_ms(200);  
    GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_SET); // RESET=1, ������λ�ź�  
}  
/**************************************************************************************
 * ��  �� : GPIO/USART1��ʼ������
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable the GPIO_LED Clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE); 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE| RCC_APB2Periph_AFIO , ENABLE); 						 
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //������� 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**************************************************************************************
 * ��  �� : MAIN����
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
int main(void)
{
	int i,j,p;
	SystemInit();			//����ϵͳʱ��72MHZ
	GPIO_Configuration();
	USART1_Init();    //��ʼ������TIM 
	USART2_Init();
	DS12C887_Init();
	LCD_Init();
		
	delay_ms(500);	
	
	printf("DS12C887 RTC\n");  
  // д��ʱ��  
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
     RTC2->DAY = 3; // ���ڱ����ֶ�д��, �޷��Զ�����  
     RTC2->CR1 = RTC2_CR1_DV1; // �򿪾���  
     RTC2->CR2 &= ~RTC2_CR2_SET; // ʱ�ӿ�ʼ��ʱ  
    } 
	
	printf("\r\n **��ʼadת��**\r\n");
	
  while(1)
  {
		a[0]=GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_0);
		a[1]=GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_1);
		a[2]=GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_2);
		a[3]=GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_3);
		a[4]=GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_4);
		a[5]=GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_5);
		a[6]=GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_6);
		a[7]=GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_7);
		GPIO_ResetBits(GPIOC , GPIO_Pin_8);
		GPIO_ResetBits(GPIOC , GPIO_Pin_9);
		GPIO_ResetBits(GPIOC , GPIO_Pin_10);
		GPIO_ResetBits(GPIOC , GPIO_Pin_11);
		GPIO_ResetBits(GPIOC , GPIO_Pin_12);
		GPIO_ResetBits(GPIOC , GPIO_Pin_13);
		GPIO_ResetBits(GPIOC , GPIO_Pin_14);
		GPIO_ResetBits(GPIOC , GPIO_Pin_15);
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
		

		p=0;
		for(i=0;i<4;i++)
		{
			for(j=0;j<2;j++)
			{
				p++;
				lcd(64*j,16*i,a[p]);
			}
		}
		
 
      
    // ��һ����������Ҫ�Ƚϳ���ʱ��ʱ�Ӳ��ܿ�ʼ��ʱ   
    printf("%02d%02d-%02d-%02d", RTC2->CENTURY, RTC2->YEAR, RTC2->MONTH, RTC2->DATE);  
    if ((RTC2->CR4 & RTC2_CR4_VRT) == 0)  
			printf("!"); // �������Ѻľ�, ����ʾ��̾��  
          
    printf(" %02d:%02d:%02d ", RTC2->HOUR, RTC2->MIN, RTC2->SEC);  
    printf("%c", "?MTWTFSS"[RTC2->DAY]); // ���ڵĵ�һ����ĸ  
    printf("%c", "?ouehrau"[RTC2->DAY]); // ���ڵĵڶ�����ĸ  
    printf("%c\n", "?neduitn"[RTC2->DAY]); // ���ڵĵ�������ĸ  
          
    // �ȴ�ʱ��������  
    while ((RTC2->CR1 & RTC2_CR1_UIP) == 0);  
    while (RTC2->CR1 & RTC2_CR1_UIP);  
    }  
	 		
}
