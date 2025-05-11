#include "stdio.h"
#include <stdint.h>
#include <stdbool.h>
#include "hw_memmap.h"
#include "hw_types.h"
#include "debug.h"
#include "fpu.h"
#include "gpio.h"
#include "pin_map.h"
#include "rom.h"
#include "sysctl.h"
#include "uart.h"
#include "uartstdio.h"
#include "SystickTime.h"
#include "Headfile.h"
#include "Usart.h"
// GPIO和PWM引脚定义
// GPIO引脚定义
#define STEP_PIN GPIO_PIN_3
#define DIR_PIN GPIO_PIN_2
#define ENABLE_PIN GPIO_PIN_3
#define MOTOR_PORT GPIO_PORTF_BASE
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

Testime Time0_Delta;
uint32_t cnt=0;


void TIMER0A_Handler(void)				//系统调度中断函数
{
  Test_Period(&Time0_Delta);
  //在此添加周期任务函数
	cnt++;
  TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
}



void Time_init(void)//系统调度定时器初始化
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);							//定时器0使能				
  TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);						//32位周期定时器				
  TimerLoadSet(TIMER0_BASE,TIMER_A,SysCtlClockGet()/100);		//设定装载值,（80M/100）*1/80M=10ms				
  IntEnable(INT_TIMER0A);																		//总中断使能				
  TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT); 					//中断输出, 设置模式;			
  TimerIntRegister(TIMER0_BASE,TIMER_A,TIMER0A_Handler);		//中断函数注册
  IntMasterEnable();			
  TimerEnable(TIMER0_BASE,TIMER_A); 												//定时器使能开始计数
  IntPrioritySet(INT_TIMER0A,USER_INT7);
}






static uint16_t period;


#define PWM_GEN_MODE PWM_GEN_MODE_UP_DOWN
#define PWM_GEN_SYNC_MODE PWM_GEN_MODE_NO_SYNC
#define MOTOR_PWM_SYSCTL_PWMDIV   SYSCTL_PWMDIV_64//80M/64=1.25M=0.8us
#define MOTOR_PERIOD_MAX_800US    1000//电机PWM频率——1.25khz
#define MOTOR_PERIOD_MAX_2500US   3125
#define MOTOR_PERIOD_MAX_10000US  12500
#define MOTOR_PERIOD_MAX_20000US  25000
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

	
  // Use pin with PWM peripheral开启PWM引脚服用
  GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_0);//M0PWM0
  GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_1);//M0PWM1
//  GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_2);//M0PWM2
//  GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_3);//M0PWM3
//	GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_4);//M0PWM4
//	GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_5);//M0PWM5
//	GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_6);//M0PWM6
//	GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_7);//M0PWM7
  // Configure the PWM generator for count down mode with immediate updates to the parameters
  PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE | PWM_GEN_SYNC_MODE);
  PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE | PWM_GEN_SYNC_MODE);
//	PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE | PWM_GEN_SYNC_MODE);
//	PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE | PWM_GEN_SYNC_MODE);
	
  // The period is set to 100us (10 KHz)
  period = MOTOR_PERIOD_MAX_800US; 
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, period); // Set the period
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, period);
//	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, period);
//	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, period);
  // Start the timers in generator 0 and 1
  PWMGenEnable(PWM0_BASE, PWM_GEN_0);
  PWMGenEnable(PWM0_BASE, PWM_GEN_1);
//	PWMGenEnable(PWM0_BASE, PWM_GEN_2);
//	PWMGenEnable(PWM0_BASE, PWM_GEN_3);
  // Enable the outputs
  PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT 
													| PWM_OUT_2_BIT | PWM_OUT_3_BIT
													| PWM_OUT_4_BIT | PWM_OUT_5_BIT
													| PWM_OUT_6_BIT | PWM_OUT_7_BIT
													,  true);									
}


extern uint16_t dl1a_distance_mm;
extern uint8_t P;
uint16_t receive_cnt;//计算成功接收数据帧次数
uint8_t confidence;
uint16_t distance,noise,reftof;
uint32_t peak,intg;
uint8_t key;
extern int q;
uint8_t arr[8];
int main(void)
{  
		ROM_FPUEnable();//使能浮点单元
		ROM_FPULazyStackingEnable();//浮点延迟堆栈,减少中断响应延迟
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |	SYSCTL_OSC_MAIN);//配置系统时钟
	//  initTime();//初始化滴答定时器
	  //Time_init();//定时器1初始化
  	OLED_Init();
	//  UART3_Init(230400);
	  //dl1a_init();
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  GPIODirModeSet(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_6, GPIO_DIR_MODE_IN);
  GPIODirModeSet(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_6, GPIO_DIR_MODE_IN);
  GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_DIR_MODE_IN);
  GPIODirModeSet(GPIO_PORTG_BASE, GPIO_PIN_6, GPIO_DIR_MODE_IN);
  GPIODirModeSet(GPIO_PORTC_BASE, GPIO_PIN_1 | GPIO_PIN_3, GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_6, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);  //上拉推挽输入
	GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_6, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);	
	GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_1 | GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);	
	GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);	
	GPIOPadConfigSet(GPIO_PORTG_BASE, GPIO_PIN_6, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	GPIODirModeSet(GPIO_PORTK_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTK_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);  //上拉推挽输入

    while(1)
		{
	arr[0]=GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_6);
	arr[1]=GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_4);
	arr[2]=GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_4);
	arr[3]=GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_5);
	arr[4]=GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_6);
	arr[5]=GPIOPinRead(GPIO_PORTG_BASE,GPIO_PIN_6);
	arr[6]=GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_1);
	arr[7]=GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_3);
	key=GPIOPinRead(GPIO_PORTK_BASE,GPIO_PIN_3);
       OLED_ShowNum(0,0,arr[0],3,6);
       OLED_ShowNum(0,7,arr[1],3,6);
       OLED_ShowNum(0,14,arr[2],3,6);
       OLED_ShowNum(0,21,arr[3],3,6);
       OLED_ShowNum(0,28,arr[4],3,6);
       OLED_ShowNum(0,35,arr[5],3,6);
       OLED_ShowNum(0,42,arr[6],3,6);
       OLED_ShowNum(0,49,arr[7],3,6);			
			       OLED_ShowNum(0,56,key,3,6);
			OLED_Update();
    }
	}
