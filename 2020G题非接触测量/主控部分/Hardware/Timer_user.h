#ifndef __TIMER_USER_H
#define __TIMER_USER_H



typedef struct
{
  float Last_Time;
  float Now_Time;
  float Time_Delta;
  uint16_t Time_Delta_INT;//µ¥Î»ms
}Testime;

void Test_Period(Testime *Time_Lab);

void Timer_init(void);
void TIMER0A_Handler(void);  

#endif
