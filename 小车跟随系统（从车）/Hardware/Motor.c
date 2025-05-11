#include "include.h"


extern float Speed1;
extern float Speed2;

extern PID PID_Motor1_Speed;
extern PID PID_Motor2_Speed;
extern PID PID_Speed;

float PWM1;
float PWM2;
float PWM_SET ;
float Speed1_Set;
float Speed2_Set;

void Motor_Init()
{
	//使能GPIOF时钟
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	
		while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
			
    }
		while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOH))
    {
			
    }
	  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
    {
			
    }
		while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
    {
			
    }
    //配置F5、F4

		
	  GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, GPIO_PIN_4);//左前  PWM1_6
    GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, GPIO_PIN_5);//左前  PWM1_6
		
		GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1);//左后  PWM0_1
		GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2);//左后  PWM0_1
		
		GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_5);//右前  PWM1_7
		GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4);//右前  PWM1_7
		
	  GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);//右后  PWM0_0
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_1);//右后  PWM0_0
		

}

void TIMER3_WID_IRQHandler(void);
void TIMER4_WID_IRQHandler(void);
/******************************************************************************************************************
*函数名: Encoder_Init()
*描	 述：编码器初始化函数
*输	 入：无
*线  路：PF6<->T3CCP0
				 PG0<->T4CCP0
******************************************************************************************************************/
void Encoder_Init(void)
{	
		//TIMER3
		SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER3);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER4);
		//使能复用
		TimerConfigure(WTIMER3_BASE,TIMER_CFG_PERIODIC_UP);
		TimerConfigure(WTIMER4_BASE,TIMER_CFG_PERIODIC_UP);
		//设置预装载值，使10ms进一次中断
		TimerLoadSet64( WTIMER3_BASE,  SysCtlClockGet()/100-1);
		TimerLoadSet64( WTIMER4_BASE,  SysCtlClockGet()/100-1);
		//使能定时器A超时中断
		TimerIntEnable( WTIMER3_BASE,  TIMER_TIMA_TIMEOUT);
		TimerIntEnable( WTIMER4_BASE,  TIMER_TIMA_TIMEOUT);
		//注册中断函数
		TimerIntRegister( WTIMER3_BASE,  TIMER_A,
                             TIMER3_WID_IRQHandler);
		TimerIntRegister( WTIMER4_BASE,  TIMER_A,
                             TIMER4_WID_IRQHandler);
		//设置优先级
		IntPrioritySet( INT_WTIMER3A,0);
	  IntPrioritySet( INT_WTIMER4A,0);
		//设置中断
		IntEnable( INT_WTIMER3A);
		IntEnable( INT_WTIMER4A);
		
		IntMasterEnable();
		
		TimerEnable( WTIMER3_BASE,  TIMER_A);
		TimerEnable( WTIMER4_BASE,  TIMER_A);
	
		// 启用Timer3模块   
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4);
    // 启用GPIO_F作为脉冲捕捉脚   
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
		
    // 配置GPIO脚为使用Timer捕捉模式   
    GPIOPinConfigure(GPIO_PF6_T3CCP0);   
    GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_6); 
		
		GPIOPinConfigure(GPIO_PG0_T4CCP0);   
    GPIOPinTypeTimer(GPIO_PORTG_BASE, GPIO_PIN_0);
		
		
		 //设置引脚方向，注意虽然这是输入捕获，但是这是由外界的PWM信号控制，属于硬件控制，要设置为GPIO_DIR_MODE_HW
		GPIODirModeSet( GPIO_PORTF_BASE,  GPIO_PIN_6,
                            GPIO_DIR_MODE_HW);
		GPIODirModeSet( GPIO_PORTG_BASE,  GPIO_PIN_0,
                            GPIO_DIR_MODE_HW);
    // 为管脚配置弱上拉模式（捕获下降沿，配置为上拉）
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_6, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);  
    GPIOPadConfigSet(GPIO_PORTG_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
		
    // 配置使用Timer3的TimerA模块为边沿触发加计数模式
    TimerConfigure(TIMER3_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_COUNT_UP);
    TimerConfigure(TIMER4_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_COUNT_UP);
		//设置为上升沿触发
		TimerControlEvent( TIMER3_BASE,  TIMER_A,
                               TIMER_EVENT_POS_EDGE);
		TimerControlEvent( TIMER4_BASE,  TIMER_A,
                               TIMER_EVENT_POS_EDGE);

		IntMasterEnable();
    //设置中断优先级
		IntPrioritySet(INT_TIMER3A,0);
		IntPrioritySet(INT_TIMER4A,0);
		
    //使能定时器中断的计时中断
		TimerIntEnable( TIMER3_BASE,TIMER_CAPA_EVENT);
		TimerIntEnable( TIMER4_BASE,TIMER_CAPA_EVENT);
		
    //使能中断
		IntEnable(INT_TIMER3A);
		TimerEnable(TIMER3_BASE,TIMER_A);
		
		IntEnable(INT_TIMER4A);
		TimerEnable(TIMER4_BASE,TIMER_A);
    /*设置装载值，在边沿计时模式下可以省略，会自己填入默认值。
    如果设置了预分频值，那么默认装载值就是2^24,如果没有预分频值，那么默认装载值就是2^16。
    相当于STM32中使用了oc——toggle模式，默认预装载值填写65535*/
    //TimerLoadSet(TIMER0_BASE, TIMER_A, Capture_LoadSet);
}



/**
  * 函    数：直流电机设置速度
  * 参    数：percent 要设置的速度，范围：-100~100
  * 返 回 值：无
  */
void Motor_SetPwm1(float percent)
{
	float Pwm1 = percent/100;
	if (Pwm1 >= 0)							//如果设置正转的速度值
	{

		GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_4, GPIO_PIN_4);
		GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_5, 0);
		
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);
	  GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2);
		SetDuty(PWM1_BASE,PWM_OUT_6,Pwm1);//PWM设置为速度值
	  SetDuty(PWM1_BASE,PWM_OUT_3,Pwm1);	
		
	}
	else									//否则，即设置反转的速度值
	{
		
		GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_4, 0);
		GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_5, GPIO_PIN_5);
		
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1);
	  GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0);
		SetDuty(PWM1_BASE,PWM_OUT_6,-Pwm1);//PWM设置为速度值
	  SetDuty(PWM1_BASE,PWM_OUT_3,-Pwm1);	
	}
}

void Motor_SetPwm2(float percent)
{
	float Pwm2 = percent/100;
	if (Pwm2 >= 0)							//如果设置正转的速度值
	{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_5, GPIO_PIN_5);
		

		
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0);
	  GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_PIN_1);
		
	
		SetDuty(PWM1_BASE,PWM_OUT_7,Pwm2);//PWM设置为速度值
    SetDuty(PWM1_BASE,PWM_OUT_2,Pwm2);
		
		
	}
	else									//否则，即设置反转的速度值
	{
		//1-p
	
				
		
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_PIN_4);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_5, 0);
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_PIN_0);
	  GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0);
		
	
		SetDuty(PWM1_BASE,PWM_OUT_7,-Pwm2);//PWM设置为速度值
    SetDuty(PWM1_BASE,PWM_OUT_2,-Pwm2);
		
	}
}



void Motor_SetSpeed(float Motor1_Speed,float Motor2_Speed)
{
	PID_Motor1_Speed.target_val=Motor1_Speed;
	PID_Motor2_Speed.target_val=Motor2_Speed;
	//Motor_SetPwm(0.4);
//	float PWM1=(PID_realize(&PID_Motor1_Speed,Speed1)+Speed1)*100;
//	float PWM2=(PID_realize(&PID_Motor2_Speed,Speed2)+Speed2)*100;
	 PWM1=PID_realize(&PID_Motor1_Speed,Speed1);
	 PWM2=PID_realize(&PID_Motor2_Speed,Speed2);
//printf("pwm1:%f,Speed1:%f\n",PWM1,Speed1);
	
	if(PWM1>90)
	{
		
		PWM1=90;
	}
	else if(PWM1<-90)
	{
		PWM1=-90;
	}
		if(PWM2>90)
	{
		PWM2=90;
	}
	else if(PWM2<-90)
	{
		PWM2=-90;
	}

	//Motor_SetPwm(PWM1,PWM2);
	//Motor_SetPwm(PWM,);
}





