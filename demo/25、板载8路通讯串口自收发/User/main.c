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
#include "hw_ints.h"
#include "uartstdio.h"
#include "interrupt.h"

#define  USER_INT0  0x00   //PPM     ң����PPM���ݽ���  0x00<<6
#define  USER_INT1  0x20   //UART6   GPS���ݽ���/ROSͨѶ����	921600/460800
#define  USER_INT2  0x40   //UART0   �ײ�OPENMV���ݽ���	256000
                           //UART3   ǰ��OPENMV���ݽ���	256000
													 
#define  USER_INT3  0x60   //UART7   ������ͨѶ����   921600
													 //UART1   �������µ���վ����	921600
													 
#define  USER_INT4  0x80   //UART2   �������ݽ���19200/�����״�230400
#define  USER_INT5  0xA0	 //TIMER1A   1ms
#define  USER_INT6  0xC0   //TIMER0		 5ms
#define  USER_INT7  0xE0   //TIMER2    10ms
													 //ADC0SS0
													 
	
/***************************************
������:	void UART0_IRQHandler(void)
˵��: UART0�жϷ�����
���:	��
����:	��
��ע:	��
����:	��������
***************************************/
void UART0_IRQHandler(void)//UART0�жϺ���
{	
  uint32_t flag = UARTIntStatus(UART0_BASE,1);//��ȡ�жϱ�־ ԭʼ�ж�״̬ �������жϱ�־		
  UARTIntClear(UART0_BASE,flag);	 //����жϱ�־		
  while(UARTCharsAvail(UART0_BASE))	 //�ж�FIFO�Ƿ�������			
  {
		uint8_t ch=UARTCharGet(UART0_BASE);
		UARTCharPut(UART0_BASE,ch);
  }
}


/***************************************
������:	void UART1_IRQHandler(void)
˵��: UART1�жϷ�����
���:	��
����:	��
��ע:	��
����:	��������
***************************************/
void UART1_IRQHandler(void)//UART1�жϺ���
{				
  uint32_t flag = UARTIntStatus(UART1_BASE,1);//��ȡ�жϱ�־ ԭʼ�ж�״̬ �����жϱ�־	
  UARTIntClear(UART1_BASE,flag);//����жϱ�־	
  while(UARTCharsAvail(UART1_BASE))//�ж�FIFO�Ƿ�������		
  {			
		uint8_t ch=UARTCharGet(UART1_BASE);
		UARTCharPut(UART1_BASE,ch);
  }
}

/***************************************
������:	void UART2_IRQHandler(void)
˵��: UART2�жϷ�����
���:	��
����:	��
��ע:	��
����:	��������
***************************************/
void UART2_IRQHandler(void)
{
  uint32_t flag = UARTIntStatus(UART2_BASE,1);//��ȡ�жϱ�־ ԭʼ�ж�״̬ �����жϱ�־		
  UARTIntClear(UART2_BASE,flag);//����жϱ�־	
  while(UARTCharsAvail(UART2_BASE))//�ж�FIFO�Ƿ�������				
  {		
		uint8_t ch=UARTCharGet(UART2_BASE);
		UARTCharPut(UART2_BASE,ch);
  }
}


/***************************************
������:	void UART3_IRQHandler(void)
˵��: UART3�жϷ�����
���:	��
����:	��
��ע:	��
����:	��������
***************************************/
void UART3_IRQHandler(void)
{		
  uint32_t flag = UARTIntStatus(UART3_BASE,1);//��ȡ�жϱ�־ ԭʼ�ж�״̬ �����жϱ�־		
  UARTIntClear(UART3_BASE,flag);//����жϱ�־			
  while(UARTCharsAvail(UART3_BASE))//�ж�FIFO�Ƿ�������		
  {	
		uint8_t ch=UARTCharGet(UART3_BASE);
		UARTCharPut(UART3_BASE,ch);
	}	
}

/***************************************
������:	void UART4_IRQHandler(void)
˵��: UART4�жϷ�����
���:	��
����:	��
��ע:	��
����:	��������
***************************************/
void UART4_IRQHandler(void)//UART4�жϺ���
{				
  uint32_t flag = UARTIntStatus(UART4_BASE,1);//��ȡ�жϱ�־ ԭʼ�ж�״̬ �����жϱ�־		
  UARTIntClear(UART4_BASE,flag);//����жϱ�־	
  while(UARTCharsAvail(UART4_BASE))//�ж�FIFO�Ƿ�������		
  {
		uint8_t ch=UARTCharGet(UART4_BASE);	
		UARTCharPut(UART4_BASE,ch);
  }
}

/***************************************
������:	void UART5_IRQHandler(void)
˵��: UART5�жϷ�����
���:	��
����:	��
��ע:	��
����:	��������
***************************************/
void UART5_IRQHandler(void)//UART5�жϺ���
{				
  uint32_t flag = UARTIntStatus(UART5_BASE,1);//��ȡ�жϱ�־ ԭʼ�ж�״̬ �����жϱ�־		
  UARTIntClear(UART5_BASE,flag);//����жϱ�־	
  while(UARTCharsAvail(UART5_BASE))//�ж�FIFO�Ƿ�������		
  {
		uint8_t ch=UARTCharGet(UART5_BASE);
		UARTCharPut(UART5_BASE,ch);
  }
}

/***************************************
������:	void UART6_IRQHandler(void)
˵��: UART6�жϷ�����
���:	��
����:	��
��ע:	��
����:	��������
***************************************/
void UART6_IRQHandler(void)
{		
  uint32_t flag = UARTIntStatus(UART6_BASE,1);//��ȡ�жϱ�־ ԭʼ�ж�״̬ �����жϱ�־		
  UARTIntClear(UART6_BASE,flag);//����жϱ�־	
  while(UARTCharsAvail(UART6_BASE))//�ж�FIFO�Ƿ�������		
  {
		uint8_t ch=UARTCharGet(UART6_BASE);
		UARTCharPut(UART6_BASE,ch);
  }
}

/***************************************
������:	void UART7_IRQHandler(void)
˵��: UART7�жϷ�����
���:	��
����:	��
��ע:	��
����:	��������
***************************************/
void UART7_IRQHandler(void)//UART7�жϺ���
{		
  uint32_t flag = UARTIntStatus(UART7_BASE,1);//��ȡ�жϱ�־ ԭʼ�ж�״̬ �����жϱ�־	
  UARTIntClear(UART7_BASE,flag);//����жϱ�־		
  while(UARTCharsAvail(UART7_BASE))//�ж�FIFO�Ƿ�������			
  {			
		uint8_t ch=UARTCharGet(UART7_BASE);
    UARTCharPut(UART7_BASE,ch);
  }
}



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
	printf("* ��������   : %s\r\n", "����8·ͨѶ�������շ�");	/* ��ӡ�������� */
	printf("* ���̰汾   : %s\r\n", "V1.0");		  /* ��ӡ���̰汾 */
	printf("* ��������   : %s\r\n", "20221125");	/* ��ӡ�������� */
	printf("* ��׼��汾 : TM4C123GH6PZT7\r\n");
	printf("* \r\n");	/* ��ӡһ�пո� */
	printf("* QQ    : 3138372165 \r\n");
	printf("* Email : 3138372165@qq.com \r\n");
	printf("* Copyright www.nameless.tech ��������\r\n");
	printf("*************************************************************\n\r");
}


/***************************************
������:	void UART0_Init(unsigned long bound)
˵��: ����0��Դ��ʼ��
���:	unsigned long bound-������
����:	��
��ע:	��
����:	��������
***************************************/
void UART0_Init(unsigned long bound)//����0��ʼ��
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//ʹ��GPIO����		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);//ʹ��UART����
  GPIOPinConfigure(GPIO_PA0_U0RX);//GPIOģʽ���� PA0--RX PA1--TX 
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);//GPIO��UARTģʽ����
	UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
	UARTStdioConfig(0, bound, 16000000);
	
  //UARTЭ������ ������115200 8λ 1ֹͣλ  ��У��λ	
  //UART����FIFO Ĭ��FIFO LevelΪ4/8 �Ĵ�����8�ֽں�����ж�	//���ú����1λ�Ͳ����ж�	
  UARTFIFODisable(UART0_BASE);//ʹ��UART0�ж�	IntEnable(INT_UART0);	
  UARTIntEnable(UART0_BASE,UART_INT_RX);//ʹ��UART0�����ж�		
  UARTIntRegister(UART0_BASE,UART0_IRQHandler);//UART�жϵ�ַע��	
  IntPrioritySet(INT_UART0, USER_INT2);
}

int fputc(int ch, FILE *f){UARTCharPut(UART0_BASE,ch);	return (ch);}//����ӳ��printf����
int fgetc(FILE *f) {int ch=UARTCharGet(UART0_BASE);	return (ch);}


/***************************************
������:	void UART1_Init(unsigned long bound)
˵��: ����1��Դ��ʼ��
���:	unsigned long bound-������
����:	��
��ע:	��
����:	��������
***************************************/
void UART1_Init(unsigned long bound)//����1��ʼ��
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//ʹ��GPIO����		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);//ʹ��UART����
  GPIOPinConfigure(GPIO_PB0_U1RX);//GPIOģʽ���� PB0--RX PB1--TX 
  GPIOPinConfigure(GPIO_PB1_U1TX);
  GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);//GPIO��UARTģʽ����
	UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), bound,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));
  UARTFIFODisable(UART1_BASE);//ʹ��UART1�ж�	
  UARTIntEnable(UART1_BASE,UART_INT_RX);//ʹ��UART1�����ж�		
  UARTIntRegister(UART1_BASE,UART1_IRQHandler);//UART1�жϵ�ַע��	
  IntPrioritySet(INT_UART1, USER_INT1);//USER_INT1
}

/***************************************
������:	void UART2_Init(unsigned long bound)
˵��: ����2��Դ��ʼ��
���:	unsigned long bound-������
����:	��
��ע:	��
����:	��������
***************************************/
void UART2_Init(unsigned long bound)//����6��ʼ��
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);//ʹ��GPIO����		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);//ʹ��UART����
  
  HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;//����PD7
  HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= GPIO_PIN_7;//ȷ��
  HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;//��������
  
  GPIOPinConfigure(GPIO_PD6_U2RX);//GPIOģʽ���� PD6--RX PD7--TX 
  GPIOPinConfigure(GPIO_PD7_U2TX);
  GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);//GPIO��UARTģʽ����
  UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), bound,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));
  UARTFIFODisable(UART2_BASE);//ʹ��UART2�ж�	
  UARTIntEnable(UART2_BASE,UART_INT_RX);//ʹ��UART6�����ж�		
	
  UARTIntRegister(UART2_BASE,UART2_IRQHandler);//UART�жϵ�ַע��	
  IntPrioritySet(INT_UART2, USER_INT4);
}

/***************************************
������:	void UART3_Init(unsigned long bound)
˵��: ����3��Դ��ʼ��
���:	unsigned long bound-������
����:	��
��ע:	��
����:	��������
***************************************/
void UART3_Init(unsigned long bound)//����3��ʼ��
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//ʹ��GPIO����		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);//ʹ��UART����
  GPIOPinConfigure(GPIO_PC6_U3RX);//GPIOģʽ���� PC6--RX PC7--TX 
  GPIOPinConfigure(GPIO_PC7_U3TX);
  GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);//GPIO��UARTģʽ����
  UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), bound,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));
  UARTFIFODisable(UART3_BASE);//ʹ��UART0�ж�	
  UARTIntEnable(UART3_BASE,UART_INT_RX);//ʹ��UART3�����ж�		
  UARTIntRegister(UART3_BASE,UART3_IRQHandler);//UART3�жϵ�ַע��	
  IntPrioritySet(INT_UART3, USER_INT3);
}


/***************************************
������:	void UART4_Init(unsigned long bound)
˵��: ����4��Դ��ʼ��
���:	unsigned long bound-������
����:	��
��ע:	��
����:	��������
***************************************/
void UART4_Init(unsigned long bound)//����4��ʼ��
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//ʹ��GPIO����		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);//ʹ��UART����
  GPIOPinConfigure(GPIO_PC4_U4RX);//GPIOģʽ���� PC4--RX PC5--TX 
  GPIOPinConfigure(GPIO_PC5_U4TX);
  GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);//GPIO��UARTģʽ����
	UARTConfigSetExpClk(UART4_BASE, SysCtlClockGet(), bound,
										 (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
										  UART_CONFIG_PAR_NONE));
	
  UARTFIFODisable(UART4_BASE);//ʹ��UART4�ж�	
  UARTIntEnable(UART4_BASE,UART_INT_RX);//ʹ��UART4�����ж�		
  UARTIntRegister(UART4_BASE,UART4_IRQHandler);//UART4�жϵ�ַע��	
  IntPrioritySet(INT_UART4, USER_INT3);//USER_INT3
} 

/***************************************
������:	void UART5_Init(unsigned long bound)
˵��: ����5��Դ��ʼ��
���:	unsigned long bound-������
����:	��
��ע:	��
����:	��������
***************************************/
void UART5_Init(unsigned long bound)//����5��ʼ��
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//ʹ��GPIO����		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);//ʹ��UART����
  GPIOPinConfigure(GPIO_PE4_U5RX);//GPIOģʽ���� PC4--RX PC5--TX 
  GPIOPinConfigure(GPIO_PE5_U5TX);
  GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);//GPIO��UARTģʽ����
	UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), bound,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));
  UARTFIFODisable(UART5_BASE);//ʹ��UART5�ж�	
  UARTIntEnable(UART5_BASE,UART_INT_RX);//ʹ��UART5�����ж�
  UARTIntRegister(UART5_BASE,UART5_IRQHandler);//UART5�жϵ�ַע��	
  IntPrioritySet(INT_UART5, USER_INT4);//USER_INT5
} 

/***************************************
������:	void UART6_Init(unsigned long bound)
˵��: ����6��Դ��ʼ��
���:	unsigned long bound-������
����:	��
��ע:	��
����:	��������
***************************************/
void UART6_Init(unsigned long bound)
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);//ʹ��GPIO����		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART6);//ʹ��UART����
  	
	GPIOPinConfigure(GPIO_PD4_U6RX);//GPIOģʽ���� PD4--RX PD5--TX 
  GPIOPinConfigure(GPIO_PD5_U6TX);
  GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);//GPIO��UARTģʽ����
  UARTConfigSetExpClk(UART6_BASE, SysCtlClockGet(), bound,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));
  UARTFIFODisable(UART6_BASE);//ʹ��UART0�ж�	
  UARTIntEnable(UART6_BASE,UART_INT_RX);//ʹ��UART6�����ж�		
	
  UARTIntRegister(UART6_BASE,UART6_IRQHandler);//UART6�жϵ�ַע��	
  IntPrioritySet(INT_UART6, USER_INT3);
}

/***************************************
������:	void UART7_Init(unsigned long bound)
˵��: ����7��Դ��ʼ��
���:	unsigned long bound-������
����:	��
��ע:	��
����:	��������
***************************************/
void UART7_Init(unsigned long bound)//����7��ʼ��
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//ʹ��GPIO����		
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);//ʹ��UART����
	GPIOPinConfigure(GPIO_PE0_U7RX);//GPIOģʽ���� PE0--RX PE1--TX 
	GPIOPinConfigure(GPIO_PE1_U7TX);
	GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);//GPIO��UARTģʽ����
	UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(), bound,
											(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
											 UART_CONFIG_PAR_NONE));	
	UARTFIFODisable(UART7_BASE);//ʹ��UART7�ж�	
	UARTIntEnable(UART7_BASE,UART_INT_RX);//ʹ��UART7�����ж�		
	UARTIntRegister(UART7_BASE,UART7_IRQHandler);//UART�жϵ�ַע��	
	IntPrioritySet(INT_UART7, USER_INT2);
}




int main(void)
{
		ROM_FPUEnable();//ʹ�ܸ��㵥Ԫ
		ROM_FPULazyStackingEnable();//�����ӳٶ�ջ,�����ж���Ӧ�ӳ� 
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |SYSCTL_OSC_MAIN);//����ϵͳʱ��
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_5);//��ɫ
		ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_6);//��ɫ
	  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4);//��ɫ
    UART0_Init(115200);
		UART1_Init(115200);
		UART2_Init(115200);
		UART3_Init(115200);
		UART4_Init(115200);
		UART5_Init(115200);
		UART6_Init(115200);
		UART7_Init(115200);
    PrintfLogo();//���ڴ�ӡ�汾��Ϣ
    while(1)
    {
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_5, 0);//�õ�λ����
			SysCtlDelay(SysCtlClockGet() / 20);    
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_5, GPIO_PIN_5);//�ø�λϨ��
			SysCtlDelay(SysCtlClockGet() / 20);

			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0);//�õ�λ����
			SysCtlDelay(SysCtlClockGet() / 20);    
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, GPIO_PIN_6);//�ø�λϨ��
			SysCtlDelay(SysCtlClockGet() / 20);

			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0);//�õ�λ����
			SysCtlDelay(SysCtlClockGet() / 20);      
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_PIN_4);//�ø�λϨ��
			SysCtlDelay(SysCtlClockGet() / 20);
    }
}
