#include "include.h"                  // Device header

/**
  * 函    数：蜂鸣器初始化
  * 参    数：无
  * 返 回 值：无
  */
void Buzzer_Init(void)
{
	  //使能GPIOF时钟
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //配置RGB灯的三个引脚为输出模式
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
	
		/*设置GPIO初始化后的默认电平*/
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);
}

/**
  * 函    数：蜂鸣器开启
  * 参    数：无
  * 返 回 值：无
  */
void Buzzer_ON(void)
{
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);		//设置PB12引脚为低电平
}

/**
  * 函    数：蜂鸣器关闭
  * 参    数：无
  * 返 回 值：无
  */
void Buzzer_OFF(void)
{
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);	//设置PB12引脚为高电平
}

/**
  * 函    数：蜂鸣器状态翻转
  * 参    数：无
  * 返 回 值：无
  */
void Buzzer_Turn(void)
{
	if (GPIOPinRead(GPIO_PORTG_BASE,GPIO_PIN_0) == 0)		//获取输出寄存器的状态，如果当前引脚输出低电平
	{
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);						//则设置引脚为高电平
	}
	else														//否则，即当前引脚输出高电平
	{
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);						//则设置引脚为低电平
	}
}
