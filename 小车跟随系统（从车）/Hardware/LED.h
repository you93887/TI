#ifndef __LED_H
#define __LED_H


//�Ե���LED����������к궨�壬����ʹ��

#define LED_ON  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7, GPIO_PIN_7)
#define LED_OFF GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7, 0)
 
//����LED����ĳ�ʼ������	
void LED_GPIO_Config(void);
 
#endif
