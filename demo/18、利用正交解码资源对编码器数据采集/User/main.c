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
#include "oled.h"
#include "Headfile.h"
#include "user.h"
#include "nqei.h"


motor_config trackless_motor={
	.left_encoder_dir_config=left_motor_encoder_dir_default,	//��������������
	.right_encoder_dir_config=right_motor_encoder_dir_default,//��������������
	.left_motion_dir_config=left_motion_dir_default,					//����˶���������
	.right_motion_dir_config=right_motion_dir_default,				//����˶���������
	.wheel_radius_cm=tire_radius_cm_default,									//�����ֵİ뾶
	.pulse_num_per_circle=pulse_cnt_per_circle_default,				//������ת��һȦʱ�������������ۼ�ֵ
	.servo_median_value1=servo_median_value1_default,					//�����е��ֵ1
	.servo_median_value2=servo_median_value2_default,					//�����е��ֵ2
};

float left_motor_speed_cmps=0,right_motor_speed_cmps=0;
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
	printf("* ��������   : %s\r\n", "��������������Դ�Ա��������ݲɼ�");	/* ��ӡ�������� */
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
void TIMER0A_Handler(void)				//ϵͳ�����жϺ���
{
  Test_Period(&Time0_Delta);
  //�ٴ��������������
	left_motor_speed_cmps =get_left_motor_speed();
	right_motor_speed_cmps=get_right_motor_speed();
  TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
}


void Time_init(void)//ϵͳ���ȶ�ʱ����ʼ��
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);							//��ʱ��0ʹ��				
  TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);						//32λ���ڶ�ʱ��				
  TimerLoadSet(TIMER0_BASE,TIMER_A,SysCtlClockGet()/50);		//�趨װ��ֵ,��80M/200��*1/80M=5ms				
  IntEnable(INT_TIMER0A);																		//���ж�ʹ��				
  TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT); 					//�ж����, ����ģʽ;			
  TimerIntRegister(TIMER0_BASE,TIMER_A,TIMER0A_Handler);		//�жϺ���ע��
  IntMasterEnable();			
  TimerEnable(TIMER0_BASE,TIMER_A); 												//��ʱ��ʹ�ܿ�ʼ����
  IntPrioritySet(INT_TIMER0A,USER_INT7);
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
		ROM_FPUEnable();//ʹ�ܸ��㵥Ԫ
		ROM_FPULazyStackingEnable();//�����ӳٶ�ջ,�����ж���Ӧ�ӳ�
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);//����ϵͳʱ��
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
		// Unlock PF0 so we can change it to a GPIO input
		// Once we have enabled (unlocked) the commit register then re-lock it
		// to prevent further changes.  PF0 is muxed with NMI thus a special case.
		HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
		HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;
		HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
		Encoder_Init();//��������Դ��ʼ��
	  initTime();//��ʼ���δ�ʱ��
    ConfigureUART();//��ʼ������0
    PrintfLogo();//���ڴ�ӡ�汾��Ϣ
	  OLED_Init();//OLED��ʾ����ʼ��
	  Time_init();//��ʱ��1��ʼ��
    while(1)
    {
			Testime t0;
			Test_Period(&t0);
			LCD_clear_L(0,0);display_6_8_string(0,0,"System  Time:  MS");
			LCD_clear_L(0,1);display_6_8_number(0,1,t0.Now_Time);
			LCD_clear_L(0,2);display_6_8_string(0,2,"Time0 Period:  MS");
			LCD_clear_L(0,3);display_6_8_number(0,3,Time0_Delta.Time_Delta);
			LCD_clear_L(0,4);display_6_8_string(0,4,"CMPS");				    
											 display_6_8_number(40,4,left_motor_speed_cmps);  
											 display_6_8_number(90,4,right_motor_speed_cmps); 		
			LCD_clear_L(0,5);display_6_8_string(0,5,"L&R_D");				    
											 display_6_8_number(40,5,NEncoder.left_motor_dir);  
											 display_6_8_number(90,5,NEncoder.right_motor_dir); 
			LCD_clear_L(0,6);display_6_8_string(0,6,"L&R_N");				    
											 display_6_8_number(40,6,NEncoder.left_motor_cnt);  
											 display_6_8_number(90,6,NEncoder.right_motor_cnt); 
			LCD_clear_L(0,7);display_6_8_string(0,7,"L&R_P");				    
											 display_6_8_number(40,7,NEncoder.left_motor_total_cnt);  
											 display_6_8_number(90,7,NEncoder.right_motor_total_cnt); 
    }
}
