#ifndef __LED_H
#define __LED_H


//对点亮LED这个操作进行宏定义，方便使用

#define LED_ON  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7, GPIO_PIN_7)
#define LED_OFF GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7, 0)
#define Buzzer_ON  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_5, 0)
#define Buzzer_OFF GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_5, GPIO_PIN_5)
//#define Laser_ON  GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0)
//#define Laser_OFF GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_PIN_0)
//定义LED外设的初始化函数	
void LED_GPIO_Config(void);
void Buzzer_Init(void);
//void Laser_Init(void);

#endif
