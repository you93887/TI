#ifndef __LED_H
#define __LED_H


//对点亮LED这个操作进行宏定义，方便使用

#define LED_ON  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7, GPIO_PIN_7)
#define LED_OFF GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7, 0)
 
//定义LED外设的初始化函数	
void LED_GPIO_Config(void);
 
#endif
