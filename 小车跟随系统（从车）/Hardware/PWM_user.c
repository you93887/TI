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
	//����PWMʱ�ӣ�����USEPWMDIV��Ƶ����
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);																									//PWMʱ�� 16M

	//ʹ��ʱ��
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);			//ʹ��PWMģ��1ʱ��	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);		//ʹ��GPIOFʱ��																		
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);	
	//ʹ�����Ÿ���PWM����
	GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_2);
	GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_3);
	GPIOPinTypePWM(GPIO_PORTA_BASE,GPIO_PIN_6);
	GPIOPinTypePWM(GPIO_PORTA_BASE,GPIO_PIN_7);
	//PWM�źŷ���
	GPIOPinConfigure(GPIO_PF2_M1PWM6);					//PF2->PWMģ��1�ź�6																							
	GPIOPinConfigure(GPIO_PF3_M1PWM7);					//PF3->PWMģ��1�ź�7																								
	GPIOPinConfigure(GPIO_PA6_M1PWM2);					//PF2->PWMģ��1�ź�6																							
	GPIOPinConfigure(GPIO_PA7_M1PWM3);
	//����PWM������
	//ģ��1->������3->���¼�������ͬ��
	PWMGenConfigure(PWM1_BASE,PWM_GEN_3,PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);	
	PWMGenConfigure(PWM1_BASE,PWM_GEN_1,PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);	
	//����PWM����
	PWMGenPeriodSet(PWM1_BASE,PWM_GEN_3,64000);			//64*10^3/16/10^6=4ms																			
	PWMGenPeriodSet(PWM1_BASE,PWM_GEN_1,64000);		
	//����PWMռ�ձ�
	//PWMPulseWidthSet(PWM1_BASE,PWM_OUT_6,PWMGenPeriodGet(PWM1_BASE, PWM_GEN_3)*0.01);		//�Ƚ�ֵΪ�ķ�֮һ�ܼ���ֵ��25%
	//PWMPulseWidthSet(PWM1_BASE,PWM_OUT_7,PWMGenPeriodGet(PWM1_BASE, PWM_GEN_3)*0.01);		//�Ƚ�ֵΪ�ķ�֮���ܼ���ֵ��75%

	//ʹ��PWMģ��1���
	PWMOutputState(PWM1_BASE,PWM_OUT_6_BIT,true);
	PWMOutputState(PWM1_BASE,PWM_OUT_7_BIT,true);
	PWMOutputState(PWM1_BASE,PWM_OUT_2_BIT,true);
	PWMOutputState(PWM1_BASE,PWM_OUT_3_BIT,true);
	//ʹ��PWM������
	PWMGenEnable(PWM1_BASE,PWM_GEN_3);
	PWMGenEnable(PWM1_BASE,PWM_GEN_1);

}

/******************************************************************************************************************
*������: SetCycle(uint32_t ui32Base,uint32_t ui32PWMOut,uint32_t cycle)
*��	 ���� ����PWM������
*��	 �룺PWMģ���š��źű�š����ڣ�ms��
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
		
	uint32_t Period = 16000 * cycle;
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
