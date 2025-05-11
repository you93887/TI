#include "include.h"
#include "LED.h"
 
void LED_GPIO_Config(void)
{
    //使能GPIOF时钟
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //配置RGB灯的三个引脚为输出模式
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_7);
	
}
