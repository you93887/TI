
#ifndef __HEADFILE_H__
#define __HEADFILE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>




#define  USER_INT0  0x00   //PPM
#define  USER_INT1  0x20   //UART2
#define  USER_INT2  0x40   //UART0
#define  USER_INT3  0x60   //UART3
#define  USER_INT4  0x80   //UART1
#define  USER_INT5  0xA0   //UART6
#define  USER_INT6  0xD0   //UART7
#define  USER_INT7  0xE0   //TIMER0

#define ABS(X)  (((X)>0)?(X):-(X))
#define MAX(a,b)  ((a)>(b)?(a):(b))
#define MIN(a,b)  ((a)>(b)?(b):(a))
#define Imu_Sampling_Freq  200
#define WP_Duty_Freq  200
#define WP_Duty_Dt  (1.0f/WP_Duty_Freq)
#define WP_Duty_Dt_Ms  (WP_Duty_Dt*1000)
#define Delta WP_Duty_Dt

#define Baro_Update_Dt  50.0f//20ms“‘…œ

typedef   signed           char int8;
typedef unsigned           char u8;
typedef unsigned           char uint8;
typedef unsigned           char byte;
typedef   signed short     int int16;
typedef unsigned short     int uint16;
typedef unsigned short     int u16;
typedef unsigned long     int u32; 


#define _YAW    0
#define _PITCH  1
#define _ROLL   2


#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_ints.h"
#include "debug.h"
#include "fpu.h"
#include "gpio.h"
#include "pin_map.h"
#include "pwm.h"
#include "rom.h"
#include "sysctl.h"
#include "uart.h"
#include "interrupt.h"
#include "timer.h"
#include "hw_gpio.h"
#include "eeprom.h"
#include "OLED.h"
#include "Openmv.h"
#include "DL1A.h"
/////////////////////////
/*
#include "WP_DataType.h"
#include "Usart.h"
#include "NCLink.h"
#include "uartstdio.h"
#include "RDroneSudio.h"
#include "Time.h"
#include "Time_Cnt.h"
#include "Schedule.h"
#include "CopyRight.h"
#include "ssd1306.h"
#include "oled.h"
#include "LCD_ST7789.h"
#include "mpu6050.h"
#include "Soft_I2C.h"
#include "ist8310.h"
#include "QMC5883.h"
#include "spl06.h"
#include "PPM.h"
#include "wp_flash.h"
#include "PID.h"
#include "Ringbuf.h"
#include "WP_PWM.h"
#include "Sensor.h"
#include "Key.h"
#include "Filter.h"
#include "SINS.h"
#include "Calibration.h"
#include "CalibrationRoutines.h"
#include "US_100.h"
#include "tfmini.h"
#include "NamelessCorun_OpticalFlow.h"
#include "OpticalFlow_LC306.h"
#include "OpticalFlow_Control.h"
#include "NamelessCotrun_SDK.h"
#include "WP_ADC.h"
#include "Bling.h"
#include "GPS.h"
#include "RC.h"
#include "SYSTEM.h"
#include "WP_Ctrl.h"
#include "control_althold.h"
#include "control_poshold.h"
#include "control_land.h"
#include "control_config.h"
#include "Temperature_Ctrl.h"
#include "Compass.h"
*/

#endif

