#include "usart.h"
#include "stm32f10x.h"
#include "delay.h"
#include "OLED.h"
volatile u8 Flag=0;
volatile u16 Rx232buffer[9];
char i=0;

void USART1_Init(void)
{
	USART_InitTypeDef				USART_InitStructure;
	USART_ClockInitTypeDef  USART_ClockInitStructure;
	NVIC_InitTypeDef				NVIC_InitStructure;

	/* Enable the PWR/BKP Clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 , ENABLE);  
	
	USART_DeInit(USART1);          //将外设USART1寄存器重设为缺省值
	USART_InitStructure.USART_BaudRate = 19200;     //设置串口1波特率为19200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //设置一个帧中传输数据位 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;     //定义发送的停止位数目为1
	USART_InitStructure.USART_Parity = USART_Parity_No;     //奇偶失能
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   //发送接收使能
	USART_Init(USART1, &USART_InitStructure);    

  USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;     //时钟低电平活动
  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;         //设置SCLK引脚上时钟输出的极性为低电平
  USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;   //时钟第一个边沿进行数据捕获
  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;  //最后一位数据的时钟脉冲不从SCLK输出
  USART_ClockInit(USART1, &USART_ClockInitStructure); 
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //使能USART1接收中断
	USART_Cmd(USART1, ENABLE);      //使能USART1外设
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //NVIC_Group:NVIC分组 0~4 总共5组,最高2位抢占
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**************************************************************************************
 * 描  述 : 重定向c库函数printf到USART1
 * 入  参 : 
 * 返回值 : 无
 **************************************************************************************/
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

/**************************************************************************************
 * 描  述 : 重定向c库函数scanf到USART1
 * 入  参 : 
 * 返回值 : 无
 **************************************************************************************/
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}

/**************************************************************************************
 * 描  述 : USART1全局中断服务
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void USART1_IRQHandler(void)
{   
  if(USART_GetITStatus(USART1,USART_IT_RXNE)!= RESET)  //接收中断
  {  
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);  //清除USART1的中断待处理位
  }  
}

/**************************************************************************************
 * 描  述 : 初始化USART2并配置USART2中断优先级
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void USART2_Init(void)
{
	USART_InitTypeDef				USART_InitStructure;
	USART_ClockInitTypeDef  USART_ClockInitStructure;
	NVIC_InitTypeDef				NVIC_InitStructure;

	/* Enable the PWR/BKP Clock */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2 , ENABLE);  
	
	USART_DeInit(USART2);          //将外设USART2寄存器重设为缺省值
	USART_InitStructure.USART_BaudRate = 9600;     //设置串口2波特率为19200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //设置一个帧中传输数据位 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;     //定义发送的停止位数目为1
	USART_InitStructure.USART_Parity = USART_Parity_No;     //奇偶失能
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   //发送接收使能
	USART_Init(USART2, &USART_InitStructure);    

  USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;     //时钟低电平活动
  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;         //设置SCLK引脚上时钟输出的极性为低电平
  USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;   //时钟第一个边沿进行数据捕获
  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;  //最后一位数据的时钟脉冲不从SCLK输出
  USART_ClockInit(USART2, &USART_ClockInitStructure); 
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  //使能USART2接收中断
	USART_Cmd(USART2, ENABLE);      //使能USART2外设
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //NVIC_Group:NVIC分组 0~4 总共5组,最高2位抢占
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**************************************************************************************
 * 描  述 : USART2发送函数
 * 入  参 : 单个字符
 * 返回值 : 无
 **************************************************************************************/
void USART2_SendByte(u8 byte)
{
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET);
	USART_SendData(USART2, byte);
}

/**************************************************************************************
 * 描  述 : USART2接收到数据后串口发送出去
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void USART2_Tx_Puts(void)
{ 
	
}

/**************************************************************************************
 * 描  述 : USART2全局中断服务
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void USART2_IRQHandler(void)
{  
  if(USART_GetITStatus(USART2,USART_IT_RXNE)!= RESET)  //接收中断
  {  
    Rx232buffer[i]= USART_ReceiveData(USART2);     //通过外设USART2接收数据
		i++;
		if(i==9)
		{
		i=0;
		Flag=1;
		}                                     //接收到数据,接收标识符有效
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);  //清除USART2的中断待处理位
  }  
}





