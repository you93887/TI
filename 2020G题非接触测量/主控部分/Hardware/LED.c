#include "include.h"

 
void LED_GPIO_Config(void)
{
    //使能GPIOF时钟
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //配置RGB灯的三个引脚为输出模式
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_7);
	LED_OFF;
}

void Buzzer_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_5);
	
	Buzzer_OFF;
}

//void Laser_Init(void)
//{
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
//    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);
//	
//	Laser_OFF;
//}