#include "Usart.h"




void UART3_Init(unsigned long bound)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//ʹ��GPIO����		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);//ʹ��UART����
  GPIOPinConfigure(GPIO_PC6_U3RX);//GPIOģʽ���� PB0--RX PB1--TX 
  GPIOPinConfigure(GPIO_PC7_U3TX);
  GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);//GPIO��UARTģʽ����
	UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), bound,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));
  UARTFIFODisable(UART3_BASE);//ʹ��UART1�ж�	
  UARTIntEnable(UART3_BASE,UART_INT_RX);//ʹ��UART1�����ж�		
  UARTIntRegister(UART3_BASE,UART3_IRQHandler);//UART1�жϵ�ַע��	
  IntPrioritySet(INT_UART3, USER_INT3);//USER_INT1
	
}


void UART3_IRQHandler(void)
{
	uint32_t flag = UARTIntStatus(UART3_BASE, 1); // ��ȡ�жϱ�־ ԭʼ�ж�״̬ �����жϱ�־	
	UARTIntClear(UART3_BASE, flag); // ����жϱ�־	
		while (UARTCharsAvail(UART3_BASE)) // �ж� FIFO �Ƿ�������		
	{	
		 uint8_t ch = UARTCharGet(UART3_BASE);
		printf("%c\n",ch);
	}
	
}

