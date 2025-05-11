/***************************************************
 * @brief   蓝牙串口初始化函数
 *          以及对应的中断服务函数定义。
 *          以及一些封装好的发送字节，字符串，数字等的函数。
 *		
 *      蓝牙使用串口3，直接插在开发板上，蓝牙工作电压是3.3
 *
 *			RX3->PC6          TX3->PC7
 *			
 *
 *      默认都是波特率为115200，8位数据位，一个停止位，无校验位。
 *      FIFO深度默认不使能，即发送一个数据或者接受一个数据进入中断，默认不开启发送中断
 *      中断优先级默认位0x0即最高优先级
 *			默认用带ROM的，不带ROM的在本文件的最后。
 *
 *      例：发送一个4为数据1234    BLE_Send(1234,4);
 *          用变量读取接收的数据   bianliang = BLE_GetNumber();
 *
 *			备注：改变串口需要把serial.c文件的串口中断函数注释掉，将蓝牙串口中断优先级修改为0x10
 *						修改 以下函数的数字
 *								 void BLEUART_Init(void)
 *								 void BLE_Send(uint32_t number,uint8_t BLE_Sendlength)
 *								 void ROM_UART6IntHandler(void)
 *
 * @date    2024/6/30
 * @author  Lei Wang
****************************************************/


#include "include.h"

uint8_t Serial_RxFlag;					//定义接收数据包标志位

uint8_t Serial_RxPacket[Serial_Getlength];
uint32_t UARTnumber;


/*************************************************************************
*  蓝牙串口初始化           
*    波特率为115200，8位数据位，一个停止位，无校验位。
*    FIFO深度默认不使能，即发送一个数据或者接受一个数据进入中断，默认不开启发送中断
*    中断优先级默认位0x0即最高优先级
***************************************************************************/
void BLEUART_Init(void)
{
	//初始化串口1
	ROM_UART1_Init();
	
}



/**
  * 函    数：串口接收数字转化
  * 参    数： 
  * 参    数：Array 要转化的数字字符数组
  * 参    数：Length 要转化数字字符的长度
  * 备		注：将蓝牙串口接收的字符形式数组转化为对应接收的数据
  */
uint32_t Serial_NumberChange(uint8_t *Array, uint8_t Length)
{
	uint32_t Get_Number;
	uint8_t i;
	for (i = 0; i < Length; i ++)		//根据数字长度遍历数字的每一位
	{
		Get_Number+=(Array[i] - '0') *Serial_Pow(10, Length - i - 1) ;
	}
	return Get_Number;
}

/**
  * 函    数：串口发送一个数据
  * 参    数：number 需要发送的数据        
  * 参    数：BLE_Sendlength 数据的长度            
  * 返 回 值：无
  * 备		注：0x28和0x29是“（”和“）”自定义的包头和包尾，3表示串口3
  */

void BLE_Send(uint32_t number,uint8_t BLE_Sendlength)
{
	uint32_t data[BLE_Sendlength];
	
	//（number）
	Serial_SendByte(1, 0x28);
	Serial_SendNumber(1,number,BLE_Sendlength);
	Serial_SendByte(1, 0x29);
	
}

/*************************************************************************
*  蓝牙串口接收中断函数           
*    
* 备	注：波特率为115200，8位数据位，一个停止位，无校验位
*    
* 			  接收数据的长度在头文件修改宏定义
*
*					接收的数据可以直接使用
*    
***************************************************************************/

//void ROM_UART1IntHandler(void)
//{
//	static uint8_t RxState = 0;		//定义表示当前状态机状态的静态变量
//	static uint8_t pRxPacket = 0;	//定义表示当前接收数据位置的静态变量
//	
//	//static uint8_t messageLength = 0;
//	
//	uint8_t rxBuf;
//	
//  uint32_t ui32IntStatus;

//	
//  ui32IntStatus = UARTIntStatus(UART1_BASE, true);
//	
//  ROM_UARTIntClear(UART1_BASE, ui32IntStatus);//先清除中断标志位
//	//此函数是判断是否接受到东西，有返回真
//	//发送中断进入if，if执行完退出中断
//	if (!ROM_UARTCharsAvail(UART1_BASE))
//	{
//		return;
//	}
//	//接受中断进入while
//	while (ROM_UARTCharsAvail(UART1_BASE))
//	{
//		rxBuf = ROM_UARTCharGetNonBlocking(UART1_BASE);
//		
//		/*当前状态为0，接收数据包包头*/
//		if (RxState == 0)
//		{
//			if (rxBuf == 0x28)			//如果数据确实是包头
//			{
//				RxState = 1;			//置下一个状态
//				pRxPacket = 0;			//数据包的位置归零
//			}
//		}
//		/*当前状态为1，接收数据包数据*/

//		else if (RxState == 1)
//		{
//			Serial_RxPacket[pRxPacket] = rxBuf;	//将数据存入数据包数组的指定位置
//			pRxPacket ++;				//数据包的位置自增
//			if (pRxPacket >= Serial_Getlength)			//如果收够4个数据
//			{
//				RxState = 2;			//置下一个状态
//			}
//		}
//		/*当前状态为2，接收数据包包尾*/
//		else if (RxState == 2)
//		{
//			if (rxBuf == 0x29)			//如果数据确实是包尾部
//			{
//				RxState = 0;			//状态归0
//				Serial_RxFlag = 1;		//接收数据包标志位置1，成功接收一个数据包
//				
//			}
//		}
//	
//		if(Serial_RxFlag == 1)
//		{
//			Serial_RxFlag =0;
//			//将接收的字符数组求和转化为数据，可直接使用
//			UARTnumber = Serial_NumberChange(Serial_RxPacket,pRxPacket); 
//		}
//							
//	}
//	
//}

/**
  * 函    数：蓝牙串口获取数据
  * 
  * 备		注：返回接收的数据
  *           默认4位数据，数据长度在对应头文件修改宏定义：Serial_Getlength
  */
uint32_t BLE_GetNumber()
{
	return UARTnumber;
}
