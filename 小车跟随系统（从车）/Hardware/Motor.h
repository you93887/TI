#ifndef __MOTOR_H
#define __MOTOR_H

void Motor_Init(void);

void Encoder_Init(void);
void Motor_SetSpeed(float Motor1_Speed,float Motor2_Speed);
void Motor_SetPwm(float percent1,float percent2);
void Motor_SetPwm1(float percent);
void Motor_SetPwm2(float percent);
uint32_t Get_PWM(void);
//void WTimer0AIntHandler(void);
//void Timer0Init();
#endif
