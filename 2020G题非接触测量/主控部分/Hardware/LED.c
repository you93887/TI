#include "include.h"

 
void LED_GPIO_Config(void)
{
    //ʹ��GPIOFʱ��
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //����RGB�Ƶ���������Ϊ���ģʽ
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