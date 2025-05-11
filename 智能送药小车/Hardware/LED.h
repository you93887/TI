#ifndef __LED_H
#define __LED_H


//�Ե���LED����������к궨�壬����ʹ��

#define LED_ON  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7, GPIO_PIN_7)
#define LED_OFF GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7, 0)
#define ReadKey1  GPIOPinRead(GPIO_PORTK_BASE , GPIO_PIN_0)
#define ReadKey2  GPIOPinRead(GPIO_PORTK_BASE , GPIO_PIN_1)
#define ReadKey3  GPIOPinRead(GPIO_PORTK_BASE , GPIO_PIN_2)
#define ReadKey4  GPIOPinRead(GPIO_PORTK_BASE , GPIO_PIN_3)
#define Object    GPIOPinRead(GPIO_PORTJ_BASE , GPIO_PIN_2)

//����LED����ĳ�ʼ������	
void LED_GPIO_Config(void);
void LED_Init();
void BEEP_Init();
void KEY_Init();

#endif
