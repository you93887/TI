#ifndef __ADC_user_H
#define __ADC_user_H


void ADC_Init(void);
uint16_t Get_ADC_Value(void);
uint16_t filter(int new_value);

#endif

