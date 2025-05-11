#include "include.h"

void Motor_Init()
{
    //使能GPIOE  H时钟
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOH)){};
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)){};
    GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, GPIO_PIN_0);
	GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, GPIO_PIN_1);
	GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, GPIO_PIN_2);
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0);
	GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_2, 0);
	GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_1, 0);
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0);
	GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_0, 0);
	PWM_Init();
}



void Motor_speed(int16_t motor_up, int16_t motor_down)
{
	// 限制速度，最大300HZ即1s转600/256°
	if(motor_up > 999)
		motor_up = 999;
	if(motor_up < -999)
		motor_up = -999;
	if(motor_down > 999)
		motor_down = 999;
	if(motor_down < -999)
		motor_down = -999;
	
	
	if( motor_up < 20 && motor_up > -20 )
		SetCycle(PWM0_BASE,PWM_OUT_1, 1200);
	else if(motor_up >= 0)
	{
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0);
		SetCycle(PWM0_BASE,PWM_OUT_1, motor_up);
	}
	else
	{
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_PIN_0);
		SetCycle(PWM0_BASE,PWM_OUT_1, -motor_up);
	}
	
	
	if( motor_down < 20 && motor_down > -20 )
		SetCycle(PWM0_BASE,PWM_OUT_2, 1200);
	else if(motor_down >= 0)
	{
		GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_0, 0);
		SetCycle(PWM0_BASE,PWM_OUT_2, motor_down);
	}
	else
	{
		GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_0, GPIO_PIN_0);
		SetCycle(PWM0_BASE,PWM_OUT_2, -motor_down);
	}
}


/**
  * 函    数：步进电机运动
  * 参    数：
  * 返 回 值：
  */
void Motor_SetStep_up(int16_t step_num_up)
{
	if (step_num_up >= 0)
	{
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0);
		delay_us(100);
		for(int16_t i = 0; i < step_num_up; i++)
		{
			GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_1, GPIO_PIN_1);
			delay_us(100);
			GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_1, 0);
			delay_us(100);
		}
	}
	else
	{
		step_num_up = -step_num_up;
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_PIN_0);
		delay_us(100);
		for(int16_t i = 0; i < step_num_up; i++)
		{
			GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_1, GPIO_PIN_1);
			delay_us(100);
			GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_1, 0);
			delay_us(100);
		}
	}
}



void Motor_SetStep_down(int16_t step_num_down)
{
	if (step_num_down >= 0)
	{
		GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_0, 0);
		delay_us(100);
		for(int16_t i = 0; i < step_num_down; i++)
		{
			GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_2, GPIO_PIN_2);
			delay_us(100);
			GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_2, 0);
			delay_us(100);
		}
	}
	else
	{
		step_num_down = -step_num_down;
		GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_0, GPIO_PIN_0);
		delay_us(100);
		for(int16_t i = 0; i < step_num_down; i++)
		{
			GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_2, GPIO_PIN_2);
			delay_us(100);
			GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_2, 0);
			delay_us(100);
		}
	}
}

