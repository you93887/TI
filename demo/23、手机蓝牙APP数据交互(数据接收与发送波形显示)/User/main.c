#include "stdio.h"
#include <stdint.h>
#include <stdbool.h>
#include "hw_memmap.h"
#include "hw_types.h"
#include "debug.h"
#include "fpu.h"
#include "gpio.h"
#include "pin_map.h"
#include "rom.h"
#include "sysctl.h"
#include "uart.h"
#include "uartstdio.h"
#include "SystickTime.h"
#include "oled.h"
#include "Headfile.h"
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif


/*
*********************************************************************************************************
*	函 数 名: PrintfLogo
*	功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的串口终端软件可以观察结果
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void PrintfLogo(void)
{
	printf("*************************************************************\n\r");
	printf("* 例程名称   : %s\r\n", "手机蓝牙APP数据交互");	/* 打印例程名称 */
	printf("* 例程版本   : %s\r\n", "V1.0");		  /* 打印例程版本 */
	printf("* 发布日期   : %s\r\n", "20221125");	/* 打印例程日期 */
	printf("* 标准库版本 : TM4C123GH6PZT7\r\n");
	printf("* \r\n");	/* 打印一行空格 */
	printf("* QQ    : 3138372165 \r\n");
	printf("* Email : 3138372165@qq.com \r\n");
	printf("* Copyright www.nameless.tech 无名创新\r\n");
	printf("*************************************************************\n\r");
}




Sensor WP_Sensor;
Testime Time0_Delta;
void TIMER0A_Handler(void)				//系统调度中断函数
{
  Test_Period(&Time0_Delta);
  //再此添加周期任务函数

  TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
}


void Time_init(void)//系统调度定时器初始化
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);							//定时器0使能				
  TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);						//32位周期定时器				
  TimerLoadSet(TIMER0_BASE,TIMER_A,SysCtlClockGet()/200);		//设定装载值,（80M/200）*1/80M=5ms				
  IntEnable(INT_TIMER0A);																		//总中断使能				
  TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT); 					//中断输出, 设置模式;			
  TimerIntRegister(TIMER0_BASE,TIMER_A,TIMER0A_Handler);		//中断函数注册
  IntMasterEnable();			
  TimerEnable(TIMER0_BASE,TIMER_A); 												//定时器使能开始计数
  IntPrioritySet(INT_TIMER0A,USER_INT7);
}



void ConfigureUART(void)
{   
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);// Enable the GPIO Peripheral used by the UART.  
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);// Enable UART0
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);// Configure GPIO Pins for UART mode.
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // Use the internal 16MHz oscillator as the UART clock source.
    // Initialize the UART for console I/O.
    UARTStdioConfig(0, 115200, 16000000);
	  UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
}


/***************************************
函数名:	void UART_SendBytes(uint32_t port,uint8_t *ubuf, uint32_t len)
说明: 发送N个字节长度的数据
入口:	uint32_t port-串口号
			uint8_t *ubuf-待发生数据地址 
			uint32_t len-待发送数据长度
出口:	无
备注:	无
作者:	无名创新
***************************************/
void UART_SendBytes(uint32_t port,uint8_t *ubuf, uint32_t len)
{
	uint32_t ui32Base=UART0_BASE;
	switch(port)
	{
		case 0:		ui32Base=UART0_BASE;	break;
		case 1:		ui32Base=UART1_BASE;	break;
		case 2:		ui32Base=UART2_BASE;	break;
		case 3:		ui32Base=UART3_BASE;	break;
		case 4:		ui32Base=UART4_BASE;	break;
		case 5:		ui32Base=UART5_BASE;	break;
		case 6:		ui32Base=UART6_BASE;	break;
		case 7:		ui32Base=UART7_BASE;	break;
		default:  ui32Base=UART0_BASE;
	}
  while(len--)
  {
    UARTCharPut(ui32Base, *ubuf++);
  }
}



int fputc(int ch, FILE *f){UARTCharPut(UART0_BASE,ch);	return (ch);}
int fgetc(FILE *f) {int ch=UARTCharGet(UART0_BASE);	return (ch);}
union
{
	unsigned char floatByte[4];
	float floatValue;
}FloatUnion;


/***************************************************************************************
@函数名：void Float2Byte(float *FloatValue, unsigned char *Byte, unsigned char Subscript)
@入口参数：FloatValue:float值
			     Byte:数组
		       Subscript:指定从数组第几个元素开始写入
@出口参数：无
功能描述：将float数据转成4字节数据并存入指定地址
@作者：无名小哥
@日期：2020年01月17日
****************************************************************************************/
void Float2Byte(float *FloatValue, unsigned char *Byte, unsigned char Subscript)
{
	FloatUnion.floatValue = (float)2;
	if(FloatUnion.floatByte[0] == 0)//小端模式
	{
		FloatUnion.floatValue = *FloatValue;
		Byte[Subscript]     = FloatUnion.floatByte[0];
		Byte[Subscript + 1] = FloatUnion.floatByte[1];
		Byte[Subscript + 2] = FloatUnion.floatByte[2];
		Byte[Subscript + 3] = FloatUnion.floatByte[3];
	}
	else//大端模式
	{
		FloatUnion.floatValue = *FloatValue;
		Byte[Subscript]     = FloatUnion.floatByte[3];
		Byte[Subscript + 1] = FloatUnion.floatByte[2];
		Byte[Subscript + 2] = FloatUnion.floatByte[1];
		Byte[Subscript + 3] = FloatUnion.floatByte[0];
	}
}

/***************************************************************************************
@函数名：void Byte2Float(unsigned char *Byte, unsigned char Subscript, float *FloatValue)
@入口参数：Byte:数组
			     Subscript:指定从数组第几个元素开始写入
		       FloatValue:float值
@出口参数：无
功能描述：从指定地址将4字节数据转成float数据
@作者：无名小哥
@日期：2020年01月17日
****************************************************************************************/
void Byte2Float(unsigned char *Byte, unsigned char Subscript, float *FloatValue)
{
	FloatUnion.floatByte[0]=Byte[Subscript];
	FloatUnion.floatByte[1]=Byte[Subscript + 1];
	FloatUnion.floatByte[2]=Byte[Subscript + 2];
	FloatUnion.floatByte[3]=Byte[Subscript + 3];
	*FloatValue=FloatUnion.floatValue;
}


#define PACK_HEAD 0xa5   
#define PACK_TAIL 0x5a 

uint8_t bt_app_output_buf[50];
uint8_t bt_app_input_buf[50];
void bluetooth_app_send(float user1,float user2,float user3,float user4,
												float user5,float user6,float user7,float user8)
{
	uint8_t _cnt=0,sum=0;
	bt_app_output_buf[_cnt]=PACK_HEAD;
	_cnt++;
	bt_app_output_buf[_cnt]=32;//数据长度
	_cnt++;
	Float2Byte(&user1,bt_app_output_buf,_cnt);//1
	_cnt+=4;
	Float2Byte(&user2,bt_app_output_buf,_cnt);//5
	_cnt+=4;
	Float2Byte(&user3,bt_app_output_buf,_cnt);//9
	_cnt+=4;
	Float2Byte(&user4,bt_app_output_buf,_cnt);//13
	_cnt+=4;
	Float2Byte(&user5,bt_app_output_buf,_cnt);//17
	_cnt+=4;
	Float2Byte(&user6,bt_app_output_buf,_cnt);//21
	_cnt+=4;
	Float2Byte(&user7,bt_app_output_buf,_cnt);//25
	_cnt+=4;
	Float2Byte(&user8,bt_app_output_buf,_cnt);//29
	_cnt+=4;	
  for(uint16_t i=1;i<_cnt;i++) sum += bt_app_output_buf[i]; 
  bt_app_output_buf[_cnt++]=sum;
	bt_app_output_buf[_cnt++]=PACK_TAIL;
	UART_SendBytes(4,bt_app_output_buf,_cnt);	
}

float bt_user_data[8]={0};
uint16_t bluetooth_ppm[8]={0};
uint8_t bt_update_flag=0;
void bluetooth_app_prase(uint8_t byte)
{
	uint8_t sum=0;
	static uint8_t state=0,cnt=0,data_len=0;
  if(state==0&&byte==PACK_HEAD)//帧头1
  {
    state=1;
    bt_app_input_buf[0]=byte;
  }
	else if(state==1&&byte<50)
	{
		state=2;
	  data_len=byte;
		bt_app_input_buf[1]=byte;
		cnt=0;
	}
	else if(state==2&&data_len>0)
	{
		data_len--;
	  bt_app_input_buf[2+cnt++]=byte;//cnt=32
		if(data_len==0) state = 3;
	}
	else if(state==3)//最后接收数据校验和
	{
		state=4;
		bt_app_input_buf[2+cnt++]=byte;//cnt=33
	}
	else if(state==4&&byte==PACK_TAIL)//帧尾
	{
		bt_app_input_buf[2+cnt++]=byte;//帧尾 cnt=34
		state=0;
		sum=0;
		for(uint16_t i=1;i<cnt;i++)		sum+=bt_app_input_buf[i];
		if(sum==bt_app_input_buf[cnt])//满足和校验
		{
			Byte2Float(bt_app_input_buf,2,&bt_user_data[0]);
			Byte2Float(bt_app_input_buf,6,&bt_user_data[1]);
			Byte2Float(bt_app_input_buf,10,&bt_user_data[2]);
			Byte2Float(bt_app_input_buf,14,&bt_user_data[3]);
			Byte2Float(bt_app_input_buf,18,&bt_user_data[4]);
			Byte2Float(bt_app_input_buf,22,&bt_user_data[5]);
			Byte2Float(bt_app_input_buf,26,&bt_user_data[6]);
			Byte2Float(bt_app_input_buf,30,&bt_user_data[7]);
			for(uint16_t j=0;j<8;j++)
			{
				bluetooth_ppm[j]=(int)bt_user_data[j];
				bt_update_flag=1;
			}
		}
	}
	else state=0;

}


/***************************************
函数名:	void UART4_IRQHandler(void)
说明: UART4中断服务函数
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void UART4_IRQHandler(void)//UART4中断函数
{				
  uint32_t flag = UARTIntStatus(UART4_BASE,1);//获取中断标志 原始中断状态 屏蔽中断标志		
  UARTIntClear(UART4_BASE,flag);//清除中断标志	
  while(UARTCharsAvail(UART4_BASE))//判断FIFO是否还有数据		
  {
		uint8_t ch=UARTCharGet(UART4_BASE);	
		bluetooth_app_prase(ch);
  }
}


/***************************************
函数名:	void UART4_Init(unsigned long bound)
说明: 串口4资源初始化
入口:	unsigned long bound-波特率
出口:	无
备注:	无
作者:	无名创新
***************************************/
void UART4_Init(unsigned long bound)//串口4初始化
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//使能GPIO外设		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);//使能UART外设
  GPIOPinConfigure(GPIO_PC4_U4RX);//GPIO模式配置 PC4--RX PC5--TX 
  GPIOPinConfigure(GPIO_PC5_U4TX);
  GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);//GPIO的UART模式配置
	UARTConfigSetExpClk(UART4_BASE, SysCtlClockGet(), bound,
										 (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
										  UART_CONFIG_PAR_NONE));
  UARTFIFODisable(UART4_BASE);//使能UART4中断	
  UARTIntEnable(UART4_BASE,UART_INT_RX);//使能UART4接收中断		
  UARTIntRegister(UART4_BASE,UART4_IRQHandler);//UART4中断地址注册	
  IntPrioritySet(INT_UART4, USER_INT3);//USER_INT3
} 



int main(void)
{  
		ROM_FPUEnable();//使能浮点单元
		ROM_FPULazyStackingEnable();//浮点延迟堆栈,减少中断响应延迟
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |	SYSCTL_OSC_MAIN);//配置系统时钟
	  initTime();//初始化滴答定时器
    ConfigureUART();//初始化串口0
		UART4_Init(9600);//初始化串口4
    PrintfLogo();//串口打印版本信息
	  OLED_Init();//OLED显示屏初始化  
	  PPM_Init();//初始化PPM
	  Time_init();//定时器1初始化
    while(1)
    {
			Testime t0;
			Test_Period(&t0);
			LCD_clear_L(0,0);display_6_8_string(0,0,"System  Time:  MS");
			LCD_clear_L(0,1);display_6_8_number(0,1,t0.Now_Time);
			LCD_clear_L(0,2);display_6_8_string(0,2,"Time0 Period:  MS");
			LCD_clear_L(0,3);display_6_8_number(0,3,Time0_Delta.Time_Delta);
			
			LCD_clear_L(0,4);  	display_6_8_string(0,4,"ch1-2:");display_6_8_number(40,4,(int16)bt_user_data[0]);
																													 display_6_8_number(80,4,(int16)bt_user_data[1]);
			LCD_clear_L(0,5); 	display_6_8_string(0,5,"ch3-4:");display_6_8_number(40,5,(int16)bt_user_data[2]);
																												   display_6_8_number(80,5,(int16)bt_user_data[3]);
			LCD_clear_L(0,6);  	display_6_8_string(0,6,"ch5-6:");display_6_8_number(40,6,(int16)bt_user_data[4]);
																												   display_6_8_number(80,6,(int16)bt_user_data[5]);
			LCD_clear_L(0,7);  	display_6_8_string(0,7,"ch7-8:");display_6_8_number(40,7,(int16)bt_user_data[6]);
																												   display_6_8_number(80,7,(int16)bt_user_data[7]);
			
			printf("ch1\tch2\tch3\tch14\tch5\tch6\tch7\tch8\t");
      printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",(int16)(bt_user_data[0]),(int16)(bt_user_data[1]),(int16)(bt_user_data[2]),(int16)(bt_user_data[3]),
																							  (int16)(bt_user_data[4]),(int16)(bt_user_data[5]),(int16)(bt_user_data[6]),(int16)(bt_user_data[7]));
			static uint32_t cnt=0;
			cnt+=1;
			//手机端app地面站发送	
			bluetooth_app_send(100*sin(0.1f*cnt),
												 75*cos(0.1f*cnt),
												 50*sin(0.1f*cnt),
												 50*sin(0.1f*cnt),
												 20*sin(0.1f*cnt),
												 20*sin(0.1f*cnt),
												 200*sin(0.1f*cnt),
												 200*sin(0.1f*cnt));
    }
}
