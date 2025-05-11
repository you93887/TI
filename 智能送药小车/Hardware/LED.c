#include "include.h"
#include "LED.h"
 
void LED_GPIO_Config(void)
{
    //使能GPIOF时钟
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //配置RGB灯的三个引脚为输出模式
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_7);
	
}

void LED_Init()
{
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
			while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
      {	
      }
			while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
      {	
      }
		  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_7|GPIO_PIN_6);
	    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7, 0);
			GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6);
	    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0);
	    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0);
}

void BEEP_Init()
{
			 SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
			 while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOG))
       {
       }
			 GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_1);
		   GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, GPIO_PIN_1);
}

void KEY_Init()
{
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
			GPIODirModeSet(GPIO_PORTK_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_DIR_MODE_IN);
			GPIODirModeSet(GPIO_PORTJ_BASE, GPIO_PIN_2 , GPIO_DIR_MODE_IN);
			GPIOPadConfigSet(GPIO_PORTK_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);  //上拉推挽输入
      GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_2 , GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);  //上拉推挽输入	
}
