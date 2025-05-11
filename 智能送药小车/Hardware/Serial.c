/***************************************************
 * @brief   ����TM4C123GH6PZоƬ��ȫ�����ڴ������UART�ĳ�ʼ������
 *          �Լ���Ӧ���жϷ��������塣
 *          �Լ�һЩ��װ�õķ����ֽڣ��ַ��������ֵȵĺ�����
 *			��ӳ���printf����������ڣ���ӡ�����ݸ�����
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
 *          Ĭ�϶��ǲ�����Ϊ115200��8λ����λ��һ��ֹͣλ����У��λ��
 *          FIFO���Ĭ�ϲ�ʹ�ܣ�������һ�����ݻ��߽���һ�����ݽ����жϣ�Ĭ�ϲ����������ж�
 *          �ж����ȼ�Ĭ��λ0x0��������ȼ�
 *			Ĭ���ô�ROM�ģ�����ROM���ڱ��ļ������
 * @date    2024/6/20
 * @author  qihang chen
****************************************************/

//���Ӵ��ڶ���TX2��PD7���ܽ��ܵ��������ܷ��ͣ��ܽ����ж�

#include "include.h"
// ����ӳ��printf����
// ֻ������ӳ��һ�����ڣ�Ĭ��UART1
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
  * ��    �������ڷ���һ���ֽ�
  * ��    ������Ҫ�Ĵ��� 
  * ��    ����Byte Ҫ���͵�һ���ֽ�
  * �� �� ֵ����
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
  * ��    �������ڷ���һ������
  * ��    ������Ҫ�Ĵ��� 
  * ��    ����Array Ҫ����������׵�ַ
  * ��    ����Length Ҫ��������ĳ���
  * �� �� ֵ����
  */
void Serial_SendArray(uint8_t uart, uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//��������
	{
		Serial_SendByte(uart, Array[i]);		//���ε���Serial_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �������ڷ���һ���ַ���
  * ��    ������Ҫ�Ĵ��� 
  * ��    ����String Ҫ�����ַ������׵�ַ
  * �� �� ֵ����
  */
void Serial_SendString(uint8_t uart, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//�����ַ����飨�ַ������������ַ���������־λ��ֹͣ
	{
		Serial_SendByte(uart, String[i]);		//���ε���Serial_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �����η��������ڲ�ʹ�ã�
  * �� �� ֵ������ֵ����X��Y�η�
  */
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	//���ý����ֵΪ1
	while (Y --)			//ִ��Y��
	{
		Result *= X;		//��X�۳˵����
	}
	return Result;
}

/**
  * ��    �������ڷ�������
  * ��    ������Ҫ�Ĵ��� 
  * ��    ����Number Ҫ���͵����֣���Χ��0~4294967295
  * ��    ����Length Ҫ�������ֵĳ��ȣ���Χ��0~10
  * �� �� ֵ����
  */
void Serial_SendNumber(uint8_t uart, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)		//�������ֳ��ȱ������ֵ�ÿһλ
	{
		Serial_SendByte(uart, Number / Serial_Pow(10, Length - i - 1) % 10 + '0');	//���ε���Serial_SendByte����ÿλ����
	}
}

/*****************************************************
* �����жϺ�������Ҫע�ᣬ��UARTIntRegister����
* ��ǰ����һ�£����嶨���ں��棬����Ҫ��������ʹ�ã�
* ����ֱ�ӰѺ����Ķ����ȡ������������ȥ
**********************************************/

/*************************************************************************
����0��һ���ʼ��           RX0->PA0          TX0->PA1
������Ϊ115200��8λ����λ��һ��ֹͣλ����У��λ��
FIFO���Ĭ�ϲ�ʹ�ܣ�������һ�����ݻ��߽���һ�����ݽ����жϣ�Ĭ�ϲ����������ж�
�ж����ȼ�Ĭ��λ0x0��������ȼ�
**************************************************************************/
void ROM_UART0_Init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);						// ���� UART ʹ�õ� GPIO ���衣
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);						// ʹ��UART0
	ROM_GPIOPinConfigure(GPIO_PA0_U0RX);									// ΪUARTģʽ����GPIO���š�
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0);						// ����UART��Χ�豸ʹ�õ����š�
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);									// ΪUARTģʽ����GPIO���š�
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_1);						// ����UART��Χ�豸ʹ�õ����š�
	// �˺���Ϊ��ʼ������һ������ΪUART�ڵ�ѡ����0--7�����ڶ�������Ϊϵͳʱ��
	// ����������Ϊ�����ʡ����漸��һ�㲻��Ϊ8������λ��һ��ֹͣλ����У��λ��
    ROM_UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//��ʹ��FIFO
	ROM_UARTFIFODisable(UART0_BASE);	
	//ʹ��FIFO
//    UARTFIFOEnable(UART0_BASE);											//ʹ��UART�Ļ�������Ĭ��λ1/2��� ��16��1/2Ϊ8λ������ѡ��
//    UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//�˺���Ϊѡ����ȵĺ�����Ŀǰѡ������Ϊ2/8 ��4λ���������ֱ�Ӱ�FIFOʧ�ܣ�disable���Ļ����Ϳ���һ���ַ�����һ���жϡ�
	
	//�����Ҫ�����ж�ֻ����   UART_INT_RX | UART_INT_TX
    ROM_UARTIntEnable(UART0_BASE, UART_INT_RX);								//���ڴ��ڽ����ж�ʹ�ܣ�
    ROM_IntPrioritySet(INT_UART0, 0x0);										// ����ָ����0����Ϊ�����жϣ�RXָ���գ� ���ȼ�λ0��� ��ѡ��0xE0-0x00
    UARTIntRegister(UART0_BASE, ROM_UART0IntHandler);						// ע���жϺ���	
    ROM_IntEnable(INT_UART0);												//�ж�ʹ��			
    ROM_IntMasterEnable();													//�ж�ʹ��			
}

void ROM_UART0IntHandler(void)
{
  uint32_t ui32IntStatus;

	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART0_BASE, true);
	
    ROM_UARTIntClear(UART0_BASE, ui32IntStatus);//������жϱ�־λ
	//�˺������ж��Ƿ���ܵ��������з�����
	//�����жϽ���if��ifִ�����˳��ж�
	if ( !ROM_UARTCharsAvail(UART0_BASE))
	{
		
		return;
	}
	//�����жϽ���while
	while (ROM_UARTCharsAvail(UART0_BASE))
	{
		//�˺�����ָ���Ľ���FIFO�л�ȡһ���ַ�
		ui8RxBuffer = (uint8_t) ROM_UARTCharGetNonBlocking(UART0_BASE);
		
		
		
		
	}
	//�����жϺ������ݿ������������
	
}

/*************************************************************************
����1��һ���ʼ��           RX1->PB0          TX1->PB1
������Ϊ115200��8λ����λ��һ��ֹͣλ����У��λ��
FIFO���Ĭ�ϲ�ʹ�ܣ�������һ�����ݻ��߽���һ�����ݽ����жϣ�Ĭ�ϲ����������ж�
�ж����ȼ�Ĭ��λ0x0��������ȼ�
**************************************************************************/
void ROM_UART1_Init(void)
{
	  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);						//���� UART ʹ�õ� GPIO ���衣
	  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);						// ʹ��UART1
	  ROM_GPIOPinConfigure(GPIO_PB0_U1RX);									//ΪUARTģʽ����GPIO���š�
    ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0);						//����UART��Χ�豸ʹ�õ����š�
    ROM_GPIOPinConfigure(GPIO_PB1_U1TX);									//ΪUARTģʽ����GPIO���š�
    ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_1);						//����UART��Χ�豸ʹ�õ����š�
	//�˺���Ϊ��ʼ������һ������ΪUART�ڵ�ѡ����0--7�����ڶ�������Ϊϵͳʱ��
	//����������Ϊ�����ʡ����漸��һ�㲻��Ϊ8������λ��һ��ֹͣλ����У��λ��
    ROM_UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//��ʹ��FIFO
	ROM_UARTFIFODisable(UART1_BASE);
	//ʹ��FIFO
//    UARTFIFOEnable(UART1_BASE);											//ʹ��UART�Ļ�������Ĭ��λ1/2��� ��16��1/2Ϊ8λ������ѡ��
//    UARTFIFOLevelSet(UART1_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//�˺���Ϊѡ����ȵĺ�����Ŀǰѡ������Ϊ2/8 ��4λ���������ֱ�Ӱ�FIFOʧ�ܣ�disable���Ļ����Ϳ���һ���ַ�����һ���жϡ�
	
	//�����Ҫ�����ж�ֻ����   UART_INT_RX | UART_INT_TX
    ROM_UARTIntEnable(UART1_BASE, UART_INT_RX);								//���ڴ��ڽ����ж�ʹ��
    ROM_IntPrioritySet(INT_UART1, 0x10);										// ����ָ����1����Ϊ�����жϣ�RXָ���գ� ���ȼ�λ0��� ��ѡ��0xE0-0x00
    UARTIntRegister(UART1_BASE, ROM_UART1IntHandler);						// ע���жϺ���	
    ROM_IntEnable(INT_UART1);												//�ж�ʹ��			
    ROM_IntMasterEnable();													//�ж�ʹ��			
}

//void ROM_UART1IntHandler(void)
//{
//    uint32_t ui32IntStatus;
//	uint8_t ui8RxBuffer;
//	
//    ui32IntStatus = UARTIntStatus(UART1_BASE, true);
//	
//    ROM_UARTIntClear(UART1_BASE, ui32IntStatus);//������жϱ�־λ
//	//�˺������ж��Ƿ���ܵ��������з�����
//	//�����жϽ���if��ifִ�����˳��ж�
//	if ( !ROM_UARTCharsAvail(UART1_BASE))
//	{
//		
//		return;
//	}
//	//�����жϽ���while
//	while (ROM_UARTCharsAvail(UART1_BASE))
//	{
//		//�˺�����ָ���Ľ���FIFO�л�ȡһ���ַ�
//		ui8RxBuffer = (uint8_t) ROM_UARTCharGetNonBlocking(UART1_BASE);
//	}
//	//�����жϺ������ݿ������������
//	
//}



/*************************************************************************
����2��һ���ʼ��           RX2->PD6          TX2->PD7
������Ϊ115200��8λ����λ��һ��ֹͣλ����У��λ��
FIFO���Ĭ�ϲ�ʹ�ܣ�������һ�����ݻ��߽���һ�����ݽ����жϣ�Ĭ�ϲ����������ж�
�ж����ȼ�Ĭ��λ0x0��������ȼ�
**************************************************************************/
void ROM_UART2_Init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);						// ���� UART ʹ�õ� GPIO ���衣
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);						// ʹ��UART2
	ROM_GPIOPinConfigure(GPIO_PD6_U2RX);									// ΪUARTģʽ����GPIO���š�
    ROM_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6);						// ����UART��Χ�豸ʹ�õ����š�
    ROM_GPIOPinConfigure(GPIO_PD7_U2TX);									// ΪUARTģʽ����GPIO���š�
    ROM_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_7);						// ����UART��Χ�豸ʹ�õ����š�
	// �˺���Ϊ��ʼ������һ������ΪUART�ڵ�ѡ����0--7�����ڶ�������Ϊϵͳʱ��
	// ����������Ϊ�����ʡ����漸��һ�㲻��Ϊ8������λ��һ��ֹͣλ����У��λ��
    ROM_UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//��ʹ��FIFO
	ROM_UARTFIFODisable(UART2_BASE);	
	//ʹ��FIFO
//    UARTFIFOEnable(UART2_BASE);											//ʹ��UART�Ļ�������Ĭ��λ1/2��� ��16��1/2Ϊ8λ������ѡ��
//    UARTFIFOLevelSet(UART2_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//�˺���Ϊѡ����ȵĺ�����Ŀǰѡ������Ϊ2/8 ��4λ���������ֱ�Ӱ�FIFOʧ�ܣ�disable���Ļ����Ϳ���һ���ַ�����һ���жϡ�
	
	//�����Ҫ�����ж�ֻ����  UART_INT_RX | UART_INT_TX
    ROM_UARTIntEnable(UART2_BASE, UART_INT_RX);								//���ڴ��ڽ����ж�ʹ�ܣ�
    ROM_IntPrioritySet(INT_UART2, 0x0);										// ����ָ����2����Ϊ�����жϣ�RXָ���գ� ���ȼ�λ0��� ��ѡ��0xE0-0x00
    UARTIntRegister(UART2_BASE, ROM_UART2IntHandler);						// ע���жϺ���	
    ROM_IntEnable(INT_UART2);												//�ж�ʹ��			
    ROM_IntMasterEnable();													//�ж�ʹ��			
}
void ROM_UART2IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART2_BASE, true);
	
    ROM_UARTIntClear(UART2_BASE, ui32IntStatus);//������жϱ�־λ
	//�˺������ж��Ƿ���ܵ��������з�����
	//�����жϽ���if��ifִ�����˳��ж�
	if ( !ROM_UARTCharsAvail(UART2_BASE))
	{
		
		return;
	}
	//�����жϽ���while
	while (ROM_UARTCharsAvail(UART2_BASE))
	{
		//�˺�����ָ���Ľ���FIFO�л�ȡһ���ַ�
		ui8RxBuffer = (uint8_t) ROM_UARTCharGetNonBlocking(UART2_BASE);
	}
	//�����жϺ������ݿ������������
	
}
/*************************************************************************
����3��һ���ʼ��           RX3->PC6          TX3->PC7
������Ϊ115200��8λ����λ��һ��ֹͣλ����У��λ��
FIFO���Ĭ�ϲ�ʹ�ܣ�������һ�����ݻ��߽���һ�����ݽ����жϣ�Ĭ�ϲ����������ж�
�ж����ȼ�Ĭ��λ0x0��������ȼ�
**************************************************************************/
void ROM_UART3_Init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);						// ���� UART ʹ�õ� GPIO ���衣
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);						// ʹ��UART3
	ROM_GPIOPinConfigure(GPIO_PC6_U3RX);									// ΪUARTģʽ����GPIO���š�
    ROM_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6);						// ����UART��Χ�豸ʹ�õ����š�
    ROM_GPIOPinConfigure(GPIO_PC7_U3TX);									// ΪUARTģʽ����GPIO���š�
    ROM_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_7);						// ����UART��Χ�豸ʹ�õ����š�
	// �˺���Ϊ��ʼ������һ������ΪUART�ڵ�ѡ����0--7�����ڶ�������Ϊϵͳʱ��
	// ����������Ϊ�����ʡ����漸��һ�㲻��Ϊ8������λ��һ��ֹͣλ����У��λ��
    ROM_UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//��ʹ��FIFO
	ROM_UARTFIFODisable(UART3_BASE);	
	//ʹ��FIFO
//    UARTFIFOEnable(UART3_BASE);											//ʹ��UART�Ļ�������Ĭ��λ1/2��� ��16��1/2Ϊ8λ������ѡ��
//    UARTFIFOLevelSet(UART3_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//�˺���Ϊѡ����ȵĺ�����Ŀǰѡ������Ϊ2/8 ��4λ���������ֱ�Ӱ�FIFOʧ�ܣ�disable���Ļ����Ϳ���һ���ַ�����һ���жϡ�
	
	//�����Ҫ�����ж�ֻ����   UART_INT_RX | UART_INT_TX
    ROM_UARTIntEnable(UART3_BASE, UART_INT_RX);								//���ڴ��ڽ����ж�ʹ�ܣ�
    ROM_IntPrioritySet(INT_UART3, 0x0);										// ����ָ����3����Ϊ�����жϣ�RXָ���գ� ���ȼ�λ0��� ��ѡ��0xE0-0x00
    UARTIntRegister(UART3_BASE, ROM_UART3IntHandler);						// ע���жϺ���	
    ROM_IntEnable(INT_UART3);												//�ж�ʹ��			
    ROM_IntMasterEnable();													//�ж�ʹ��			
}

//void ROM_UART3IntHandler(void)
//{
//    uint32_t ui32IntStatus;
//	uint8_t ui8RxBuffer;
//	
//    ui32IntStatus = UARTIntStatus(UART3_BASE, true);
//	
//    ROM_UARTIntClear(UART3_BASE, ui32IntStatus);//������жϱ�־λ
//	//�˺������ж��Ƿ���ܵ��������з�����
//	//�����жϽ���if��ifִ�����˳��ж�
//	if ( !ROM_UARTCharsAvail(UART3_BASE))
//	{
//		
//		return;
//	}
//	//�����жϽ���while
//	while (ROM_UARTCharsAvail(UART3_BASE))
//	{
//		//�˺�����ָ���Ľ���FIFO�л�ȡһ���ַ�
//		ui8RxBuffer = (uint8_t) ROM_UARTCharGetNonBlocking(UART3_BASE);
//	}
//	//�����жϺ������ݿ������������
//	
//}

/*************************************************************************
����4��һ���ʼ��           RX4->PC4          TX4->PC5
������Ϊ115200��8λ����λ��һ��ֹͣλ����У��λ��
FIFO���Ĭ�ϲ�ʹ�ܣ�������һ�����ݻ��߽���һ�����ݽ����жϣ�Ĭ�ϲ����������ж�
�ж����ȼ�Ĭ��λ0x0��������ȼ�
**************************************************************************/
void ROM_UART4_Init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);						// ���� UART ʹ�õ� GPIO ���衣
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);						// ʹ��UART4
	ROM_GPIOPinConfigure(GPIO_PC4_U4RX);									// ΪUARTģʽ����GPIO���š�
    ROM_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4);						// ����UART��Χ�豸ʹ�õ����š�
    ROM_GPIOPinConfigure(GPIO_PC5_U4TX);									// ΪUARTģʽ����GPIO���š�
    ROM_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_5);						// ����UART��Χ�豸ʹ�õ����š�
	// �˺���Ϊ��ʼ������һ������ΪUART�ڵ�ѡ����0--7�����ڶ�������Ϊϵͳʱ��
	// ����������Ϊ�����ʡ����漸��һ�㲻��Ϊ8������λ��һ��ֹͣλ����У��λ��
    ROM_UARTConfigSetExpClk(UART4_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//��ʹ��FIFO
	ROM_UARTFIFODisable(UART4_BASE);	
	//ʹ��FIFO
//    UARTFIFOEnable(UART4_BASE);											//ʹ��UART�Ļ�������Ĭ��λ1/2��� ��16��1/2Ϊ8λ������ѡ��
//    UARTFIFOLevelSet(UART4_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//�˺���Ϊѡ����ȵĺ�����Ŀǰѡ������Ϊ2/8 ��4λ���������ֱ�Ӱ�FIFOʧ�ܣ�disable���Ļ����Ϳ���һ���ַ�����һ���жϡ�
	
	//�����Ҫ�����ж�ֻ����   UART_INT_RX | UART_INT_TX
    ROM_UARTIntEnable(UART4_BASE, UART_INT_RX);								//���ڴ��ڽ����ж�ʹ�ܣ�
    ROM_IntPrioritySet(INT_UART4, 0x0);										// ����ָ����4����Ϊ�����жϣ�RXָ���գ� ���ȼ�λ0��� ��ѡ��0xE0-0x00
    UARTIntRegister(UART4_BASE, ROM_UART4IntHandler);						// ע���жϺ���	
    ROM_IntEnable(INT_UART4);												//�ж�ʹ��			
    ROM_IntMasterEnable();													//�ж�ʹ��			
}

void ROM_UART4IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART4_BASE, true);
	
    ROM_UARTIntClear(UART4_BASE, ui32IntStatus);//������жϱ�־λ
	//�˺������ж��Ƿ���ܵ��������з�����
	//�����жϽ���if��ifִ�����˳��ж�
	if ( !ROM_UARTCharsAvail(UART4_BASE))
	{
		
		return;
	}
	//�����жϽ���while
	while (ROM_UARTCharsAvail(UART4_BASE))
	{
		//�˺�����ָ���Ľ���FIFO�л�ȡһ���ַ�
		ui8RxBuffer = (uint8_t) ROM_UARTCharGetNonBlocking(UART4_BASE);
	}
	//�����жϺ������ݿ������������
	
}

/*************************************************************************
����5��һ���ʼ��           RX5->PE4          TX5->PE5
������Ϊ115200��8λ����λ��һ��ֹͣλ����У��λ��
FIFO���Ĭ�ϲ�ʹ�ܣ�������һ�����ݻ��߽���һ�����ݽ����жϣ�Ĭ�ϲ����������ж�
�ж����ȼ�Ĭ��λ0x0��������ȼ�
**************************************************************************/
void ROM_UART5_Init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);						// ���� UART ʹ�õ� GPIO ���衣
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);						// ʹ��UART5
	ROM_GPIOPinConfigure(GPIO_PE4_U5RX);									// ΪUARTģʽ����GPIO���š�
    ROM_GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4);						// ����UART��Χ�豸ʹ�õ����š�
    ROM_GPIOPinConfigure(GPIO_PE5_U5TX);									// ΪUARTģʽ����GPIO���š�
    ROM_GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_5);						// ����UART��Χ�豸ʹ�õ����š�
	// �˺���Ϊ��ʼ������һ������ΪUART�ڵ�ѡ����0--7�����ڶ�������Ϊϵͳʱ��
	// ����������Ϊ�����ʡ����漸��һ�㲻��Ϊ8������λ��һ��ֹͣλ����У��λ��
    ROM_UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//��ʹ��FIFO
	ROM_UARTFIFODisable(UART5_BASE);	
	//ʹ��FIFO
//    UARTFIFOEnable(UART5_BASE);											//ʹ��UART�Ļ�������Ĭ��λ1/2��� ��16��1/2Ϊ8λ������ѡ��
//    UARTFIFOLevelSet(UART5_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//�˺���Ϊѡ����ȵĺ�����Ŀǰѡ������Ϊ2/8 ��4λ���������ֱ�Ӱ�FIFOʧ�ܣ�disable���Ļ����Ϳ���һ���ַ�����һ���жϡ�
	
	//�����Ҫ�����ж�ֻ����   UART_INT_RX | UART_INT_TX
    ROM_UARTIntEnable(UART5_BASE, UART_INT_RX);								//���ڴ��ڽ����ж�ʹ�ܣ�
    ROM_IntPrioritySet(INT_UART5, 0x0);										// ����ָ����5����Ϊ�����жϣ�RXָ���գ� ���ȼ�λ0��� ��ѡ��0xE0-0x00
    UARTIntRegister(UART5_BASE, ROM_UART5IntHandler);						// ע���жϺ���	
    ROM_IntEnable(INT_UART5);												//�ж�ʹ��			
    ROM_IntMasterEnable();													//�ж�ʹ��			
}

//void ROM_UART5IntHandler(void)
//{
//    uint32_t ui32IntStatus;
//	uint8_t ui8RxBuffer;
//	
//    ui32IntStatus = UARTIntStatus(UART5_BASE, true);
//	
//    ROM_UARTIntClear(UART5_BASE, ui32IntStatus);//������жϱ�־λ
//	//�˺������ж��Ƿ���ܵ��������з�����
//	//�����жϽ���if��ifִ�����˳��ж�
//	if ( !ROM_UARTCharsAvail(UART5_BASE))
//	{
//		
//		return;
//	}
//	//�����жϽ���while
//	while (ROM_UARTCharsAvail(UART5_BASE))
//	{
//		//�˺�����ָ���Ľ���FIFO�л�ȡһ���ַ�
//		ui8RxBuffer = (uint8_t) ROM_UARTCharGetNonBlocking(UART5_BASE);
//	}
//	//�����жϺ������ݿ������������
//	
//}

/*************************************************************************
����6��һ���ʼ��           RX6->PD4          TX6->PD5
������Ϊ115200��8λ����λ��һ��ֹͣλ����У��λ��
FIFO���Ĭ�ϲ�ʹ�ܣ�������һ�����ݻ��߽���һ�����ݽ����жϣ�Ĭ�ϲ����������ж�
�ж����ȼ�Ĭ��λ0x0��������ȼ�
**************************************************************************/
void ROM_UART6_Init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);						// ���� UART ʹ�õ� GPIO ���衣
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART6);						// ʹ��UART6
	ROM_GPIOPinConfigure(GPIO_PD4_U6RX);									// ΪUARTģʽ����GPIO���š�
    ROM_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4);						// ����UART��Χ�豸ʹ�õ����š�
    ROM_GPIOPinConfigure(GPIO_PD5_U6TX);									// ΪUARTģʽ����GPIO���š�
    ROM_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_5);						// ����UART��Χ�豸ʹ�õ����š�
	// �˺���Ϊ��ʼ������һ������ΪUART�ڵ�ѡ����0--7�����ڶ�������Ϊϵͳʱ��
	// ����������Ϊ�����ʡ����漸��һ�㲻��Ϊ8������λ��һ��ֹͣλ����У��λ��
    ROM_UARTConfigSetExpClk(UART6_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//��ʹ��FIFO
	ROM_UARTFIFODisable(UART6_BASE);	
	//ʹ��FIFO
//    UARTFIFOEnable(UART6_BASE);											//ʹ��UART�Ļ�������Ĭ��λ1/2��� ��16��1/2Ϊ8λ������ѡ��
//    UARTFIFOLevelSet(UART6_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//�˺���Ϊѡ����ȵĺ�����Ŀǰѡ������Ϊ2/8 ��4λ���������ֱ�Ӱ�FIFOʧ�ܣ�disable���Ļ����Ϳ���һ���ַ�����һ���жϡ�
	
	//�����Ҫ�����ж�ֻ����   UART_INT_RX | UART_INT_TX
    ROM_UARTIntEnable(UART6_BASE, UART_INT_RX);								//���ڴ��ڽ����ж�ʹ�ܣ�
    ROM_IntPrioritySet(INT_UART6, 0x0);										// ����ָ����6����Ϊ�����жϣ�RXָ���գ� ���ȼ�λ0��� ��ѡ��0xE0-0x00
    UARTIntRegister(UART6_BASE, ROM_UART6IntHandler);						// ע���жϺ���	
    ROM_IntEnable(INT_UART6);												//�ж�ʹ��			
    ROM_IntMasterEnable();													//�ж�ʹ��			
}

//void ROM_UART6IntHandler(void)
//{
//    uint32_t ui32IntStatus;
//	uint8_t ui8RxBuffer;
//	
//    ui32IntStatus = UARTIntStatus(UART6_BASE, true);
//	
//    ROM_UARTIntClear(UART6_BASE, ui32IntStatus);//������жϱ�־λ
//	//�˺������ж��Ƿ���ܵ��������з�����
//	//�����жϽ���if��ifִ�����˳��ж�
//	if ( !ROM_UARTCharsAvail(UART6_BASE))
//	{
//		
//		return;
//	}
//	//�����жϽ���while
//	while (ROM_UARTCharsAvail(UART6_BASE))
//	{
//		//�˺�����ָ���Ľ���FIFO�л�ȡһ���ַ�
//		ui8RxBuffer = (uint8_t) ROM_UARTCharGetNonBlocking(UART6_BASE);
//	}
//	//�����жϺ������ݿ������������
//	
//}

/*************************************************************************
����7��һ���ʼ��           RX7->PE0          TX7->PE1
������Ϊ115200��8λ����λ��һ��ֹͣλ����У��λ��
FIFO���Ĭ�ϲ�ʹ�ܣ�������һ�����ݻ��߽���һ�����ݽ����жϣ�Ĭ�ϲ����������ж�
�ж����ȼ�Ĭ��λ0x0��������ȼ�
**************************************************************************/
void ROM_UART7_Init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);						// ���� UART ʹ�õ� GPIO ���衣
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);						// ʹ��UART7
	ROM_GPIOPinConfigure(GPIO_PE0_U7RX);									// ΪUARTģʽ����GPIO���š�
    ROM_GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0);						// ����UART��Χ�豸ʹ�õ����š�
    ROM_GPIOPinConfigure(GPIO_PE1_U7TX);									// ΪUARTģʽ����GPIO���š�
    ROM_GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_1);						// ����UART��Χ�豸ʹ�õ����š�
	// �˺���Ϊ��ʼ������һ������ΪUART�ڵ�ѡ����0--7�����ڶ�������Ϊϵͳʱ��
	// ����������Ϊ�����ʡ����漸��һ�㲻��Ϊ8������λ��һ��ֹͣλ����У��λ��
    ROM_UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//��ʹ��FIFO
	ROM_UARTFIFODisable(UART7_BASE);	
	//ʹ��FIFO
//    UARTFIFOEnable(UART7_BASE);											//ʹ��UART�Ļ�������Ĭ��λ1/2��� ��16��1/2Ϊ8λ������ѡ��
//    UARTFIFOLevelSet(UART7_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//�˺���Ϊѡ����ȵĺ�����Ŀǰѡ������Ϊ2/8 ��4λ���������ֱ�Ӱ�FIFOʧ�ܣ�disable���Ļ����Ϳ���һ���ַ�����һ���жϡ�
	
	//�����Ҫ�����ж�ֻ����   UART_INT_RX | UART_INT_TX
    ROM_UARTIntEnable(UART7_BASE, UART_INT_RX);								//���ڴ��ڽ����ж�ʹ�ܣ�
    ROM_IntPrioritySet(INT_UART7, 0x0);										// ����ָ����7����Ϊ�����жϣ�RXָ���գ� ���ȼ�λ0��� ��ѡ��0xE0-0x00
    UARTIntRegister(UART7_BASE, ROM_UART7IntHandler);						// ע���жϺ���	
    ROM_IntEnable(INT_UART7);												//�ж�ʹ��			
    ROM_IntMasterEnable();													//�ж�ʹ��			
}
void ROM_UART7IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART7_BASE, true);
	
    ROM_UARTIntClear(UART7_BASE, ui32IntStatus);//������жϱ�־λ
	//�˺������ж��Ƿ���ܵ��������з�����
	//�����жϽ���if��ifִ�����˳��ж�
	if ( !ROM_UARTCharsAvail(UART7_BASE))
	{
		
		return;
	}
	//�����жϽ���while
	while (ROM_UARTCharsAvail(UART7_BASE))
	{
		//�˺�����ָ���Ľ���FIFO�л�ȡһ���ַ�
		ui8RxBuffer = (uint8_t) ROM_UARTCharGetNonBlocking(UART7_BASE);
	}
	//�����жϺ������ݿ������������
	
}


//***********************************************************************************************************************************************
void UART0_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);						// ���� UART ʹ�õ� GPIO ���衣
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);						// ʹ��UART0
	GPIOPinConfigure(GPIO_PA0_U0RX);									// ΪUARTģʽ����GPIO���š�
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0);						// ����UART��Χ�豸ʹ�õ����š�
    GPIOPinConfigure(GPIO_PA1_U0TX);									// ΪUARTģʽ����GPIO���š�
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_1);						// ����UART��Χ�豸ʹ�õ����š�
	// �˺���Ϊ��ʼ������һ������ΪUART�ڵ�ѡ����0--7�����ڶ�������Ϊϵͳʱ��
	// ����������Ϊ�����ʡ����漸��һ�㲻��Ϊ8������λ��һ��ֹͣλ����У��λ��
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//��ʹ��FIFO
	UARTFIFODisable(UART0_BASE);	
	//ʹ��FIFO
//    UARTFIFOEnable(UART0_BASE);											//ʹ��UART�Ļ�������Ĭ��λ1/2��� ��16��1/2Ϊ8λ������ѡ��
//    UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//�˺���Ϊѡ����ȵĺ�����Ŀǰѡ������Ϊ2/8 ��4λ���������ֱ�Ӱ�FIFOʧ�ܣ�disable���Ļ����Ϳ���һ���ַ�����һ���жϡ�
	
	//�����Ҫ�����ж�ֻ����   UART_INT_RX||UART_INT_TX
    UARTIntEnable(UART0_BASE, UART_INT_RX);								//���ڴ��ڽ����ж�ʹ�ܣ�
    IntPrioritySet(INT_UART0, 0x0);										// ����ָ����0����Ϊ�����жϣ�RXָ���գ� ���ȼ�λ0��� ��ѡ��0xE0-0x00
    UARTIntRegister(UART0_BASE, UART0IntHandler);						// ע���жϺ���	
    IntEnable(INT_UART0);												//�ж�ʹ��			
    IntMasterEnable();													//�ж�ʹ��			
}

void UART1_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);						//���� UART ʹ�õ� GPIO ���衣
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);						// ʹ��UART1
	GPIOPinConfigure(GPIO_PB0_U1RX);									//ΪUARTģʽ����GPIO���š�
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0);						//����UART��Χ�豸ʹ�õ����š�
    GPIOPinConfigure(GPIO_PB1_U1TX);									//ΪUARTģʽ����GPIO���š�
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_1);						//����UART��Χ�豸ʹ�õ����š�
	//�˺���Ϊ��ʼ������һ������ΪUART�ڵ�ѡ����0--7�����ڶ�������Ϊϵͳʱ��
	//����������Ϊ�����ʡ����漸��һ�㲻��Ϊ8������λ��һ��ֹͣλ����У��λ��
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//��ʹ��FIFO
	UARTFIFODisable(UART1_BASE);
	//ʹ��FIFO
//    UARTFIFOEnable(UART1_BASE);											//ʹ��UART�Ļ�������Ĭ��λ1/2��� ��16��1/2Ϊ8λ������ѡ��
//    UARTFIFOLevelSet(UART1_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//�˺���Ϊѡ����ȵĺ�����Ŀǰѡ������Ϊ2/8 ��4λ���������ֱ�Ӱ�FIFOʧ�ܣ�disable���Ļ����Ϳ���һ���ַ�����һ���жϡ�
	
	//�����Ҫ�����ж�ֻ����   UART_INT_RX | UART_INT_TX
    UARTIntEnable(UART1_BASE, UART_INT_RX);								//���ڴ��ڽ����ж�ʹ��
	
    IntPrioritySet(INT_UART1, 0x0);										// ����ָ����1����Ϊ�����жϣ�RXָ���գ� ���ȼ�λ0��� ��ѡ��0xE0-0x00
    UARTIntRegister(UART1_BASE, UART1IntHandler);						// ע���жϺ���	
    IntEnable(INT_UART1);												//�ж�ʹ��			
    IntMasterEnable();													//�ж�ʹ��			
}


void UART2_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);						// ���� UART ʹ�õ� GPIO ���衣
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);						// ʹ��UART2
	GPIOPinConfigure(GPIO_PD6_U2RX);									// ΪUARTģʽ����GPIO���š�
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6);						// ����UART��Χ�豸ʹ�õ����š�
    GPIOPinConfigure(GPIO_PD7_U2TX);									// ΪUARTģʽ����GPIO���š�
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_7);						// ����UART��Χ�豸ʹ�õ����š�
	// �˺���Ϊ��ʼ������һ������ΪUART�ڵ�ѡ����0--7�����ڶ�������Ϊϵͳʱ��
	// ����������Ϊ�����ʡ����漸��һ�㲻��Ϊ8������λ��һ��ֹͣλ����У��λ��
    UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//��ʹ��FIFO
	UARTFIFODisable(UART2_BASE);	
	//ʹ��FIFO
//    UARTFIFOEnable(UART2_BASE);											//ʹ��UART�Ļ�������Ĭ��λ1/2��� ��16��1/2Ϊ8λ������ѡ��
//    UARTFIFOLevelSet(UART2_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//�˺���Ϊѡ����ȵĺ�����Ŀǰѡ������Ϊ2/8 ��4λ���������ֱ�Ӱ�FIFOʧ�ܣ�disable���Ļ����Ϳ���һ���ַ�����һ���жϡ�
	
	//�����Ҫ�����ж�ֻ����   UART_INT_RX||UART_INT_TX
    UARTIntEnable(UART2_BASE, UART_INT_RX);								//���ڴ��ڽ����ж�ʹ�ܣ�
    IntPrioritySet(INT_UART2, 0x0);										// ����ָ����2����Ϊ�����жϣ�RXָ���գ� ���ȼ�λ0��� ��ѡ��0xE0-0x00
    UARTIntRegister(UART2_BASE, UART2IntHandler);						// ע���жϺ���	
    IntEnable(INT_UART2);												//�ж�ʹ��			
    IntMasterEnable();													//�ж�ʹ��			
}


void UART3_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);						// ���� UART ʹ�õ� GPIO ���衣
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);						// ʹ��UART3
	GPIOPinConfigure(GPIO_PC6_U3RX);									// ΪUARTģʽ����GPIO���š�
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6);						// ����UART��Χ�豸ʹ�õ����š�
    GPIOPinConfigure(GPIO_PC7_U3TX);									// ΪUARTģʽ����GPIO���š�
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_7);						// ����UART��Χ�豸ʹ�õ����š�
	// �˺���Ϊ��ʼ������һ������ΪUART�ڵ�ѡ����0--7�����ڶ�������Ϊϵͳʱ��
	// ����������Ϊ�����ʡ����漸��һ�㲻��Ϊ8������λ��һ��ֹͣλ����У��λ��
    UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), 9600,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//��ʹ��FIFO
	UARTFIFODisable(UART3_BASE);	
	//ʹ��FIFO
//    UARTFIFOEnable(UART3_BASE);											//ʹ��UART�Ļ�������Ĭ��λ1/2��� ��16��1/2Ϊ8λ������ѡ��
//    UARTFIFOLevelSet(UART3_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//�˺���Ϊѡ����ȵĺ�����Ŀǰѡ������Ϊ2/8 ��4λ���������ֱ�Ӱ�FIFOʧ�ܣ�disable���Ļ����Ϳ���һ���ַ�����һ���жϡ�
	
	//�����Ҫ�����ж�ֻ����   UART_INT_RX||UART_INT_TX
    UARTIntEnable(UART3_BASE, UART_INT_RX);								//���ڴ��ڽ����ж�ʹ�ܣ�
    IntPrioritySet(INT_UART3, 0x0);										// ����ָ����3����Ϊ�����жϣ�RXָ���գ� ���ȼ�λ0��� ��ѡ��0xE0-0x00
    UARTIntRegister(UART3_BASE, UART0IntHandler);						// ע���жϺ���	
    IntEnable(INT_UART3);												//�ж�ʹ��			
    IntMasterEnable();													//�ж�ʹ��			
}

void UART4_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);						// ���� UART ʹ�õ� GPIO ���衣
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);						// ʹ��UART4
	GPIOPinConfigure(GPIO_PC4_U4RX);									// ΪUARTģʽ����GPIO���š�
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4);						// ����UART��Χ�豸ʹ�õ����š�
    GPIOPinConfigure(GPIO_PC5_U4TX);									// ΪUARTģʽ����GPIO���š�
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_5);						// ����UART��Χ�豸ʹ�õ����š�
	// �˺���Ϊ��ʼ������һ������ΪUART�ڵ�ѡ����0--7�����ڶ�������Ϊϵͳʱ��
	// ����������Ϊ�����ʡ����漸��һ�㲻��Ϊ8������λ��һ��ֹͣλ����У��λ��
    UARTConfigSetExpClk(UART4_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//��ʹ��FIFO
	UARTFIFODisable(UART4_BASE);	
	//ʹ��FIFO
//    UARTFIFOEnable(UART4_BASE);											//ʹ��UART�Ļ�������Ĭ��λ1/2��� ��16��1/2Ϊ8λ������ѡ��
//    UARTFIFOLevelSet(UART4_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//�˺���Ϊѡ����ȵĺ�����Ŀǰѡ������Ϊ2/8 ��4λ���������ֱ�Ӱ�FIFOʧ�ܣ�disable���Ļ����Ϳ���һ���ַ�����һ���жϡ�
	
	//�����Ҫ�����ж�ֻ����   UART_INT_RX||UART_INT_TX
    UARTIntEnable(UART4_BASE, UART_INT_RX);								//���ڴ��ڽ����ж�ʹ�ܣ�
    IntPrioritySet(INT_UART4, 0x0);										// ����ָ����4����Ϊ�����жϣ�RXָ���գ� ���ȼ�λ0��� ��ѡ��0xE0-0x00
    UARTIntRegister(UART4_BASE, UART0IntHandler);						// ע���жϺ���	
    IntEnable(INT_UART4);												//�ж�ʹ��			
    IntMasterEnable();													//�ж�ʹ��			
}

void UART5_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);						// ���� UART ʹ�õ� GPIO ���衣
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);						// ʹ��UART5
	GPIOPinConfigure(GPIO_PE4_U5RX);									// ΪUARTģʽ����GPIO���š�
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4);						// ����UART��Χ�豸ʹ�õ����š�
    GPIOPinConfigure(GPIO_PE5_U5TX);									// ΪUARTģʽ����GPIO���š�
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_5);						// ����UART��Χ�豸ʹ�õ����š�
	// �˺���Ϊ��ʼ������һ������ΪUART�ڵ�ѡ����0--7�����ڶ�������Ϊϵͳʱ��
	// ����������Ϊ�����ʡ����漸��һ�㲻��Ϊ8������λ��һ��ֹͣλ����У��λ��
    UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//��ʹ��FIFO
	UARTFIFODisable(UART5_BASE);	
	//ʹ��FIFO
//    UARTFIFOEnable(UART5_BASE);											//ʹ��UART�Ļ�������Ĭ��λ1/2��� ��16��1/2Ϊ8λ������ѡ��
//    UARTFIFOLevelSet(UART5_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//�˺���Ϊѡ����ȵĺ�����Ŀǰѡ������Ϊ2/8 ��4λ���������ֱ�Ӱ�FIFOʧ�ܣ�disable���Ļ����Ϳ���һ���ַ�����һ���жϡ�
	
	//�����Ҫ�����ж�ֻ����   UART_INT_RX||UART_INT_TX
    UARTIntEnable(UART5_BASE, UART_INT_RX);								//���ڴ��ڽ����ж�ʹ�ܣ�
    IntPrioritySet(INT_UART5, 0x0);										// ����ָ����5����Ϊ�����жϣ�RXָ���գ� ���ȼ�λ0��� ��ѡ��0xE0-0x00
    UARTIntRegister(UART5_BASE, UART0IntHandler);						// ע���жϺ���	
    IntEnable(INT_UART5);												//�ж�ʹ��			
    IntMasterEnable();													//�ж�ʹ��			
}

void UART6_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);						// ���� UART ʹ�õ� GPIO ���衣
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART6);						// ʹ��UART6
	GPIOPinConfigure(GPIO_PD4_U6RX);									// ΪUARTģʽ����GPIO���š�
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4);						// ����UART��Χ�豸ʹ�õ����š�
    GPIOPinConfigure(GPIO_PD5_U6TX);									// ΪUARTģʽ����GPIO���š�
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_5);						// ����UART��Χ�豸ʹ�õ����š�
	// �˺���Ϊ��ʼ������һ������ΪUART�ڵ�ѡ����0--7�����ڶ�������Ϊϵͳʱ��
	// ����������Ϊ�����ʡ����漸��һ�㲻��Ϊ8������λ��һ��ֹͣλ����У��λ��
    UARTConfigSetExpClk(UART6_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//��ʹ��FIFO
	UARTFIFODisable(UART6_BASE);	
	//ʹ��FIFO
//    UARTFIFOEnable(UART6_BASE);											//ʹ��UART�Ļ�������Ĭ��λ1/2��� ��16��1/2Ϊ8λ������ѡ��
//    UARTFIFOLevelSet(UART6_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//�˺���Ϊѡ����ȵĺ�����Ŀǰѡ������Ϊ2/8 ��4λ���������ֱ�Ӱ�FIFOʧ�ܣ�disable���Ļ����Ϳ���һ���ַ�����һ���жϡ�
	
	//�����Ҫ�����ж�ֻ����   UART_INT_RX||UART_INT_TX
    UARTIntEnable(UART6_BASE, UART_INT_RX);								//���ڴ��ڽ����ж�ʹ�ܣ�
    IntPrioritySet(INT_UART6, 0x0);										// ����ָ����6����Ϊ�����жϣ�RXָ���գ� ���ȼ�λ0��� ��ѡ��0xE0-0x00
    UARTIntRegister(UART6_BASE, UART0IntHandler);						// ע���жϺ���	
    IntEnable(INT_UART6);												//�ж�ʹ��			
    IntMasterEnable();													//�ж�ʹ��			
}

void UART7_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);						// ���� UART ʹ�õ� GPIO ���衣
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);						// ʹ��UART7
	GPIOPinConfigure(GPIO_PE0_U7RX);									// ΪUARTģʽ����GPIO���š�
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0);						// ����UART��Χ�豸ʹ�õ����š�
    GPIOPinConfigure(GPIO_PE1_U7TX);									// ΪUARTģʽ����GPIO���š�
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_1);						// ����UART��Χ�豸ʹ�õ����š�
	// �˺���Ϊ��ʼ������һ������ΪUART�ڵ�ѡ����0--7�����ڶ�������Ϊϵͳʱ��
	// ����������Ϊ�����ʡ����漸��һ�㲻��Ϊ8������λ��һ��ֹͣλ����У��λ��
    UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	//��ʹ��FIFO
	UARTFIFODisable(UART7_BASE);	
	//ʹ��FIFO
//    UARTFIFOEnable(UART7_BASE);											//ʹ��UART�Ļ�������Ĭ��λ1/2��� ��16��1/2Ϊ8λ������ѡ��
//    UARTFIFOLevelSet(UART7_BASE, UART_FIFO_TX2_8, UART_FIFO_RX2_8);		//�˺���Ϊѡ����ȵĺ�����Ŀǰѡ������Ϊ2/8 ��4λ���������ֱ�Ӱ�FIFOʧ�ܣ�disable���Ļ����Ϳ���һ���ַ�����һ���жϡ�
	
	//�����Ҫ�����ж�ֻ����   UART_INT_RX||UART_INT_TX
    UARTIntEnable(UART7_BASE, UART_INT_RX);								//���ڴ��ڽ����ж�ʹ�ܣ�
    IntPrioritySet(INT_UART7, 0x0);										// ����ָ����7����Ϊ�����жϣ�RXָ���գ� ���ȼ�λ0��� ��ѡ��0xE0-0x00
    UARTIntRegister(UART7_BASE, UART0IntHandler);						// ע���жϺ���	
    IntEnable(INT_UART7);												//�ж�ʹ��			
    IntMasterEnable();													//�ж�ʹ��			
}

void UART0IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART0_BASE, true);
	
    UARTIntClear(UART0_BASE, ui32IntStatus);//������жϱ�־λ
	//�˺������ж��Ƿ���ܵ��������з�����
	//�����жϽ���if��ifִ�����˳��ж�
	if ( !UARTCharsAvail(UART0_BASE))
	{
		
		return;
	}
	//�����жϽ���while
	while (UARTCharsAvail(UART0_BASE))
	{
		//�˺�����ָ���Ľ���FIFO�л�ȡһ���ַ�
		ui8RxBuffer = (uint8_t) UARTCharGetNonBlocking(UART0_BASE);
	}
	//�����жϺ������ݿ������������
	
	
}

void UART1IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART1_BASE, true);
	
    UARTIntClear(UART1_BASE, ui32IntStatus);//������жϱ�־λ
	//�˺������ж��Ƿ���ܵ��������з�����
	//�����жϽ���if��ifִ�����˳��ж�
	if ( !UARTCharsAvail(UART1_BASE))
	{
		
		return;
	}
	//�����жϽ���while
	while (UARTCharsAvail(UART1_BASE))
	{
		//�˺�����ָ���Ľ���FIFO�л�ȡһ���ַ�
		ui8RxBuffer = (uint8_t) UARTCharGetNonBlocking(UART1_BASE);
	}
	//�����жϺ������ݿ������������
	
	
}



void UART2IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART2_BASE, true);
	
    UARTIntClear(UART2_BASE, ui32IntStatus);//������жϱ�־λ
	//�˺������ж��Ƿ���ܵ��������з�����
	//�����жϽ���if��ifִ�����˳��ж�
	if ( !UARTCharsAvail(UART2_BASE))
	{
		
		return;
	}
	//�����жϽ���while
	while (UARTCharsAvail(UART2_BASE))
	{
		//�˺�����ָ���Ľ���FIFO�л�ȡһ���ַ�
		ui8RxBuffer = (uint8_t) UARTCharGetNonBlocking(UART2_BASE);
	}
	//�����жϺ������ݿ������������
	
	
}



void UART3IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART3_BASE, true);
	
    UARTIntClear(UART3_BASE, ui32IntStatus);//������жϱ�־λ
	//�˺������ж��Ƿ���ܵ��������з�����
	//�����жϽ���if��ifִ�����˳��ж�
	if ( !UARTCharsAvail(UART3_BASE))
	{
		
		return;
	}
	//�����жϽ���while
	while (UARTCharsAvail(UART3_BASE))
	{
		//�˺�����ָ���Ľ���FIFO�л�ȡһ���ַ�
		ui8RxBuffer = (uint8_t) UARTCharGetNonBlocking(UART3_BASE);
	}
	//�����жϺ������ݿ������������
	
	
}



void UART4IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART4_BASE, true);
	
    UARTIntClear(UART4_BASE, ui32IntStatus);//������жϱ�־λ
	//�˺������ж��Ƿ���ܵ��������з�����
	//�����жϽ���if��ifִ�����˳��ж�
	if ( !UARTCharsAvail(UART4_BASE))
	{
		
		return;
	}
	//�����жϽ���while
	while (UARTCharsAvail(UART4_BASE))
	{
		//�˺�����ָ���Ľ���FIFO�л�ȡһ���ַ�
		ui8RxBuffer = (uint8_t) UARTCharGetNonBlocking(UART4_BASE);
	}
	//�����жϺ������ݿ������������
	
	
}



void UART5IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART5_BASE, true);
	
    UARTIntClear(UART5_BASE, ui32IntStatus);//������жϱ�־λ
	//�˺������ж��Ƿ���ܵ��������з�����
	//�����жϽ���if��ifִ�����˳��ж�
	if ( !UARTCharsAvail(UART5_BASE))
	{
		
		return;
	}
	//�����жϽ���while
	while (UARTCharsAvail(UART5_BASE))
	{
		//�˺�����ָ���Ľ���FIFO�л�ȡһ���ַ�
		ui8RxBuffer = (uint8_t) UARTCharGetNonBlocking(UART5_BASE);
	}
	//�����жϺ������ݿ������������
	
	
}



void UART6IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART6_BASE, true);
	
    UARTIntClear(UART6_BASE, ui32IntStatus);//������жϱ�־λ
	//�˺������ж��Ƿ���ܵ��������з�����
	//�����жϽ���if��ifִ�����˳��ж�
	if ( !UARTCharsAvail(UART6_BASE))
	{
		
		return;
	}
	//�����жϽ���while
	while (UARTCharsAvail(UART6_BASE))
	{
		//�˺�����ָ���Ľ���FIFO�л�ȡһ���ַ�
		ui8RxBuffer = (uint8_t) UARTCharGetNonBlocking(UART6_BASE);
	}
	//�����жϺ������ݿ������������
	
	
}



void UART7IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
	
    ui32IntStatus = UARTIntStatus(UART7_BASE, true);
	
    UARTIntClear(UART7_BASE, ui32IntStatus);//������жϱ�־λ
	//�˺������ж��Ƿ���ܵ��������з�����
	//�����жϽ���if��ifִ�����˳��ж�
	if ( !UARTCharsAvail(UART7_BASE))
	{
		
		return;
	}
	//�����жϽ���while
	while (UARTCharsAvail(UART7_BASE))
	{
		//�˺�����ָ���Ľ���FIFO�л�ȡһ���ַ�
		ui8RxBuffer = (uint8_t) UARTCharGetNonBlocking(UART7_BASE);
	}
	//�����жϺ������ݿ������������
	
	
}
