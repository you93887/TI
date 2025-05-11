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
#include "npwm.h"

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
	printf("* 例程名称   : %s\r\n", "系统板16路硬件PWM输出");	/* 打印例程名称 */
	printf("* 例程版本   : %s\r\n", "V1.0");		  /* 打印例程版本 */
	printf("* 发布日期   : %s\r\n", "20221125");	/* 打印例程日期 */
	printf("* 标准库版本 : TM4C123GH6PZT7\r\n");
	printf("* \r\n");	/* 打印一行空格 */
	printf("* QQ    : 3138372165 \r\n");
	printf("* Email : 3138372165@qq.com \r\n");
	printf("* Copyright www.nameless.tech 无名创新\r\n");
	printf("*************************************************************\n\r");
}



void ConfigureUART(void)
{ 
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);// Enable the GPIO Peripheral used by the UART.   
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);// Enable UART0
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);// Configure GPIO Pins for UART mode.
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // Use the internal 16MHz oscillator as the UART clock source.
    // Initialize the UART for console I/O.
    UARTStdioConfig(0, 115200, 16000000);
		UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
}

int fputc(int ch, FILE *f){UARTCharPut(UART0_BASE,ch);	return (ch);}
int fgetc(FILE *f) {int ch=UARTCharGet(UART0_BASE);	return (ch);}
int main(void)
{
		ROM_FPUEnable();//使能浮点单元
		ROM_FPULazyStackingEnable();//浮点延迟堆栈,减少中断响应延迟  
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |	SYSCTL_OSC_MAIN);//配置系统时钟
		PWM0_Init();						//PWM0初始化M0P0、M0P1、M0P2、M0P3、M0P4、M0P5、M0P6、M0P7
		PWM1_Init();						//PWM1初始化M1P0、M1P1、M1P2、M1P3、M1P4、M1P5、M1P6、M1P7		
	  initTime();//初始化滴答定时器
    ConfigureUART();//初始化串口0
    PrintfLogo();//串口打印版本信息
	  OLED_Init();//OLED显示屏初始化
    while(1)
    {
			Testime t0;
			Test_Period(&t0);
			LCD_clear_L(0,0);
			display_6_8_string(0,0,"System  Time:  MS");
			LCD_clear_L(0,1);
			display_6_8_number(0,1,t0.Now_Time);
			PWM_Output(250,0,0,250);
			static int16_t dir=1;
			static int16_t cnt=-1000;
			cnt+=dir;
			if(cnt>1000||cnt<-1000)  dir*=(-1);
			
			steer_servo_pwm_m1p0(1500+cnt);
			steer_servo_pwm_m1p1(1500+cnt);
			steer_servo_pwm_m1p2(1500+cnt);
			steer_servo_pwm_m1p3(1500+cnt);
			steer_servo_pwm_m1p4(1500+cnt);
			steer_servo_pwm_m1p5(1500+cnt);
			steer_servo_pwm_m1p6(1500+cnt);
			steer_servo_pwm_m1p7(1500+cnt);
			LCD_clear_L(0,2);
			display_6_8_number(0,2,1500+cnt);
    }
}
