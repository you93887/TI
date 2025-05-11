#include "Headfile.h"
#include "myiic.h"
#include "mpu6050.h"

uint8_t IMU_ID=0x68; 
/***********************************************************
@函数名：Init_MPU6050
@入口参数：无
@出口参数：无
功能描述：MPU6050初始化
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void MPU6050_Init(void)//MPU6050初始化
{
	IMU_ID=Single_ReadI2C(MPU_ADRESS,WHO_AM_I);
  if(IMU_ID==IMU_MPU6050_ID)//MPU6050
	{
		i2cWrite(MPU_ADRESS,PWR_MGMT_1  , 0x00);//关闭所有中断,解除休眠
		i2cWrite(MPU_ADRESS,SMPLRT_DIV  , 0x00); // sample rate.  Fsample= 1Khz/(<this value>+1) = 1000Hz
		//i2cWrite(MPU_ADRESS,MPU_CONFIG  , 0x02); //内部低通滤波频率，加速度计94hz，陀螺仪98hz
		i2cWrite(MPU_ADRESS,MPU_CONFIG  , 0x03);//内部低通滤波频率，加速度计44hz，陀螺仪42hz
		i2cWrite(MPU_ADRESS,GYRO_CONFIG , 0x10);//1000deg/s
		i2cWrite(MPU_ADRESS,ACCEL_CONFIG, 0x18);// Accel scale 16g (2048 LSB/g)
	}
	else if(IMU_ID==IMU_ICM20689_ID)//ICM20689
	{	
		i2cWrite(MPU_ADRESS,PWR_MGMT_1  , 0x00);//关闭所有中断,解除休眠
		i2cWrite(MPU_ADRESS,SMPLRT_DIV  , 0x00); // sample rate.  Fsample= 1Khz/(<this value>+1) = 1000Hz
		//i2cWrite(MPU_ADRESS,MPU_CONFIG  , 0x02); //内部低通滤波频率，加速度计94hz，陀螺仪98hz		
		i2cWrite(MPU_ADRESS,MPU_CONFIG  , 0x03);//设置陀螺仪、温度内部低通滤波频率范围，陀螺仪41hz，噪声带宽59hz，温度42hz	
		i2cWrite(MPU_ADRESS,GYRO_CONFIG , 0x10);//设置陀螺仪量程，1000deg/s
		i2cWrite(MPU_ADRESS,ACCEL_CONFIG, 0x18);// Accel scale 16g (2048 LSB/g)	
		i2cWrite(MPU_ADRESS,ACCEL_CONFIG2,0x03);//设置加速度计内部低通滤波频率范围，加速度44.8hz，噪声带宽61.5hz
	}
  delay_ms(500);
  IMU_Calibration();
}

/***********************************************************
@函数名：MPU6050_Read_Data
@入口参数：vector3f *gyro,vector3f *accel
@出口参数：无
功能描述：MPU6050数据采集
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void MPU6050_Read_Data(vector3f *gyro,vector3f *accel,float *temperature)//读取MPU6050数据
{
	uint8_t buf[14];
	int16_t temp;
	i2cReadData(MPU_ADRESS,ACCEL_XOUT_H,buf,14);
	accel->x=(int16_t)((buf[0]<<8)|buf[1]);
	accel->y=(int16_t)((buf[2]<<8)|buf[3]);
	accel->z=(int16_t)((buf[4]<<8)|buf[5]);	
	temp		=(int16_t)((buf[6]<<8)|buf[7]);
	gyro->x	=(int16_t)((buf[8]<<8)|buf[9]);
	gyro->y	=(int16_t)((buf[10]<<8)|buf[11]);
	gyro->z	=(int16_t)((buf[12]<<8)|buf[13]);	
	if(IMU_ID==IMU_MPU6050_ID)
	{
		//MPU6050
		//Sensitivity=340 LSB/°C
		//Temperature Offset=35 °C
		*temperature=36.53f+(double)(temp/340.0f);
	}
	if(IMU_ID==IMU_ICM20689_ID)
	{
		//ICM20689
		//Room Temperature Offset=25 °C
		//Sensitivity=326.8 LSB/°C
		//TEMP_degC = ((TEMP_OUT –RoomTemp_Offset)/Temp_Sensitivity) +25degC
		*temperature=25.0f+(double)((temp-25.0f)/326.8f);
	}	
}


Vector3f gyro_offset;
s32 g_Gyro_xoffset = 0, g_Gyro_yoffset = 0, g_Gyro_zoffset = 0;
/***********************************************************
@函数名：IMU_Calibration
@入口参数：无
@出口参数：无
功能描述：陀螺仪开机零偏标定
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void IMU_Calibration(void)
{
  u8 i;
	vector3f gyro_offset_temp;
//	ReadFlashParameterOne(GYRO_X_OFFSET,&gyro_offset_temp.x);
//	ReadFlashParameterOne(GYRO_Y_OFFSET,&gyro_offset_temp.y);
//	ReadFlashParameterOne(GYRO_Z_OFFSET,&gyro_offset_temp.z);	
	if(isnan(gyro_offset_temp.x)==0
		&&isnan(gyro_offset_temp.y)==0
		 &&isnan(gyro_offset_temp.z)==0)//如果之前已经温度校准过，开机时直接用之前校准的数据 
	{
		 gyro_offset.x=gyro_offset_temp.x;
		 gyro_offset.y=gyro_offset_temp.y;
		 gyro_offset.z=gyro_offset_temp.z;
	}
	else
	{
		delay_ms(500);
		for (i = 0; i < 100; i++)			//连续采样30次，一共耗时30*3=90ms
		{
			g_Gyro_xoffset +=Double_ReadI2C(MPU_ADRESS,GYRO_XOUT_H);
			g_Gyro_yoffset +=Double_ReadI2C(MPU_ADRESS,GYRO_YOUT_H);
			g_Gyro_zoffset +=Double_ReadI2C(MPU_ADRESS,GYRO_ZOUT_H);
			delay_ms(10);
		}
		gyro_offset.x =(g_Gyro_xoffset/100);//得到标定偏移
		gyro_offset.y =(g_Gyro_yoffset/100);
		gyro_offset.z =(g_Gyro_zoffset/100);
	}
}

