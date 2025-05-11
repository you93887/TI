#ifndef __REDSENSOR_H__
#define __REDSENSOR_H__

#define Read_RS_L   GPIOPinRead(GPIO_PORTG_BASE,GPIO_PIN_3)!=0?0x01:0x00
#define Read_RS_LM  GPIOPinRead(GPIO_PORTG_BASE,GPIO_PIN_2)!=0?0x01:0x00
#define Read_RS_RM  GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_2)!=0?0x01:0x00
#define Read_RS_R   GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_3)!=0?0x01:0x00
#define Read_RS_M1   GPIOPinRead(GPIO_PORTH_BASE,GPIO_PIN_7)!=0?0x01:0x00
#define Read_RS_M2   GPIOPinRead(GPIO_PORTG_BASE,GPIO_PIN_7)!=0?0x01:0x00
void Red_Sensor_Init(void);

	
#endif
