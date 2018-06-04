#include "stm32f10x.h"  
#include "stdio.h"  
  
#define  DAC_TEST_1 1       //设置DAC输出固定电压  
#define  DAC_TEST_2 0           //设置DAC输出三角波形  
  
void DAC_Configuration(void);  

int DAC_init(void)  
{  
    DAC_Configuration();  
  
#if DAC_TEST_1   
  
    DAC_SetChannel1Data(DAC_Align_12b_R, 4095);     //刷新DA值,数据右对齐 取值范围 0~ 4095    
                                                    //板子上接的参考电压时3.3v 所以4095时为3.3v     
    DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);   //软件触发,DA值更新  
  
    printf("\r\n The Value is : %d \r\n",DAC_GetDataOutputValue(DAC_Channel_1));  //读取DAC引脚输出的值  
  
#elif DAC_TEST_2  
  
    TIM_Configuration();  
  
#endif   
		return 0;
  
}  
 
  
void DAC_Configuration(void)  
{  
    DAC_InitTypeDef DAC_InitStructure;   
  
#if DAC_TEST_1   
  
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;             //软件触发,不使用定时器 或者外部中断等触发  
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;   //不产生三角波（DAC_Wave_Triangle） 或者 噪声波（DAC_Wave_Noise）  
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;     //提高驱动能力可以打开缓冲  
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);   
  
#elif DAC_TEST_2   
  
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;    //选择定时器2作外部触发源  
    DAC_InitStructure.DAC_WaveGeneration =DAC_Wave_Triangle;   //产生三角波  
    DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_2047; //三角波的高为2047  最高可以为4095          
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;    //无输出缓冲 提高驱动能力可以打开缓冲  
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);   
  
#endif  
  
    DAC_DMACmd(DAC_Channel_1, DISABLE);     //不使用DMA  
  
    DAC_Cmd(DAC_Channel_1, ENABLE);  
  
}  
  
  
void TIM_Configuration(void)  
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
  
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);  
  
    TIM_TimeBaseStructure.TIM_Period = 72;          //初装值 72 每秒产生1 000 000次更新  
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;        
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;   
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);       
        
    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update); //使用更新事件作为触发输出  
      
    TIM_Cmd(TIM2, ENABLE);  
}
