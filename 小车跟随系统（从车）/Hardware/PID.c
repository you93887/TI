#include "include.h"
#include "Pid.h"


PID PID_Motor1_Speed;
PID PID_Motor2_Speed;
PID PID_RedSensor_Speed;


uint8_t PID_Speed1_Kp;
uint8_t PID_Speed1_Ki;
uint8_t PID_Speed1_Kd;

uint8_t PID_Speed2_Kp;
uint8_t PID_Speed2_Ki;
uint8_t PID_Speed2_Kd;



uint8_t PIDSpeed_Kd;


void Set_P(uint8_t Kp)
{
		PID_Speed1_Kp = Kp;
}

void Set_I(uint8_t Ki)
{
		PID_Speed1_Ki = Ki;
}

void Set_D(uint8_t Kd)
{
		PID_Speed1_Kd = Kd;
}

void PID_Init()
{
	//速度PI
	PID_Motor1_Speed.Error=0.0;
	PID_Motor1_Speed.actual_val=0.0;
	PID_Motor1_Speed.Kd=0;
	PID_Motor1_Speed.Ki=60;
	PID_Motor1_Speed.Kp=50;//PID_Speed1_Kp
	PID_Motor1_Speed.LastError=0.0;
	PID_Motor1_Speed.SumError=0.0;
	PID_Motor1_Speed.target_val=0;
	PID_Motor1_Speed.LastError2=0;
	PID_Motor1_Speed.Output=0;
	
	PID_Motor2_Speed.Error=0.0;
	PID_Motor2_Speed.actual_val=0.0;
	PID_Motor2_Speed.Kd=0;
	PID_Motor2_Speed.Ki=4;
	PID_Motor2_Speed.Kp=108;
	PID_Motor2_Speed.LastError=0.0;
	PID_Motor2_Speed.SumError=0.0;
	PID_Motor2_Speed.target_val=0;
	PID_Motor1_Speed.LastError2=0;

	PID_RedSensor_Speed.Error=0.0;
	PID_RedSensor_Speed.actual_val=0.0;
	PID_RedSensor_Speed.Kd=PID_Speed1_Kd;
	PID_RedSensor_Speed.Ki=PID_Speed1_Ki;
	PID_RedSensor_Speed.Kp=-10;
	PID_RedSensor_Speed.LastError=0.0;
	PID_RedSensor_Speed.SumError=0.0;
	PID_RedSensor_Speed.target_val=0.00;

	

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
	pid->actual_val=actual_val;
	pid->Error=pid->target_val-pid->actual_val;
	pid->SumError+=pid->Error;
	
	pid->Output+=pid->Kp*pid->Error+pid->Ki*pid->SumError+pid->Kd*(pid->LastError2+pid->Error-2*pid->LastError);
	pid->LastError2=pid->LastError;
	pid->LastError=pid->Error;
	return pid->Output;
}


