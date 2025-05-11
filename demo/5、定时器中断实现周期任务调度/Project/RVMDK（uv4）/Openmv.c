#include "Openmv.h"
#include "Headfile.h"

static uint8_t Cx=0,Cy=0,Cw=0,Ch=0;
void Openmv_Receive(uint8_t com_data )
{
	static uint8_t RxCounter1 = 0;
	static uint8_t RxBuffer1[10] = {0}; // ���ݰ�����Ϊ12�ֽ�
	static uint8_t RxState = 0;	
	static uint8_t RxFlag1 = 0;
	uint8_t i;
		if (RxState == 0 && com_data == 0x2C) // 0x2C ֡ͷ
		{
			RxState = 1;
			RxBuffer1[RxCounter1++] = com_data;
		}
		else if (RxState == 1 && com_data == 0x12) // 0x12 ֡ͷ
		{	
		
			RxState = 2;
			RxBuffer1[RxCounter1++] = com_data;
		}
		else if (RxState == 2)
		{
			RxBuffer1[RxCounter1++] = com_data;

			if (RxCounter1 >= 10||com_data == 0x5B) // �������ݰ��������
			{
				RxState = 3;
				RxFlag1 = 1;
				Cx=RxBuffer1[RxCounter1-5];
						Cy=RxBuffer1[RxCounter1-4];
						Cw=RxBuffer1[RxCounter1-3];
						Ch=RxBuffer1[RxCounter1-2];
			}
		}
		else if (RxState == 3) // ����Ƿ���յ�������־
		{
			if (RxBuffer1[RxCounter1-1] == 0x5B)
			{
				UARTIntDisable(UART1_BASE, UART_INT_RX); // �رս����ж�
				if (RxFlag1)
				{
					OLED_ShowNum(0, 0, Cx, 3, OLED_8X16);
					OLED_ShowNum(0, 17, Cy, 3, OLED_8X16);
					OLED_ShowNum(0, 33, Cw, 3, OLED_8X16);
					OLED_ShowNum(0, 49, Ch, 3, OLED_8X16);
					OLED_Update();
				}
				RxFlag1 = 0;
				RxCounter1 = 0;
				RxState = 0;
				UARTIntEnable(UART1_BASE, UART_INT_RX); // ʹ�ܽ����ж�		
			}
			else // ���մ���
			{
				RxState = 0;
				RxCounter1 = 0;
				for (i = 0; i < 10; i++)
				{
					RxBuffer1[i] = 0x00; // �����������������
				}
			}
		}
		else // �����쳣
		{
			RxState = 0;
			RxCounter1 = 0;
			for (i = 0; i < 10; i++)
			{
				RxBuffer1[i] = 0x00; // �����������������
			}
		
	}
	
}
