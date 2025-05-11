#include "include.h"


PID PID_Motor1_Speed;
PID PID_Motor2_Speed;


void PID_Init()
{
	//速度PI
	
	PID_Motor1_Speed.Error=0.0;
	PID_Motor1_Speed.actual_val=0.0;
	PID_Motor1_Speed.Kd = -2;			//18    50  
	PID_Motor1_Speed.Ki=0.0;
	PID_Motor1_Speed.Kp=-30 ;            //PID_Speed1_Kp7    15
	PID_Motor1_Speed.LastError=0.0;
	PID_Motor1_Speed.SumError=0.0;
	PID_Motor1_Speed.target_val=0;
	
	
	PID_Motor2_Speed.Error=0.0;
	PID_Motor2_Speed.actual_val=0.0;
	PID_Motor2_Speed.Kd=-2;			//18    50
	PID_Motor2_Speed.Ki=0;
	PID_Motor2_Speed.Kp=-30;   	//7    15    
	PID_Motor2_Speed.LastError=0.0;
	PID_Motor2_Speed.SumError=0.0;
	PID_Motor2_Speed.target_val=0;

}


float P_realize(PID*pid,float actual_val)
{
	pid->actual_val=actual_val;
	pid->Error=pid->target_val-pid->actual_val;
	
	pid->actual_val=pid->Kp*pid->Error;
	return pid->actual_val;
}

float PI_realize(PID*pid,float actual_val)
{
	pid->actual_val=actual_val;
	pid->Error=pid->target_val-pid->actual_val;
	pid->SumError+=pid->Error;
	
	pid->actual_val=pid->Kp*pid->Error+pid->Ki*pid->SumError;
	return pid->actual_val;
}

float PID_realize(PID*pid,float actual_val)
{
	pid->actual_val = actual_val;
	pid->Error = pid->target_val - pid->actual_val;
	pid->SumError += pid->Error;
	
	pid->actual_val = pid->Kp * pid->Error + pid->Ki * pid->SumError + pid->Kd * (pid->Error - pid->LastError);
	pid->LastError=pid->Error;
	return pid->actual_val;
}


