#ifndef __PID_H
#define	__PID_H

typedef struct
{
	float target_val;   //目标值
	float Error;          /*第 k 次偏差 */
	float LastError;     /* Error[-1],第 k-1 次偏差 */
	float LastError2;
	float SumError;    /* Error[-2],第 k-2 次偏差 */
	float Kp,Ki,Kd;     //比例、积分、微分系数
	float actual_val;     //积分值
	float Output;

}PID;

void PID_Init();
float P_realize(PID*pid,float actual_val);
float PI_realize(PID*pid,float actual_val);
float PID_realize(PID*pid,float actual_val);

void Set_P(uint8_t Kp);
void Set_I(uint8_t Ki);
void Set_D(uint8_t Kd);


#endif
