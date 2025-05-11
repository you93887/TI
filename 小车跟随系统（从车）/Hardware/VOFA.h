#ifndef __VOFA_H
#define	__VOFA_H
void Float_to_Byte(float Fdata,  uint8_t *ArrayByte);
void JustFloat(void);

float Get_Data(void);
void USART_PID_Adjust(uint8_t Motor_n);
#endif
