/***************************************************
 * @brief   ����TM4C123GH6PZоƬADC��ʼ��
 *			�Լ�ADCֵ�Ķ�ȡ����������һ����ֵ�˲�����
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
@��������ADC_Init
@��ڲ�������
@���ڲ�������
������������ѹ�����˿�PE3����ΪADC0��ͨ��0��ʼ��
*************************************************************/
/*
ע��TM4��ADC��һ�������С��ĸ����4�����зֱ���SS0,SS1,SS2,SS3����ӦFIFO���Ϊ8��4��2��1��
��������溯���н��ͣ��������������Ϊ8�Ļ���8�β��������
DCSequenceConfigure(ADC0_BASE, 3,ADC_TRIGGER_PROCESSOR, 0);
	ADC����ַ/����3/������ /���ȼ���ߣ���ѡ0~3��
	ADC��������ADC0��ADC1
	���У�0~3����ӦFIFO��Ȳ�ͬ������0��Ӧ���8������3��Ӧ���1
	����Դ ����ж�ʱ���������߱Ƚ�����PWM��������û����
	���ȼ���0~3��0��ߡ�
	
ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0|ADC_CTL_IE |ADC_CTL_END);

	����ַ/���к�/step(�Ҳ��Ǹ�������һ�Σ�)/��ѡ�ͨ��0���жϡ�ADC_CTL_END��ʾ���˸ôβ������н�����


*/

void ADC_Init(void)//ADC��ʼ������   
{    
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);// Enable the ADC1 module.
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));// Wait for the ADC1 module to be ready.	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);    //ʹ��PE��
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

 //�������з������Ĳ������� FIFO ���
    //���з����� | ������ | FIFO���
    //   SS0     |   8    |   8
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
   //���ò������еĽ��ġ�  
	ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0 );
	ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH0 );
	ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH0 );
	ADCSequenceStepConfigure(ADC0_BASE, 0, 3, ADC_CTL_CH0 );
	ADCSequenceStepConfigure(ADC0_BASE, 0, 4, ADC_CTL_CH0 );
	ADCSequenceStepConfigure(ADC0_BASE, 0, 5, ADC_CTL_CH0 );
	ADCSequenceStepConfigure(ADC0_BASE, 0, 6, ADC_CTL_CH0 );
    //��̵����һ�����ֽ�ʱ ȷ��END bit��λ(ADC_CTL_END�� ���ߵ��²���Ԥ�����
    ADCSequenceStepConfigure(ADC0_BASE, 0, 7, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
	
	ADCSequenceEnable(ADC0_BASE, 0);
	ADCIntClear(ADC0_BASE, 0);    
  //ADCIntEnable(ADC0_BASE, 0);   
} 
/*
***********************************************************
���ػ�ȡͨ��0��ADC��ֵ����0--4095
û���˲�����Ҫ�Լ���Ӻ�����
***********************************************************
*/
uint32_t ADCvalue[8];
uint16_t Get_ADC_Value(void)//ADC��ȡ   
{
	uint16_t sum = 0;
	ADCProcessorTrigger(ADC0_BASE, 0);      //�ڶ�������Ϊ���к�Ϊ0 1 2 3 0Ϊ�ɼ�8��
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


#define FILTER_SIZE 20 // �˲������ڴ�С
uint16_t filter(int new_value) 
{
    static uint16_t ADCdata[FILTER_SIZE] = {0}; // ��̬����洢��������
    static uint16_t ADCindex = 0; // ��������
    
    static uint16_t filtered_value = 0;
    
    // ������������
    ADCdata[ADCindex] = new_value;
    ADCindex = (ADCindex + 1) % FILTER_SIZE;
    
    // ʹ���ƶ�ƽ���˲�����ƽ������
    for (int i = 0; i < FILTER_SIZE; i++) 
	{
        filtered_value += ADCdata[i];
    }
    
    filtered_value /= FILTER_SIZE;
    
    return filtered_value;
}

