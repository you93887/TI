#ifndef __MOTOR_H
#define __MOTOR_H
#include "include.h"
void Motor_Init(void);
void Motor_SetStep_up(int16_t step_num_up);
void Motor_SetStep_down(int16_t step_num_down);
void Motor_speed(int16_t motor_up, int16_t motor_down);
#endif
