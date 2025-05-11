/***************************************************
 * @brief   ����TM4C123GH6PZоƬ��PWM��ʼ��
 *          �Լ���Ӧ�ĵ���ռ�ձȺ������͵������ں���
 *
 *			PWM0_0   PH0						PWM1_0   PD0, PG2
 *			PWM0_1   PH1						PWM1_1   PD1, PG3
 *			PWM0_2   PB4, PH2					PWM1_2   PA6, PE4, PG4
 *			PWM0_3   PB5, PH3					PWM1_3   PA7, PE5, PG5
 *			PWM0_4   PE4, PG4, PH4				PWM1_4   PF0
 *			PWM0_5   PE5, PG5, PH5				PWM1_5   PF1
 *			PWM0_6   PC4, PD0, PG5, PH5			PWM1_6   PF2
 *			PWM0_7   PC5, PD1, PG5, PH5			PWM1_7   PF3
 *
 *          Ĭ����PWM�ĵ�һ�У���������������
 *
 * @date    2024/6/21
 * @author  qihang chen
****************************************************/

#include "include.h"

/******************************************************************************************************************
*������: PWM_Init()
*��	 ����PWM��ʼ������
*��	 �룺��
	���ڷ�������PWM0��1��Ӧ������0��PWM_GEN_0�� 
	2��3��ӦPWM_GEN_1  ......
	PWM���� = PWMGenPeriodSet���������� / PWMʱ�� ��Ĭ��16M��
	����ռ�ձ���PWMPulseWidthSet��������PWMGenPeriodGet��ȡ PWM ������ģ������ڣ�Ȼ��* 0~1֮���ֵ
******************************************************************************************************************/
void PWM_Init(void)
{
	//����PWMʱ�ӣ�����USEPWMDIV��Ƶ���� 16��Ƶ ��40M/16 = 2.5M
	SysCtlPWMClockSet(SYSCTL_PWMDIV_16);																									//PWMʱ�� 16M

	//ʹ��ʱ��
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);			//ʹ��PWMģ��0ʱ��																		
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);		//ʹ��GPIOHʱ��																		
	
	//ʹ�����Ÿ���PWM����
	GPIOPinTypePWM(GPIO_PORTH_BASE,GPIO_PIN_1);
	GPIOPinTypePWM(GPIO_PORTH_BASE,GPIO_PIN_2);
	//PWM�źŷ���
	GPIOPinConfigure(GPIO_PH1_M0PWM1);					//PH1->PWMģ��0�ź�1																																									
	GPIOPinConfigure(GPIO_PH2_M0PWM2);					//PH2->PWMģ��0�ź�2
	
	//����PWM������
	//ģ��0->������0->���¼�������ͬ��
	//ģ��0->������1->���¼�������ͬ��
	PWMGenConfigure(PWM0_BASE,PWM_GEN_0,PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);	
	PWMGenConfigure(PWM0_BASE,PWM_GEN_1,PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);	
	
	//����PWM����
	PWMGenPeriodSet(PWM0_BASE,PWM_GEN_0,10000);			// 2.5*10^6 / x	 HZ	  											
	PWMGenPeriodSet(PWM0_BASE,PWM_GEN_1,10000);			// 2.5*10^6 / x  HZ     
	
	//����PWMռ�ձ�
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_1,PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*0.5);		//50%
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_2,PWMGenPeriodGet(PWM0_BASE, PWM_GEN_1)*0.5);		//50%
	
	//ʹ��PWMģ��0���
	PWMOutputState(PWM0_BASE,PWM_OUT_1_BIT,true);
	PWMOutputState(PWM0_BASE,PWM_OUT_2_BIT,true);
	
	//ʹ��PWM������
	PWMGenEnable(PWM0_BASE,PWM_GEN_0);
	PWMGenEnable(PWM0_BASE,PWM_GEN_1);
}

/******************************************************************************************************************
*������: SetCycle(uint32_t ui32Base,uint32_t ui32PWMOut,uint32_t cycle)
*��	 ���� ����PWM��Ƶ��
*��	 �룺PWMģ���š��źű�š�Ƶ�ʣ�HZ��
******************************************************************************************************************/
void SetCycle(uint32_t ui32Base,uint32_t ui32PWMOut,uint32_t cycle)
{
	uint32_t ui32Gen;
	uint32_t ui32OutBits;
	
	switch(ui32PWMOut)
	{
		case PWM_OUT_0:	ui32Gen=PWM_GEN_0,ui32OutBits=PWM_OUT_0_BIT;	break;
		case PWM_OUT_1:	ui32Gen=PWM_GEN_0,ui32OutBits=PWM_OUT_1_BIT;	break;
		case PWM_OUT_2:	ui32Gen=PWM_GEN_1,ui32OutBits=PWM_OUT_2_BIT;	break;
		case PWM_OUT_3:	ui32Gen=PWM_GEN_1,ui32OutBits=PWM_OUT_3_BIT;	break;
		case PWM_OUT_4:	ui32Gen=PWM_GEN_2,ui32OutBits=PWM_OUT_4_BIT;	break;
		case PWM_OUT_5:	ui32Gen=PWM_GEN_2,ui32OutBits=PWM_OUT_5_BIT;	break;
		case PWM_OUT_6:	ui32Gen=PWM_GEN_3,ui32OutBits=PWM_OUT_6_BIT;	break;
		case PWM_OUT_7:	ui32Gen=PWM_GEN_3,ui32OutBits=PWM_OUT_7_BIT;	break;
	}
		
	uint32_t Period = 5000000 / cycle;
    //����PWM����
    PWMGenPeriodSet(ui32Base,ui32Gen,Period);
    PWMOutputState(ui32Base, ui32OutBits, true);
    //ʹ�ܷ�����ģ��
    PWMGenEnable(ui32Base, ui32Gen);
}

/******************************************************************************************************************
*������: SetDuty(uint32_t ui32Base,uint32_t ui32PWMOut,float duty)
*��	 ����PWM����ռ�ձȺ���
*��	 �룺PWMģ���š��źű�š�ռ�ձ�
******************************************************************************************************************/
void SetDuty(uint32_t ui32Base,uint32_t ui32PWMOut,float duty)
{
	uint32_t ui32Gen;
	uint32_t ui32OutBits;
	
	switch(ui32PWMOut)
	{
		case PWM_OUT_0:	ui32Gen=PWM_GEN_0,ui32OutBits=PWM_OUT_0_BIT;	break;
		case PWM_OUT_1:	ui32Gen=PWM_GEN_0,ui32OutBits=PWM_OUT_1_BIT;	break;
		case PWM_OUT_2:	ui32Gen=PWM_GEN_1,ui32OutBits=PWM_OUT_2_BIT;	break;
		case PWM_OUT_3:	ui32Gen=PWM_GEN_1,ui32OutBits=PWM_OUT_3_BIT;	break;
		case PWM_OUT_4:	ui32Gen=PWM_GEN_2,ui32OutBits=PWM_OUT_4_BIT;	break;
		case PWM_OUT_5:	ui32Gen=PWM_GEN_2,ui32OutBits=PWM_OUT_5_BIT;	break;
		case PWM_OUT_6:	ui32Gen=PWM_GEN_3,ui32OutBits=PWM_OUT_6_BIT;	break;
		case PWM_OUT_7:	ui32Gen=PWM_GEN_3,ui32OutBits=PWM_OUT_7_BIT;	break;
	}
	
    //����ռ�ձ�
    PWMPulseWidthSet(ui32Base, ui32PWMOut, PWMGenPeriodGet(ui32Base, ui32Gen)*duty);
    PWMOutputState(ui32Base, ui32OutBits, true);
    //ʹ�ܷ�����ģ��
    PWMGenEnable(ui32Base, ui32Gen);
}