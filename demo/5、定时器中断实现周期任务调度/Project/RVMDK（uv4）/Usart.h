#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include <stdint.h>
#include <stdbool.h>
#include "Headfile.h"

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern char  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint16_t USART_RX_STA;         		//����״̬���	

#define false 0
#define true 1

#define HEADER 0xAA							/* ��ʼ�� */
#define device_address 0x00     /* �豸��ַ */
#define chunk_offset 0x00       /* ƫ�Ƶ�ַ���� */
#define PACK_GET_DISTANCE 0x02 	/* ��ȡ������������ */
#define PACK_RESET_SYSTEM 0x0D 	/* ��λ���� */
#define PACK_STOP 0x0F 				  /* ֹͣ�������ݴ������� */
#define PACK_ACK 0x10           /* Ӧ�������� */
#define PACK_VERSION 0x14       /* ��ȡ��������Ϣ���� */

typedef struct {
	int16_t distance;  						/* �������ݣ�����Ŀ����뵥λ mm */
	uint16_t noise;		 						/* ������������ǰ���������µ��ⲿ����������Խ��˵������Խ�� */
	uint32_t peak;								/* ����ǿ����Ϣ������Ŀ�귴��صĹ�ǿ�� */
	uint8_t confidence;						/* ���Ŷȣ��ɻ��������ͽ���ǿ����Ϣ�ںϺ�Ĳ�����Ŀ��Ŷ� */
	uint32_t intg;     						/* ���ִ�������ǰ�����������Ļ��ִ��� */
	int16_t reftof;   						/* �¶ȱ���ֵ������оƬ�ڲ��¶ȱ仯����ֵ��ֻ��һ���¶ȱ仯���޷�����ʵ�¶ȶ�Ӧ */
}LidarPointTypedef;

struct AckResultData{
	uint8_t ack_cmd_id;						/* �𸴵����� id */
	uint8_t result; 							/* 1��ʾ�ɹ�,0��ʾʧ�� */
};

struct LiManuConfig
{
	uint32_t version; 						/* ����汾�� */
	uint32_t hardware_version; 		/* Ӳ���汾�� */
	uint32_t manufacture_date; 		/* �������� */
	uint32_t manufacture_time; 		/* ����ʱ�� */
	uint32_t id1; 								/* �豸 id1 */
	uint32_t id2; 								/* �豸 id2 */
	uint32_t id3; 								/* �豸 id3 */
	uint8_t sn[8]; 								/* sn */
	uint16_t pitch_angle[4]; 			/* �Ƕ���Ϣ */
	uint16_t blind_area[2]; 			/* ä����Ϣ */
	uint32_t frequence; 					/* ���ݵ�Ƶ */
};

//void ROM_UART3IntHandler(void);
void data_process(void);
uint8_t Hand(char *a) ;
void CLR_Buf(void)  ;
void Tof_ReceiveData(uint8_t temp_data);



void UART3_Init(unsigned long bound);
void UART3_IRQHandler(void);


int fputc(int ch, FILE *f){UARTCharPut(UART3_BASE,ch);	return (ch);}
int fgetc(FILE *f) {int ch=UARTCharGet(UART3_BASE);	return (ch);}
		
#endif
