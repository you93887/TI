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

#define  USER_INT0  0x00   //PPM     遥控器PPM数据解析  0x00<<6
#define  USER_INT1  0x20   //UART6   GPS数据解析/ROS通讯串口	921600/460800
#define  USER_INT2  0x40   //UART0   底部OPENMV数据解析	256000
                           //UART3   前部OPENMV数据解析	256000
													 
#define  USER_INT3  0x60   //UART7   激光测距通讯串口   921600
													 //UART1   无名创新地面站串口	921600
													 
#define  USER_INT4  0x80   //UART2   光流数据解析19200/激光雷达230400
#define  USER_INT5  0xA0	 //TIMER1A   1ms
#define  USER_INT6  0xC0   //TIMER0		 5ms
#define  USER_INT7  0xE0   //TIMER2    10ms
													 //ADC0SS0
													 
	
/***************************************
函数名:	void UART0_IRQHandler(void)
说明: UART0中断服务函数
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void UART0_IRQHandler(void)//UART0中断函数
{	
  uint32_t flag = UARTIntStatus(UART0_BASE,1);//获取中断标志 原始中断状态 不屏蔽中断标志		
  UARTIntClear(UART0_BASE,flag);	 //清除中断标志		
  while(UARTCharsAvail(UART0_BASE))	 //判断FIFO是否还有数据			
  {
		uint8_t ch=UARTCharGet(UART0_BASE);
		UARTCharPut(UART0_BASE,ch);
  }
}


/***************************************
函数名:	void UART1_IRQHandler(void)
说明: UART1中断服务函数
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void UART1_IRQHandler(void)//UART1中断函数
{				
  uint32_t flag = UARTIntStatus(UART1_BASE,1);//获取中断标志 原始中断状态 屏蔽中断标志	
  UARTIntClear(UART1_BASE,flag);//清除中断标志	
  while(UARTCharsAvail(UART1_BASE))//判断FIFO是否还有数据		
  {			
		uint8_t ch=UARTCharGet(UART1_BASE);
		UARTCharPut(UART1_BASE,ch);
  }
}

/***************************************
函数名:	void UART2_IRQHandler(void)
说明: UART2中断服务函数
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void UART2_IRQHandler(void)
{
  uint32_t flag = UARTIntStatus(UART2_BASE,1);//获取中断标志 原始中断状态 屏蔽中断标志		
  UARTIntClear(UART2_BASE,flag);//清除中断标志	
  while(UARTCharsAvail(UART2_BASE))//判断FIFO是否还有数据				
  {		
		uint8_t ch=UARTCharGet(UART2_BASE);
		UARTCharPut(UART2_BASE,ch);
  }
}


/***************************************
函数名:	void UART3_IRQHandler(void)
说明: UART3中断服务函数
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void UART3_IRQHandler(void)
{		
  uint32_t flag = UARTIntStatus(UART3_BASE,1);//获取中断标志 原始中断状态 屏蔽中断标志		
  UARTIntClear(UART3_BASE,flag);//清除中断标志			
  while(UARTCharsAvail(UART3_BASE))//判断FIFO是否还有数据		
  {	
		uint8_t ch=UARTCharGet(UART3_BASE);
		UARTCharPut(UART3_BASE,ch);
	}	
}

/***************************************
函数名:	void UART4_IRQHandler(void)
说明: UART4中断服务函数
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void UART4_IRQHandler(void)//UART4中断函数
{				
  uint32_t flag = UARTIntStatus(UART4_BASE,1);//获取中断标志 原始中断状态 屏蔽中断标志		
  UARTIntClear(UART4_BASE,flag);//清除中断标志	
  while(UARTCharsAvail(UART4_BASE))//判断FIFO是否还有数据		
  {
		uint8_t ch=UARTCharGet(UART4_BASE);	
		UARTCharPut(UART4_BASE,ch);
  }
}

/***************************************
函数名:	void UART5_IRQHandler(void)
说明: UART5中断服务函数
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void UART5_IRQHandler(void)//UART5中断函数
{				
  uint32_t flag = UARTIntStatus(UART5_BASE,1);//获取中断标志 原始中断状态 屏蔽中断标志		
  UARTIntClear(UART5_BASE,flag);//清除中断标志	
  while(UARTCharsAvail(UART5_BASE))//判断FIFO是否还有数据		
  {
		uint8_t ch=UARTCharGet(UART5_BASE);
		UARTCharPut(UART5_BASE,ch);
  }
}

/***************************************
函数名:	void UART6_IRQHandler(void)
说明: UART6中断服务函数
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void UART6_IRQHandler(void)
{		
  uint32_t flag = UARTIntStatus(UART6_BASE,1);//获取中断标志 原始中断状态 屏蔽中断标志		
  UARTIntClear(UART6_BASE,flag);//清除中断标志	
  while(UARTCharsAvail(UART6_BASE))//判断FIFO是否还有数据		
  {
		uint8_t ch=UARTCharGet(UART6_BASE);
		UARTCharPut(UART6_BASE,ch);
  }
}

/***************************************
函数名:	void UART7_IRQHandler(void)
说明: UART7中断服务函数
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void UART7_IRQHandler(void)//UART7中断函数
{		
  uint32_t flag = UARTIntStatus(UART7_BASE,1);//获取中断标志 原始中断状态 屏蔽中断标志	
  UARTIntClear(UART7_BASE,flag);//清除中断标志		
  while(UARTCharsAvail(UART7_BASE))//判断FIFO是否还有数据			
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
*	函 数 名: PrintfLogo
*	功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的串口终端软件可以观察结果
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void PrintfLogo(void)
{
	printf("*************************************************************\n\r");
	printf("* 例程名称   : %s\r\n", "板载8路通讯串口自收发");	/* 打印例程名称 */
	printf("* 例程版本   : %s\r\n", "V1.0");		  /* 打印例程版本 */
	printf("* 发布日期   : %s\r\n", "20221125");	/* 打印例程日期 */
	printf("* 标准库版本 : TM4C123GH6PZT7\r\n");
	printf("* \r\n");	/* 打印一行空格 */
	printf("* QQ    : 3138372165 \r\n");
	printf("* Email : 3138372165@qq.com \r\n");
	printf("* Copyright www.nameless.tech 无名创新\r\n");
	printf("*************************************************************\n\r");
}


/***************************************
函数名:	void UART0_Init(unsigned long bound)
说明: 串口0资源初始化
入口:	unsigned long bound-波特率
出口:	无
备注:	无
作者:	无名创新
***************************************/
void UART0_Init(unsigned long bound)//串口0初始化
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//使能GPIO外设		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);//使能UART外设
  GPIOPinConfigure(GPIO_PA0_U0RX);//GPIO模式配置 PA0--RX PA1--TX 
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);//GPIO的UART模式配置
	UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
	UARTStdioConfig(0, bound, 16000000);
	
  //UART协议配置 波特率115200 8位 1停止位  无校验位	
  //UART禁用FIFO 默认FIFO Level为4/8 寄存器满8字节后产生中断	//禁用后接收1位就产生中断	
  UARTFIFODisable(UART0_BASE);//使能UART0中断	IntEnable(INT_UART0);	
  UARTIntEnable(UART0_BASE,UART_INT_RX);//使能UART0接收中断		
  UARTIntRegister(UART0_BASE,UART0_IRQHandler);//UART中断地址注册	
  IntPrioritySet(INT_UART0, USER_INT2);
}

int fputc(int ch, FILE *f){UARTCharPut(UART0_BASE,ch);	return (ch);}//重新映射printf函数
int fgetc(FILE *f) {int ch=UARTCharGet(UART0_BASE);	return (ch);}


/***************************************
函数名:	void UART1_Init(unsigned long bound)
说明: 串口1资源初始化
入口:	unsigned long bound-波特率
出口:	无
备注:	无
作者:	无名创新
***************************************/
void UART1_Init(unsigned long bound)//串口1初始化
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//使能GPIO外设		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);//使能UART外设
  GPIOPinConfigure(GPIO_PB0_U1RX);//GPIO模式配置 PB0--RX PB1--TX 
  GPIOPinConfigure(GPIO_PB1_U1TX);
  GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);//GPIO的UART模式配置
	UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), bound,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));
  UARTFIFODisable(UART1_BASE);//使能UART1中断	
  UARTIntEnable(UART1_BASE,UART_INT_RX);//使能UART1接收中断		
  UARTIntRegister(UART1_BASE,UART1_IRQHandler);//UART1中断地址注册	
  IntPrioritySet(INT_UART1, USER_INT1);//USER_INT1
}

/***************************************
函数名:	void UART2_Init(unsigned long bound)
说明: 串口2资源初始化
入口:	unsigned long bound-波特率
出口:	无
备注:	无
作者:	无名创新
***************************************/
void UART2_Init(unsigned long bound)//串口6初始化
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);//使能GPIO外设		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);//使能UART外设
  
  HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;//解锁PD7
  HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= GPIO_PIN_7;//确认
  HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;//重新锁定
  
  GPIOPinConfigure(GPIO_PD6_U2RX);//GPIO模式配置 PD6--RX PD7--TX 
  GPIOPinConfigure(GPIO_PD7_U2TX);
  GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);//GPIO的UART模式配置
  UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), bound,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));
  UARTFIFODisable(UART2_BASE);//使能UART2中断	
  UARTIntEnable(UART2_BASE,UART_INT_RX);//使能UART6接收中断		
	
  UARTIntRegister(UART2_BASE,UART2_IRQHandler);//UART中断地址注册	
  IntPrioritySet(INT_UART2, USER_INT4);
}

/***************************************
函数名:	void UART3_Init(unsigned long bound)
说明: 串口3资源初始化
入口:	unsigned long bound-波特率
出口:	无
备注:	无
作者:	无名创新
***************************************/
void UART3_Init(unsigned long bound)//串口3初始化
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//使能GPIO外设		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);//使能UART外设
  GPIOPinConfigure(GPIO_PC6_U3RX);//GPIO模式配置 PC6--RX PC7--TX 
  GPIOPinConfigure(GPIO_PC7_U3TX);
  GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);//GPIO的UART模式配置
  UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), bound,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));
  UARTFIFODisable(UART3_BASE);//使能UART0中断	
  UARTIntEnable(UART3_BASE,UART_INT_RX);//使能UART3接收中断		
  UARTIntRegister(UART3_BASE,UART3_IRQHandler);//UART3中断地址注册	
  IntPrioritySet(INT_UART3, USER_INT3);
}


/***************************************
函数名:	void UART4_Init(unsigned long bound)
说明: 串口4资源初始化
入口:	unsigned long bound-波特率
出口:	无
备注:	无
作者:	无名创新
***************************************/
void UART4_Init(unsigned long bound)//串口4初始化
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//使能GPIO外设		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);//使能UART外设
  GPIOPinConfigure(GPIO_PC4_U4RX);//GPIO模式配置 PC4--RX PC5--TX 
  GPIOPinConfigure(GPIO_PC5_U4TX);
  GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);//GPIO的UART模式配置
	UARTConfigSetExpClk(UART4_BASE, SysCtlClockGet(), bound,
										 (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
										  UART_CONFIG_PAR_NONE));
	
  UARTFIFODisable(UART4_BASE);//使能UART4中断	
  UARTIntEnable(UART4_BASE,UART_INT_RX);//使能UART4接收中断		
  UARTIntRegister(UART4_BASE,UART4_IRQHandler);//UART4中断地址注册	
  IntPrioritySet(INT_UART4, USER_INT3);//USER_INT3
} 

/***************************************
函数名:	void UART5_Init(unsigned long bound)
说明: 串口5资源初始化
入口:	unsigned long bound-波特率
出口:	无
备注:	无
作者:	无名创新
***************************************/
void UART5_Init(unsigned long bound)//串口5初始化
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//使能GPIO外设		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);//使能UART外设
  GPIOPinConfigure(GPIO_PE4_U5RX);//GPIO模式配置 PC4--RX PC5--TX 
  GPIOPinConfigure(GPIO_PE5_U5TX);
  GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);//GPIO的UART模式配置
	UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), bound,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));
  UARTFIFODisable(UART5_BASE);//使能UART5中断	
  UARTIntEnable(UART5_BASE,UART_INT_RX);//使能UART5接收中断
  UARTIntRegister(UART5_BASE,UART5_IRQHandler);//UART5中断地址注册	
  IntPrioritySet(INT_UART5, USER_INT4);//USER_INT5
} 

/***************************************
函数名:	void UART6_Init(unsigned long bound)
说明: 串口6资源初始化
入口:	unsigned long bound-波特率
出口:	无
备注:	无
作者:	无名创新
***************************************/
void UART6_Init(unsigned long bound)
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);//使能GPIO外设		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART6);//使能UART外设
  	
	GPIOPinConfigure(GPIO_PD4_U6RX);//GPIO模式配置 PD4--RX PD5--TX 
  GPIOPinConfigure(GPIO_PD5_U6TX);
  GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);//GPIO的UART模式配置
  UARTConfigSetExpClk(UART6_BASE, SysCtlClockGet(), bound,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));
  UARTFIFODisable(UART6_BASE);//使能UART0中断	
  UARTIntEnable(UART6_BASE,UART_INT_RX);//使能UART6接收中断		
	
  UARTIntRegister(UART6_BASE,UART6_IRQHandler);//UART6中断地址注册	
  IntPrioritySet(INT_UART6, USER_INT3);
}

/***************************************
函数名:	void UART7_Init(unsigned long bound)
说明: 串口7资源初始化
入口:	unsigned long bound-波特率
出口:	无
备注:	无
作者:	无名创新
***************************************/
void UART7_Init(unsigned long bound)//串口7初始化
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//使能GPIO外设		
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);//使能UART外设
	GPIOPinConfigure(GPIO_PE0_U7RX);//GPIO模式配置 PE0--RX PE1--TX 
	GPIOPinConfigure(GPIO_PE1_U7TX);
	GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);//GPIO的UART模式配置
	UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(), bound,
											(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
											 UART_CONFIG_PAR_NONE));	
	UARTFIFODisable(UART7_BASE);//使能UART7中断	
	UARTIntEnable(UART7_BASE,UART_INT_RX);//使能UART7接收中断		
	UARTIntRegister(UART7_BASE,UART7_IRQHandler);//UART中断地址注册	
	IntPrioritySet(INT_UART7, USER_INT2);
}




int main(void)
{
		ROM_FPUEnable();//使能浮点单元
		ROM_FPULazyStackingEnable();//浮点延迟堆栈,减少中断响应延迟 
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |SYSCTL_OSC_MAIN);//配置系统时钟
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_5);//红色
		ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_6);//绿色
	  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4);//蓝色
    UART0_Init(115200);
		UART1_Init(115200);
		UART2_Init(115200);
		UART3_Init(115200);
		UART4_Init(115200);
		UART5_Init(115200);
		UART6_Init(115200);
		UART7_Init(115200);
    PrintfLogo();//串口打印版本信息
    while(1)
    {
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_5, 0);//置低位点亮
			SysCtlDelay(SysCtlClockGet() / 20);    
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_5, GPIO_PIN_5);//置高位熄灭
			SysCtlDelay(SysCtlClockGet() / 20);

			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0);//置低位点亮
			SysCtlDelay(SysCtlClockGet() / 20);    
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, GPIO_PIN_6);//置高位熄灭
			SysCtlDelay(SysCtlClockGet() / 20);

			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0);//置低位点亮
			SysCtlDelay(SysCtlClockGet() / 20);      
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_PIN_4);//置高位熄灭
			SysCtlDelay(SysCtlClockGet() / 20);
    }
}
