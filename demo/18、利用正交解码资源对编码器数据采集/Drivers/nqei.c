#include "headfile.h"
#include "nqei.h"
#include "user.h"
#include "SystickTime.h"

extern motor_config trackless_motor;


encoder NEncoder;
Testime timer_qei1,timer_qei0;


/***************************************
函数名:	void QEI0_IRQHandler(void)
说明: QEI0中断服务函数
入口:	无
出口:	无
备注:	无
作者:	无名创新
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
函数名:	void QEI1_IRQHandler(void)
说明: QEI1中断服务函数
入口:	无
出口:	无
备注:	无
作者:	无名创新
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
函数名:	void get_left_motor_speed(void)
说明: 获取左边轮子实际速度值
入口:	无
出口:	无
备注:	将单位时间内的脉冲数,转化成rpm、cm/s
作者:	无名创新
***************************************/
float get_left_motor_speed(void)
{
#if quadrature_decoder_enable
	//将速度转化成转每分钟
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
		//将速度转化成转每分钟
		NEncoder.left_motor_speed_rpm=60*(NEncoder.left_motor_period_cnt*1.0f/trackless_motor.pulse_num_per_circle)
																	/(NEncoder.left_motor_period_ms*0.001f);	
		NEncoder.left_motor_speed_cmps=2*3.14f*trackless_motor.wheel_radius_cm*(NEncoder.left_motor_speed_rpm/60.0f);
		NEncoder.left_motor_cnt=NEncoder.left_motor_period_cnt;
		NEncoder.left_motor_period_cnt=0;
		
		NEncoder.left_motor_gyro_rps=NEncoder.left_motor_speed_rpm*2*3.14f/60.0f;//转化成角速度rad/s
	}
	return NEncoder.left_motor_speed_cmps;
#endif
}

/***************************************
函数名:	void get_right_motor_speed(void)
说明: 获取右边轮子实际速度值
入口:	无
出口:	无
备注:	将单位时间内的脉冲数,转化成rpm、cm/s
作者:	无名创新
***************************************/
float get_right_motor_speed(void)
{
#if quadrature_decoder_enable
	//将速度转化成转每分钟
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
		//将速度转化成转每分钟
		NEncoder.right_motor_speed_rpm=60*(NEncoder.right_motor_period_cnt*1.0f/trackless_motor.pulse_num_per_circle)
																	/(NEncoder.right_motor_period_ms*0.001f);
		NEncoder.right_motor_speed_cmps=2*3.14f*trackless_motor.wheel_radius_cm*(NEncoder.right_motor_speed_rpm/60);
		NEncoder.right_motor_cnt=NEncoder.right_motor_period_cnt;
		NEncoder.right_motor_period_cnt=0;
		
		NEncoder.right_motor_gyro_rps=NEncoder.right_motor_speed_rpm*2*3.14f/60.0f;//转化成角速度rad/s
	}
	return NEncoder.right_motor_speed_cmps;
#endif
}

/***************************************
函数名:	void motor_total_cnt_reset(void)
说明: 总脉冲计数复位
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void motor_total_cnt_reset(void)
{
	NEncoder.left_motor_total_cnt =0;
	NEncoder.right_motor_total_cnt=0;
}



/***************************************
函数名:	void mPulse_Handler(void)
说明: 采样外部中断采集测速时的中断服务函数
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void mPulse_Handler(void)
{
  uint32_t flag=GPIOIntStatus(GPIO_PORTF_BASE,true);//读取中断状态
	if((flag&GPIO_PIN_0)==GPIO_PIN_0)//判断是否为GPIOF_PIN_0引起的中断
	{
		GPIOIntClear(GPIO_PORTF_BASE,GPIO_PIN_0);//清除中断标志
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
	
	if((flag&GPIO_PIN_1)==GPIO_PIN_1)//判断是否为GPIOF_PIN_1引起的中断
	{
		GPIOIntClear(GPIO_PORTF_BASE,GPIO_PIN_1);//清除中断标志
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
函数名:	void Encoder_Init(void)
说明: 编码器采集初始化
入口:	无
出口:	无
备注:	无
作者:	无名创新
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
	
	QEIIntRegister(QEI1_BASE,QEI1_IRQHandler);		//中断函数注册
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
	
	QEIIntRegister(QEI0_BASE,QEI0_IRQHandler);		//中断函数注册
	QEIIntEnable(QEI0_BASE,QEI_INTTIMER);
	IntEnable(INT_QEI0);
	
	//IntPrioritySet(INT_QEI0,USER_INT1);
	//IntPrioritySet(INT_QEI1,USER_INT1);
#else
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);//使能GPIO外设
	
  GPIODirModeSet(GPIO_PORTG_BASE, GPIO_PIN_0,GPIO_DIR_MODE_IN);//M1D
  GPIODirModeSet(GPIO_PORTG_BASE, GPIO_PIN_1,GPIO_DIR_MODE_IN);//M2D
	GPIOPadConfigSet(GPIO_PORTG_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPD);
  GPIOPadConfigSet(GPIO_PORTG_BASE,GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPD);
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//使能GPIO外设
  GPIOIntRegister(GPIO_PORTF_BASE, mPulse_Handler);//GPIO注册中断
  GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);//PF0作为中断输入源
  GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPD);//配置PF0为下拉
  GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0 , GPIO_RISING_EDGE);//中断触发类型为上升沿触发
  GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0);//使能PF0中断
  //IntPrioritySet(INT_GPIOF,USER_INT0);

  //GPIOIntRegister(GPIO_PORTF_BASE, mPulse_Handler);//GPIO注册中断
  GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_1);//PF1作为中断输入源
  GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPD);//配置PF1为下拉
  GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_1 , GPIO_RISING_EDGE);//中断触发类型为上升沿触发
  GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_1);//使能PF1中断
  IntPrioritySet(INT_GPIOF,USER_INT0);
#endif
}


