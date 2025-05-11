/***************************************************
 * @brief   �������ڳ�ʼ������
 *          �Լ���Ӧ���жϷ��������塣
 *          �Լ�һЩ��װ�õķ����ֽڣ��ַ��������ֵȵĺ�����
 *		
 *      ����ʹ�ô���3��ֱ�Ӳ��ڿ������ϣ�����������ѹ��3.3
 *
 *			RX3->PC6          TX3->PC7
 *			
 *
 *      Ĭ�϶��ǲ�����Ϊ115200��8λ����λ��һ��ֹͣλ����У��λ��
 *      FIFO���Ĭ�ϲ�ʹ�ܣ�������һ�����ݻ��߽���һ�����ݽ����жϣ�Ĭ�ϲ����������ж�
 *      �ж����ȼ�Ĭ��λ0x0��������ȼ�
 *			Ĭ���ô�ROM�ģ�����ROM���ڱ��ļ������
 *
 *      ��������һ��4Ϊ����1234    BLE_Send(1234,4);
 *          �ñ�����ȡ���յ�����   bianliang = BLE_GetNumber();
 *
 *			��ע���ı䴮����Ҫ��serial.c�ļ��Ĵ����жϺ���ע�͵��������������ж����ȼ��޸�Ϊ0x10
 *						�޸� ���º���������
 *								 void BLEUART_Init(void)
 *								 void BLE_Send(uint32_t number,uint8_t BLE_Sendlength)
 *								 void ROM_UART6IntHandler(void)
 *
 * @date    2024/6/30
 * @author  Lei Wang
****************************************************/


#include "include.h"

uint8_t Serial_RxFlag;					//����������ݰ���־λ

uint8_t Serial_RxPacket[Serial_Getlength];
uint32_t UARTnumber;


/*************************************************************************
*  �������ڳ�ʼ��           
*    ������Ϊ115200��8λ����λ��һ��ֹͣλ����У��λ��
*    FIFO���Ĭ�ϲ�ʹ�ܣ�������һ�����ݻ��߽���һ�����ݽ����жϣ�Ĭ�ϲ����������ж�
*    �ж����ȼ�Ĭ��λ0x0��������ȼ�
***************************************************************************/
void BLEUART_Init(void)
{
	//��ʼ������1
	ROM_UART1_Init();
	
}



/**
  * ��    �������ڽ�������ת��
  * ��    ���� 
  * ��    ����Array Ҫת���������ַ�����
  * ��    ����Length Ҫת�������ַ��ĳ���
  * ��		ע�����������ڽ��յ��ַ���ʽ����ת��Ϊ��Ӧ���յ�����
  */
uint32_t Serial_NumberChange(uint8_t *Array, uint8_t Length)
{
	uint32_t Get_Number;
	uint8_t i;
	for (i = 0; i < Length; i ++)		//�������ֳ��ȱ������ֵ�ÿһλ
	{
		Get_Number+=(Array[i] - '0') *Serial_Pow(10, Length - i - 1) ;
	}
	return Get_Number;
}

/**
  * ��    �������ڷ���һ������
  * ��    ����number ��Ҫ���͵�����        
  * ��    ����BLE_Sendlength ���ݵĳ���            
  * �� �� ֵ����
  * ��		ע��0x28��0x29�ǡ������͡������Զ���İ�ͷ�Ͱ�β��3��ʾ����3
  */

void BLE_Send(uint32_t number,uint8_t BLE_Sendlength)
{
	uint32_t data[BLE_Sendlength];
	
	//��number��
	Serial_SendByte(1, 0x28);
	Serial_SendNumber(1,number,BLE_Sendlength);
	Serial_SendByte(1, 0x29);
	
}

/*************************************************************************
*  �������ڽ����жϺ���           
*    
* ��	ע��������Ϊ115200��8λ����λ��һ��ֹͣλ����У��λ
*    
* 			  �������ݵĳ�����ͷ�ļ��޸ĺ궨��
*
*					���յ����ݿ���ֱ��ʹ��
*    
***************************************************************************/

//void ROM_UART1IntHandler(void)
//{
//	static uint8_t RxState = 0;		//�����ʾ��ǰ״̬��״̬�ľ�̬����
//	static uint8_t pRxPacket = 0;	//�����ʾ��ǰ��������λ�õľ�̬����
//	
//	//static uint8_t messageLength = 0;
//	
//	uint8_t rxBuf;
//	
//  uint32_t ui32IntStatus;

//	
//  ui32IntStatus = UARTIntStatus(UART1_BASE, true);
//	
//  ROM_UARTIntClear(UART1_BASE, ui32IntStatus);//������жϱ�־λ
//	//�˺������ж��Ƿ���ܵ��������з�����
//	//�����жϽ���if��ifִ�����˳��ж�
//	if (!ROM_UARTCharsAvail(UART1_BASE))
//	{
//		return;
//	}
//	//�����жϽ���while
//	while (ROM_UARTCharsAvail(UART1_BASE))
//	{
//		rxBuf = ROM_UARTCharGetNonBlocking(UART1_BASE);
//		
//		/*��ǰ״̬Ϊ0���������ݰ���ͷ*/
//		if (RxState == 0)
//		{
//			if (rxBuf == 0x28)			//�������ȷʵ�ǰ�ͷ
//			{
//				RxState = 1;			//����һ��״̬
//				pRxPacket = 0;			//���ݰ���λ�ù���
//			}
//		}
//		/*��ǰ״̬Ϊ1���������ݰ�����*/

//		else if (RxState == 1)
//		{
//			Serial_RxPacket[pRxPacket] = rxBuf;	//�����ݴ������ݰ������ָ��λ��
//			pRxPacket ++;				//���ݰ���λ������
//			if (pRxPacket >= Serial_Getlength)			//����չ�4������
//			{
//				RxState = 2;			//����һ��״̬
//			}
//		}
//		/*��ǰ״̬Ϊ2���������ݰ���β*/
//		else if (RxState == 2)
//		{
//			if (rxBuf == 0x29)			//�������ȷʵ�ǰ�β��
//			{
//				RxState = 0;			//״̬��0
//				Serial_RxFlag = 1;		//�������ݰ���־λ��1���ɹ�����һ�����ݰ�
//				
//			}
//		}
//	
//		if(Serial_RxFlag == 1)
//		{
//			Serial_RxFlag =0;
//			//�����յ��ַ��������ת��Ϊ���ݣ���ֱ��ʹ��
//			UARTnumber = Serial_NumberChange(Serial_RxPacket,pRxPacket); 
//		}
//							
//	}
//	
//}

/**
  * ��    �����������ڻ�ȡ����
  * 
  * ��		ע�����ؽ��յ�����
  *           Ĭ��4λ���ݣ����ݳ����ڶ�Ӧͷ�ļ��޸ĺ궨�壺Serial_Getlength
  */
uint32_t BLE_GetNumber()
{
	return UARTnumber;
}
