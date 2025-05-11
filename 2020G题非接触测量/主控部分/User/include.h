#ifndef __INCLUDE_H
#define __INCLUDE_H

#include "stdio.h"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_gpio.h"
#include "debug.h"
#include "fpu.h"
#include "gpio.h"
#include "pin_map.h"
#include "rom.h"
#include "sysctl.h"
#include "SystickTime.h"
#include "systick.h"
#include "Time.h"
//******************************
//串口的一些库
#include "uart.h"
#include "uartstdio.h"

#include "Serial.h"
//******************************
//开启定时器要加入这三个头文件
#include "timer.h"
#include "interrupt.h"
#include "hw_ints.h"

#include "Timer_user.h"
//******************************
//PWM的库  此芯片有两个PWM口
#include "pwm.h" 

#include "PWM_user.h"
//******************************
//ADC的库
#include "adc.h"

#include "ADC_user.h"
//******************************
#include "OLED.h"
#include "OLED_Data.h"
//******************************
#include "LED.h"
//******************************
#include "ButtonsInterrupt.h"
//******************************
#include "Motor.h"

#include "Tof.h"
#include "PID.h"

#endif
