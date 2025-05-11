#include "include.h"

char USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
uint16_t point1 ;
LidarPointTypedef Pack_Data[12];/* �״���յ����ݴ������������֮�� */
LidarPointTypedef Pack_sum;     /* ���������� */
extern uint16_t receive_cnt;
extern uint8_t confidence;
extern uint16_t distance,noise,reftof;
extern uint32_t peak,intg;
int q=0;
uint16_t distance_last = 0;  //�����˲�

void ROM_UART6IntHandler(void)
{


	uint32_t ui32IntStatus;

	uint8_t temp_data;
	
    ui32IntStatus = UARTIntStatus(UART6_BASE, true);
	
    ROM_UARTIntClear(UART6_BASE, ui32IntStatus);//������жϱ�־λ
	
	//�����жϽ���while
	while (ROM_UARTCharsAvail(UART6_BASE))
	{
		//�˺�����ָ���Ľ���FIFO�л�ȡһ���ַ�
		temp_data = (uint8_t) ROM_UARTCharGetNonBlocking(UART6_BASE);
		Tof_ReceiveData(temp_data);
	}
	//�����жϺ������ݿ������������
	
}


void Tof_ReceiveData(uint8_t temp_data)
{
	static uint8_t state = 0;			//״̬λ	
	static uint8_t crc = 0;				//У���
	static uint8_t cnt = 0;				//����һ֡12����ļ���
	static uint8_t PACK_FLAG = 0;  		//�����־λ
	static uint8_t data_len  = 0;  		//���ݳ���
	static uint32_t timestamp = 0; 		//ʱ���
	static uint8_t state_flag = 1; 		//ת�����ݽ��ձ�־λ
	
	if(state< 4) 																					 /* ��ʼ����֤ ǰ4�����ݾ�Ϊ0xAA */
	{                                          
		if(temp_data == HEADER) state ++;
		else state = 0;
	}
	else if(state<10&&state>3)
	{
		switch(state)
		{
			case 4:   
				if(temp_data == device_address)              /* �豸��ַ��֤ */
				{							
					state ++;
					crc = crc + temp_data;									
					break;        
				} 
				else state = 0,crc = 0;
			case 5:   
				if(temp_data == PACK_GET_DISTANCE)					 /* ��ȡ������������ */
				{  
					PACK_FLAG = PACK_GET_DISTANCE;
					state ++;	
					crc = crc + temp_data;	
					break;									
				}		 

				else if(temp_data == PACK_RESET_SYSTEM) 		 /* ��λ���� */
				{
					PACK_FLAG = PACK_RESET_SYSTEM;
					state ++; 
					crc = crc + temp_data;	
					break;	
				}
				else if(temp_data == PACK_STOP)							 /* ֹͣ�������ݴ������� */
				{ 
					PACK_FLAG = PACK_STOP;
					state ++; 
					crc = crc + temp_data;	
					break;
				}
				else if(temp_data == PACK_ACK)							 /* Ӧ�������� */
				{  
					PACK_FLAG = PACK_ACK;
					state ++;
					crc = crc + temp_data;	
					break;
				}			 				 
				else if(temp_data == PACK_VERSION)					 /* ��ȡ��������Ϣ���� */
				{ 
					PACK_FLAG = PACK_VERSION,
					state ++,
					crc = crc + temp_data;	   	     
					break;
				}
				else state = 0,crc = 0;
			case 6: if(temp_data == chunk_offset)          /* ƫ�Ƶ�ַ */
					{  
						state ++;
						crc = crc + temp_data;
						break; 	  
					}	
					else state = 0,crc = 0;
			case 7: if(temp_data == chunk_offset)
					{  
						state ++;
						crc = crc + temp_data;
						break;
					}
					else state = 0,crc = 0;
			case 8: 
					data_len = (uint16_t)temp_data;								 /* ���ݳ��ȵͰ�λ */
					state ++; 
					crc = crc + temp_data;
					break;																			 
			case 9: 
					data_len = data_len + ((uint16_t)temp_data<<8); 			 /* ���ݳ��ȸ߰�λ */
					state ++;
					crc = crc + temp_data;
					break; 
			default: break;
		}
	}
	else if(state == 10 ) state_flag = 0;                    /*��switch������ʱstateΪ10����temp_data��Ϊ���볤�ȸ߰�λ���ݣ�������һ���ж�*/
	if(PACK_FLAG == PACK_GET_DISTANCE&&state_flag == 0)      /* ��ȡһ֡���ݲ�У�� */
	{
		if(state>9)
		{
			if(state<190)
			{
				static uint8_t state_num;
				state_num = (state-10)%15;
				switch(state_num)
				{
					case 0: 
						Pack_Data[cnt].distance = (uint16_t)temp_data ;				 /* �������ݵͰ�λ */
						crc = crc + temp_data;
						state++;
						break;        
					case 1: 
						Pack_Data[cnt].distance = ((uint16_t)temp_data<<8) + Pack_Data[cnt].distance;	 /* �������� */
						crc = crc + temp_data;
						state++;
						break; 
					case 2:
						Pack_Data[cnt].noise = (uint16_t)temp_data;				 /* ���������Ͱ�λ */
						crc = crc + temp_data;
						state++;
						break; 
					case 3:
						Pack_Data[cnt].noise = ((uint16_t)temp_data<<8) + Pack_Data[cnt].noise;				 /* �������� */
						crc = crc + temp_data;
						state++;
						break; 
					case 4:
						Pack_Data[cnt].peak = (uint32_t)temp_data;				 										 /* ����ǿ����Ϣ�Ͱ�λ */
						crc = crc + temp_data;
						state++;
						break; 
					case 5:
						Pack_Data[cnt].peak = ((uint32_t)temp_data<<8) + Pack_Data[cnt].peak;
						crc = crc + temp_data;
						state++;
						break; 
					case 6:
						Pack_Data[cnt].peak = ((uint32_t)temp_data<<16) + Pack_Data[cnt].peak;	
						crc = crc + temp_data;
						state++;
						break; 
					case 7:
						Pack_Data[cnt].peak = ((uint32_t)temp_data<<24) + Pack_Data[cnt].peak;				    /* ����ǿ����Ϣ */
						crc = crc + temp_data;
						state++;
						break; 
					case 8:
						Pack_Data[cnt].confidence = temp_data;				 /* ���Ŷ� */
						crc = crc + temp_data;
						state++;
						break; 
					case 9:
						Pack_Data[cnt].intg = (uint32_t)temp_data;															/* ���ִ����Ͱ�λ */
						crc = crc + temp_data;
						state++;
						break; 
					case 10:
						Pack_Data[cnt].intg = ((uint32_t)temp_data<<8) + Pack_Data[cnt].intg;
						crc = crc + temp_data;
						state++;
						break; 
					case 11:
						Pack_Data[cnt].intg = ((uint32_t)temp_data<<16) + Pack_Data[cnt].intg;
						crc = crc + temp_data;
						state++;
						break; 
					case 12:
						Pack_Data[cnt].intg = ((uint32_t)temp_data<<24) + Pack_Data[cnt].intg;				  	 /* ���ִ��� */
						crc = crc + temp_data;
						state++;
						break; 
					case 13:
						Pack_Data[cnt].reftof = (int16_t)temp_data;				 								 /* �¶ȱ���ֵ�Ͱ�λ */
						crc = crc + temp_data;
						state++;
						break; 
					case 14:
						Pack_Data[cnt].reftof = ((int16_t)temp_data<<8) +Pack_Data[cnt].reftof;			/* �¶ȱ���ֵ */
						crc = crc + temp_data;
						state++;
						cnt++;							 /* ������һ�������� */
						break; 
					default: break;
				}
			}
			/* ʱ��� */
			if(state == 190) timestamp = temp_data,state++,crc = crc + temp_data;
			else if(state == 191) timestamp = ((uint32_t)temp_data<<8) + timestamp,state++,crc = crc + temp_data; 
			else if(state == 192) timestamp = ((uint32_t)temp_data<<16) + timestamp,state++,crc = crc + temp_data;
			else if(state == 193) timestamp = ((uint32_t)temp_data<<24) + timestamp,state++,crc = crc + temp_data; 
			else if(state==194)
			{
				if(temp_data == crc)   /* У��ɹ� */
				{
					data_process();  	 /* ���ݴ����������һ֮֡��ɽ������ݴ��� */
					receive_cnt++;	 	 /* ������յ���ȷ���ݵĴ��� */
				} 
				distance = Pack_Data[0].distance;
				if (Pack_Data[0].distance < 3500)
				{
					distance = Pack_Data[0].distance;
					distance_last = distance;
				}
				
				else
					distance = distance_last;
				
				crc = 0;
				state = 0;
				state_flag = 1;
				cnt = 0; 							 /* ��λ*/
			}
		}
	}
}


uint8_t Hand(char *a)                   // ��������ʶ����
{ 
    if(strstr(USART_RX_BUF,a)!=NULL)
	    return 1;
	else
		return 0;
}

void CLR_Buf(void)                           // ���ڻ�������
{
	memset(USART_RX_BUF, 0, USART_REC_LEN);      //���
  point1 = 0;                    
}

void data_process(void)/*���ݴ����������һ֮֡��ɽ������ݴ���*/
{
	/* ������� */
	static uint8_t cnt = 0;
	uint8_t i;
	static uint16_t count = 0;
	static uint32_t sum = 0;
	LidarPointTypedef Pack_sum;
	for(i=0;i<12;i++)									/* 12����ȡƽ�� */
	{
		if(Pack_Data[i].distance != 0)  /* ȥ��0�ĵ� */
		{
			count++;
			Pack_sum.distance += Pack_Data[i].distance;
			Pack_sum.noise += Pack_Data[i].noise;
			Pack_sum.peak += Pack_Data[i].peak;
			Pack_sum.confidence += Pack_Data[i].confidence;
			Pack_sum.intg += Pack_Data[i].intg;
			Pack_sum.reftof += Pack_Data[i].reftof;
		}
	}
	if(count !=0)
	{
		distance = Pack_sum.distance/count;
		noise = Pack_sum.noise/count;
		peak = Pack_sum.peak/count;
		confidence = Pack_sum.confidence/count;
		intg = Pack_sum.intg/count;
		reftof = Pack_sum.reftof/count;
		Pack_sum.distance = 0;
		Pack_sum.noise = 0;
		Pack_sum.peak = 0;
		Pack_sum.confidence = 0;
		Pack_sum.intg = 0;
		Pack_sum.reftof = 0;
		count = 0;
	}
}


#define FILTER_SIZE 20 // �˲������ڴ�С
int16_t distance_filter(int16_t new_value) 
{
    static int16_t distancedata[FILTER_SIZE] = {0}; // ��̬����洢��������
    static int16_t distanceindex = 0; // ��������
    
    static int16_t filtered_value = 0;
    
    // ������������
    distancedata[distanceindex] = new_value;
    distanceindex = (distanceindex + 1) % FILTER_SIZE;
    
    // ʹ���ƶ�ƽ���˲�����ƽ������
    for (int i = 0; i < FILTER_SIZE; i++) 
	{
        filtered_value += distancedata[i];
    }
    
    filtered_value /= FILTER_SIZE;
    
    return filtered_value;
}
