#include "Usart.h"

uint8_t RxBuffer[1];//���ڽ��ջ���
uint16_t RxLine = 0;//ָ���
uint8_t DataBuff[200];//ָ������
uint8_t P;

void UART3_Init(unsigned long bound)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//ʹ��GPIO����		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);//ʹ��UART����
  GPIOPinConfigure(GPIO_PC6_U3RX);//GPIOģʽ���� PB0--RX PB1--TX 
  GPIOPinConfigure(GPIO_PC7_U3TX);
  GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);//GPIO��UARTģʽ����
	UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), bound,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));
  UARTFIFODisable(UART3_BASE);//ʹ��UART1�ж�	
  UARTIntEnable(UART3_BASE,UART_INT_RX);//ʹ��UART1�����ж�		
  UARTIntRegister(UART3_BASE,UART3_IRQHandler);//UART1�жϵ�ַע��	
  IntPrioritySet(INT_UART3, USER_INT3);//USER_INT1
	
}



float Get_Data(void)
{
    uint8_t data_Start_Num = 0; // ��¼����λ��ʼ�ĵط�
    uint8_t data_End_Num = 0; // ��¼����λ�����ĵط�
    uint8_t data_Num = 0; // ��¼����λ��
    uint8_t minus_Flag = 0; // �ж��ǲ��Ǹ���
    float data_return = 0; // �����õ�������
    for(uint8_t i=0;i<200;i++) // ���ҵȺź͸�̾�ŵ�λ��
    {
        if(DataBuff[i] == '=') data_Start_Num = i + 1; // +1��ֱ�Ӷ�λ��������ʼλ
        if(DataBuff[i] == '!')
        {
            data_End_Num = i - 1;
            break;
        }
    }
    if(DataBuff[data_Start_Num] == '-') // ����Ǹ���
    {
        data_Start_Num += 1; // ����һλ������λ
        minus_Flag = 1; // ����flag
    }
    data_Num = data_End_Num - data_Start_Num + 1;
    if(data_Num == 4) // ���ݹ�4λ
    {
        data_return = (DataBuff[data_Start_Num]-48)  + (DataBuff[data_Start_Num+2]-48)*0.1f +
                (DataBuff[data_Start_Num+3]-48)*0.01f;
    }
    else if(data_Num == 5) // ���ݹ�5λ
    {
        data_return = (DataBuff[data_Start_Num]-48)*10 + (DataBuff[data_Start_Num+1]-48) + (DataBuff[data_Start_Num+3]-48)*0.1f +
                (DataBuff[data_Start_Num+4]-48)*0.01f;
    }
    else if(data_Num == 6) // ���ݹ�6λ
    {
        data_return = (DataBuff[data_Start_Num]-48)*100 + (DataBuff[data_Start_Num+1]-48)*10 + (DataBuff[data_Start_Num+2]-48) +
                (DataBuff[data_Start_Num+4]-48)*0.1f + (DataBuff[data_Start_Num+5]-48)*0.01f;
    }
    if(minus_Flag == 1)  data_return = -data_return;
//    printf("data=%.2f\r\n",data_return);
    return data_return;
}




void USART_PID_Adjust(uint8_t Motor_n)
{
    float data_Get = Get_Data(); // ��Ž��յ�������
//    printf("data=%.2f\r\n",data_Get);
   // if(Motor_n == 1)//��ߵ��
   // {
        if(DataBuff[0]=='P' && DataBuff[1]=='2') // λ�û�P
          //  pid_l_position.kp = data_Get;
				   P=data_Get;
				printf("P=%f\n",data_Get);
//        else if(DataBuff[0]=='I' && DataBuff[1]=='1') // λ�û�I
//            pid_l_position.ki = data_Get;
//        else if(DataBuff[0]=='D' && DataBuff[1]=='1') // λ�û�D
//            pid_l_position.kd = data_Get;
//        else if(DataBuff[0]=='P' && DataBuff[1]=='2') // �ٶȻ�P
//            pid_l_speed.kp = data_Get;
//        else if(DataBuff[0]=='I' && DataBuff[1]=='2') // �ٶȻ�I
//            pid_l_speed.ki = data_Get;
//        else if(DataBuff[0]=='D' && DataBuff[1]=='2') // �ٶȻ�D
//            pid_l_speed.kd = data_Get;
//        else if((DataBuff[0]=='S' && DataBuff[1]=='p') && DataBuff[2]=='e') //Ŀ���ٶ�
//            L_Target_Speed = data_Get;
//        else if((DataBuff[0]=='P' && DataBuff[1]=='o') && DataBuff[2]=='s') //Ŀ��λ��
//            L_Target_Position = data_Get;
   // }
//    else if(Motor_n == 0) // �ұߵ��
//    {
//        if(DataBuff[0]=='P' && DataBuff[1]=='1') // λ�û�P
//            pid_r_position.kp = data_Get;
//        else if(DataBuff[0]=='I' && DataBuff[1]=='1') // λ�û�I
//            pid_r_position.ki = data_Get;
//        else if(DataBuff[0]=='D' && DataBuff[1]=='1') // λ�û�D
//            pid_r_position.kd = data_Get;
//        else if(DataBuff[0]=='P' && DataBuff[1]=='2') // �ٶȻ�P
//            pid_r_speed.kp = data_Get;
//        else if(DataBuff[0]=='I' && DataBuff[1]=='2') // �ٶȻ�I
//            pid_r_speed.ki = data_Get;
//        else if(DataBuff[0]=='D' && DataBuff[1]=='2') // �ٶȻ�D
//            pid_r_speed.kd = data_Get;
//        else if((DataBuff[0]=='S' && DataBuff[1]=='p') && DataBuff[2]=='e') //Ŀ���ٶ�
//            R_Target_Speed = data_Get;
//        else if((DataBuff[0]=='P' && DataBuff[1]=='o') && DataBuff[2]=='s') //Ŀ��λ��
//            R_Target_Position = data_Get;
//    }
}

char USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
uint16_t point1 ;
LidarPointTypedef Pack_Data[12];/* �״���յ����ݴ������������֮�� */
LidarPointTypedef Pack_sum;     /* ���������� */
extern uint16_t receive_cnt;
extern uint8_t confidence;
extern uint16_t distance,noise,reftof;
extern uint32_t peak,intg;
int q=0;





void UART3_IRQHandler(void)
{
	uint32_t flag = UARTIntStatus(UART3_BASE, 1); // ��ȡ�жϱ�־ ԭʼ�ж�״̬ �����жϱ�־	
	UARTIntClear(UART3_BASE, flag); // ����жϱ�־
		uint8_t temp_data;
		//printf("RXLen=%d\r\n",RxLine);	
		while (UARTCharsAvail(UART3_BASE)) // �ж� FIFO �Ƿ�������		
	{	
		temp_data = (uint8_t) ROM_UARTCharGetNonBlocking(UART3_BASE);
		Tof_ReceiveData(temp_data);
        
	}
	
	
}

void Tof_ReceiveData(uint8_t temp_data)
{
	
	static uint8_t state = 0;			//״̬λ	
	static uint8_t crc = 0;				//У���
	static uint8_t cnt = 0;				//����һ֡12����ļ���
	static uint8_t PACK_FLAG = 0;  //�����־λ
	static uint8_t data_len  = 0;  //���ݳ���
	static uint32_t timestamp = 0; //ʱ���
	static uint8_t state_flag = 1; //ת�����ݽ��ձ�־λ
	
	if(state< 4) 																					 /* ��ʼ����֤ ǰ4�����ݾ�Ϊ0xAA */
				{                                          
						if(temp_data == HEADER) 
						{
							state ++;
					
						}
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
															OLED_ShowString(4,0,"Di:",6);			/* ��ʾ���� */
		                     OLED_ShowNum(14,0,distance,5,6);
															OLED_Update();	
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