#include "include.h"                  // Device header



void Red_Sensor_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
	GPIODirModeSet(GPIO_PORTH_BASE,  GPIO_PIN_7,GPIO_DIR_MODE_IN);
	GPIODirModeSet(GPIO_PORTB_BASE,  GPIO_PIN_2|GPIO_PIN_3,GPIO_DIR_MODE_IN);
  GPIODirModeSet(GPIO_PORTG_BASE,  GPIO_PIN_7|GPIO_PIN_3|GPIO_PIN_2,GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_2|GPIO_PIN_3,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPD);
	GPIOPadConfigSet(GPIO_PORTG_BASE,GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_7,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPD);
	GPIOPadConfigSet(GPIO_PORTH_BASE,GPIO_PIN_7,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPD);
}


//gray_state.gray.bit9 =GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_0)!=0?0x01:0x00;
//	gray_state.gray.bit10=GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_1)!=0?0x01:0x00;
//	gray_state.gray.bit11=GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_2)!=0?0x01:0x00;
//	gray_state.gray.bit12=GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_3)!=0?0x01:0x00;
	
//uint16_t Red_Sensor_Status1(void)
//{
//	return GPIO_ReadInputDataBit(GPIOA,Red_Sensor_R);
//}

//uint16_t Red_Sensor_Status2(void)
//{
//	return GPIO_ReadInputDataBit(GPIOA,Red_Sensor_M);
//}

//uint16_t Red_Sensor_Status3(void)
//{
//	return GPIO_ReadInputDataBit(GPIOA,Red_Sensor_L);
//}
