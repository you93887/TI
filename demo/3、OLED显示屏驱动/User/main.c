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
*	�� �� ��: PrintfLogo
*	����˵��: ��ӡ�������ƺ����̷�������, ���ϴ����ߺ󣬴�PC���Ĵ����ն�������Թ۲���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void PrintfLogo(void)
{
	printf("*************************************************************\n\r");
	printf("* ��������   : %s\r\n", "OLED��ʾ������");	/* ��ӡ�������� */
	printf("* ���̰汾   : %s\r\n", "V1.0");		  /* ��ӡ���̰汾 */
	printf("* ��������   : %s\r\n", "20221125");	/* ��ӡ�������� */
	printf("* ��׼��汾 : TM4C123GH6PZT7\r\n");
	printf("* \r\n");	/* ��ӡһ�пո� */
	printf("* QQ    : 3138372165 \r\n");
	printf("* Email : 3138372165@qq.com \r\n");
	printf("* Copyright www.nameless.tech ��������\r\n");
	printf("*************************************************************\n\r");
}

Testime Time0_Delta;
uint32_t cnt=0;
void TIMER0A_Handler(void)				//ϵͳ�����жϺ���
{
  Test_Period(&Time0_Delta);
  //�ڴ��������������
	cnt++;
  TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
}


void ConfigureUART(void)//��ʼ������1
{ 
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);// Enable the GPIO Peripheral used by the UART.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);// Enable UART1
	  ROM_GPIOPinConfigure(GPIO_PB0_U1RX);// Configure GPI1 Pins for UART mode.
    ROM_GPIOPinConfigure(GPIO_PB1_U1TX);
    ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // Use the internal 16MHz oscillator as the UART clock source.
    // Initialize the UART for console I/O.
    // ����UART����
    UARTConfigSetExpClk(UART1_BASE, 16000000, 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));
		UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);
}

void TIM0_Init()//��ʱ��0��ʼ��
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);							//��ʱ��0ʹ��				
  TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);						//32λ���ڶ�ʱ��				
  TimerLoadSet(TIMER0_BASE,TIMER_A,SysCtlClockGet()/1);		//�趨װ��ֵ,��80M/1��*1/80M=1s				
  IntEnable(INT_TIMER0A);																		//���ж�ʹ��				
  TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT); 					//�ж����, ����ģʽ;			
  TimerIntRegister(TIMER0_BASE,TIMER_A,TIMER0A_Handler);		//�жϺ���ע��
  IntMasterEnable();			
  TimerEnable(TIMER0_BASE,TIMER_A); 												//��ʱ��ʹ�ܿ�ʼ����
  IntPrioritySet(INT_TIMER0A,USER_INT7);
	
}

int fputc(int ch, FILE *f){UARTCharPut(UART1_BASE,ch);	return (ch);}
int fgetc(FILE *f) {int ch=UARTCharGet(UART1_BASE);	return (ch);}
int main(void)
{  
		ROM_FPUEnable();//ʹ�ܸ��㵥Ԫ
		ROM_FPULazyStackingEnable();//�����ӳٶ�ջ,�����ж���Ӧ�ӳ�  
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);//����ϵͳʱ��
	  initTime();//��ʼ���δ�ʱ��
    ConfigureUART();//��ʼ������0
			//	PrintfLogo();//���ڴ�ӡ�汾��Ϣ
	  OLED_Init();//OLED��ʾ����ʼ��
	  TIM0_Init();
	    const char *message = "Hello, UART!\r\n";
	    while (*message)
    {
        // ���͵����ַ�
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
			        // ����Ƿ������ݿɽ���
        if (UARTCharsAvail(UART1_BASE))
        {
            // �� UART ����һ���ַ�
            char receivedChar = UARTCharGet(UART1_BASE);

            // �ش����յ����ַ�
            UARTCharPut(UART1_BASE, receivedChar);
        }
		//	printf("%d\n",cnt);
//			LCD_clear_L(0,4);
//			display_6_8_number(0,4,(int)(cnt/100));
//			display_6_8_string(70,4,"second");
    }
}
