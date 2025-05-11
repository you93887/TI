#include "headfile.h"
#include "nqei.h"
#include "user.h"
#include "SystickTime.h"

extern motor_config trackless_motor;


encoder NEncoder;
Testime timer_qei1,timer_qei0;


/***************************************
������:	void QEI0_IRQHandler(void)
˵��: QEI0�жϷ�����
���:	��
����:	��
��ע:	��
����:	��������
***************************************/
void QEI0_IRQHandler(void)
{
	Test_Period(&timer_qei0);
  QEIIntClear(QEI0_BASE,QEI_INTTIMER);
	NEncoder.right_motor_period_ms=timer_qei0.Time_Delta_INT;
	NEncoder.right_motor_cnt=QEIVelocityGet(QEI0_BASE);
	if(trackless_motor.right_encoder_dir_config==0)
	{
		if(QEIDirectionGet(QEI0_BASE)==1)	NEncoder.right_motor_dir=1.0f;
		else NEncoder.right_motor_dir=-1.0f;
	}
	else
	{
		if(QEIDirectionGet(QEI0_BASE)==1)	NEncoder.right_motor_dir=-1.0f;
		else NEncoder.right_motor_dir=1.0f;
	}
	NEncoder.right_motor_total_cnt+=NEncoder.right_motor_cnt*NEncoder.right_motor_dir;
}

/***************************************
������:	void QEI1_IRQHandler(void)
˵��: QEI1�жϷ�����
���:	��
����:	��
��ע:	��
����:	��������
***************************************/
void QEI1_IRQHandler(void)
{
	Test_Period(&timer_qei1);
  QEIIntClear(QEI1_BASE,QEI_INTTIMER);
	NEncoder.left_motor_period_ms=timer_qei1.Time_Delta_INT;
	NEncoder.left_motor_cnt=QEIVelocityGet(QEI1_BASE);
	if(trackless_motor.left_encoder_dir_config==0)
	{
		if(QEIDirectionGet(QEI1_BASE)==1)	NEncoder.left_motor_dir=-1.0f;
		else NEncoder.left_motor_dir=1.0f;
	}
	else
	{
		if(QEIDirectionGet(QEI1_BASE)==1)	NEncoder.left_motor_dir=1.0f;
		else NEncoder.left_motor_dir=-1.0f;
	}
	
	NEncoder.left_motor_total_cnt+=NEncoder.left_motor_cnt*NEncoder.left_motor_dir;
}

/***************************************
������:	void get_left_motor_speed(void)
˵��: ��ȡ�������ʵ���ٶ�ֵ
���:	��
����:	��
��ע:	����λʱ���ڵ�������,ת����rpm��cm/s
����:	��������
***************************************/
float get_left_motor_speed(void)
{
#if quadrature_decoder_enable
	//���ٶ�ת����תÿ����
	NEncoder.left_motor_speed_rpm=60*(NEncoder.left_motor_cnt*NEncoder.left_motor_dir/trackless_motor.pulse_num_per_circle)
																/(NEncoder.left_motor_period_ms*0.001f);	
	NEncoder.left_motor_speed_cmps=2*3.14f*trackless_motor.wheel_radius_cm*(NEncoder.left_motor_speed_rpm/60.0f);
	return NEncoder.left_motor_speed_cmps;
#else
	static uint8_t cnt1=0;
	cnt1++;
	if(cnt1>=4)
	{
		cnt1=0;
		NEncoder.left_motor_period_ms=20;
		
		NEncoder.left_motor_cnt_clear=1;
		//���ٶ�ת����תÿ����
		NEncoder.left_motor_speed_rpm=60*(NEncoder.left_motor_period_cnt*1.0f/trackless_motor.pulse_num_per_circle)
																	/(NEncoder.left_motor_period_ms*0.001f);	
		NEncoder.left_motor_speed_cmps=2*3.14f*trackless_motor.wheel_radius_cm*(NEncoder.left_motor_speed_rpm/60.0f);
		NEncoder.left_motor_cnt=NEncoder.left_motor_period_cnt;
		NEncoder.left_motor_period_cnt=0;
		
		NEncoder.left_motor_gyro_rps=NEncoder.left_motor_speed_rpm*2*3.14f/60.0f;//ת���ɽ��ٶ�rad/s
	}
	return NEncoder.left_motor_speed_cmps;
#endif
}

/***************************************
������:	void get_right_motor_speed(void)
˵��: ��ȡ�ұ�����ʵ���ٶ�ֵ
���:	��
����:	��
��ע:	����λʱ���ڵ�������,ת����rpm��cm/s
����:	��������
***************************************/
float get_right_motor_speed(void)
{
#if quadrature_decoder_enable
	//���ٶ�ת����תÿ����
	NEncoder.right_motor_speed_rpm=60*(NEncoder.right_motor_cnt*NEncoder.right_motor_dir/trackless_motor.pulse_num_per_circle)
																/(NEncoder.right_motor_period_ms*0.001f);
	NEncoder.right_motor_speed_cmps=2*3.14f*trackless_motor.wheel_radius_cm*(NEncoder.right_motor_speed_rpm/60);
	return NEncoder.right_motor_speed_cmps;
#else
	static uint8_t cnt2=0;
	cnt2++;
	if(cnt2>=4)
	{
		cnt2=0;
		NEncoder.right_motor_period_ms=20;
		NEncoder.right_motor_cnt_clear=1;
		//���ٶ�ת����תÿ����
		NEncoder.right_motor_speed_rpm=60*(NEncoder.right_motor_period_cnt*1.0f/trackless_motor.pulse_num_per_circle)
																	/(NEncoder.right_motor_period_ms*0.001f);
		NEncoder.right_motor_speed_cmps=2*3.14f*trackless_motor.wheel_radius_cm*(NEncoder.right_motor_speed_rpm/60);
		NEncoder.right_motor_cnt=NEncoder.right_motor_period_cnt;
		NEncoder.right_motor_period_cnt=0;
		
		NEncoder.right_motor_gyro_rps=NEncoder.right_motor_speed_rpm*2*3.14f/60.0f;//ת���ɽ��ٶ�rad/s
	}
	return NEncoder.right_motor_speed_cmps;
#endif
}

/***************************************
������:	void motor_total_cnt_reset(void)
˵��: �����������λ
���:	��
����:	��
��ע:	��
����:	��������
***************************************/
void motor_total_cnt_reset(void)
{
	NEncoder.left_motor_total_cnt =0;
	NEncoder.right_motor_total_cnt=0;
}



/***************************************
������:	void mPulse_Handler(void)
˵��: �����ⲿ�жϲɼ�����ʱ���жϷ�����
���:	��
����:	��
��ע:	��
����:	��������
***************************************/
void mPulse_Handler(void)
{
  uint32_t flag=GPIOIntStatus(GPIO_PORTF_BASE,true);//��ȡ�ж�״̬
	if((flag&GPIO_PIN_0)==GPIO_PIN_0)//�ж��Ƿ�ΪGPIOF_PIN_0������ж�
	{
		GPIOIntClear(GPIO_PORTF_BASE,GPIO_PIN_0);//����жϱ�־
		NEncoder.right_motor_dir=(GPIOPinRead(GPIO_PORTG_BASE,GPIO_PIN_0)!=0?1:-1);
		if(trackless_motor.right_encoder_dir_config==0)
		{		
			if(NEncoder.right_motor_dir==1)	NEncoder.right_motor_period_cnt++;
			else NEncoder.right_motor_period_cnt--;
			NEncoder.right_motor_total_cnt+=NEncoder.right_motor_dir;	
		}
		else
		{
			if(NEncoder.right_motor_dir==1)	NEncoder.right_motor_period_cnt--;
			else NEncoder.right_motor_period_cnt++;
			NEncoder.right_motor_total_cnt-=NEncoder.right_motor_dir;				
		}	
		
		
		if(NEncoder.right_motor_cnt_clear==1)
		{
			NEncoder.right_motor_cnt_clear=0;
			NEncoder.right_motor_period_cnt=0;
		}
	}	
	
	if((flag&GPIO_PIN_1)==GPIO_PIN_1)//�ж��Ƿ�ΪGPIOF_PIN_1������ж�
	{
		GPIOIntClear(GPIO_PORTF_BASE,GPIO_PIN_1);//����жϱ�־
		NEncoder.left_motor_dir=(GPIOPinRead(GPIO_PORTG_BASE,GPIO_PIN_1)!=0?-1:1);
		if(trackless_motor.left_encoder_dir_config==0)
		{	
			if(NEncoder.left_motor_dir==1)	NEncoder.left_motor_period_cnt++;
			else NEncoder.left_motor_period_cnt--;
			NEncoder.left_motor_total_cnt+=NEncoder.left_motor_dir;
		}
		else
		{
			if(NEncoder.left_motor_dir==1)	NEncoder.left_motor_period_cnt--;
			else NEncoder.left_motor_period_cnt++;
			NEncoder.left_motor_total_cnt-=NEncoder.left_motor_dir;
		}	
		
		if(NEncoder.left_motor_cnt_clear==1)
		{
			NEncoder.left_motor_cnt_clear=0;
			NEncoder.left_motor_period_cnt=0;
		}		
	}
}

/***************************************
������:	void Encoder_Init(void)
˵��: �������ɼ���ʼ��
���:	��
����:	��
��ע:	��
����:	��������
***************************************/
void Encoder_Init(void)
{
#if quadrature_decoder_enable
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);// Enable the QEI1 peripheral
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_QEI1)) ;// Wait for the QEI1 module to be ready
	GPIOPinConfigure(GPIO_PG0_PHA1);
	GPIOPinConfigure(GPIO_PG1_PHB1);
	GPIOPinTypeQEI(GPIO_PORTG_BASE,GPIO_PIN_0);
	GPIOPinTypeQEI(GPIO_PORTG_BASE,GPIO_PIN_1);
	GPIOPadConfigSet(GPIO_PORTG_BASE,GPIO_PIN_0,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPU);
	GPIOPadConfigSet(GPIO_PORTG_BASE,GPIO_PIN_1,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPU);
	
	QEIConfigure(QEI1_BASE, (QEI_CONFIG_CAPTURE_A_B// Count on ChA and ChB edges
													|QEI_CONFIG_NO_RESET	 // Do not reset on index pulse
													|QEI_CONFIG_QUADRATURE // ChA and ChB are quadrature	
													|QEI_CONFIG_NO_SWAP),	 // Do not swap ChA and ChB
													0);
	QEIVelocityConfigure(QEI1_BASE,QEI_VELDIV_1,SysCtlClockGet()/50);//0ms
	QEIVelocityEnable(QEI1_BASE);
	QEIEnable(QEI1_BASE);// Enable the quadrature encoder
	
	QEIIntRegister(QEI1_BASE,QEI1_IRQHandler);		//�жϺ���ע��
	QEIIntEnable(QEI1_BASE,QEI_INTTIMER);
	IntEnable(INT_QEI1);
	
	/********************************************************************************/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);// Enable the QEI1 peripheral
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_QEI0)) ;// Wait for the QEI1 module to be ready
	GPIOPinConfigure(GPIO_PF0_PHA0);
	GPIOPinConfigure(GPIO_PF1_PHB0);
	GPIOPinTypeQEI(GPIO_PORTF_BASE,GPIO_PIN_0);
	GPIOPinTypeQEI(GPIO_PORTF_BASE,GPIO_PIN_1);
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPU);//GPIO_PIN_TYPE_STD_WPU
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_1,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPU);
	
	QEIConfigure(QEI0_BASE, (QEI_CONFIG_CAPTURE_A_B// Count on ChA and ChB edges
													|QEI_CONFIG_NO_RESET	 // Do not reset on index pulse
													|QEI_CONFIG_QUADRATURE // ChA and ChB are quadrature	
													|QEI_CONFIG_NO_SWAP),	 // Do not swap ChA and ChB
													0);
	QEIVelocityConfigure(QEI0_BASE,QEI_VELDIV_1,SysCtlClockGet()/50);//20ms
	QEIVelocityEnable(QEI0_BASE);
	QEIEnable(QEI0_BASE);// Enable the quadrature encoder
	
	QEIIntRegister(QEI0_BASE,QEI0_IRQHandler);		//�жϺ���ע��
	QEIIntEnable(QEI0_BASE,QEI_INTTIMER);
	IntEnable(INT_QEI0);
	
	//IntPrioritySet(INT_QEI0,USER_INT1);
	//IntPrioritySet(INT_QEI1,USER_INT1);
#else
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);//ʹ��GPIO����
	
  GPIODirModeSet(GPIO_PORTG_BASE, GPIO_PIN_0,GPIO_DIR_MODE_IN);//M1D
  GPIODirModeSet(GPIO_PORTG_BASE, GPIO_PIN_1,GPIO_DIR_MODE_IN);//M2D
	GPIOPadConfigSet(GPIO_PORTG_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPD);
  GPIOPadConfigSet(GPIO_PORTG_BASE,GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPD);
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//ʹ��GPIO����
  GPIOIntRegister(GPIO_PORTF_BASE, mPulse_Handler);//GPIOע���ж�
  GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);//PF0��Ϊ�ж�����Դ
  GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPD);//����PF0Ϊ����
  GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0 , GPIO_RISING_EDGE);//�жϴ�������Ϊ�����ش���
  GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0);//ʹ��PF0�ж�
  //IntPrioritySet(INT_GPIOF,USER_INT0);

  //GPIOIntRegister(GPIO_PORTF_BASE, mPulse_Handler);//GPIOע���ж�
  GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_1);//PF1��Ϊ�ж�����Դ
  GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPD);//����PF1Ϊ����
  GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_1 , GPIO_RISING_EDGE);//�жϴ�������Ϊ�����ش���
  GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_1);//ʹ��PF1�ж�
  IntPrioritySet(INT_GPIOF,USER_INT0);
#endif
}


