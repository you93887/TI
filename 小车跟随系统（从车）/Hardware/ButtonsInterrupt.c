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
//extern float speed;
float aaa = 10;
void GPIO_interrupt(void)
{
	static uint8_t trigger=0;
	
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
//			if(speed >= 100 )
//				aaa = -10;
//			if(speed <= -100)
//				aaa = 10;
//			speed = speed + aaa;
		}
		delay_ms(50);
	}
	//����ж���ж���Ҫif( (s&GPIO_PIN_4) == GPIO_PIN_4 )
}


/*
************************************************************************
SW1 ��F4       SW2 ��F0
������ʼ������
�����ж���Ҫ�����ѯɨ�躯��
************************************************************************
*/

#define ReadKey1  GPIOPinRead(GPIO_PORTF_BASE , GPIO_PIN_4)
#define ReadKey2  GPIOPinRead(GPIO_PORTF_BASE , GPIO_PIN_0)
void Key_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4,GPIO_DIR_MODE_IN);//SW1
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);  //������������
	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0,GPIO_DIR_MODE_IN);//SW2
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);  //������������
}

