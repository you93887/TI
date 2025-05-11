#ifndef __LED_H
#define __LED_H


//�Ե���LED����������к궨�壬����ʹ��

#define LED_ON  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7, GPIO_PIN_7)
#define LED_OFF GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7, 0)
#define Buzzer_ON  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_5, 0)
#define Buzzer_OFF GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_5, GPIO_PIN_5)
//#define Laser_ON  GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0)
//#define Laser_OFF GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_PIN_0)
//����LED����ĳ�ʼ������	
void LED_GPIO_Config(void);
void Buzzer_Init(void);
//void Laser_Init(void);

#endif
