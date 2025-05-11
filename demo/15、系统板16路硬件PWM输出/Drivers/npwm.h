#ifndef __NPWM_H
#define __NPWM_H


void PWM0_Init(void);
void PWM1_Init(void);

void PWM_Output(uint16_t width1,uint16_t width2,uint16_t width3,uint16_t width4);


void steer_servo_pwm_m1p0(uint16_t us);
void steer_servo_pwm_m1p1(uint16_t us);
void steer_servo_pwm_m1p2(uint16_t us);
void steer_servo_pwm_m1p3(uint16_t us);
void steer_servo_pwm_m1p4(uint16_t us);
void steer_servo_pwm_m1p5(uint16_t us);
void steer_servo_pwm_m1p6(uint16_t us);
void steer_servo_pwm_m1p7(uint16_t us);
#endif




