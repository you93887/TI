#ifndef __SERIAL_H
#define __SERIAL_H

//串口的中断函数
void ROM_UART0IntHandler(void);
void ROM_UART1IntHandler(void);
void ROM_UART2IntHandler(void);
void ROM_UART3IntHandler(void);
void ROM_UART4IntHandler(void);
void ROM_UART5IntHandler(void);
void ROM_UART6IntHandler(void);
void ROM_UART7IntHandler(void);
//串口初始化函数
void ROM_UART0_Init(void);
void ROM_UART1_Init(void);
void ROM_UART2_Init(void);
void ROM_UART3_Init(void);
void ROM_UART4_Init(void);
void ROM_UART5_Init(void);
void ROM_UART6_Init(void);
void ROM_UART7_Init(void);

void Serial_SendByte(uint8_t uart,uint8_t Byte); 		//串口发送一个字节
void Serial_SendArray(uint8_t uart, uint8_t *Array, uint16_t Length); //串口发送一个数组
void Serial_SendString(uint8_t uart, char *String);   //串口发送一个字符串
uint32_t Serial_Pow(uint32_t X, uint32_t Y);   //次方函数（内部使用）
void Serial_SendNumber(uint8_t uart, uint32_t Number, uint8_t Length);   //串口发送数字

void UART0IntHandler(void);
void UART1IntHandler(void);
void UART2IntHandler(void);
void UART3IntHandler(void);
void UART4IntHandler(void);
void UART5IntHandler(void);
void UART6IntHandler(void);
void UART7IntHandler(void);
void UART0_Init(void);
void UART1_Init(void);
void UART2_Init(void);
void UART3_Init(void);
void UART4_Init(void);
void UART5_Init(void);
void UART6_Init(void);
void UART7_Init(void);

#endif
