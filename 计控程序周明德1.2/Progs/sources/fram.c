//spi总线的铁电存储器驱动
//作者：周明德
#include "fram.h"
/********************************************************
//project：STM32F1  SPI2读写MB85RS256A驱动代码  
//annotation:适用铁电存储器mb85系列读写代码，直接读写一个字节
**********************************************************/
//初始化SPI2 FRAM的IO口
void SPI_Fram_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;  //SPI CS/HOLD
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//PB14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_15);

	SPI_I2S_DeInit(SPI2);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;	//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
	SPIx_ReadWriteByte(0x00);//0x00,0xff，0xaa都行
}  

u8 SPIx_ReadWriteByte(u8 TxData)                           //SPI 发送接收字节
{
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据	
}

void SPI_write_MB85RS256A(u8 data,u16 address)//写入一个字节到特定地址空间
{
	u8 addr_tempH,addr_tempL;
	addr_tempH = (u8)((address&0xff00)>>8);   //获取高8位地址
	addr_tempL = (u8)(address&0x00ff);        //获取低8位地址
	MB_CS_L;
	SPIx_ReadWriteByte(MB85RS256A_WREN_INST);    //写使能
 	MB_CS_H;
// 	delay(1000);
 	MB_CS_L;
	SPIx_ReadWriteByte(MB85RS256A_WRITE_INST);   //写存储器寄存器操作吗
	SPIx_ReadWriteByte(addr_tempH);            //写入高八位地址，高3位忽略
	SPIx_ReadWriteByte(addr_tempL);            //
	SPIx_ReadWriteByte(data);                  //写入数据
	MB_CS_H;
// 	MB_CS_L;
// 	SPIx_ReadWriteByte(MB85RS256A_WRDI_INST);
// 	MB_CS_H;
}

u8 SPI_read_MB85RS256A(u16 address)//读出特定地址空间的数据
{
	u8 dat,addr_tempH,addr_tempL;
	addr_tempH = (u8)((address&0xff00)>>8);
	addr_tempL = (u8)(address&0x00ff);
	MB_CS_L;
	SPIx_ReadWriteByte(MB85RS256A_READ_INST);      //读存储器寄存器操作码
	SPIx_ReadWriteByte(addr_tempH);
	SPIx_ReadWriteByte(addr_tempL);
	dat=SPIx_ReadWriteByte(0x00);               //读取数据，0xAA给予读取数据所需的时钟
	MB_CS_H;
	return (dat);
}

void MB85RS256A_WRITE(u16 add, u8 *buf, u16 len)
{
	MB_CS_L;
	SPIx_ReadWriteByte(MB85RS256A_WREN_INST);                   /* step 1 .  WEL = 1            */
 	MB_CS_H;
// 	delay(1000);
 	MB_CS_L;
	SPIx_ReadWriteByte(MB85RS256A_WRITE_INST);
    SPIx_ReadWriteByte((u8)(add>>8));
    SPIx_ReadWriteByte((u8)(add));
    for (; len > 0; len--)
	{                                            				/* step 4 . send out bytes      */
        SPIx_ReadWriteByte(*buf++);
	}
	MB_CS_H;
}

void MB85RS256A_READ(u16 add, u8 *buf, u16 len)
{	
	MB_CS_L;
	SPIx_ReadWriteByte(MB85RS256A_READ_INST);
    SPIx_ReadWriteByte((u8)(add>>8));
    SPIx_ReadWriteByte((u8)(add));
    for (;len > 0; len--) 
	{
       *buf++ = (SPIx_ReadWriteByte(0x00));
	}
	MB_CS_H;
}
