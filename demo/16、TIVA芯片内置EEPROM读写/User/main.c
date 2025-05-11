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
#include "eeprom.h"
#include "uart.h"
#include "uartstdio.h"
#include "SystickTime.h"
#include "oled.h"
#include "neeprom.h"

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif


/*
*********************************************************************************************************
*	�� �� ��: PrintfLogo
*	����˵��: ��ӡ�������ƺ����̷�������, ���ϴ����ߺ󣬴�PC���Ĵ����ն�������Թ۲���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void PrintfLogo(void)
{
	printf("*************************************************************\n\r");
	printf("* ��������   : %s\r\n", "оƬ����EEPROM��д");	/* ��ӡ�������� */
	printf("* ���̰汾   : %s\r\n", "V1.0");		  /* ��ӡ���̰汾 */
	printf("* ��������   : %s\r\n", "20221125");	/* ��ӡ�������� */
	printf("* ��׼��汾 : TM4C123GH6PZT7\r\n");
	printf("* \r\n");	/* ��ӡһ�пո� */
	printf("* QQ    : 3138372165 \r\n");
	printf("* Email : 3138372165@qq.com \r\n");
	printf("* Copyright www.nameless.tech ��������\r\n");
	printf("*************************************************************\n\r");
}


float write_data[5]={0.01f,3.1415f,6,-50,300.0f};
float read_data[5]={0};

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
		ROM_FPUEnable();//ʹ�ܸ��㵥Ԫ
		ROM_FPULazyStackingEnable();//�����ӳٶ�ջ,�����ж���Ӧ�ӳ�
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);//����ϵͳʱ��
		EEPROM_Init();//EEPROM��ʼ��
	  initTime();//��ʼ���δ�ʱ��
    ConfigureUART();//��ʼ������0
    PrintfLogo();//���ڴ�ӡ�汾��Ϣ
	  OLED_Init();//OLED��ʾ����ʼ��
		//EEPROMд�����
		EEPROMProgram((uint32_t *)(&write_data[0]),0,4);
		EEPROMProgram((uint32_t *)(&write_data[1]),4,4);
		EEPROMProgram((uint32_t *)(&write_data[2]),8,4);
		EEPROMProgram((uint32_t *)(&write_data[3]),12,4);
		EEPROMProgram((uint32_t *)(&write_data[4]),16,4);
	 //EEPROM��ȡ����
		EEPROMRead((uint32_t *)(&read_data[0]),0,4);
		EEPROMRead((uint32_t *)(&read_data[1]),4,4);
		EEPROMRead((uint32_t *)(&read_data[2]),8,4);
		EEPROMRead((uint32_t *)(&read_data[3]),12,4);
		EEPROMRead((uint32_t *)(&read_data[4]),16,4);
    while(1)
    {
			Testime t0;
			Test_Period(&t0);
			LCD_clear_L(0,0);display_6_8_string(0,0,"System  Time:  MS");
			LCD_clear_L(0,1);display_6_8_number(0,1,t0.Now_Time);
			LCD_clear_L(0,2);display_6_8_string(0,2,"write      read");
			LCD_clear_L(0,3);display_6_8_number(0,3,write_data[0]);display_6_8_number(65,3,read_data[0]);
			LCD_clear_L(0,4);display_6_8_number(0,4,write_data[1]);display_6_8_number(65,4,read_data[1]);
			LCD_clear_L(0,5);display_6_8_number(0,5,write_data[2]);display_6_8_number(65,5,read_data[2]);
			LCD_clear_L(0,6);display_6_8_number(0,6,write_data[3]);display_6_8_number(65,6,read_data[3]);
			LCD_clear_L(0,7);display_6_8_number(0,7,write_data[4]);display_6_8_number(65,7,read_data[4]);
    }
}
