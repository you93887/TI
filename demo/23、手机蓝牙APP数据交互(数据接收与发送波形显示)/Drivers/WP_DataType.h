/* Copyright (c)  2018-2025 Wuhan Nameless Innovation Technology Co.,Ltd. All rights reserved.*/
/*----------------------------------------------------------------------------------------------------------------------/
*               ������ֻ��������ѧϰʹ�ã���Ȩ����Ȩ���������ƴ��Ŷӣ������ƴ��Ŷӽ��ɿس���Դ���ṩ�������ߣ�
*               ������ҪΪ�����ƴ��Ŷ��ṩ������δ��������ɣ����ý�Դ�����ṩ�����ˣ����ý�Դ����ŵ����Ϲ�����������أ� 
*               �������Դ�����Ĳ�����緢��������Ϊ�������ƴ��Ŷӽ���֮�Է��ɽ��������
-----------------------------------------------------------------------------------------------------------------------/
*               ������Ϣ���ܶ���ֹ��ǰ�����������˳���������
*               ��Դ���ף���ѧ����ϧ��ף������Ϯ�����׳ɹ�������
*               ѧϰ�����ߣ��������Ƽ���DJI��ZEROTECH��XAG��AEE��GDU��AUTEL��EWATT��HIGH GREAT�ȹ�˾��ҵ
*               ��ְ�����뷢�ͣ�15671678205@163.com���豸ע��ְ����λ����λ��������
*               �����ƴ���Դ�ɿ�QQȺ��540707961
*               CSDN���ͣ�http://blog.csdn.net/u011992534
*               �ſ�ID��NamelessCotrun����С��
*               Bվ��ѧ��Ƶ��https://space.bilibili.com/67803559/#/video
*               �ͻ�ʹ���ĵá��Ľ������������http://www.openedv.com/forum.php?mod=viewthread&tid=234214&extra=page=1
*               �Ա����̣�https://shop348646912.taobao.com/?spm=2013.1.1000126.2.5ce78a88ht1sO2
*               �ٶ�����:�����ƴ���Դ�ɿ�
*               ��˾����:www.nameless.tech
*               �޸�����:2019/4/12
*               �汾����Ӯ�ߡ���CarryPilot_V1.0
*               ��Ȩ���У�����ؾ���
*               Copyright(C) 2017-2025 �人�������¿Ƽ����޹�˾ 
*               All rights reserved
*               ��Ҫ��ʾ��
*               �����Ա�����ת�ֵķɿء��������ѡ�����ѧ�ܵĶ����Խ��ۺ�Ⱥѧϰ������
*               ����ֱ�����������������������ϣ��������´�����������Ȩ�����˲��ý�
*               ���ϴ��봫���Ϲ��������أ�������ı��ΪĿ���������ϴ��룬�����д˲�
*               ���ߣ���˾����ǰ��֪����1���ڼ�ʱ�����������ѧУ����λ����������
*               ������ַ��Ϣ�ᱻ�����ڹ�˾�������ٷ�΢�Ź���ƽ̨���ٷ��������͡�֪��
*               ר���Լ��Ա�������ҳ���Թ�ʾ���棬����������Ϊ�����Ϊ�����۵㣬Ӱ��
*               ��ѧ���ҹ���������������ܿ�ͺ������˻����������������ء�
*               �����Ϊ����˾����ش���ʧ�ߣ����Է���;���������л���ĺ�����лл������
----------------------------------------------------------------------------------------------------------------------*/
#ifndef __WP_DATATYPE_H
#define __WP_DATATYPE_H



#include "math.h"
#include "WP_Math.h"


#define DEG2RAD (PI / 180.0f)
#define RAD2DEG (180.0f / PI)


#define int16  short
#define uint16 unsigned short
#define int32  int
#define uint32 unsigned int
#define uint8  unsigned char
#define s32    int32	


#define Int_Sort (int16_t)
#define TRUE  true
#define FALSE false
	
typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;




typedef struct
{
int16_t x;
int16_t y;
int16_t z;	
}vector3i;






typedef struct
{
  int32_t x;
  int32_t y;
}Vector2i;

typedef struct
{
  float x;
  float y;
}Vector2f;


typedef struct
{
  float x;
  float y;
  float z;
}Vector3f;


typedef struct
{
  float q0;
  float q1;
  float q2;
  float q3;
}Vector4q;


typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
}Vector3i;



typedef struct
{
  float E;
  float N;
  float U;
}Vector3_Nav;

typedef struct
{
  float E;
  float N;
}Vector2f_Nav;



typedef struct
{
  int32_t lat;
  int32_t lng;
}Vector2_Nav;



typedef struct
{
  float x;
  float y;
  float z;
}Vector3_Body;


typedef struct
{
  float Pit;
  float Rol;
}Vector2_Ang;


typedef struct
{
  float Pit;
  float Rol;
}Vector2_Body;


typedef struct
{
  float Pit;
  float Rol;
  float Yaw;
}Vector3f_Body;


typedef struct
{
  float North;
  float East;
}Vector2_Earth;

typedef struct
{
  Vector3f a;
  Vector3f b;
  Vector3f c;
}Matrix3f;



typedef struct
{
  float max;
  float min;
  float middle;
  float deadband;
}Vector_RC;


typedef struct
{
  Vector3f gyro_raw;
  Vector3f accel_raw;
  Vector3f mag_raw;
	float acce_filter[3];
	float temperature;
  float baro_temp_raw;
	float baro_pressure_raw;
	float baro_presure_offset;
	float baro_altitude;
	float last_baro_altitude;
	float baro_altitude_div;
	float last_baro_altitude_div;
	float baro_altitude_acc;
	uint8_t imu_updtate_flag;
	uint8_t baro_updtate_flag;
	uint8_t us100_updtate_flag;
	uint8_t tfmini_updtate_flag;
}Sensor;

#define vector3f Vector3f
#define Axis_Num  3
#define Num  50
typedef struct
{
 float Position[Axis_Num];//λ�ù�����
 float Speed[Axis_Num];//�ٶȹ�����
 float Acceleration[Axis_Num];//���ٶȹ�����
 float Pos_History[Axis_Num][Num];//��ʷ�ߵ�λ��
 float Vel_History[Axis_Num][Num];//��ʷ�ߵ��ٶ�
 float Acce_History[Axis_Num][Num];//��ʷ�ߵ��ٶ�
 float Acce_Bias[Axis_Num];//�ߵ����ٶ�Ư����������
 float Acce_Bias_All[Axis_Num];//�ߵ����ٶ�Ư����������
 float Last_Acceleration[Axis_Num];
 float Last_Speed[Axis_Num];
}SINS;

typedef struct
{
  uint8_t Mpu_Health;
  uint8_t Baro_Health;
  uint8_t Mag_Health;
  uint8_t Gps_Health;
  uint8_t Hcsr04_Health;
}Sensor_Health;

#endif

