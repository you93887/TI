#include "headfile.h" 
#include "npwm.h"


static uint16_t period;


#define PWM_GEN_MODE PWM_GEN_MODE_UP_DOWN
#define PWM_GEN_SYNC_MODE PWM_GEN_MODE_NO_SYNC
#define MOTOR_PWM_SYSCTL_PWMDIV   SYSCTL_PWMDIV_64//80M/64=1.25M=0.8us
#define MOTOR_PERIOD_MAX_800US    1000//电机PWM频率——1.25khz
#define MOTOR_PERIOD_MAX_2500US   3125
#define MOTOR_PERIOD_MAX_10000US  12500
#define MOTOR_PERIOD_MAX_20000US  25000


/***************************************
函数名:	void PWM0_Init(void)
说明: PWM0芯片资源初始化
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void PWM0_Init(void)
{
  SysCtlPWMClockSet(MOTOR_PWM_SYSCTL_PWMDIV); // Set divider to 80M/8=10M=0.1us
  SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0); // Enable PWM peripheral
  SysCtlDelay(2); // Insert a few cycles after enabling the peripheral to allow the clock to be fully activated
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH); // Enable GPIOB peripheral
  SysCtlDelay(2); // Insert a few cycles after enabling the peripheral to allow the clock to be fully activated
  // Use alternate function
  GPIOPinConfigure(GPIO_PH0_M0PWM0);
	GPIOPinConfigure(GPIO_PH1_M0PWM1);
	GPIOPinConfigure(GPIO_PH2_M0PWM2);
	GPIOPinConfigure(GPIO_PH3_M0PWM3);
	GPIOPinConfigure(GPIO_PH4_M0PWM4);
	GPIOPinConfigure(GPIO_PH5_M0PWM5);
	GPIOPinConfigure(GPIO_PH6_M0PWM6);
	GPIOPinConfigure(GPIO_PH7_M0PWM7);
	
  // Use pin with PWM peripheral
  GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_0);//M0PWM0
  GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_1);//M0PWM1
  GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_2);//M0PWM2
  GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_3);//M0PWM3
	GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_4);//M0PWM4
	GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_5);//M0PWM5
	GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_6);//M0PWM6
	GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_7);//M0PWM7
  // Configure the PWM generator for count down mode with immediate updates to the parameters
  PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE | PWM_GEN_SYNC_MODE);
  PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE | PWM_GEN_SYNC_MODE);
	PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE | PWM_GEN_SYNC_MODE);
	PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE | PWM_GEN_SYNC_MODE);
	
  // The period is set to 100us (10 KHz)
  period = MOTOR_PERIOD_MAX_800US; 
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, period); // Set the period
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, period);
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, period);
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, period);
  // Start the timers in generator 0 and 1
  PWMGenEnable(PWM0_BASE, PWM_GEN_0);
  PWMGenEnable(PWM0_BASE, PWM_GEN_1);
	PWMGenEnable(PWM0_BASE, PWM_GEN_2);
	PWMGenEnable(PWM0_BASE, PWM_GEN_3);
  // Enable the outputs
  PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT 
													| PWM_OUT_2_BIT | PWM_OUT_3_BIT
													| PWM_OUT_4_BIT | PWM_OUT_5_BIT
													| PWM_OUT_6_BIT | PWM_OUT_7_BIT
													,  true);									
}

/***************************************
函数名:	void PWM1_Init(void)
说明: PWM1芯片资源初始化
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void PWM1_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  // Unlock PF0 so we can change it to a GPIO input
  // Once we have enabled (unlocked) the commit register then re-lock it
  // to prevent further changes.  PF0 is muxed with NMI thus a special case.
  HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
  HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;
  HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
	
	
  SysCtlPWMClockSet(MOTOR_PWM_SYSCTL_PWMDIV); // Set divider to 80M/8=10M=0.1us
  SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1); // Enable PWM peripheral
  SysCtlDelay(2); // Insert a few cycles after enabling the peripheral to allow the clock to be fully activated
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG); // Enable GPIOB peripheral
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Enable GPIOB peripheral
  SysCtlDelay(2); // Insert a few cycles after enabling the peripheral to allow the clock to be fully activated
  // Use alternate function
  GPIOPinConfigure(GPIO_PG2_M1PWM0);
	GPIOPinConfigure(GPIO_PG3_M1PWM1);
	GPIOPinConfigure(GPIO_PG4_M1PWM2);
	GPIOPinConfigure(GPIO_PG5_M1PWM3);
  GPIOPinConfigure(GPIO_PF0_M1PWM4);
  GPIOPinConfigure(GPIO_PF1_M1PWM5);
  GPIOPinConfigure(GPIO_PF2_M1PWM6);
  GPIOPinConfigure(GPIO_PF3_M1PWM7);
	
  // Use pin with PWM peripheral
  GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_2);//M1PWM0
  GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_3);//M1PWM1
  GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_4);//M1PWM2
  GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_5);//M1PWM3
  GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_0);//M1PWM4
  GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);//M1PWM5
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);//M1PWM6
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);//M1PWM7
	
  // Configure the PWM generator for count down mode with immediate updates to the parameters
  PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE | PWM_GEN_SYNC_MODE);
  PWMGenConfigure(PWM1_BASE, PWM_GEN_1, PWM_GEN_MODE | PWM_GEN_SYNC_MODE);
  PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE | PWM_GEN_SYNC_MODE);
  PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE | PWM_GEN_SYNC_MODE);
	
  // The period is set to 2.5ms (400 Hz)
  period = MOTOR_PERIOD_MAX_20000US; 
  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, period); // Set the period
  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, period);
  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, period); // Set the period
  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, period);
  // Start the timers in generator 0 and 1
  PWMGenEnable(PWM1_BASE, PWM_GEN_0);
  PWMGenEnable(PWM1_BASE, PWM_GEN_1);
  PWMGenEnable(PWM1_BASE, PWM_GEN_2);
	PWMGenEnable(PWM1_BASE, PWM_GEN_3);
  // Enable the outputs
  PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT 
													| PWM_OUT_2_BIT | PWM_OUT_3_BIT
													| PWM_OUT_4_BIT | PWM_OUT_5_BIT
													| PWM_OUT_6_BIT | PWM_OUT_7_BIT
													,  true);
}


/***************************************
函数名:	void PWM_Output((uint16_t width1,uint16_t width2,uint16_t width3,uint16_t width4))
说明: uint16_t width1-待输出的脉冲宽度1
			uint16_t width2-待输出的脉冲宽度2
			uint16_t width3-待输出的脉冲宽度3
			uint16_t width4-待输出的脉冲宽度4
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void PWM_Output(uint16_t width1,uint16_t width2,uint16_t width3,uint16_t width4)
{
	uint16_t pwm[4]={0};
	pwm[0]=width1;
	pwm[1]=width2;
	pwm[2]=width3;
	pwm[3]=width4;

	//EPWM1端口M0PWM0、M0PWM1、M0PWM2、M0PWM3
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_0,pwm[0]);//PH0——M0PWM0 
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_1,pwm[1]);//PH1——M0PWM1 
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_2,pwm[2]);//PH2——M0PWM2 
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_3,pwm[3]);//PH3——M0PWM3 	
	
	//EPWM2端口M0PWM4、M0PWM5、M0PWM6、M0PWM7	 
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_4,pwm[0]);//PH4——M0PWM4
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_5,pwm[1]);//PH5——M0PWM5
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_6,pwm[2]);//PH6——M0PWM6
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_7,pwm[3]);//PH7——M0PWM7 
}      

/***************************************
函数名:	void steer_servo_pwm_m1p0(uint16_t us)
说明: uint16_t us-输出脉宽值
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void steer_servo_pwm_m1p0(uint16_t us)//M1PWM0——PG2
{
	PWMPulseWidthSet(PWM1_BASE,PWM_OUT_0,1.25*us);//PG2——M1PWM0——排针PWM端口
}

/***************************************
函数名:	void steer_servo_pwm_m1p1(uint16_t us)
说明: uint16_t us-输出脉宽值
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void steer_servo_pwm_m1p1(uint16_t us)//M1PWM1——PG3
{
	PWMPulseWidthSet(PWM1_BASE,PWM_OUT_1,1.25*us);//PG3——M1PWM1——排针PWM端口
}

/***************************************
函数名:	void steer_servo_pwm_m1p2(uint16_t us)
说明: uint16_t us-输出脉宽值
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void steer_servo_pwm_m1p2(uint16_t us)//M1PWM2——PG4
{
	PWMPulseWidthSet(PWM1_BASE,PWM_OUT_2,1.25*us);//PG4——M1PWM2——排针PWM端口
}

/***************************************
函数名:	void steer_servo_pwm_m1p3(uint16_t us)
说明: uint16_t us-输出脉宽值
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void steer_servo_pwm_m1p3(uint16_t us)//M1PWM3——PG5
{
	PWMPulseWidthSet(PWM1_BASE,PWM_OUT_3,1.25*us);//PG5——M1PWM3——排针PWM端口
}


/***************************************
函数名:	void steer_servo_pwm_m1p4(uint16_t us)
说明: uint16_t us-输出脉宽值
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void steer_servo_pwm_m1p4(uint16_t us)//M1PWM4——PH4
{
	PWMPulseWidthSet(PWM1_BASE,PWM_OUT_4,1.25*us);//PH4——M1PWM4——排针PWM端口
}

/***************************************
函数名:	void steer_servo_pwm_m1p5(uint16_t us)
说明: uint16_t us-输出脉宽值
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void steer_servo_pwm_m1p5(uint16_t us)//M1PWM5——PH5
{
	PWMPulseWidthSet(PWM1_BASE,PWM_OUT_5,1.25*us);//PH5——M1PWM5——排针PWM端口
}

/***************************************
函数名:	void steer_servo_pwm_m1p6(uint16_t us)
说明: uint16_t us-输出脉宽值
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void steer_servo_pwm_m1p6(uint16_t us)//M1PWM6——PH6
{
	PWMPulseWidthSet(PWM1_BASE,PWM_OUT_6,1.25*us);//PH6——M1PWM6——排针PWM端口
}

/***************************************
函数名:	void steer_servo_pwm_m1p7(uint16_t us)
说明: uint16_t us-输出脉宽值
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void steer_servo_pwm_m1p7(uint16_t us)//M1PWM7——PH7
{
	PWMPulseWidthSet(PWM1_BASE,PWM_OUT_7,1.25*us);//PH7——M1PWM7——排针PWM端口
}
