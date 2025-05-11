/*
2020年电赛G题无接触测量，
MCU为TM4C123GH6ZP
目前用到的接口
PC0 PC1 PC2 PC3 -> STLink烧录口
PA4 PA5 ->  	OLED
PF7   PF5 -> LED灯   蜂鸣器
PD0   ->  激光笔（待定）

PD5 PD6 -> 串口6用于激光测距
PB0 PB1 -> 串口1用于连接openmv
PE5 PE4 -> 串口5用于连接语音
PK0 PK1 PK2 PK3 -> 四个按键 分别为任务1 2 3 4

PH1 PE0    ->   PH1控制电机步进 PE0控制电机方向  （上）
PH2 PH0    ->   PH2控制电机步进 PH0控制电机方向  （下）


openmv数据传输：
传给单片机特征 ： 0x20 0x** 0x** 0x** 0x** 0xDD 
其中第一个0x**为特征 
0x51为红色圆形 			0x52为绿色圆形			0x53为蓝色圆形
0x54为红色正方形 		0x55为绿色正方形			0x56为蓝色正方形
0x57为红色三角形 		0x58为绿色三角形			0x59为蓝色三角形
0x5A为篮球 				0x5B为排球				0x5C为足球
后面三个为边长或直径位次是从高到底，传字符串类型数字就行如   "123"


传给单片机特征 ： 0x20 0x** 0x** 0x** 0x** 0x** 0x** 0x** 0x** 0x** 0x** 0x** 0x** 0x** 0x** 0x** 0xDD 
从左到右 传输的是图像中心点xy坐标，激光点xy坐标，共四个3位数，传字符串类型数字就行如   "123"

2024/7/9
qihang chen
*/

#include "include.h"



 #ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif


//激光测距
uint16_t receive_cnt;//计算成功接收数据帧次数
uint8_t confidence;
uint16_t distance,noise,reftof;
uint32_t peak,intg;
uint8_t dis;
extern int q;


uint8_t OPENMV_buff[100];     	//openmv串口数组
uint8_t OPENMV_index = 0;		//openmv串口指引

uint8_t ASR_buff[100]; 		//语音串口数组
uint8_t ASR_index = 0;			//语音串口指引

uint8_t key1_num = 0;			//按键1
uint8_t key2_num = 0;			//按键2
uint8_t key3_num = 0;			//按键3
uint8_t key4_num = 0;			//按键4


uint16_t Wtimer0A_cnt = 0;        //定时器0的计数器

uint8_t out_flag = 0;				//  输出测量结果标志位
uint8_t out_flag_first = 1;			//  只输出一次测量结果0

uint16_t target_x=0;      //目标点的x坐标      
uint16_t target_y=0;	    //目标点的y坐标
uint16_t middle_x = 0;	  //中心点的x坐标
uint16_t middle_y = 0;	  //中心点的y坐标	
uint16_t point_x = 0;			//激光点的x坐标
uint16_t point_y = 0;			//激光点的y坐标
uint16_t x1=0;
uint16_t y1=0;
uint16_t x2=0;
uint16_t y2=0;
uint16_t x3=0;
uint16_t y3=0;
uint16_t x4=0;
uint16_t y4=0;
uint16_t x5=0;
uint16_t y5=0;
uint16_t x6=0;
uint16_t y6=0;
uint16_t x7=0;
uint16_t y7=0;
uint16_t x8=0;
uint16_t y8=0;

// 图形颜色形状特征边长



extern PID PID_Motor1_Speed;				//上电机的PID
extern PID PID_Motor2_Speed;				//下电机的PID

int16_t Motor1_Speed = 0;					//上电机需要的相对速度
int16_t Motor2_Speed = 0;					//下电机需要的相对速度
int flag=1;
int flag1=0;
	  uint16_t tx,ty=0;
int main(void)
{     
	//时钟总线40M
	ROM_FPUEnable();//使能浮点单元
	ROM_FPULazyStackingEnable();//浮点延迟堆栈,减少中断响应延迟 
	SysCtlClockSet( SYSCTL_SYSDIV_2_5|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN|SYSCTL_USE_PLL);
	//一些延时函数的初始化函数
	initTime();

	LED_GPIO_Config();
	Buzzer_Init();
	OLED_Init();
	Key_Init();
	PID_Init();
	ROM_UART6_Init();
	ROM_UART5_Init();
	ROM_UART1_Init();
	Motor_Init();
	Motor_speed(0,0);
	OLED_Update();
	Timer_init();

	while(1)
	{
		if(flag)
		//按键检测
		if ( ReadKey1 == 0)
		{
			flag=1;
			key1_num ++;
			key2_num = 0;
			key3_num = 0;
			key4_num=0;
		}
		else if( ReadKey2 == 0)
		{
			key2_num ++;
			key1_num = 0;
			key3_num = 0;
			key4_num=0;
		}
		else if( ReadKey3 == 0)
		{
			key3_num ++;
			key1_num = 0;
			key2_num = 0;
			key4_num=0;
		}
		else if( ReadKey4 == 0)
		{
			key4_num ++;
			key1_num = 0;
			key2_num = 0;
			key3_num=0;
		}
		else;
		OLED_Clear();
		OLED_ShowNum(0,0,middle_x,4,6);
		OLED_ShowNum(0,7,middle_y,4,6);
		OLED_ShowNum(0,21,point_x,4,6);
		OLED_ShowNum(0,28,point_y,4,6);
		OLED_ShowNum(30,0,x1,4,6);
		OLED_ShowNum(30,7,y1,4,6);
		OLED_ShowNum(30,14,x2,4,6);
		OLED_ShowNum(30,21,y2,4,6);
		OLED_ShowNum(30,28,x3,4,6);
		OLED_ShowNum(30,35,y3,4,6);
		OLED_ShowNum(30,42,x4,4,6);
		OLED_ShowNum(30,49,y4,4,6);
		OLED_ShowNum(60,0,x5,4,6);
		OLED_ShowNum(60,7,y5,4,6);
		OLED_ShowNum(60,14,x6,4,6);
		OLED_ShowNum(60,21,y6,4,6);
		OLED_ShowNum(60,28,x7,4,6);
		OLED_ShowNum(60,35,y7,4,6);
		OLED_ShowNum(60,42,x8,4,6);
		OLED_ShowNum(60,49,y8,4,6);
				OLED_ShowNum(90,49,tx,4,6);
		OLED_Update();
	}
}




uint16_t abs_user(uint16_t x,uint16_t y)
{
	if(x>y)
		return x-y;
	else
		return y-x;
	
}


// 1ms定时器中断
void TIMER0A_Handler(void)				//系统调度中断函数  是自己定义函数，需要用函数开启TimerIntRegister（）函数如下定时器初始化中
{
	//在此添加周期任务函数
	  Wtimer0A_cnt++;

  	ROM_TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
  	//清除计时器中断源  第一个参数为开启的定时器 ， 第二个参数为定时器的模式再TimerIntEnable（）函数的第二个参数
	if(key1_num)
	  {
				tx=middle_x;
				ty=middle_y;
			    PID_Motor1_Speed.target_val = ty;
		Motor1_Speed = PID_realize(&PID_Motor1_Speed,point_y);
	  PID_Motor2_Speed.target_val = tx;
		Motor2_Speed = PID_realize(&PID_Motor2_Speed,point_x); 
		Motor_speed(Motor1_Speed,Motor2_Speed);
	  }
	else if(key2_num)
		{
			
			if(flag)
			{
	     tx=x1;
		   ty=y1;
			}
	     if(abs_user(point_x,x1)<10&&abs_user(point_y,y1)<10)
		    {
      	    tx=x2;
		        ty=y2;
					  flag=0;
		    }
		   else if(abs_user(point_x,x2)<10&&abs_user(point_y,y2)<10)
		    {
     	      tx=x3;
		        ty=y3;
					
				}
			else if(abs_user(point_x,x3)<10&&abs_user(point_y,y3)<10)
				{
						tx=x4;
						ty=y4;
				}
			else if(abs_user(point_x,x4)<10&&abs_user(point_y,y4)<10)
				{
						tx=x1;
						ty=y1;
				}
		PID_Motor1_Speed.target_val = ty;
		Motor1_Speed = PID_realize(&PID_Motor1_Speed,point_y);
	  PID_Motor2_Speed.target_val = tx;
		Motor2_Speed = PID_realize(&PID_Motor2_Speed,point_x); 
		Motor_speed(Motor1_Speed,Motor2_Speed);
		}
	else if(key3_num)
	 {
		 if(flag)
		 {
		 	  tx=x5;
		    ty=y5;
		 }
	    if(abs_user(point_x,x5)<10&&abs_user(point_y,y5)<10)
		   {
				 if(flag1==0)
				 {
				 tx=x5;
         ty=y5;
				 }
				   flag1=1;
				    flag=0;
		   }
		  else if(abs_user(point_x,x6)<10&&abs_user(point_y,y6)<10)
		   {
				 if(flag1==1)
				 {
				 tx=x6;
         ty=y6;
				 }
           flag1=2;
		   }
		  else if(abs_user(point_x,x7)<10&&abs_user(point_y,y7)<10)
		   {
				 if(flag1==2)
				 {
				 tx=x7;
         ty=y7;
				 }
				 flag1=3;
		   }
		  else if(abs_user(point_x,x8)<10&&abs_user(point_y,y8)<10)
	  	 {
				 if(flag1==3)
				 {
				 tx=x8;
         ty=y8;
				 }
				 flag1=4;
		   }
			 
			 if(flag1==1)
			 {
				 	if(abs_user(point_x,tx)<5&&abs_user(point_y,ty)<5)
				 {
				    tx=(x6-x5)/10+tx;
				    ty=(y6-y5)/10+ty;
				 }
			 }
			 else if(flag1==2)
			 {
				if(abs_user(point_x,tx)<5&&abs_user(point_y,ty)<5)
				 {
				    tx=(x7-x6)/10+tx;
				    ty=(y7-y6)/10+ty;
				 }
				 
			 }
			 else if(flag1==3)
			 {
				 if(abs_user(point_x,tx)<5&&abs_user(point_y,ty)<5)
				 {
				    tx=(x8-x7)/10+tx;
				    ty=(y8-y7)/10+ty;
				 }
				 
			 }
			 else if(flag1==4)
			 {
				 	if(abs_user(point_x,tx)<5&&abs_user(point_y,ty)<5)
				 {
				    tx=(x5-x8)/10+tx;
				    ty=(y5-y8)/10+ty;
				 }
			 }
			 else
			 {
				 tx=x5;
         ty=y5;
				 flag1=0;
			 }
			 
		PID_Motor1_Speed.target_val = ty;
		Motor1_Speed = PID_realize(&PID_Motor1_Speed,point_y);
	  PID_Motor2_Speed.target_val = tx;
		Motor2_Speed = PID_realize(&PID_Motor2_Speed,point_x); 
		Motor_speed(Motor1_Speed,Motor2_Speed);
	 }
	 else if(key4_num)
	 {
		 		Motor_speed(0,0);
	 }
	 

	
} 


// 树莓派接受中断
void ROM_UART1IntHandler(void)
{
  uint32_t ui32IntStatus;
	uint8_t Com_Data;
  ui32IntStatus = UARTIntStatus(UART1_BASE, true);
  ROM_UARTIntClear(UART1_BASE, ui32IntStatus);//先清除中断标志位
	uint8_t i,j;
	static uint8_t RxCounter1=0;//
	static uint8_t RxBuffer1[100]={0};
	static uint16_t arr[30];
	static uint8_t RxState = 0;	
	static uint8_t RxFlag1 = 0;
	//接受中断进入while
	while (ROM_UARTCharsAvail(UART1_BASE))
	{
		//此函数从指定的接收FIFO中获取一个字符
		Com_Data = (uint8_t) ROM_UARTCharGetNonBlocking(UART1_BASE);
		if(RxState==0&&Com_Data==0x2C)  //0x2c
		{
          
			RxState=1;
		}
	  else if(RxState==1)
		{
           
			RxBuffer1[RxCounter1++]=Com_Data;
			if(RxCounter1>=70||Com_Data == 0x5B)      
				{
					RxState=3;
					for(i=0,j=0;i<20;i++,j=j+3)
					{
						arr[i]=(RxBuffer1[j]-'0')*100+(RxBuffer1[j+1]-'0')*10+(RxBuffer1[j+2]-'0');
					}
					middle_x=arr[0];
					middle_y=arr[1];
					point_x=arr[2];
					point_y=arr[3];
					x1=arr[4];
					y1=arr[5];
					x2=arr[6];
					y2=arr[7];
					x3=arr[8];
					y3=arr[9];
					x4=arr[10];
					y4=arr[11];
					x5=arr[12];
					y5=arr[13];
					x6=arr[14];
					y6=arr[15];
					x7=arr[16];
					y7=arr[17];
					x8=arr[18];
					y8=arr[19];

//					OLED_ShowNum(0,0,middle_x,3,6);
//					OLED_ShowNum(0,0,middle_y,3,6);
					}
			}
		
				else if(RxState==3)//
				{
						if(RxBuffer1[RxCounter1-1] == 0x5B)
						{
									
							    if(RxFlag1==0)
									{
							    OLED_Clear();
									}
									RxFlag1++;
									RxCounter1 = 0;
									RxState = 0;
		      	}
						else   
						{
									RxState = 0;
									RxCounter1=0;
									for(i=0;i<100;i++)
									{
											RxBuffer1[i]=0x00;      //
									}
						}
				} 
	
				else   
				{
						RxState = 0;
						RxCounter1=0;
						for(i=0;i<100;i++)
						{
								RxBuffer1[i]=0x00;      //
						}
				}
	}
}






// 语音模块接受中断
void ROM_UART5IntHandler(void)
{
    uint32_t ui32IntStatus;
	uint8_t ui8RxBuffer;
    ui32IntStatus = UARTIntStatus(UART5_BASE, true);
    ROM_UARTIntClear(UART5_BASE, ui32IntStatus);//先清除中断标志位
	while (ROM_UARTCharsAvail(UART5_BASE))
	{
		//此函数从指定的接收FIFO中获取一个字符
		ui8RxBuffer = (uint8_t) ROM_UARTCharGetNonBlocking(UART5_BASE);
		
		if(ui8RxBuffer == 0x01)
            flag=1;
		
	}
	
}