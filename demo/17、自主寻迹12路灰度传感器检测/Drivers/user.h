#ifndef __USER_H
#define __USER_H


/***********************************************************************************************************************/
#define left_motor_encoder_dir_default   0
#define right_motor_encoder_dir_default  0
#define left_motion_dir_default   0
#define right_motion_dir_default  0
#define tire_radius_cm_default  2.40f			 //��̥�뾶����λΪcm
#define pulse_cnt_per_circle_default  1060 //ʵ������ת��һ�ܵ��������� 530 1080 40750
#define servo_median_value1_default   1500  //�����е��ֵ
#define servo_median_value2_default   1500  //�����е��ֵ
#define rangefinder_type_default 0

#define no_voltage_enable_default 0
#define no_voltage_upper_default 7.2f
#define no_voltage_lower_default 5.5f


/***********************************************************************************************************************/
#define balance_speed_ctrl_enable_default 1			//��ƽ�����ʱ,�ٶȿ���ʹ��Ĭ��ֵ
#define balance_steer_ctrl_enable_default 0			//��ƽ�����ʱ,ת�����ʹ��Ĭ��ֵ
#define balance_ctrl_number_enable_default 1		//��ƽ�����ģʽĬ��ֵ,��δ�õ�



/***********************************************************************************************************************/
#define speed_expect_default 			80.0f			//����Ѱ��ʱ���ٶ�Ĭ���趨ֵ
#define work_mode_default   			0 				//С������ģʽ����Ĭ��ֵ



/***********************************************************************************************************************/
#define turn_kp_default1						30.0f 	//���ں���Թ�ʶ��켣ʱ,����Ѱ����λ�ÿ�������������KP	
#define turn_ki_default1						0.0f		//���ں���Թ�ʶ��켣ʱ,����Ѱ����λ�ÿ��������ֲ���KI	
#define turn_kd_default1						150			//���ں���Թ�ʶ��켣ʱ,����Ѱ����λ�ÿ�����΢�ֲ���KD	
#define turn_scale_default        	0.15f		//ת����������ת����������������ʱ������ת��ϵ��
#define turn_kp_default2						20.0f		//�����Ӿ�openmvʶ��켣ʱ,����Ѱ����λ�ÿ�������������KP
#define turn_ki_default2						0.0f		//�����Ӿ�openmvʶ��켣ʱ,����Ѱ����λ�ÿ��������ֲ���KI
#define turn_kd_default2						5.0f	  //�����Ӿ�openmvʶ��켣ʱ,����Ѱ����λ�ÿ�����΢�ֲ���KD	



/***********************************************************************************************************************/
#define steer_gyro_kp_default  		1.5f			//ת�������ǿ���ʱ�����ٶ��ڻ�����ϵ��KP
#define steer_gyro_ki_default  		0.0f			//ת�������ǿ���ʱ�����ٶ��ڻ�����ϵ��KI
#define steer_gyro_kd_default  		3.0f			//ת�������ǿ���ʱ�����ٶ��ڻ�΢��ϵ��KD	
#define	steer_gyro_scale_default	0.5f			//ת�������ǿ���ʱ�����ٶ��ڻ����������ת����������������ʱ������ת��ϵ��



/***********************************************************************************************************************/
#define speed_kp_default  				8.0f			//�ٶȿ�������������KP
#define	speed_ki_default					0.6f			//�ٶȿ��������ֲ���KI
#define	speed_kd_default					0.0f			//�ٶȿ�����΢�ֲ���KD



/***********************************************************************************************************************/
#define balance_angle_kp1_default  				135.0f	//��ƽ�ⵥ���Ƕȿ�������������KP
#define balance_angle_ki1_default  				0.0f		//��ƽ�ⵥ���Ƕȿ��������ֲ���KI
#define balance_angle_kd1_default  				5.0f		//��ƽ�ⵥ���Ƕȿ�����΢�ֲ���KD
#define balance_angle_default       			51.5f

#define balance_speed_kp_default        	10			//��ƽ��ʱ�ٶȿ�������������KP
#define balance_speed_ki_default					3			  //��ƽ��ʱ�ٶȿ��������ֲ���KI
#define balance_speed_kd_default        	0				//��ƽ��ʱ�ٶȿ�����΢�ֲ���KD



/***********************************************************************************************************************/
#define balance_speed_scale_default 			0.04f		//��ƽ��˫������ʱ�����ٶȿ��������ת����������̬���ʱ������ת��ϵ��  0.04f

#define balance_angle_kp2_default  				95.0f		//��ƽ��˫������ʱ,�Ƕȿ�������������KP
#define balance_angle_ki2_default  				0.0f		//��ƽ��˫������ʱ,�Ƕȿ��������ֲ���KI
#define balance_angle_kd2_default  				3.5f		//��ƽ��˫������ʱ,�Ƕȿ�����΢�ֲ���KD

#define balance_gyro_kp_default  					8.0f		//��ƽ��˫������ʱ,���ٶȿ�������������KP
#define balance_gyro_ki_default  					0.5f		//��ƽ��˫������ʱ,���ٶȿ��������ֲ���KI
#define balance_gyro_kd_default  					1.5f		//��ƽ��˫������ʱ,���ٶȿ�����΢�ֲ���KD



#endif




