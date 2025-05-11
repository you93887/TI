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
#include "gray_detection.h"

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
	printf("* 例程名称   : %s\r\n", "自主寻迹12路灰度传感器检测");	/* 打印例程名称 */
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
		gpio_input_init();//灰度管gpio初始化
	  initTime();//初始化滴答定时器
    ConfigureUART();//初始化串口0
    PrintfLogo();//串口打印版本信息
	  OLED_Init();//OLED显示屏初始化
    while(1)
    {
			Testime t0;
			Test_Period(&t0);
			LCD_clear_L(0,0);display_6_8_string(0,0,"System  Time:  MS");
			LCD_clear_L(0,1);display_6_8_number(0,1,t0.Now_Time);
			LCD_clear_L(0,2);display_6_8_string(0,2,"gray_state");display_6_8_number(90,2,gray_status[0]);
			LCD_clear_L(0,3);
			display_6_8_number(0,3,gray_state.gray.bit12);
			display_6_8_number(10,3,gray_state.gray.bit11);
			display_6_8_number(20,3,gray_state.gray.bit10);
			display_6_8_number(30,3,gray_state.gray.bit9);
			display_6_8_number(40,3,gray_state.gray.bit8);
			display_6_8_number(50,3,gray_state.gray.bit7);
			display_6_8_number(60,3,gray_state.gray.bit6);
			display_6_8_number(70,3,gray_state.gray.bit5);
			display_6_8_number(80,3,gray_state.gray.bit4);
			display_6_8_number(90,3,gray_state.gray.bit3);
			display_6_8_number(100,3,gray_state.gray.bit2);
			display_6_8_number(110,3,gray_state.gray.bit1);
			LCD_clear_L(0,4);display_6_8_string(0,4,"left           right");
			gpio_input_check_channel_12();//灰度管检测并量化偏差
    }
}
