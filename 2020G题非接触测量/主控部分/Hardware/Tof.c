#include "include.h"

char USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
uint16_t point1 ;
LidarPointTypedef Pack_Data[12];/* 雷达接收的数据储存在这个变量之中 */
LidarPointTypedef Pack_sum;     /* 输出结果储存 */
extern uint16_t receive_cnt;
extern uint8_t confidence;
extern uint16_t distance,noise,reftof;
extern uint32_t peak,intg;
int q=0;
uint16_t distance_last = 0;  //用于滤波

void ROM_UART6IntHandler(void)
{


	uint32_t ui32IntStatus;

	uint8_t temp_data;
	
    ui32IntStatus = UARTIntStatus(UART6_BASE, true);
	
    ROM_UARTIntClear(UART6_BASE, ui32IntStatus);//先清除中断标志位
	
	//接受中断进入while
	while (ROM_UARTCharsAvail(UART6_BASE))
	{
		//此函数从指定的接收FIFO中获取一个字符
		temp_data = (uint8_t) ROM_UARTCharGetNonBlocking(UART6_BASE);
		Tof_ReceiveData(temp_data);
	}
	//接受中断后续内容可以在下面添加
	
}


void Tof_ReceiveData(uint8_t temp_data)
{
	static uint8_t state = 0;			//状态位	
	static uint8_t crc = 0;				//校验和
	static uint8_t cnt = 0;				//用于一帧12个点的计数
	static uint8_t PACK_FLAG = 0;  		//命令标志位
	static uint8_t data_len  = 0;  		//数据长度
	static uint32_t timestamp = 0; 		//时间戳
	static uint8_t state_flag = 1; 		//转入数据接收标志位
	
	if(state< 4) 																					 /* 起始符验证 前4个数据均为0xAA */
	{                                          
		if(temp_data == HEADER) state ++;
		else state = 0;
	}
	else if(state<10&&state>3)
	{
		switch(state)
		{
			case 4:   
				if(temp_data == device_address)              /* 设备地址验证 */
				{							
					state ++;
					crc = crc + temp_data;									
					break;        
				} 
				else state = 0,crc = 0;
			case 5:   
				if(temp_data == PACK_GET_DISTANCE)					 /* 获取测量数据命令 */
				{  
					PACK_FLAG = PACK_GET_DISTANCE;
					state ++;	
					crc = crc + temp_data;	
					break;									
				}		 

				else if(temp_data == PACK_RESET_SYSTEM) 		 /* 复位命令 */
				{
					PACK_FLAG = PACK_RESET_SYSTEM;
					state ++; 
					crc = crc + temp_data;	
					break;	
				}
				else if(temp_data == PACK_STOP)							 /* 停止测量数据传输命令 */
				{ 
					PACK_FLAG = PACK_STOP;
					state ++; 
					crc = crc + temp_data;	
					break;
				}
				else if(temp_data == PACK_ACK)							 /* 应答码命令 */
				{  
					PACK_FLAG = PACK_ACK;
					state ++;
					crc = crc + temp_data;	
					break;
				}			 				 
				else if(temp_data == PACK_VERSION)					 /* 获取传感器信息命令 */
				{ 
					PACK_FLAG = PACK_VERSION,
					state ++,
					crc = crc + temp_data;	   	     
					break;
				}
				else state = 0,crc = 0;
			case 6: if(temp_data == chunk_offset)          /* 偏移地址 */
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
					data_len = (uint16_t)temp_data;								 /* 数据长度低八位 */
					state ++; 
					crc = crc + temp_data;
					break;																			 
			case 9: 
					data_len = data_len + ((uint16_t)temp_data<<8); 			 /* 数据长度高八位 */
					state ++;
					crc = crc + temp_data;
					break; 
			default: break;
		}
	}
	else if(state == 10 ) state_flag = 0;                    /*由switch跳出来时state为10，但temp_data仍为距离长度高八位数据，需跳过一次中断*/
	if(PACK_FLAG == PACK_GET_DISTANCE&&state_flag == 0)      /* 获取一帧数据并校验 */
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
						Pack_Data[cnt].distance = (uint16_t)temp_data ;				 /* 距离数据低八位 */
						crc = crc + temp_data;
						state++;
						break;        
					case 1: 
						Pack_Data[cnt].distance = ((uint16_t)temp_data<<8) + Pack_Data[cnt].distance;	 /* 距离数据 */
						crc = crc + temp_data;
						state++;
						break; 
					case 2:
						Pack_Data[cnt].noise = (uint16_t)temp_data;				 /* 环境噪音低八位 */
						crc = crc + temp_data;
						state++;
						break; 
					case 3:
						Pack_Data[cnt].noise = ((uint16_t)temp_data<<8) + Pack_Data[cnt].noise;				 /* 环境噪音 */
						crc = crc + temp_data;
						state++;
						break; 
					case 4:
						Pack_Data[cnt].peak = (uint32_t)temp_data;				 										 /* 接受强度信息低八位 */
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
						Pack_Data[cnt].peak = ((uint32_t)temp_data<<24) + Pack_Data[cnt].peak;				    /* 接受强度信息 */
						crc = crc + temp_data;
						state++;
						break; 
					case 8:
						Pack_Data[cnt].confidence = temp_data;				 /* 置信度 */
						crc = crc + temp_data;
						state++;
						break; 
					case 9:
						Pack_Data[cnt].intg = (uint32_t)temp_data;															/* 积分次数低八位 */
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
						Pack_Data[cnt].intg = ((uint32_t)temp_data<<24) + Pack_Data[cnt].intg;				  	 /* 积分次数 */
						crc = crc + temp_data;
						state++;
						break; 
					case 13:
						Pack_Data[cnt].reftof = (int16_t)temp_data;				 								 /* 温度表征值低八位 */
						crc = crc + temp_data;
						state++;
						break; 
					case 14:
						Pack_Data[cnt].reftof = ((int16_t)temp_data<<8) +Pack_Data[cnt].reftof;			/* 温度表征值 */
						crc = crc + temp_data;
						state++;
						cnt++;							 /* 进入下一个测量点 */
						break; 
					default: break;
				}
			}
			/* 时间戳 */
			if(state == 190) timestamp = temp_data,state++,crc = crc + temp_data;
			else if(state == 191) timestamp = ((uint32_t)temp_data<<8) + timestamp,state++,crc = crc + temp_data; 
			else if(state == 192) timestamp = ((uint32_t)temp_data<<16) + timestamp,state++,crc = crc + temp_data;
			else if(state == 193) timestamp = ((uint32_t)temp_data<<24) + timestamp,state++,crc = crc + temp_data; 
			else if(state==194)
			{
				if(temp_data == crc)   /* 校验成功 */
				{
					data_process();  	 /* 数据处理函数，完成一帧之后可进行数据处理 */
					receive_cnt++;	 	 /* 输出接收到正确数据的次数 */
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
				cnt = 0; 							 /* 复位*/
			}
		}
	}
}


uint8_t Hand(char *a)                   // 串口命令识别函数
{ 
    if(strstr(USART_RX_BUF,a)!=NULL)
	    return 1;
	else
		return 0;
}

void CLR_Buf(void)                           // 串口缓存清理
{
	memset(USART_RX_BUF, 0, USART_REC_LEN);      //清空
  point1 = 0;                    
}

void data_process(void)/*数据处理函数，完成一帧之后可进行数据处理*/
{
	/* 计算距离 */
	static uint8_t cnt = 0;
	uint8_t i;
	static uint16_t count = 0;
	static uint32_t sum = 0;
	LidarPointTypedef Pack_sum;
	for(i=0;i<12;i++)									/* 12个点取平均 */
	{
		if(Pack_Data[i].distance != 0)  /* 去除0的点 */
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


#define FILTER_SIZE 20 // 滤波器窗口大小
int16_t distance_filter(int16_t new_value) 
{
    static int16_t distancedata[FILTER_SIZE] = {0}; // 静态数组存储输入数据
    static int16_t distanceindex = 0; // 窗口索引
    
    static int16_t filtered_value = 0;
    
    // 更新数据数组
    distancedata[distanceindex] = new_value;
    distanceindex = (distanceindex + 1) % FILTER_SIZE;
    
    // 使用移动平均滤波器来平滑数据
    for (int i = 0; i < FILTER_SIZE; i++) 
	{
        filtered_value += distancedata[i];
    }
    
    filtered_value /= FILTER_SIZE;
    
    return filtered_value;
}
