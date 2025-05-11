#ifndef __BUTTONSINTERRUPT_H
#define __BUTTONSINTERRUPT_H

#define ReadKey1  GPIOPinRead(GPIO_PORTK_BASE , GPIO_PIN_0)
#define ReadKey2  GPIOPinRead(GPIO_PORTK_BASE , GPIO_PIN_1)
#define ReadKey3  GPIOPinRead(GPIO_PORTK_BASE , GPIO_PIN_2)
#define ReadKey4  GPIOPinRead(GPIO_PORTK_BASE , GPIO_PIN_3)

void ButtonsInit(void);
void GPIO_interrupt(void);
void Key_Init(void);
#endif

