#include "stdio.h"
#include <stdint.h>
#include <stdbool.h>
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_gpio.h"
#include "debug.h"
#include "fpu.h"
#include "gpio.h"
#include "pin_map.h"
#include "rom.h"
#include "sysctl.h"
#include "uart.h"
#include "uartstdio.h"
#include "SystickTime.h"
#include "oled.h"
#include "timer.h"
#include "interrupt.h"
#include "Headfile.h"
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif


/*
*********************************************************************************************************
*	函 数 名: PrintfLogo
*	功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的串口终端软件可以观察结果
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void PrintfLogo(void)
{
	printf("*************************************************************\n\r");
	printf("* 例程名称   : %s\r\n", "OLED显示屏驱动");	/* 打印例程名称 */
	printf("* 例程版本   : %s\r\n", "V1.0");		  /* 打印例程版本 */
	printf("* 发布日期   : %s\r\n", "20221125");	/* 打印例程日期 */
	printf("* 标准库版本 : TM4C123GH6PZT7\r\n");
	printf("* \r\n");	/* 打印一行空格 */
	printf("* QQ    : 3138372165 \r\n");
	printf("* Email : 3138372165@qq.com \r\n");
	printf("* Copyright www.nameless.tech 无名创新\r\n");
	printf("*************************************************************\n\r");
}

Testime Time0_Delta;
uint32_t cnt=0;
void TIMER0A_Handler(void)				//系统调度中断函数
{
  Test_Period(&Time0_Delta);
  //在此添加周期任务函数
	cnt++;
  TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
}


void ConfigureUART(void)//初始化串口1
{ 
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);// Enable the GPIO Peripheral used by the UART.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);// Enable UART1
	  ROM_GPIOPinConfigure(GPIO_PB0_U1RX);// Configure GPI1 Pins for UART mode.
    ROM_GPIOPinConfigure(GPIO_PB1_U1TX);
    ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // Use the internal 16MHz oscillator as the UART clock source.
    // Initialize the UART for console I/O.
    // 配置UART参数
    UARTConfigSetExpClk(UART1_BASE, 16000000, 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));
		UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);
}

void TIM0_Init()//定时器0初始化
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);							//定时器0使能				
  TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);						//32位周期定时器				
  TimerLoadSet(TIMER0_BASE,TIMER_A,SysCtlClockGet()/1);		//设定装载值,（80M/1）*1/80M=1s				
  IntEnable(INT_TIMER0A);																		//总中断使能				
  TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT); 					//中断输出, 设置模式;			
  TimerIntRegister(TIMER0_BASE,TIMER_A,TIMER0A_Handler);		//中断函数注册
  IntMasterEnable();			
  TimerEnable(TIMER0_BASE,TIMER_A); 												//定时器使能开始计数
  IntPrioritySet(INT_TIMER0A,USER_INT7);
	
}

int fputc(int ch, FILE *f){UARTCharPut(UART1_BASE,ch);	return (ch);}
int fgetc(FILE *f) {int ch=UARTCharGet(UART1_BASE);	return (ch);}
int main(void)
{  
		ROM_FPUEnable();//使能浮点单元
		ROM_FPULazyStackingEnable();//浮点延迟堆栈,减少中断响应延迟  
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);//配置系统时钟
	  initTime();//初始化滴答定时器
    ConfigureUART();//初始化串口0
			//	PrintfLogo();//串口打印版本信息
	  OLED_Init();//OLED显示屏初始化
	  TIM0_Init();
	    const char *message = "Hello, UART!\r\n";
	    while (*message)
    {
        // 发送单个字符
        UARTCharPut(UART1_BASE, *message++);
    }
    while(1)
    {
	
//			Testime t0;
//			Test_Period(&t0);
//			display_6_8_string(0,0,"System  Time:  MS");
//			display_6_8_number(0,1,t0.Now_Time);
//						Testime t0;
//			Test_Period(&t0);
//			LCD_clear_L(0,0);display_6_8_string(0,0,"System  Time:  MS");
//			LCD_clear_L(0,1);display_6_8_number(0,1,t0.Now_Time);
//			LCD_clear_L(0,2);display_6_8_string(0,2,"Time0 Period:  MS");
//			LCD_clear_L(0,3);
			display_6_8_number(0,3,Time0_Delta.Time_Delta);
			display_6_8_number(70,3,cnt);
			display_6_8_number(0,5,12.8);
			float k=23.83;
			write_6_8_number(0,7,k);
			        // 检查是否有数据可接收
        if (UARTCharsAvail(UART1_BASE))
        {
            // 从 UART 接收一个字符
            char receivedChar = UARTCharGet(UART1_BASE);

            // 回传接收到的字符
            UARTCharPut(UART1_BASE, receivedChar);
        }
		//	printf("%d\n",cnt);
//			LCD_clear_L(0,4);
//			display_6_8_number(0,4,(int)(cnt/100));
//			display_6_8_string(70,4,"second");
    }
}
