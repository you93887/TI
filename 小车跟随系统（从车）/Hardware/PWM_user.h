#ifndef __PWM_USER_H
#define __PWM_USER_H




/*�����궨��*********************/


//**************�޸��������ž�����ʹ��*****************


/*********************�����궨��*/

/*��������*********************/
void PWM_Init(void);
void SetCycle(uint32_t ui32Base,uint32_t ui32PWMOut,uint32_t cycle);
void SetDuty(uint32_t ui32Base,uint32_t ui32PWMOut,float duty);
/*********************��������*/	
#endif
