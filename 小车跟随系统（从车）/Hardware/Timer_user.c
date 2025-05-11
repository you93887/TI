/***************************************************
 * @brief   关于TM4C123GH6PZ芯片的定时器初始化
 *          以及对应的中断服务函数定义。
 *          包括一个返回当前时间函数
 *
 *					T*CCP0    T*CCP1									WT*CCP0    WT*CCP1
 *			TIMER0   PF0  PF1		    	        			WTIMER0   PC4  PC5、  PG4  PG5
 *			TIMER1   PB4  PB5、  PF2  PF3、  PJ0  PJ1		 	WTIMER1   PC6  PC7、  PG6  PG7
 *			TIMER2   PB0  PB1、  PF5  PF4、  ***  PJ2		    WTIMER2   PD0  PD1、  PH0  PH1
 *			TIMER3   PB2  PB3、  PF6  PF7		    			WTIMER3   PD2  PD3、  PH4  PH5
 *			TIMER4   PG0  PG1、  PC0  PC1		 	    		WTIMER4   PD4  PD5、  PH6  PH7
 *			TIMER5   PG2  PG3、  PC2  PC3		 	    		WTIMER5   PD6  PD7、  PH2  PH3
 *
 *          默认是定时器0，不分A B， 向上计数  
 *          1ms进入中断一次，优先级为0
 * @date    2024/6/21
 * @author  qihang chen
****************************************************/

#include "include.h"


/*
定时器模式参考https://blog.csdn.net/weixin_56003594/article/details/125662058

1.SysCtlPeripheralEnable(uint32_t ui32Peripheral)
参数：ui32Peripheral如果为16/32bit的定时器就是TIMER，如果是32/64bit的定时器就是WTIMER。
作用：使能外设

2. TimerConfigure(uint32_t ui32Base, uint32_t ui32Config)
参数：ui32Base为定时器基地址，ui32Config定时器所配置模式
作用：如果在定时器不拆分的情况下，可以将ui32Config设置为以下模式之一：
        TIMER_CFG_ONE_SHOT – 单次减计数模式
        TIMER_CFG_ONE_SHOT_UP – 单次加计数模式
        TIMER_CFG_PERIODIC – 连续减计数模式
        TIMER_CFG_PERIODIC_UP – 连续加计数模式
        TIMER_CFG_RTC – 实时时钟模式
如果将定时器拆分的话则将ui32Config设置为TIMER_CFG_SPLIT_PAIR（分裂为一对）然后与以下模式进行或运算：
        TIMER_CFG_A_ONE_SHOT – 定时器A单次减计数
        TIMER_CFG_A_ONE_SHOT_UP –定时器A单次加计数
        TIMER_CFG_A_PERIODIC – 定时器A连续减计数
        TIMER_CFG_A_PERIODIC_UP – 定时器A连续加计数

        TIMER_CFG_B_ONE_SHOT – 定时器B单次减计数
        TIMER_CFG_B_ONE_SHOT_UP –定时器B单次加计数
        TIMER_CFG_B_PERIODIC – 定时器B连续减计数
        TIMER_CFG_B_PERIODIC_UP – 定时器B连续加计数

3.TimerLoadSet(uint32_t ui32Base, uint32_t ui32Timer, uint32_t ui32Value)
参数：ui32Base为定时器基地址，ui32Timer有TIMER_A，TIMER_B和TIMER_BOTH（单独A，单独B，AB都用）三种选择，
在定时器拆分的情况下用哪个就设置哪个，在不拆分的情况下就设置为TIMER_A（对于其他函数也是，如果在级联的情况下还需要选择，那默认为选择TIMER_A），
ui32Value为定时器加载值，一般来说定时时间=1/N，那么定时器加载值=SysCtlClockGet()/N-1，即根据自己的定时时间求出N，即可得定时器加载值。
此函数适用于16bit，32bit的定时器，即适用于16/32bit的定时器拆分与级联，32/64bit定时器的拆分三种情况，对于64bit的定时器，即32/64bit定时器的级联使用TimerLoadSet64。
作用：配置定时器加载值。

4. IntEnable(INT_TIMER0A); 总中断使能

5.TimerIntEnable(uint32_t ui32Base, uint32_t ui32IntFlags)
参数：ui32Base为定时器基地址，ui32IntFlags为中断模式，在定时器状态下一般设置为TIMER_TIMB_TIMEOUT或TIMER_TIMA_TIMEOUT
作用：使能定时器中断

6.TimerIntRegister(uint32_t ui32Base, uint32_t ui32Timer,void (*pfnHandler)(void))
参数：ui32Base为定时器基地址，ui32Timer也是有TIMER_A，TIMER_B和TIMER_BOTH三种，对于级联的定时器设置为TIMER_A即可，void (*pfnHandler)(void)为中断函数名
作用：注册中断函数

7. IntMasterEnable() 启用处理器中断

8.TimerEnable(uint32_t ui32Base, uint32_t ui32Timer)
参数：ui32Base为定时器基地址，ui32Timer也是有TIMER_A，TIMER_B和TIMER_BOTH三种。
作用： 定时器使能开始计数

9. IntPrioritySet(INT_TIMER0A, 0xA0);    
第一个参数 ：指定要设置优先级的中断。   第二个参数：指定中断的优先级  如0xE0
作用 设置中断优先级   

*/



/*
*  定时器初始化函数
*/
void TIMER0A_Handler(void);
void Timer_init(void)//系统调度定时器初始化
{
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);					//定时器0使能	  可以查看括号里面的参数看有哪些参数	
  ROM_TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);						//32位周期定时器			  查看函数定义有参数选择 如向上计数   	
  ROM_TimerLoadSet(TIMER0_BASE,TIMER_A,SysCtlClockGet()/100);		//设定装载值,（80M/1000）*1/80M=1ms				
  ROM_IntEnable(INT_TIMER0A);										//总中断使能			右键查看参数位置可知响应的头文件和类似的参数	
  ROM_TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT); 					//中断输出, 设置模式;	查看函数定于里面有模式选择		
  TimerIntRegister(TIMER0_BASE,TIMER_A,TIMER0A_Handler);		//中断函数注册  第三个参数为中断反馈寒素 
  ROM_IntMasterEnable();			
  ROM_TimerEnable(TIMER0_BASE,TIMER_A); 												//定时器使能开始计数
  ROM_IntPrioritySet(INT_TIMER0A, 0xA0);    //设置中断优先级   第一个参数 ：指定要设置优先级的中断。   第二个参数：指定中断的优先级  如0xE0
}


/*
次结构体可以保存   用到的话可以用
float Last_Time：用于存储上一个时间点的时间，以浮点数表示。
float Now_Time：用于存储当前时间点的时间，以浮点数表示。
float Time_Delta：用于存储两个时间点之间的时间差，以浮点数表示。
uint16_t Time_Delta_INT：用于存储时间差的整数部分，以16位无符号整数表示，单位为毫秒。
*/
//用到需要包括 SystickTime.h文件
void Test_Period(Testime *Time_Lab)
{
  Time_Lab->Last_Time=Time_Lab->Now_Time;
  Time_Lab->Now_Time=micros()/1000.0f;             //(10000*TIME_ISR_CNT+TimerValueGet(TIMER1_BASE,TIMER_A)/80.0f)/1000.0f;//单位ms
  Time_Lab->Time_Delta=(Time_Lab->Now_Time-Time_Lab->Last_Time);
  Time_Lab->Time_Delta_INT=(uint16_t)(Time_Lab->Time_Delta);
}

//返回的时间是ms
Testime Time0_Delta;
//void TIMER0A_Handler(void)				//系统调度中断函数  是自己定义函数，需要用函数开启TimerIntRegister（）函数如下定时器初始化中
//{
//	Test_Period(&Time0_Delta);
//	//在此添加周期任务函数
//	
//	ROM_TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);     //清除计时器中断源  第一个参数为开启的定时器 ， 第二个参数为定时器的模式再TimerIntEnable（）函数的第二个参数
//}



/****************************************
一下函数未使用，不清楚如何使用，
2024.6.10

************************************/



