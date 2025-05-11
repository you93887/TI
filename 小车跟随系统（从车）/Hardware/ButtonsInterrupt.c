/***************************************************
 * @brief   关于TM4C123GH6PZ芯片的GPIO按键函数
 *
 * @date    2024/6/21
 * @author  qihang chen
****************************************************/
#include "include.h"

/*
************************************************************************
此函数是初始化的PD6
按键外部中断初始化函数
************************************************************************
*/
void ButtonsInit(void)
{
	//使能PF时钟
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	
	//设置PF4为输入，上拉（没按就是高电平，按下就是低电平）
	GPIODirModeSet(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_DIR_MODE_IN);	
	
	//方向为输入
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);//推挽上拉

	//PF4配置为下降沿中断
	GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_FALLING_EDGE);//下降沿
	
	//给PF组注册一个中断函数
	GPIOIntRegister(GPIO_PORTD_BASE, GPIO_interrupt);
	
	// 设置中断优先级，TM4C123G的中断优先级有8个，0最高
	IntPrioritySet(INT_GPIOD, 0xE0);
	
	//开启PD6的中断
	GPIOIntEnable(GPIO_PORTD_BASE, GPIO_INT_PIN_6);
	IntEnable(INT_GPIOD);
	IntMasterEnable();
}


/******************************************************************************************************************
*函数名: GPIO_interrupt
*描	 述：GPIO外部中断处理函数
*输	 入：无
*线  路：PD6按键
******************************************************************************************************************/
extern uint16_t key_num;
//extern float speed;
float aaa = 10;
void GPIO_interrupt(void)
{
	static uint8_t trigger=0;
	
	//获取中断状态
	uint32_t s = GPIOIntStatus(GPIO_PORTD_BASE, true);
	//清除发生的中断标志
 	GPIOIntClear(GPIO_PORTD_BASE, s);
    
  	if( (s & GPIO_PIN_6) == GPIO_PIN_6 )
  	{
		//按键消抖
		delay_ms(50);
		if(!GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_6))//等待按键松开
		{
			
			//在此下面加入一些中断函数
//			if(speed >= 100 )
//				aaa = -10;
//			if(speed <= -100)
//				aaa = 10;
//			speed = speed + aaa;
		}
		delay_ms(50);
	}
	//如果有多个中断需要if( (s&GPIO_PIN_4) == GPIO_PIN_4 )
}


/*
************************************************************************
SW1 是F4       SW2 是F0
按键初始化函数
不用中断需要配合轮询扫描函数
************************************************************************
*/

#define ReadKey1  GPIOPinRead(GPIO_PORTF_BASE , GPIO_PIN_4)
#define ReadKey2  GPIOPinRead(GPIO_PORTF_BASE , GPIO_PIN_0)
void Key_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4,GPIO_DIR_MODE_IN);//SW1
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);  //上拉推挽输入
	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0,GPIO_DIR_MODE_IN);//SW2
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);  //上拉推挽输入
}

