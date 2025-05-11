#include "include.h"

//timer0A输入捕获，脉冲从PB6输入（PF0作T0CCP0）
//int timer0A_cnt=0;

void TIMER3_WID_IRQHandler(void);
//void TIMER3_WID_IRQHandler(void);
/******************************************************************************************************************
*函数名: Encoder_Init()
*描	 述：编码器初始化函数
*输	 入：无
*线  路：PF6<->T3CCP0
				 
******************************************************************************************************************/
void Encoder_Init(void)
{	
	
		//TIMER3
		SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER3);
		//使能复用
		TimerConfigure(WTIMER3_BASE,TIMER_CFG_PERIODIC_UP);
		//设置预装载值，使1s进一次中断
		TimerLoadSet64( WTIMER3_BASE,  SysCtlClockGet()/1000-1);
		//使能定时器A超时中断
		TimerIntEnable( WTIMER3_BASE,  TIMER_TIMA_TIMEOUT);
		//注册中断函数
		TimerIntRegister( WTIMER3_BASE,  TIMER_A, TIMER3_WID_IRQHandler);
		//设置优先级
		IntPrioritySet( INT_WTIMER3A,0);
		//设置中断
		IntEnable( INT_WTIMER3A);
		IntMasterEnable();
		TimerEnable( WTIMER3_BASE,  TIMER_A);
	
		// 启用Timer3模块   
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
   
    // 启用GPIO_F作为脉冲捕捉脚   
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
   
    // 配置GPIO脚为使用Timer2捕捉模式   
    GPIOPinConfigure(GPIO_PF6_T3CCP0);   
    GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_6); 
   
		 //设置引脚方向，注意虽然这是输入捕获，但是这是由外界的PWM信号控制，属于硬件控制，要设置为GPIO_DIR_MODE_HW
		GPIODirModeSet( GPIO_PORTF_BASE,  GPIO_PIN_6,
                            GPIO_DIR_MODE_HW);

    // 为管脚配置弱上拉模式（捕获下降沿，配置为上拉）
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_6, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);  
   
    // 配置使用Timer4的TimerA模块为边沿触发加计数模式
    TimerConfigure(TIMER3_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_COUNT_UP);
   
		//设置为上升沿触发
		TimerControlEvent( TIMER3_BASE,  TIMER_A,
                               TIMER_EVENT_POS_EDGE);
//    //注册中断函数
//		TimerIntRegister( TIMER3_BASE,  TIMER_A,
//															PWM_IN_IRQHandler);
		IntMasterEnable();
    //设置中断优先级
		IntPrioritySet(INT_TIMER3A,0);
		
    //使能定时器中断的计时中断
		TimerIntEnable( TIMER3_BASE,TIMER_CAPA_EVENT);
		
    //使能中断
		IntEnable(INT_TIMER3A);
		
		
		
		TimerEnable(TIMER3_BASE,TIMER_A);
    /*设置装载值，在边沿计时模式下可以省略，会自己填入默认值。
    如果设置了预分频值，那么默认装载值就是2^24,如果没有预分频值，那么默认装载值就是2^16。
    相当于STM32中使用了oc——toggle模式，默认预装载值填写65535*/
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
  //第一次进中断是由于检测到了上升沿，然后将计时值读取，并将边沿检测变为下降沿
	
	time_count++;
    //1s打印一次，但是不知道为什么用UARTprintf打印一次就不打印了，我还在改进，但是输入捕获那块是没问题的
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
			//第二次进中断是因为检测到了下降沿，然后将计时值读取，这时就已经获得了高电平数，
			//可以计算出占空比，并将边沿检测变为上升沿
		else if(timer_flag==1)
		{
			TimerControlEvent( TIMER3_BASE,  TIMER_A,
																 TIMER_EVENT_POS_EDGE);
			capture_2=TimerValueGet( TIMER3_BASE,TIMER_A);	
			timer_flag=0;

					/* ____   ___ capture_1相当于检测到第一个上升沿记的数，
						 |  |   |   capture_2相当于检测到第一个下降沿记的数
						_|  |___|   所以capture_2与capture_1之间即为高电平数
					capture_3相当于检测到第二个上升沿记的数，所以capture_3与capture_1之间为周期数*/
					/*  /|   /| 现在要求占空比和频率，因为设置的是定时器A周期性加计数，
						 / |  / | 定时器A记的数先到最大然后再从0开始计数，现在通过求高电平和低电平时间来计算
						/  | /  | 如果capture_1与capture_2都在第一个计数周期的上升阶段，那1与2的差就是高电平
					 /   |/   | 如果1与2分别落在两个周期的上升阶段，那高电平就要通过0xffff-capture_1+capture_2获得。
					如果capture_2与capture_3都在第一个计数周期的上升阶段，那2与3的差就是低电平，
					如果分别落在两个周期上升阶段，低电平就要通过0xffff-capture_2+capture_3来获得*/
		
					//频率用主频除周期即可得到

			freq = (capture_2-capture_1)*100;//(up_count+down_count)*100;
					//占空比为高电平占周期的比值即可得到
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
