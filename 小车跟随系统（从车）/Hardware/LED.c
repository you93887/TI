#include "include.h"
#include "LED.h"
 
void LED_GPIO_Config(void)
{
    //ʹ��GPIOFʱ��
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //����RGB�Ƶ���������Ϊ���ģʽ
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_7);
	
}
