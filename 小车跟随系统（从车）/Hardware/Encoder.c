#include "include.h"

//timer0A���벶�������PB6���루PF0��T0CCP0��
//int timer0A_cnt=0;

void TIMER3_WID_IRQHandler(void);
//void TIMER3_WID_IRQHandler(void);
/******************************************************************************************************************
*������: Encoder_Init()
*��	 ������������ʼ������
*��	 �룺��
*��  ·��PF6<->T3CCP0
				 
******************************************************************************************************************/
void Encoder_Init(void)
{	
	
		//TIMER3
		SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER3);
		//ʹ�ܸ���
		TimerConfigure(WTIMER3_BASE,TIMER_CFG_PERIODIC_UP);
		//����Ԥװ��ֵ��ʹ1s��һ���ж�
		TimerLoadSet64( WTIMER3_BASE,  SysCtlClockGet()/1000-1);
		//ʹ�ܶ�ʱ��A��ʱ�ж�
		TimerIntEnable( WTIMER3_BASE,  TIMER_TIMA_TIMEOUT);
		//ע���жϺ���
		TimerIntRegister( WTIMER3_BASE,  TIMER_A, TIMER3_WID_IRQHandler);
		//�������ȼ�
		IntPrioritySet( INT_WTIMER3A,0);
		//�����ж�
		IntEnable( INT_WTIMER3A);
		IntMasterEnable();
		TimerEnable( WTIMER3_BASE,  TIMER_A);
	
		// ����Timer3ģ��   
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
   
    // ����GPIO_F��Ϊ���岶׽��   
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
   
    // ����GPIO��Ϊʹ��Timer2��׽ģʽ   
    GPIOPinConfigure(GPIO_PF6_T3CCP0);   
    GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_6); 
   
		 //�������ŷ���ע����Ȼ�������벶�񣬵�������������PWM�źſ��ƣ�����Ӳ�����ƣ�Ҫ����ΪGPIO_DIR_MODE_HW
		GPIODirModeSet( GPIO_PORTF_BASE,  GPIO_PIN_6,
                            GPIO_DIR_MODE_HW);

    // Ϊ�ܽ�����������ģʽ�������½��أ�����Ϊ������
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_6, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);  
   
    // ����ʹ��Timer4��TimerAģ��Ϊ���ش����Ӽ���ģʽ
    TimerConfigure(TIMER3_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_COUNT_UP);
   
		//����Ϊ�����ش���
		TimerControlEvent( TIMER3_BASE,  TIMER_A,
                               TIMER_EVENT_POS_EDGE);
//    //ע���жϺ���
//		TimerIntRegister( TIMER3_BASE,  TIMER_A,
//															PWM_IN_IRQHandler);
		IntMasterEnable();
    //�����ж����ȼ�
		IntPrioritySet(INT_TIMER3A,0);
		
    //ʹ�ܶ�ʱ���жϵļ�ʱ�ж�
		TimerIntEnable( TIMER3_BASE,TIMER_CAPA_EVENT);
		
    //ʹ���ж�
		IntEnable(INT_TIMER3A);
		
		
		
		TimerEnable(TIMER3_BASE,TIMER_A);
    /*����װ��ֵ���ڱ��ؼ�ʱģʽ�¿���ʡ�ԣ����Լ�����Ĭ��ֵ��
    ���������Ԥ��Ƶֵ����ôĬ��װ��ֵ����2^24,���û��Ԥ��Ƶֵ����ôĬ��װ��ֵ����2^16��
    �൱��STM32��ʹ����oc����toggleģʽ��Ĭ��Ԥװ��ֵ��д65535*/
    //TimerLoadSet(TIMER0_BASE, TIMER_A, Capture_LoadSet);
}


uint32_t zhouqi=0;
uint32_t freq=0;
uint32_t timer_flag=0;
uint32_t capture_1=0,capture_2=0,capture_3=0;
uint32_t up_count=0,down_count=0;
void TIMER3_WID_IRQHandler(void)
{
	static uint32_t time_count=0;
  uint32_t status=TimerIntStatus( WTIMER3_BASE,  true);
	TimerIntClear( WTIMER3_BASE,  status);
  //��һ�ν��ж������ڼ�⵽�������أ�Ȼ�󽫼�ʱֵ��ȡ���������ؼ���Ϊ�½���
	
	time_count++;
    //1s��ӡһ�Σ����ǲ�֪��Ϊʲô��UARTprintf��ӡһ�ξͲ���ӡ�ˣ��һ��ڸĽ����������벶���ǿ���û�����
	if(time_count==10)
	{
		time_count=0;
		if(timer_flag==0)
		{
			TimerControlEvent( TIMER3_BASE,  TIMER_A,
																 TIMER_EVENT_POS_EDGE);
//			TimerControlEvent( TIMER3_BASE,  TIMER_A,
//																 TIMER_EVENT_NEG_EDGE);
			capture_1=TimerValueGet( TIMER3_BASE,  TIMER_A);
			timer_flag=1;
		}
			//�ڶ��ν��ж�����Ϊ��⵽���½��أ�Ȼ�󽫼�ʱֵ��ȡ����ʱ���Ѿ�����˸ߵ�ƽ����
			//���Լ����ռ�ձȣ��������ؼ���Ϊ������
		else if(timer_flag==1)
		{
			TimerControlEvent( TIMER3_BASE,  TIMER_A,
																 TIMER_EVENT_POS_EDGE);
			capture_2=TimerValueGet( TIMER3_BASE,TIMER_A);	
			timer_flag=0;

					/* ____   ___ capture_1�൱�ڼ�⵽��һ�������ؼǵ�����
						 |  |   |   capture_2�൱�ڼ�⵽��һ���½��ؼǵ���
						_|  |___|   ����capture_2��capture_1֮�伴Ϊ�ߵ�ƽ��
					capture_3�൱�ڼ�⵽�ڶ��������ؼǵ���������capture_3��capture_1֮��Ϊ������*/
					/*  /|   /| ����Ҫ��ռ�ձȺ�Ƶ�ʣ���Ϊ���õ��Ƕ�ʱ��A�����ԼӼ�����
						 / |  / | ��ʱ��A�ǵ����ȵ����Ȼ���ٴ�0��ʼ����������ͨ����ߵ�ƽ�͵͵�ƽʱ��������
						/  | /  | ���capture_1��capture_2���ڵ�һ���������ڵ������׶Σ���1��2�Ĳ���Ǹߵ�ƽ
					 /   |/   | ���1��2�ֱ������������ڵ������׶Σ��Ǹߵ�ƽ��Ҫͨ��0xffff-capture_1+capture_2��á�
					���capture_2��capture_3���ڵ�һ���������ڵ������׶Σ���2��3�Ĳ���ǵ͵�ƽ��
					����ֱ������������������׶Σ��͵�ƽ��Ҫͨ��0xffff-capture_2+capture_3�����*/
		
					//Ƶ������Ƶ�����ڼ��ɵõ�

			freq = (capture_2-capture_1)*100;//(up_count+down_count)*100;
					//ռ�ձ�Ϊ�ߵ�ƽռ���ڵı�ֵ���ɵõ�
	//		duty=up_count*100/(up_count+down_count);
		 }
		
	 }

}


float Get_Speed(void)
{
	float Speed;
	Speed =freq*2*3.1416*0.024/260;
	return Speed;
}
