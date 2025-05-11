#include "Usart.h"




void UART3_Init(unsigned long bound)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//使能GPIO外设		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);//使能UART外设
  GPIOPinConfigure(GPIO_PC6_U3RX);//GPIO模式配置 PB0--RX PB1--TX 
  GPIOPinConfigure(GPIO_PC7_U3TX);
  GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);//GPIO的UART模式配置
	UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), bound,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));
  UARTFIFODisable(UART3_BASE);//使能UART1中断	
  UARTIntEnable(UART3_BASE,UART_INT_RX);//使能UART1接收中断		
  UARTIntRegister(UART3_BASE,UART3_IRQHandler);//UART1中断地址注册	
  IntPrioritySet(INT_UART3, USER_INT3);//USER_INT1
	
}


void UART3_IRQHandler(void)
{
	uint32_t flag = UARTIntStatus(UART3_BASE, 1); // 获取中断标志 原始中断状态 屏蔽中断标志	
	UARTIntClear(UART3_BASE, flag); // 清除中断标志	
		while (UARTCharsAvail(UART3_BASE)) // 判断 FIFO 是否还有数据		
	{	
		 uint8_t ch = UARTCharGet(UART3_BASE);
		printf("%c\n",ch);
	}
	
}

