/***************************************************
 * @brief   ����TM4C123GH6PZоƬ��GPIO��������
 *
 * @date    2024/6/21
 * @author  qihang chen
****************************************************/
#include "include.h"

/*
************************************************************************
�˺����ǳ�ʼ����PD6
�����ⲿ�жϳ�ʼ������
************************************************************************
*/
void ButtonsInit(void)
{
	//ʹ��PFʱ��
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	
	//����PF4Ϊ���룬������û�����Ǹߵ�ƽ�����¾��ǵ͵�ƽ��
	GPIODirModeSet(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_DIR_MODE_IN);	
	
	//����Ϊ����
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);//��������

	//PF4����Ϊ�½����ж�
	GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_FALLING_EDGE);//�½���
	
	//��PF��ע��һ���жϺ���
	GPIOIntRegister(GPIO_PORTD_BASE, GPIO_interrupt);
	
	// �����ж����ȼ���TM4C123G���ж����ȼ���8����0���
	IntPrioritySet(INT_GPIOD, 0xE0);
	
	//����PD6���ж�
	GPIOIntEnable(GPIO_PORTD_BASE, GPIO_INT_PIN_6);
	IntEnable(INT_GPIOD);
	IntMasterEnable();
}


/******************************************************************************************************************
*������: GPIO_interrupt
*��	 ����GPIO�ⲿ�жϴ�����
*��	 �룺��
*��  ·��PD6����
******************************************************************************************************************/
extern uint16_t key_num;
void GPIO_interrupt(void)
{

	//��ȡ�ж�״̬
	uint32_t s = GPIOIntStatus(GPIO_PORTD_BASE, true);
	//����������жϱ�־
 	GPIOIntClear(GPIO_PORTD_BASE, s);
    
  	if( (s & GPIO_PIN_6) == GPIO_PIN_6 )
  	{
		//��������
		delay_ms(50);
		if(!GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_6))//�ȴ������ɿ�
		{
			
			//�ڴ��������һЩ�жϺ���
		}
		delay_ms(50);
	}
	//����ж���ж���Ҫif( (s&GPIO_PIN_4) == GPIO_PIN_4 )
}


/*
************************************************************************
KEY1 ��K0     KEY2 ��K1   KEY3 ��K2   KEY4 ��K3   
������ʼ������
�����ж���Ҫ�����ѯɨ�躯��
************************************************************************
*/

void Key_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	GPIODirModeSet(GPIO_PORTK_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTK_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);  //������������
}
