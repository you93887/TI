/***************************************************
 * @brief   关于TM4C123GH6PZ芯片ADC初始化
 *			以及ADC值的读取函数，还有一个均值滤波函数
 *			PE3		AIN0			PD3		AIN12
 *			PE2		AIN1			PD2		AIN13
 *			PE1		AIN2			PD1		AIN14
 *			PE0		AIN3			PD0		AIN15
 *			PD7		AIN4			PH0		AIN16
 *			PD6		AIN5			PH1		AIN17
 *			PD5		AIN6			PH2		AIN18
 *			PD4		AIN7			PH3		AIN19
 *			PE5		AIN8			PE7		AIN20
 *			PE4		AIN9			PE6		AIN21
 *			PB4		AIN10			***		AIN22
 *			PB5		AIN11			***		AIN23
 * @date    2024/6/22
 * @author  qihang chen
****************************************************/
#include "include.h"

/***********************************************************
@函数名：ADC_Init
@入口参数：无
@出口参数：无
功能描述：电压测量端口PE3，作为ADC0的通道0初始化
*************************************************************/
/*
注意TM4的ADC有一个”序列“的概念。有4种序列分别是SS0,SS1,SS2,SS3。对应FIFO深度为8，4，2，1。
这个在下面函数有解释，简单来讲就是深度为8的话采8次才算结束。
DCSequenceConfigure(ADC0_BASE, 3,ADC_TRIGGER_PROCESSOR, 0);
	ADC基地址/序列3/触发器 /优先级最高（可选0~3）
	ADC有两个：ADC0和ADC1
	序列：0~3，对应FIFO深度不同，序列0对应深度8，序列3对应深度1
	触发源 这个有定时器触发或者比较器、PWM触发但是没看懂
	优先级：0~3，0最高。
	
ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0|ADC_CTL_IE |ADC_CTL_END);

	基地址/序列号/step(我猜是隔几个采一次？)/或选项（通道0、中断、ADC_CTL_END表示到此该次采样序列结束）


*/

void ADC_Init(void)//ADC初始化配置   
{    
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);// Enable the ADC1 module.
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));// Wait for the ADC1 module to be ready.	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);    //使能PE口
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

 //采样序列发生器的采样数和 FIFO 深度
    //序列发生器 | 采样数 | FIFO深度
    //   SS0     |   8    |   8
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
   //配置采样序列的节拍。  
	ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0 );
	ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH0 );
	ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH0 );
	ADCSequenceStepConfigure(ADC0_BASE, 0, 3, ADC_CTL_CH0 );
	ADCSequenceStepConfigure(ADC0_BASE, 0, 4, ADC_CTL_CH0 );
	ADCSequenceStepConfigure(ADC0_BASE, 0, 5, ADC_CTL_CH0 );
	ADCSequenceStepConfigure(ADC0_BASE, 0, 6, ADC_CTL_CH0 );
    //编程到最后一个半字节时 确定END bit置位(ADC_CTL_END） 否者导致不可预测错误
    ADCSequenceStepConfigure(ADC0_BASE, 0, 7, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
	
	ADCSequenceEnable(ADC0_BASE, 0);
	ADCIntClear(ADC0_BASE, 0);    
  //ADCIntEnable(ADC0_BASE, 0);   
} 
/*
***********************************************************
返回获取通道0的ADC的值，是0--4095
没有滤波，需要自己添加函数。
***********************************************************
*/
uint32_t ADCvalue[8];
uint16_t Get_ADC_Value(void)//ADC获取   
{
	uint16_t sum = 0;
	ADCProcessorTrigger(ADC0_BASE, 0);      //第二个参数为序列号为0 1 2 3 0为采集8次
	while(!ADCIntStatus(ADC0_BASE, 0, false)) {;}	
	ADCIntClear(ADC0_BASE, 0);   
	ADCSequenceDataGet(ADC0_BASE, 0, ADCvalue);
	for(int i = 0; i < 8; i ++)
	{
		sum += ADCvalue[i];
	}
	sum = sum / 8;
	return sum;
}


#define FILTER_SIZE 20 // 滤波器窗口大小
uint16_t filter(int new_value) 
{
    static uint16_t ADCdata[FILTER_SIZE] = {0}; // 静态数组存储输入数据
    static uint16_t ADCindex = 0; // 窗口索引
    
    static uint16_t filtered_value = 0;
    
    // 更新数据数组
    ADCdata[ADCindex] = new_value;
    ADCindex = (ADCindex + 1) % FILTER_SIZE;
    
    // 使用移动平均滤波器来平滑数据
    for (int i = 0; i < FILTER_SIZE; i++) 
	{
        filtered_value += ADCdata[i];
    }
    
    filtered_value /= FILTER_SIZE;
    
    return filtered_value;
}

