#ifndef __PWM_USER_H
#define __PWM_USER_H




/*参数宏定义*********************/


//**************修改任意引脚均可以使用*****************


/*********************参数宏定义*/

/*函数声明*********************/
void PWM_Init(void);
void SetCycle(uint32_t ui32Base,uint32_t ui32PWMOut,uint32_t cycle);
void SetDuty(uint32_t ui32Base,uint32_t ui32PWMOut,float duty);
/*********************函数声明*/	
#endif
