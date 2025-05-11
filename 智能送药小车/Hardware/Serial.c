/***************************************************
 * @brief   关于TM4C123GH6PZ芯片的全部串口代码包括UART的初始化函数
 *          以及对应的中断服务函数定义。
 *          以及一些封装好的发送字节，字符串，数字等的函数。
 *			重映射的printf函数输出串口，打印多数据更方便
 *
 *			RX0->PA0          TX0->PA1
 *			RX1->PB0          TX1->PB1
 *			RX2->PD6          TX2->PD7
 *			RX3->PC6          TX3->PC7
 *			RX4->PC4          TX4->PC5
 *			RX5->PE4          TX5->PE5
 *			RX6->PD4          TX6->PD5
 *		 	RX7->PE0          TX7->PE1
 *
 *          默认都是波特率为115200，8位数据位，一个停止位，无校验位。
 *          FIFO深度默认不使能，即发送一个数据或者接受一个数据进入中断，默认不开启发送中断
 *          中断优先级默认位0x0即最高优先级
 *			默认用带ROM的，不带ROM的在本文件的最后。
 * @date    2024/6/20
 * @author  qihang chen
****************************************************/

//板子串口二的TX2即PD7不能接受到，但是能发送，能进入中断

#include "include.h"
// 重新映射printf函数
// 只能重新映射一个串口，默认UART1
int fputc(int ch, FILE *f)
{
	UARTCharPut(UART5_BASE,ch);	
	return (ch);
}
int fgetc(FILE *f) 
{
	int ch = UARTCharGet(UART5_BASE);	
	return (ch);
}


/**
  * 函    数：串口发送一个字节
  * 参    数：需要的串口 
  * 参    数：Byte 要发送的一个字节
  * 返 回 值：无
  */
void Serial_SendByte(uint8_t uart,uint8_t Byte)
{
	if (uart == 0)
		UARTCharPut(UART0_BASE, Byte);
	else if (uart == 1)
		UARTCharPut(UART1_BASE, Byte);
	else if (uart == 2)
		UARTCharPut(UART2_BASE, Byte);
	else if (uart == 3)
		UARTCharPut(UART3_BASE, Byte);
	else if (uart == 4)
		UARTCharPut(UART4_BASE, Byte);
	else if (uart == 5)
		UARTCharPut(UART5_BASE, Byte);
	else if (uart == 6)
		UARTCharPut(UART6_BASE, Byte);
	else if (uart == 7)
		UARTCharPut(UART7_BASE, Byte);
	else
		return;
}

/**
  * 函    数：串口发送一个数组
  * 参    数：需要的串口 
  * 参    数：Array 要发送数组的首地址
  * 参    数：Length 要发送数组的长度
  * 返 回 值：无
  */
void Serial_SendArray(uint8_t uart, uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//遍历数组
	{
		Serial_SendByte(uart, Array[i]);		//依次调用Serial_SendByte发送每个字节数据
	}
}

/**
  * 函    数：串口发送一个字符串
  * 参    数：需要的串口 
  * 参    数：String 要发送字符串的首地址
  * 返 回 值：无
  */
void Serial_SendString(uint8_t uart, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
	{
		Serial_SendByte(uart, String[i]);		//依次调用Serial_SendByte发送每个字节数据
	}
}

/**
  * 函    数：次方函数（内部使用）
  * 返 回 值：返回值等于X的Y次方
  */
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	//设置结果初值为1
	while (Y --)			//执行Y次
	{
		Result *= X;		//将X累乘到结果
	}
	return Result;
}

/**
  * 函    数：串口发送数字
  * 参    数：需要的串口 
  * 参    数：Number 要发送的数字，范围：0~4294967295
  * 参    数：Length 要发送数字的长度，范围：0~10
  * 返 回 值：无
  */
void Serial_SendNumber(uint8_t uart, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)		//根据数字长度遍历数字的每一位
	{
		Serial_SendByte(uart, Number / Serial_Pow(10, Length - i - 1) % 10 + '0');	//依次调用Serial_SendByte发送每位数字
	}
}

/*****************************************************
* 串口中断函数，需要注册，用UARTIntRegister函数
* 提前声明一下，具体定义在后面，如需要在主函数使用，
* 可以直接把函数的定义截取到主函数后面去
**********************************************/

/*************************************************************************
串口0的一般初始化           RX0->PA0          TX0->PA1
波特率为115200，8位数据位，一个停止位，无校验位。
FIFO深度默认不使能，即发送一个数据或者接受一个数据进入中断，默认不开启发送中断
中断优先级默认位0x0即最高优先级
**************************************************************************/
void ROM_UART0_Init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);						// 启用 UART 使用的 GPIO 外设。
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);						// 使能UART0
	ROM_GPIOPinConfigure(GPIO_PA0_U0RX);									// 为UART模式配置GPIO引脚。
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0);						// 配置UART外围设备使用的引脚。
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);									// 为UART模式配置GPIO引脚。
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_1);						// 配置UART外围设备使用的引脚。
	// 此函数为初始化，第一个参数为UART口的选择有0--7个、第二个参数为系统时钟
	// 第三个参数为波特率、后面几个一般不改为8个数据位，一个停止位，无校验位。
    ROM_UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//不使能FIFO
	ROM_UARTFIFODisable(UART0_BASE);	
	//使能FIFO
//    UARTFIFOEnable(UART0_BASE);											//使能UART的缓存区，默认位1/2深度 即16的1/2为8位，可以选择
//    UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//此函数为选择深度的函数，目前选择的深度为2/8 即4位。。。如果直接把FIFO失能（disable）的话，就可以一个字符触发一次中断。
	
	//如果需要发送中断只需变成   UART_INT_RX | UART_INT_TX
    ROM_UARTIntEnable(UART0_BASE, UART_INT_RX);								//对于串口接收中断使能，
    ROM_IntPrioritySet(INT_UART0, 0x0);										// 这里指串口0配置为接收中断（RX指接收） 优先级位0最大 可选择0xE0-0x00
    UARTIntRegister(UART0_BASE, ROM_UART0IntHandler);						// 注册中断函数	
    ROM_IntEnable(INT_UART0);												//中断使能			
    ROM_IntMasterEnable();													//中断使能			
}

void ROM_UART0IntHandler(void)
{
  uint32_t ui32IntStatus;

	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART0_BASE, true);
	
    ROM_UARTIntClear(UART0_BASE, ui32IntStatus);//先清除中断标志位
	//此函数是判断是否接受到东西，有返回真
	//发送中断进入if，if执行完退出中断
	if ( !ROM_UARTCharsAvail(UART0_BASE))
	{
		
		return;
	}
	//接受中断进入while
	while (ROM_UARTCharsAvail(UART0_BASE))
	{
		//此函数从指定的接收FIFO中获取一个字符
		ui8RxBuffer = (uint8_t) ROM_UARTCharGetNonBlocking(UART0_BASE);
		
		
		
		
	}
	//接受中断后续内容可以在下面添加
	
}

/*************************************************************************
串口1的一般初始化           RX1->PB0          TX1->PB1
波特率为115200，8位数据位，一个停止位，无校验位。
FIFO深度默认不使能，即发送一个数据或者接受一个数据进入中断，默认不开启发送中断
中断优先级默认位0x0即最高优先级
**************************************************************************/
void ROM_UART1_Init(void)
{
	  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);						//启用 UART 使用的 GPIO 外设。
	  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);						// 使能UART1
	  ROM_GPIOPinConfigure(GPIO_PB0_U1RX);									//为UART模式配置GPIO引脚。
    ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0);						//配置UART外围设备使用的引脚。
    ROM_GPIOPinConfigure(GPIO_PB1_U1TX);									//为UART模式配置GPIO引脚。
    ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_1);						//配置UART外围设备使用的引脚。
	//此函数为初始化，第一个参数为UART口的选择有0--7个、第二个参数为系统时钟
	//第三个参数为波特率、后面几个一般不改为8个数据位，一个停止位，无校验位。
    ROM_UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//不使能FIFO
	ROM_UARTFIFODisable(UART1_BASE);
	//使能FIFO
//    UARTFIFOEnable(UART1_BASE);											//使能UART的缓存区，默认位1/2深度 即16的1/2为8位，可以选择
//    UARTFIFOLevelSet(UART1_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//此函数为选择深度的函数，目前选择的深度为2/8 即4位。。。如果直接把FIFO失能（disable）的话，就可以一个字符触发一次中断。
	
	//如果需要发送中断只需变成   UART_INT_RX | UART_INT_TX
    ROM_UARTIntEnable(UART1_BASE, UART_INT_RX);								//对于串口接收中断使能
    ROM_IntPrioritySet(INT_UART1, 0x10);										// 这里指串口1配置为接收中断（RX指接收） 优先级位0最大 可选择0xE0-0x00
    UARTIntRegister(UART1_BASE, ROM_UART1IntHandler);						// 注册中断函数	
    ROM_IntEnable(INT_UART1);												//中断使能			
    ROM_IntMasterEnable();													//中断使能			
}

//void ROM_UART1IntHandler(void)
//{
//    uint32_t ui32IntStatus;
//	uint8_t ui8RxBuffer;
//	
//    ui32IntStatus = UARTIntStatus(UART1_BASE, true);
//	
//    ROM_UARTIntClear(UART1_BASE, ui32IntStatus);//先清除中断标志位
//	//此函数是判断是否接受到东西，有返回真
//	//发送中断进入if，if执行完退出中断
//	if ( !ROM_UARTCharsAvail(UART1_BASE))
//	{
//		
//		return;
//	}
//	//接受中断进入while
//	while (ROM_UARTCharsAvail(UART1_BASE))
//	{
//		//此函数从指定的接收FIFO中获取一个字符
//		ui8RxBuffer = (uint8_t) ROM_UARTCharGetNonBlocking(UART1_BASE);
//	}
//	//接受中断后续内容可以在下面添加
//	
//}



/*************************************************************************
串口2的一般初始化           RX2->PD6          TX2->PD7
波特率为115200，8位数据位，一个停止位，无校验位。
FIFO深度默认不使能，即发送一个数据或者接受一个数据进入中断，默认不开启发送中断
中断优先级默认位0x0即最高优先级
**************************************************************************/
void ROM_UART2_Init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);						// 启用 UART 使用的 GPIO 外设。
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);						// 使能UART2
	ROM_GPIOPinConfigure(GPIO_PD6_U2RX);									// 为UART模式配置GPIO引脚。
    ROM_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6);						// 配置UART外围设备使用的引脚。
    ROM_GPIOPinConfigure(GPIO_PD7_U2TX);									// 为UART模式配置GPIO引脚。
    ROM_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_7);						// 配置UART外围设备使用的引脚。
	// 此函数为初始化，第一个参数为UART口的选择有0--7个、第二个参数为系统时钟
	// 第三个参数为波特率、后面几个一般不改为8个数据位，一个停止位，无校验位。
    ROM_UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//不使能FIFO
	ROM_UARTFIFODisable(UART2_BASE);	
	//使能FIFO
//    UARTFIFOEnable(UART2_BASE);											//使能UART的缓存区，默认位1/2深度 即16的1/2为8位，可以选择
//    UARTFIFOLevelSet(UART2_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//此函数为选择深度的函数，目前选择的深度为2/8 即4位。。。如果直接把FIFO失能（disable）的话，就可以一个字符触发一次中断。
	
	//如果需要发送中断只需变成  UART_INT_RX | UART_INT_TX
    ROM_UARTIntEnable(UART2_BASE, UART_INT_RX);								//对于串口接收中断使能，
    ROM_IntPrioritySet(INT_UART2, 0x0);										// 这里指串口2配置为接收中断（RX指接收） 优先级位0最大 可选择0xE0-0x00
    UARTIntRegister(UART2_BASE, ROM_UART2IntHandler);						// 注册中断函数	
    ROM_IntEnable(INT_UART2);												//中断使能			
    ROM_IntMasterEnable();													//中断使能			
}
void ROM_UART2IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART2_BASE, true);
	
    ROM_UARTIntClear(UART2_BASE, ui32IntStatus);//先清除中断标志位
	//此函数是判断是否接受到东西，有返回真
	//发送中断进入if，if执行完退出中断
	if ( !ROM_UARTCharsAvail(UART2_BASE))
	{
		
		return;
	}
	//接受中断进入while
	while (ROM_UARTCharsAvail(UART2_BASE))
	{
		//此函数从指定的接收FIFO中获取一个字符
		ui8RxBuffer = (uint8_t) ROM_UARTCharGetNonBlocking(UART2_BASE);
	}
	//接受中断后续内容可以在下面添加
	
}
/*************************************************************************
串口3的一般初始化           RX3->PC6          TX3->PC7
波特率为115200，8位数据位，一个停止位，无校验位。
FIFO深度默认不使能，即发送一个数据或者接受一个数据进入中断，默认不开启发送中断
中断优先级默认位0x0即最高优先级
**************************************************************************/
void ROM_UART3_Init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);						// 启用 UART 使用的 GPIO 外设。
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);						// 使能UART3
	ROM_GPIOPinConfigure(GPIO_PC6_U3RX);									// 为UART模式配置GPIO引脚。
    ROM_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6);						// 配置UART外围设备使用的引脚。
    ROM_GPIOPinConfigure(GPIO_PC7_U3TX);									// 为UART模式配置GPIO引脚。
    ROM_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_7);						// 配置UART外围设备使用的引脚。
	// 此函数为初始化，第一个参数为UART口的选择有0--7个、第二个参数为系统时钟
	// 第三个参数为波特率、后面几个一般不改为8个数据位，一个停止位，无校验位。
    ROM_UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//不使能FIFO
	ROM_UARTFIFODisable(UART3_BASE);	
	//使能FIFO
//    UARTFIFOEnable(UART3_BASE);											//使能UART的缓存区，默认位1/2深度 即16的1/2为8位，可以选择
//    UARTFIFOLevelSet(UART3_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//此函数为选择深度的函数，目前选择的深度为2/8 即4位。。。如果直接把FIFO失能（disable）的话，就可以一个字符触发一次中断。
	
	//如果需要发送中断只需变成   UART_INT_RX | UART_INT_TX
    ROM_UARTIntEnable(UART3_BASE, UART_INT_RX);								//对于串口接收中断使能，
    ROM_IntPrioritySet(INT_UART3, 0x0);										// 这里指串口3配置为接收中断（RX指接收） 优先级位0最大 可选择0xE0-0x00
    UARTIntRegister(UART3_BASE, ROM_UART3IntHandler);						// 注册中断函数	
    ROM_IntEnable(INT_UART3);												//中断使能			
    ROM_IntMasterEnable();													//中断使能			
}

//void ROM_UART3IntHandler(void)
//{
//    uint32_t ui32IntStatus;
//	uint8_t ui8RxBuffer;
//	
//    ui32IntStatus = UARTIntStatus(UART3_BASE, true);
//	
//    ROM_UARTIntClear(UART3_BASE, ui32IntStatus);//先清除中断标志位
//	//此函数是判断是否接受到东西，有返回真
//	//发送中断进入if，if执行完退出中断
//	if ( !ROM_UARTCharsAvail(UART3_BASE))
//	{
//		
//		return;
//	}
//	//接受中断进入while
//	while (ROM_UARTCharsAvail(UART3_BASE))
//	{
//		//此函数从指定的接收FIFO中获取一个字符
//		ui8RxBuffer = (uint8_t) ROM_UARTCharGetNonBlocking(UART3_BASE);
//	}
//	//接受中断后续内容可以在下面添加
//	
//}

/*************************************************************************
串口4的一般初始化           RX4->PC4          TX4->PC5
波特率为115200，8位数据位，一个停止位，无校验位。
FIFO深度默认不使能，即发送一个数据或者接受一个数据进入中断，默认不开启发送中断
中断优先级默认位0x0即最高优先级
**************************************************************************/
void ROM_UART4_Init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);						// 启用 UART 使用的 GPIO 外设。
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);						// 使能UART4
	ROM_GPIOPinConfigure(GPIO_PC4_U4RX);									// 为UART模式配置GPIO引脚。
    ROM_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4);						// 配置UART外围设备使用的引脚。
    ROM_GPIOPinConfigure(GPIO_PC5_U4TX);									// 为UART模式配置GPIO引脚。
    ROM_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_5);						// 配置UART外围设备使用的引脚。
	// 此函数为初始化，第一个参数为UART口的选择有0--7个、第二个参数为系统时钟
	// 第三个参数为波特率、后面几个一般不改为8个数据位，一个停止位，无校验位。
    ROM_UARTConfigSetExpClk(UART4_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//不使能FIFO
	ROM_UARTFIFODisable(UART4_BASE);	
	//使能FIFO
//    UARTFIFOEnable(UART4_BASE);											//使能UART的缓存区，默认位1/2深度 即16的1/2为8位，可以选择
//    UARTFIFOLevelSet(UART4_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//此函数为选择深度的函数，目前选择的深度为2/8 即4位。。。如果直接把FIFO失能（disable）的话，就可以一个字符触发一次中断。
	
	//如果需要发送中断只需变成   UART_INT_RX | UART_INT_TX
    ROM_UARTIntEnable(UART4_BASE, UART_INT_RX);								//对于串口接收中断使能，
    ROM_IntPrioritySet(INT_UART4, 0x0);										// 这里指串口4配置为接收中断（RX指接收） 优先级位0最大 可选择0xE0-0x00
    UARTIntRegister(UART4_BASE, ROM_UART4IntHandler);						// 注册中断函数	
    ROM_IntEnable(INT_UART4);												//中断使能			
    ROM_IntMasterEnable();													//中断使能			
}

void ROM_UART4IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART4_BASE, true);
	
    ROM_UARTIntClear(UART4_BASE, ui32IntStatus);//先清除中断标志位
	//此函数是判断是否接受到东西，有返回真
	//发送中断进入if，if执行完退出中断
	if ( !ROM_UARTCharsAvail(UART4_BASE))
	{
		
		return;
	}
	//接受中断进入while
	while (ROM_UARTCharsAvail(UART4_BASE))
	{
		//此函数从指定的接收FIFO中获取一个字符
		ui8RxBuffer = (uint8_t) ROM_UARTCharGetNonBlocking(UART4_BASE);
	}
	//接受中断后续内容可以在下面添加
	
}

/*************************************************************************
串口5的一般初始化           RX5->PE4          TX5->PE5
波特率为115200，8位数据位，一个停止位，无校验位。
FIFO深度默认不使能，即发送一个数据或者接受一个数据进入中断，默认不开启发送中断
中断优先级默认位0x0即最高优先级
**************************************************************************/
void ROM_UART5_Init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);						// 启用 UART 使用的 GPIO 外设。
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);						// 使能UART5
	ROM_GPIOPinConfigure(GPIO_PE4_U5RX);									// 为UART模式配置GPIO引脚。
    ROM_GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4);						// 配置UART外围设备使用的引脚。
    ROM_GPIOPinConfigure(GPIO_PE5_U5TX);									// 为UART模式配置GPIO引脚。
    ROM_GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_5);						// 配置UART外围设备使用的引脚。
	// 此函数为初始化，第一个参数为UART口的选择有0--7个、第二个参数为系统时钟
	// 第三个参数为波特率、后面几个一般不改为8个数据位，一个停止位，无校验位。
    ROM_UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//不使能FIFO
	ROM_UARTFIFODisable(UART5_BASE);	
	//使能FIFO
//    UARTFIFOEnable(UART5_BASE);											//使能UART的缓存区，默认位1/2深度 即16的1/2为8位，可以选择
//    UARTFIFOLevelSet(UART5_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//此函数为选择深度的函数，目前选择的深度为2/8 即4位。。。如果直接把FIFO失能（disable）的话，就可以一个字符触发一次中断。
	
	//如果需要发送中断只需变成   UART_INT_RX | UART_INT_TX
    ROM_UARTIntEnable(UART5_BASE, UART_INT_RX);								//对于串口接收中断使能，
    ROM_IntPrioritySet(INT_UART5, 0x0);										// 这里指串口5配置为接收中断（RX指接收） 优先级位0最大 可选择0xE0-0x00
    UARTIntRegister(UART5_BASE, ROM_UART5IntHandler);						// 注册中断函数	
    ROM_IntEnable(INT_UART5);												//中断使能			
    ROM_IntMasterEnable();													//中断使能			
}

//void ROM_UART5IntHandler(void)
//{
//    uint32_t ui32IntStatus;
//	uint8_t ui8RxBuffer;
//	
//    ui32IntStatus = UARTIntStatus(UART5_BASE, true);
//	
//    ROM_UARTIntClear(UART5_BASE, ui32IntStatus);//先清除中断标志位
//	//此函数是判断是否接受到东西，有返回真
//	//发送中断进入if，if执行完退出中断
//	if ( !ROM_UARTCharsAvail(UART5_BASE))
//	{
//		
//		return;
//	}
//	//接受中断进入while
//	while (ROM_UARTCharsAvail(UART5_BASE))
//	{
//		//此函数从指定的接收FIFO中获取一个字符
//		ui8RxBuffer = (uint8_t) ROM_UARTCharGetNonBlocking(UART5_BASE);
//	}
//	//接受中断后续内容可以在下面添加
//	
//}

/*************************************************************************
串口6的一般初始化           RX6->PD4          TX6->PD5
波特率为115200，8位数据位，一个停止位，无校验位。
FIFO深度默认不使能，即发送一个数据或者接受一个数据进入中断，默认不开启发送中断
中断优先级默认位0x0即最高优先级
**************************************************************************/
void ROM_UART6_Init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);						// 启用 UART 使用的 GPIO 外设。
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART6);						// 使能UART6
	ROM_GPIOPinConfigure(GPIO_PD4_U6RX);									// 为UART模式配置GPIO引脚。
    ROM_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4);						// 配置UART外围设备使用的引脚。
    ROM_GPIOPinConfigure(GPIO_PD5_U6TX);									// 为UART模式配置GPIO引脚。
    ROM_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_5);						// 配置UART外围设备使用的引脚。
	// 此函数为初始化，第一个参数为UART口的选择有0--7个、第二个参数为系统时钟
	// 第三个参数为波特率、后面几个一般不改为8个数据位，一个停止位，无校验位。
    ROM_UARTConfigSetExpClk(UART6_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//不使能FIFO
	ROM_UARTFIFODisable(UART6_BASE);	
	//使能FIFO
//    UARTFIFOEnable(UART6_BASE);											//使能UART的缓存区，默认位1/2深度 即16的1/2为8位，可以选择
//    UARTFIFOLevelSet(UART6_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//此函数为选择深度的函数，目前选择的深度为2/8 即4位。。。如果直接把FIFO失能（disable）的话，就可以一个字符触发一次中断。
	
	//如果需要发送中断只需变成   UART_INT_RX | UART_INT_TX
    ROM_UARTIntEnable(UART6_BASE, UART_INT_RX);								//对于串口接收中断使能，
    ROM_IntPrioritySet(INT_UART6, 0x0);										// 这里指串口6配置为接收中断（RX指接收） 优先级位0最大 可选择0xE0-0x00
    UARTIntRegister(UART6_BASE, ROM_UART6IntHandler);						// 注册中断函数	
    ROM_IntEnable(INT_UART6);												//中断使能			
    ROM_IntMasterEnable();													//中断使能			
}

//void ROM_UART6IntHandler(void)
//{
//    uint32_t ui32IntStatus;
//	uint8_t ui8RxBuffer;
//	
//    ui32IntStatus = UARTIntStatus(UART6_BASE, true);
//	
//    ROM_UARTIntClear(UART6_BASE, ui32IntStatus);//先清除中断标志位
//	//此函数是判断是否接受到东西，有返回真
//	//发送中断进入if，if执行完退出中断
//	if ( !ROM_UARTCharsAvail(UART6_BASE))
//	{
//		
//		return;
//	}
//	//接受中断进入while
//	while (ROM_UARTCharsAvail(UART6_BASE))
//	{
//		//此函数从指定的接收FIFO中获取一个字符
//		ui8RxBuffer = (uint8_t) ROM_UARTCharGetNonBlocking(UART6_BASE);
//	}
//	//接受中断后续内容可以在下面添加
//	
//}

/*************************************************************************
串口7的一般初始化           RX7->PE0          TX7->PE1
波特率为115200，8位数据位，一个停止位，无校验位。
FIFO深度默认不使能，即发送一个数据或者接受一个数据进入中断，默认不开启发送中断
中断优先级默认位0x0即最高优先级
**************************************************************************/
void ROM_UART7_Init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);						// 启用 UART 使用的 GPIO 外设。
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);						// 使能UART7
	ROM_GPIOPinConfigure(GPIO_PE0_U7RX);									// 为UART模式配置GPIO引脚。
    ROM_GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0);						// 配置UART外围设备使用的引脚。
    ROM_GPIOPinConfigure(GPIO_PE1_U7TX);									// 为UART模式配置GPIO引脚。
    ROM_GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_1);						// 配置UART外围设备使用的引脚。
	// 此函数为初始化，第一个参数为UART口的选择有0--7个、第二个参数为系统时钟
	// 第三个参数为波特率、后面几个一般不改为8个数据位，一个停止位，无校验位。
    ROM_UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//不使能FIFO
	ROM_UARTFIFODisable(UART7_BASE);	
	//使能FIFO
//    UARTFIFOEnable(UART7_BASE);											//使能UART的缓存区，默认位1/2深度 即16的1/2为8位，可以选择
//    UARTFIFOLevelSet(UART7_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//此函数为选择深度的函数，目前选择的深度为2/8 即4位。。。如果直接把FIFO失能（disable）的话，就可以一个字符触发一次中断。
	
	//如果需要发送中断只需变成   UART_INT_RX | UART_INT_TX
    ROM_UARTIntEnable(UART7_BASE, UART_INT_RX);								//对于串口接收中断使能，
    ROM_IntPrioritySet(INT_UART7, 0x0);										// 这里指串口7配置为接收中断（RX指接收） 优先级位0最大 可选择0xE0-0x00
    UARTIntRegister(UART7_BASE, ROM_UART7IntHandler);						// 注册中断函数	
    ROM_IntEnable(INT_UART7);												//中断使能			
    ROM_IntMasterEnable();													//中断使能			
}
void ROM_UART7IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART7_BASE, true);
	
    ROM_UARTIntClear(UART7_BASE, ui32IntStatus);//先清除中断标志位
	//此函数是判断是否接受到东西，有返回真
	//发送中断进入if，if执行完退出中断
	if ( !ROM_UARTCharsAvail(UART7_BASE))
	{
		
		return;
	}
	//接受中断进入while
	while (ROM_UARTCharsAvail(UART7_BASE))
	{
		//此函数从指定的接收FIFO中获取一个字符
		ui8RxBuffer = (uint8_t) ROM_UARTCharGetNonBlocking(UART7_BASE);
	}
	//接受中断后续内容可以在下面添加
	
}


//***********************************************************************************************************************************************
void UART0_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);						// 启用 UART 使用的 GPIO 外设。
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);						// 使能UART0
	GPIOPinConfigure(GPIO_PA0_U0RX);									// 为UART模式配置GPIO引脚。
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0);						// 配置UART外围设备使用的引脚。
    GPIOPinConfigure(GPIO_PA1_U0TX);									// 为UART模式配置GPIO引脚。
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_1);						// 配置UART外围设备使用的引脚。
	// 此函数为初始化，第一个参数为UART口的选择有0--7个、第二个参数为系统时钟
	// 第三个参数为波特率、后面几个一般不改为8个数据位，一个停止位，无校验位。
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//不使能FIFO
	UARTFIFODisable(UART0_BASE);	
	//使能FIFO
//    UARTFIFOEnable(UART0_BASE);											//使能UART的缓存区，默认位1/2深度 即16的1/2为8位，可以选择
//    UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//此函数为选择深度的函数，目前选择的深度为2/8 即4位。。。如果直接把FIFO失能（disable）的话，就可以一个字符触发一次中断。
	
	//如果需要发送中断只需变成   UART_INT_RX||UART_INT_TX
    UARTIntEnable(UART0_BASE, UART_INT_RX);								//对于串口接收中断使能，
    IntPrioritySet(INT_UART0, 0x0);										// 这里指串口0配置为接收中断（RX指接收） 优先级位0最大 可选择0xE0-0x00
    UARTIntRegister(UART0_BASE, UART0IntHandler);						// 注册中断函数	
    IntEnable(INT_UART0);												//中断使能			
    IntMasterEnable();													//中断使能			
}

void UART1_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);						//启用 UART 使用的 GPIO 外设。
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);						// 使能UART1
	GPIOPinConfigure(GPIO_PB0_U1RX);									//为UART模式配置GPIO引脚。
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0);						//配置UART外围设备使用的引脚。
    GPIOPinConfigure(GPIO_PB1_U1TX);									//为UART模式配置GPIO引脚。
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_1);						//配置UART外围设备使用的引脚。
	//此函数为初始化，第一个参数为UART口的选择有0--7个、第二个参数为系统时钟
	//第三个参数为波特率、后面几个一般不改为8个数据位，一个停止位，无校验位。
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//不使能FIFO
	UARTFIFODisable(UART1_BASE);
	//使能FIFO
//    UARTFIFOEnable(UART1_BASE);											//使能UART的缓存区，默认位1/2深度 即16的1/2为8位，可以选择
//    UARTFIFOLevelSet(UART1_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//此函数为选择深度的函数，目前选择的深度为2/8 即4位。。。如果直接把FIFO失能（disable）的话，就可以一个字符触发一次中断。
	
	//如果需要发送中断只需变成   UART_INT_RX | UART_INT_TX
    UARTIntEnable(UART1_BASE, UART_INT_RX);								//对于串口接收中断使能
	
    IntPrioritySet(INT_UART1, 0x0);										// 这里指串口1配置为接收中断（RX指接收） 优先级位0最大 可选择0xE0-0x00
    UARTIntRegister(UART1_BASE, UART1IntHandler);						// 注册中断函数	
    IntEnable(INT_UART1);												//中断使能			
    IntMasterEnable();													//中断使能			
}


void UART2_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);						// 启用 UART 使用的 GPIO 外设。
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);						// 使能UART2
	GPIOPinConfigure(GPIO_PD6_U2RX);									// 为UART模式配置GPIO引脚。
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6);						// 配置UART外围设备使用的引脚。
    GPIOPinConfigure(GPIO_PD7_U2TX);									// 为UART模式配置GPIO引脚。
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_7);						// 配置UART外围设备使用的引脚。
	// 此函数为初始化，第一个参数为UART口的选择有0--7个、第二个参数为系统时钟
	// 第三个参数为波特率、后面几个一般不改为8个数据位，一个停止位，无校验位。
    UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//不使能FIFO
	UARTFIFODisable(UART2_BASE);	
	//使能FIFO
//    UARTFIFOEnable(UART2_BASE);											//使能UART的缓存区，默认位1/2深度 即16的1/2为8位，可以选择
//    UARTFIFOLevelSet(UART2_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//此函数为选择深度的函数，目前选择的深度为2/8 即4位。。。如果直接把FIFO失能（disable）的话，就可以一个字符触发一次中断。
	
	//如果需要发送中断只需变成   UART_INT_RX||UART_INT_TX
    UARTIntEnable(UART2_BASE, UART_INT_RX);								//对于串口接收中断使能，
    IntPrioritySet(INT_UART2, 0x0);										// 这里指串口2配置为接收中断（RX指接收） 优先级位0最大 可选择0xE0-0x00
    UARTIntRegister(UART2_BASE, UART2IntHandler);						// 注册中断函数	
    IntEnable(INT_UART2);												//中断使能			
    IntMasterEnable();													//中断使能			
}


void UART3_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);						// 启用 UART 使用的 GPIO 外设。
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);						// 使能UART3
	GPIOPinConfigure(GPIO_PC6_U3RX);									// 为UART模式配置GPIO引脚。
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6);						// 配置UART外围设备使用的引脚。
    GPIOPinConfigure(GPIO_PC7_U3TX);									// 为UART模式配置GPIO引脚。
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_7);						// 配置UART外围设备使用的引脚。
	// 此函数为初始化，第一个参数为UART口的选择有0--7个、第二个参数为系统时钟
	// 第三个参数为波特率、后面几个一般不改为8个数据位，一个停止位，无校验位。
    UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), 9600,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//不使能FIFO
	UARTFIFODisable(UART3_BASE);	
	//使能FIFO
//    UARTFIFOEnable(UART3_BASE);											//使能UART的缓存区，默认位1/2深度 即16的1/2为8位，可以选择
//    UARTFIFOLevelSet(UART3_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//此函数为选择深度的函数，目前选择的深度为2/8 即4位。。。如果直接把FIFO失能（disable）的话，就可以一个字符触发一次中断。
	
	//如果需要发送中断只需变成   UART_INT_RX||UART_INT_TX
    UARTIntEnable(UART3_BASE, UART_INT_RX);								//对于串口接收中断使能，
    IntPrioritySet(INT_UART3, 0x0);										// 这里指串口3配置为接收中断（RX指接收） 优先级位0最大 可选择0xE0-0x00
    UARTIntRegister(UART3_BASE, UART0IntHandler);						// 注册中断函数	
    IntEnable(INT_UART3);												//中断使能			
    IntMasterEnable();													//中断使能			
}

void UART4_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);						// 启用 UART 使用的 GPIO 外设。
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);						// 使能UART4
	GPIOPinConfigure(GPIO_PC4_U4RX);									// 为UART模式配置GPIO引脚。
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4);						// 配置UART外围设备使用的引脚。
    GPIOPinConfigure(GPIO_PC5_U4TX);									// 为UART模式配置GPIO引脚。
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_5);						// 配置UART外围设备使用的引脚。
	// 此函数为初始化，第一个参数为UART口的选择有0--7个、第二个参数为系统时钟
	// 第三个参数为波特率、后面几个一般不改为8个数据位，一个停止位，无校验位。
    UARTConfigSetExpClk(UART4_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//不使能FIFO
	UARTFIFODisable(UART4_BASE);	
	//使能FIFO
//    UARTFIFOEnable(UART4_BASE);											//使能UART的缓存区，默认位1/2深度 即16的1/2为8位，可以选择
//    UARTFIFOLevelSet(UART4_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//此函数为选择深度的函数，目前选择的深度为2/8 即4位。。。如果直接把FIFO失能（disable）的话，就可以一个字符触发一次中断。
	
	//如果需要发送中断只需变成   UART_INT_RX||UART_INT_TX
    UARTIntEnable(UART4_BASE, UART_INT_RX);								//对于串口接收中断使能，
    IntPrioritySet(INT_UART4, 0x0);										// 这里指串口4配置为接收中断（RX指接收） 优先级位0最大 可选择0xE0-0x00
    UARTIntRegister(UART4_BASE, UART0IntHandler);						// 注册中断函数	
    IntEnable(INT_UART4);												//中断使能			
    IntMasterEnable();													//中断使能			
}

void UART5_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);						// 启用 UART 使用的 GPIO 外设。
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);						// 使能UART5
	GPIOPinConfigure(GPIO_PE4_U5RX);									// 为UART模式配置GPIO引脚。
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4);						// 配置UART外围设备使用的引脚。
    GPIOPinConfigure(GPIO_PE5_U5TX);									// 为UART模式配置GPIO引脚。
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_5);						// 配置UART外围设备使用的引脚。
	// 此函数为初始化，第一个参数为UART口的选择有0--7个、第二个参数为系统时钟
	// 第三个参数为波特率、后面几个一般不改为8个数据位，一个停止位，无校验位。
    UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//不使能FIFO
	UARTFIFODisable(UART5_BASE);	
	//使能FIFO
//    UARTFIFOEnable(UART5_BASE);											//使能UART的缓存区，默认位1/2深度 即16的1/2为8位，可以选择
//    UARTFIFOLevelSet(UART5_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//此函数为选择深度的函数，目前选择的深度为2/8 即4位。。。如果直接把FIFO失能（disable）的话，就可以一个字符触发一次中断。
	
	//如果需要发送中断只需变成   UART_INT_RX||UART_INT_TX
    UARTIntEnable(UART5_BASE, UART_INT_RX);								//对于串口接收中断使能，
    IntPrioritySet(INT_UART5, 0x0);										// 这里指串口5配置为接收中断（RX指接收） 优先级位0最大 可选择0xE0-0x00
    UARTIntRegister(UART5_BASE, UART0IntHandler);						// 注册中断函数	
    IntEnable(INT_UART5);												//中断使能			
    IntMasterEnable();													//中断使能			
}

void UART6_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);						// 启用 UART 使用的 GPIO 外设。
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART6);						// 使能UART6
	GPIOPinConfigure(GPIO_PD4_U6RX);									// 为UART模式配置GPIO引脚。
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4);						// 配置UART外围设备使用的引脚。
    GPIOPinConfigure(GPIO_PD5_U6TX);									// 为UART模式配置GPIO引脚。
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_5);						// 配置UART外围设备使用的引脚。
	// 此函数为初始化，第一个参数为UART口的选择有0--7个、第二个参数为系统时钟
	// 第三个参数为波特率、后面几个一般不改为8个数据位，一个停止位，无校验位。
    UARTConfigSetExpClk(UART6_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//不使能FIFO
	UARTFIFODisable(UART6_BASE);	
	//使能FIFO
//    UARTFIFOEnable(UART6_BASE);											//使能UART的缓存区，默认位1/2深度 即16的1/2为8位，可以选择
//    UARTFIFOLevelSet(UART6_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//此函数为选择深度的函数，目前选择的深度为2/8 即4位。。。如果直接把FIFO失能（disable）的话，就可以一个字符触发一次中断。
	
	//如果需要发送中断只需变成   UART_INT_RX||UART_INT_TX
    UARTIntEnable(UART6_BASE, UART_INT_RX);								//对于串口接收中断使能，
    IntPrioritySet(INT_UART6, 0x0);										// 这里指串口6配置为接收中断（RX指接收） 优先级位0最大 可选择0xE0-0x00
    UARTIntRegister(UART6_BASE, UART0IntHandler);						// 注册中断函数	
    IntEnable(INT_UART6);												//中断使能			
    IntMasterEnable();													//中断使能			
}

void UART7_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);						// 启用 UART 使用的 GPIO 外设。
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);						// 使能UART7
	GPIOPinConfigure(GPIO_PE0_U7RX);									// 为UART模式配置GPIO引脚。
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0);						// 配置UART外围设备使用的引脚。
    GPIOPinConfigure(GPIO_PE1_U7TX);									// 为UART模式配置GPIO引脚。
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_1);						// 配置UART外围设备使用的引脚。
	// 此函数为初始化，第一个参数为UART口的选择有0--7个、第二个参数为系统时钟
	// 第三个参数为波特率、后面几个一般不改为8个数据位，一个停止位，无校验位。
    UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//不使能FIFO
	UARTFIFODisable(UART7_BASE);	
	//使能FIFO
//    UARTFIFOEnable(UART7_BASE);											//使能UART的缓存区，默认位1/2深度 即16的1/2为8位，可以选择
//    UARTFIFOLevelSet(UART7_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//此函数为选择深度的函数，目前选择的深度为2/8 即4位。。。如果直接把FIFO失能（disable）的话，就可以一个字符触发一次中断。
	
	//如果需要发送中断只需变成   UART_INT_RX||UART_INT_TX
    UARTIntEnable(UART7_BASE, UART_INT_RX);								//对于串口接收中断使能，
    IntPrioritySet(INT_UART7, 0x0);										// 这里指串口7配置为接收中断（RX指接收） 优先级位0最大 可选择0xE0-0x00
    UARTIntRegister(UART7_BASE, UART0IntHandler);						// 注册中断函数	
    IntEnable(INT_UART7);												//中断使能			
    IntMasterEnable();													//中断使能			
}

void UART0IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART0_BASE, true);
	
    UARTIntClear(UART0_BASE, ui32IntStatus);//先清除中断标志位
	//此函数是判断是否接受到东西，有返回真
	//发送中断进入if，if执行完退出中断
	if ( !UARTCharsAvail(UART0_BASE))
	{
		
		return;
	}
	//接受中断进入while
	while (UARTCharsAvail(UART0_BASE))
	{
		//此函数从指定的接收FIFO中获取一个字符
		ui8RxBuffer = (uint8_t) UARTCharGetNonBlocking(UART0_BASE);
	}
	//接受中断后续内容可以在下面添加
	
	
}

void UART1IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART1_BASE, true);
	
    UARTIntClear(UART1_BASE, ui32IntStatus);//先清除中断标志位
	//此函数是判断是否接受到东西，有返回真
	//发送中断进入if，if执行完退出中断
	if ( !UARTCharsAvail(UART1_BASE))
	{
		
		return;
	}
	//接受中断进入while
	while (UARTCharsAvail(UART1_BASE))
	{
		//此函数从指定的接收FIFO中获取一个字符
		ui8RxBuffer = (uint8_t) UARTCharGetNonBlocking(UART1_BASE);
	}
	//接受中断后续内容可以在下面添加
	
	
}



void UART2IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART2_BASE, true);
	
    UARTIntClear(UART2_BASE, ui32IntStatus);//先清除中断标志位
	//此函数是判断是否接受到东西，有返回真
	//发送中断进入if，if执行完退出中断
	if ( !UARTCharsAvail(UART2_BASE))
	{
		
		return;
	}
	//接受中断进入while
	while (UARTCharsAvail(UART2_BASE))
	{
		//此函数从指定的接收FIFO中获取一个字符
		ui8RxBuffer = (uint8_t) UARTCharGetNonBlocking(UART2_BASE);
	}
	//接受中断后续内容可以在下面添加
	
	
}



void UART3IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART3_BASE, true);
	
    UARTIntClear(UART3_BASE, ui32IntStatus);//先清除中断标志位
	//此函数是判断是否接受到东西，有返回真
	//发送中断进入if，if执行完退出中断
	if ( !UARTCharsAvail(UART3_BASE))
	{
		
		return;
	}
	//接受中断进入while
	while (UARTCharsAvail(UART3_BASE))
	{
		//此函数从指定的接收FIFO中获取一个字符
		ui8RxBuffer = (uint8_t) UARTCharGetNonBlocking(UART3_BASE);
	}
	//接受中断后续内容可以在下面添加
	
	
}



void UART4IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART4_BASE, true);
	
    UARTIntClear(UART4_BASE, ui32IntStatus);//先清除中断标志位
	//此函数是判断是否接受到东西，有返回真
	//发送中断进入if，if执行完退出中断
	if ( !UARTCharsAvail(UART4_BASE))
	{
		
		return;
	}
	//接受中断进入while
	while (UARTCharsAvail(UART4_BASE))
	{
		//此函数从指定的接收FIFO中获取一个字符
		ui8RxBuffer = (uint8_t) UARTCharGetNonBlocking(UART4_BASE);
	}
	//接受中断后续内容可以在下面添加
	
	
}



void UART5IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART5_BASE, true);
	
    UARTIntClear(UART5_BASE, ui32IntStatus);//先清除中断标志位
	//此函数是判断是否接受到东西，有返回真
	//发送中断进入if，if执行完退出中断
	if ( !UARTCharsAvail(UART5_BASE))
	{
		
		return;
	}
	//接受中断进入while
	while (UARTCharsAvail(UART5_BASE))
	{
		//此函数从指定的接收FIFO中获取一个字符
		ui8RxBuffer = (uint8_t) UARTCharGetNonBlocking(UART5_BASE);
	}
	//接受中断后续内容可以在下面添加
	
	
}



void UART6IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART6_BASE, true);
	
    UARTIntClear(UART6_BASE, ui32IntStatus);//先清除中断标志位
	//此函数是判断是否接受到东西，有返回真
	//发送中断进入if，if执行完退出中断
	if ( !UARTCharsAvail(UART6_BASE))
	{
		
		return;
	}
	//接受中断进入while
	while (UARTCharsAvail(UART6_BASE))
	{
		//此函数从指定的接收FIFO中获取一个字符
		ui8RxBuffer = (uint8_t) UARTCharGetNonBlocking(UART6_BASE);
	}
	//接受中断后续内容可以在下面添加
	
	
}



void UART7IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART7_BASE, true);
	
    UARTIntClear(UART7_BASE, ui32IntStatus);//先清除中断标志位
	//此函数是判断是否接受到东西，有返回真
	//发送中断进入if，if执行完退出中断
	if ( !UARTCharsAvail(UART7_BASE))
	{
		
		return;
	}
	//接受中断进入while
	while (UARTCharsAvail(UART7_BASE))
	{
		//此函数从指定的接收FIFO中获取一个字符
		ui8RxBuffer = (uint8_t) UARTCharGetNonBlocking(UART7_BASE);
	}
	//接受中断后续内容可以在下面添加
	
	
}
