typedef __packed struct  
{  
    __IO uint8_t SEC;  
    __IO uint8_t SECALR;  
    __IO uint8_t MIN;  
    __IO uint8_t MINALR;  
    __IO uint8_t HOUR;  
    __IO uint8_t HOURALR;  
    __IO uint8_t DAY;  
    __IO uint8_t DATE;  
    __IO uint8_t MONTH;  
    __IO uint8_t YEAR;  
    __IO uint8_t CR1;  
    __IO uint8_t CR2;  
    __IO uint8_t CR3;  
    __IO uint8_t CR4;  
    __IO uint8_t RAM1[36]; // 0x0e-0x31  
    __IO uint8_t CENTURY;  
    __IO uint8_t RAM2[77]; // 0x33-0x7f  
} DS12C887_TypeDef;  
  
#define RTC2 ((DS12C887_TypeDef *)0x60000000)  
  
#define RTC2_CR1_UIP 0x80  
#define RTC2_CR1_DV2 0x40  
#define RTC2_CR1_DV1 0x20  
#define RTC2_CR1_DV0 0x10  
#define RTC2_CR1_RS3 0x08  
#define RTC2_CR1_RS2 0x04  
#define RTC2_CR1_RS1 0x02  
#define RTC2_CR1_RS0 0x01  
  
#define RTC2_CR2_SET 0x80  
#define RTC2_CR2_PIE 0x40  
#define RTC2_CR2_AIE 0x20  
#define RTC2_CR2_UIE 0x10  
#define RTC2_CR2_SQWE 0x08  
#define RTC2_CR2_DM 0x04  
#define RTC2_CR2_24_12 0x02  
#define RTC2_CR2_DSE 0x01  
  
#define RTC2_CR3_IRQF 0x80  
#define RTC2_CR3_PF 0x40  
#define RTC2_CR3_AF 0x20  
#define RTC2_CR3_UF 0x10  
  
#define RTC2_CR4_VRT 0x80  
  
int DS12C887_Init(void); 
  
