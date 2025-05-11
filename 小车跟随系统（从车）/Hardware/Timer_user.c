/***************************************************
 * @brief   ����TM4C123GH6PZоƬ�Ķ�ʱ����ʼ��
 *          �Լ���Ӧ���жϷ��������塣
 *          ����һ�����ص�ǰʱ�亯��
 *
 *					T*CCP0    T*CCP1									WT*CCP0    WT*CCP1
 *			TIMER0   PF0  PF1		    	        			WTIMER0   PC4  PC5��  PG4  PG5
 *			TIMER1   PB4  PB5��  PF2  PF3��  PJ0  PJ1		 	WTIMER1   PC6  PC7��  PG6  PG7
 *			TIMER2   PB0  PB1��  PF5  PF4��  ***  PJ2		    WTIMER2   PD0  PD1��  PH0  PH1
 *			TIMER3   PB2  PB3��  PF6  PF7		    			WTIMER3   PD2  PD3��  PH4  PH5
 *			TIMER4   PG0  PG1��  PC0  PC1		 	    		WTIMER4   PD4  PD5��  PH6  PH7
 *			TIMER5   PG2  PG3��  PC2  PC3		 	    		WTIMER5   PD6  PD7��  PH2  PH3
 *
 *          Ĭ���Ƕ�ʱ��0������A B�� ���ϼ���  
 *          1ms�����ж�һ�Σ����ȼ�Ϊ0
 * @date    2024/6/21
 * @author  qihang chen
****************************************************/

#include "include.h"


/*
��ʱ��ģʽ�ο�https://blog.csdn.net/weixin_56003594/article/details/125662058

1.SysCtlPeripheralEnable(uint32_t ui32Peripheral)
������ui32Peripheral���Ϊ16/32bit�Ķ�ʱ������TIMER�������32/64bit�Ķ�ʱ������WTIMER��
���ã�ʹ������

2. TimerConfigure(uint32_t ui32Base, uint32_t ui32Config)
������ui32BaseΪ��ʱ������ַ��ui32Config��ʱ��������ģʽ
���ã�����ڶ�ʱ������ֵ�����£����Խ�ui32Config����Ϊ����ģʽ֮һ��
        TIMER_CFG_ONE_SHOT �C ���μ�����ģʽ
        TIMER_CFG_ONE_SHOT_UP �C ���μӼ���ģʽ
        TIMER_CFG_PERIODIC �C ����������ģʽ
        TIMER_CFG_PERIODIC_UP �C �����Ӽ���ģʽ
        TIMER_CFG_RTC �C ʵʱʱ��ģʽ
�������ʱ����ֵĻ���ui32Config����ΪTIMER_CFG_SPLIT_PAIR������Ϊһ�ԣ�Ȼ��������ģʽ���л����㣺
        TIMER_CFG_A_ONE_SHOT �C ��ʱ��A���μ�����
        TIMER_CFG_A_ONE_SHOT_UP �C��ʱ��A���μӼ���
        TIMER_CFG_A_PERIODIC �C ��ʱ��A����������
        TIMER_CFG_A_PERIODIC_UP �C ��ʱ��A�����Ӽ���

        TIMER_CFG_B_ONE_SHOT �C ��ʱ��B���μ�����
        TIMER_CFG_B_ONE_SHOT_UP �C��ʱ��B���μӼ���
        TIMER_CFG_B_PERIODIC �C ��ʱ��B����������
        TIMER_CFG_B_PERIODIC_UP �C ��ʱ��B�����Ӽ���

3.TimerLoadSet(uint32_t ui32Base, uint32_t ui32Timer, uint32_t ui32Value)
������ui32BaseΪ��ʱ������ַ��ui32Timer��TIMER_A��TIMER_B��TIMER_BOTH������A������B��AB���ã�����ѡ��
�ڶ�ʱ����ֵ���������ĸ��������ĸ����ڲ���ֵ�����¾�����ΪTIMER_A��������������Ҳ�ǣ�����ڼ���������»���Ҫѡ����Ĭ��Ϊѡ��TIMER_A����
ui32ValueΪ��ʱ������ֵ��һ����˵��ʱʱ��=1/N����ô��ʱ������ֵ=SysCtlClockGet()/N-1���������Լ��Ķ�ʱʱ�����N�����ɵö�ʱ������ֵ��
�˺���������16bit��32bit�Ķ�ʱ������������16/32bit�Ķ�ʱ������뼶����32/64bit��ʱ���Ĳ���������������64bit�Ķ�ʱ������32/64bit��ʱ���ļ���ʹ��TimerLoadSet64��
���ã����ö�ʱ������ֵ��

4. IntEnable(INT_TIMER0A); ���ж�ʹ��

5.TimerIntEnable(uint32_t ui32Base, uint32_t ui32IntFlags)
������ui32BaseΪ��ʱ������ַ��ui32IntFlagsΪ�ж�ģʽ���ڶ�ʱ��״̬��һ������ΪTIMER_TIMB_TIMEOUT��TIMER_TIMA_TIMEOUT
���ã�ʹ�ܶ�ʱ���ж�

6.TimerIntRegister(uint32_t ui32Base, uint32_t ui32Timer,void (*pfnHandler)(void))
������ui32BaseΪ��ʱ������ַ��ui32TimerҲ����TIMER_A��TIMER_B��TIMER_BOTH���֣����ڼ����Ķ�ʱ������ΪTIMER_A���ɣ�void (*pfnHandler)(void)Ϊ�жϺ�����
���ã�ע���жϺ���

7. IntMasterEnable() ���ô������ж�

8.TimerEnable(uint32_t ui32Base, uint32_t ui32Timer)
������ui32BaseΪ��ʱ������ַ��ui32TimerҲ����TIMER_A��TIMER_B��TIMER_BOTH���֡�
���ã� ��ʱ��ʹ�ܿ�ʼ����

9. IntPrioritySet(INT_TIMER0A, 0xA0);    
��һ������ ��ָ��Ҫ�������ȼ����жϡ�   �ڶ���������ָ���жϵ����ȼ�  ��0xE0
���� �����ж����ȼ�   

*/



/*
*  ��ʱ����ʼ������
*/
void TIMER0A_Handler(void);
void Timer_init(void)//ϵͳ���ȶ�ʱ����ʼ��
{
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);					//��ʱ��0ʹ��	  ���Բ鿴��������Ĳ���������Щ����	
  ROM_TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);						//32λ���ڶ�ʱ��			  �鿴���������в���ѡ�� �����ϼ���   	
  ROM_TimerLoadSet(TIMER0_BASE,TIMER_A,SysCtlClockGet()/100);		//�趨װ��ֵ,��80M/1000��*1/80M=1ms				
  ROM_IntEnable(INT_TIMER0A);										//���ж�ʹ��			�Ҽ��鿴����λ�ÿ�֪��Ӧ��ͷ�ļ������ƵĲ���	
  ROM_TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT); 					//�ж����, ����ģʽ;	�鿴��������������ģʽѡ��		
  TimerIntRegister(TIMER0_BASE,TIMER_A,TIMER0A_Handler);		//�жϺ���ע��  ����������Ϊ�жϷ������� 
  ROM_IntMasterEnable();			
  ROM_TimerEnable(TIMER0_BASE,TIMER_A); 												//��ʱ��ʹ�ܿ�ʼ����
  ROM_IntPrioritySet(INT_TIMER0A, 0xA0);    //�����ж����ȼ�   ��һ������ ��ָ��Ҫ�������ȼ����жϡ�   �ڶ���������ָ���жϵ����ȼ�  ��0xE0
}


/*
�νṹ����Ա���   �õ��Ļ�������
float Last_Time�����ڴ洢��һ��ʱ����ʱ�䣬�Ը�������ʾ��
float Now_Time�����ڴ洢��ǰʱ����ʱ�䣬�Ը�������ʾ��
float Time_Delta�����ڴ洢����ʱ���֮���ʱ���Ը�������ʾ��
uint16_t Time_Delta_INT�����ڴ洢ʱ�����������֣���16λ�޷���������ʾ����λΪ���롣
*/
//�õ���Ҫ���� SystickTime.h�ļ�
void Test_Period(Testime *Time_Lab)
{
  Time_Lab->Last_Time=Time_Lab->Now_Time;
  Time_Lab->Now_Time=micros()/1000.0f;             //(10000*TIME_ISR_CNT+TimerValueGet(TIMER1_BASE,TIMER_A)/80.0f)/1000.0f;//��λms
  Time_Lab->Time_Delta=(Time_Lab->Now_Time-Time_Lab->Last_Time);
  Time_Lab->Time_Delta_INT=(uint16_t)(Time_Lab->Time_Delta);
}

//���ص�ʱ����ms
Testime Time0_Delta;
//void TIMER0A_Handler(void)				//ϵͳ�����жϺ���  ���Լ����庯������Ҫ�ú�������TimerIntRegister�����������¶�ʱ����ʼ����
//{
//	Test_Period(&Time0_Delta);
//	//�ڴ��������������
//	
//	ROM_TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);     //�����ʱ���ж�Դ  ��һ������Ϊ�����Ķ�ʱ�� �� �ڶ�������Ϊ��ʱ����ģʽ��TimerIntEnable���������ĵڶ�������
//}



/****************************************
һ�º���δʹ�ã���������ʹ�ã�
2024.6.10

************************************/



