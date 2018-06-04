#include "stm32f10x.h"  
#include "stdio.h"  
  
#define  DAC_TEST_1 1       //����DAC����̶���ѹ  
#define  DAC_TEST_2 0           //����DAC������ǲ���  
  
void DAC_Configuration(void);  

int DAC_init(void)  
{  
    DAC_Configuration();  
  
#if DAC_TEST_1   
  
    DAC_SetChannel1Data(DAC_Align_12b_R, 4095);     //ˢ��DAֵ,�����Ҷ��� ȡֵ��Χ 0~ 4095    
                                                    //�����ϽӵĲο���ѹʱ3.3v ����4095ʱΪ3.3v     
    DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);   //�������,DAֵ����  
  
    printf("\r\n The Value is : %d \r\n",DAC_GetDataOutputValue(DAC_Channel_1));  //��ȡDAC���������ֵ  
  
#elif DAC_TEST_2  
  
    TIM_Configuration();  
  
#endif   
		return 0;
  
}  
 
  
void DAC_Configuration(void)  
{  
    DAC_InitTypeDef DAC_InitStructure;   
  
#if DAC_TEST_1   
  
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;             //�������,��ʹ�ö�ʱ�� �����ⲿ�жϵȴ���  
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;   //���������ǲ���DAC_Wave_Triangle�� ���� ��������DAC_Wave_Noise��  
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;     //��������������Դ򿪻���  
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);   
  
#elif DAC_TEST_2   
  
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;    //ѡ��ʱ��2���ⲿ����Դ  
    DAC_InitStructure.DAC_WaveGeneration =DAC_Wave_Triangle;   //�������ǲ�  
    DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_2047; //���ǲ��ĸ�Ϊ2047  ��߿���Ϊ4095          
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;    //��������� ��������������Դ򿪻���  
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);   
  
#endif  
  
    DAC_DMACmd(DAC_Channel_1, DISABLE);     //��ʹ��DMA  
  
    DAC_Cmd(DAC_Channel_1, ENABLE);  
  
}  
  
  
void TIM_Configuration(void)  
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
  
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);  
  
    TIM_TimeBaseStructure.TIM_Period = 72;          //��װֵ 72 ÿ�����1 000 000�θ���  
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;        
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;   
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);       
        
    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update); //ʹ�ø����¼���Ϊ�������  
      
    TIM_Cmd(TIM2, ENABLE);  
}
