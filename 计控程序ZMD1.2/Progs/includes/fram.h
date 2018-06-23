//spi铁电存储器接口
//作者：周明德
#ifndef __FRAM_H
#define __FRAM_H			              
#include "stm32f10x_spi.h"     

#define MB85RS256A_WREN_INST  0x06		//设置写使能锁存器	//MB85RS256A寄存器定义
#define MB85RS256A_WRDI_INST  0x04		//写禁止
#define MB85RS256A_RDSR_INST  0x05		//读状态寄存器
#define MB85RS256A_WRSR_INST  0x01		//写状态寄存器
#define MB85RS256A_READ_INST  0x03		//读存储器数据
#define MB85RS256A_WRITE_INST 0x02		//写存储器数据
#define MB85RS256A_STATUS_REG 0x0		//
#define MB85RS256A_INIT_STATE 0x09		//
#define MB85RS256A_RDID_INST  0x9F		//读器件ID

#define MB_CS_L		GPIO_ResetBits(GPIOB,GPIO_Pin_11)
#define MB_CS_H		GPIO_SetBits(GPIOB,GPIO_Pin_11)
#define MB_CS		 	GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_11)

#define MB_WP_L		GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define MB_WP_H		GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define MB_WP 		GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_12)

#define MB_SCLK_L	GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define MB_SCLK_H	GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define MB_MOSI_L	GPIO_ResetBits(GPIOB,GPIO_Pin_15)
#define MB_MOSI_H	GPIO_SetBits(GPIOB,GPIO_Pin_15)

void SPI_Fram_Init(void);
u8 SPIx_ReadWriteByte(u8 data);
void SPI_write_MB85RS256A(u8 data,u16 address);//写入一个字节到特定地址空间
u8 SPI_read_MB85RS256A(u16 address);//读出特定地址空间的数据
void MB85RS256A_WRITE(u16 add, u8 *buf, u16 len);
void MB85RS256A_READ(u16 add, u8 *buf, u16 len);
extern void delay(u32 i);
extern void delay_ms(u32 i);
#endif
