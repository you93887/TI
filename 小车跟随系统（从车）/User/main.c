#include "include.h"

#include "VOFA.h"


 #ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

#define Speed_Fast 65
#define Speed_Low  35
uint8_t RxBu = 0;
uint8_t RxBuffer[1];//���ڽ��ջ���
uint16_t RxLine = 0;//ָ���
uint8_t DataBuff[200];//ָ������

int main_speed=0; //���������ٶ�
float Speed1; //�����ٶ�
float Speed2; //�����ٶ�
float s1;     //���־���
float s2;     //���־���
float s;    //��·�ھ���
float s3;   //���پ���
float s5;   //�������־���
float s4;   //�������־���
extern float PWM1;
extern float PWM2;


uint16_t timer0_cont=0;
uint8_t i=0;
int Wtimer0A_cnt = 0;


//PIDѭ��
uint8_t RS_Pin  [6]={0};//�Ҷȴ���������
int LastSensor=0;
float error1=0;
float error2=0;
float kp=50;//50
float kd=10;//10
float out=0;
int Sensor=0;//�Ҷ����

//����
int last_motor=0; //���յ�����һ������ֵ
int motors=0;     //ģʽ��ű�־λ
int motor;        //���յ�������ֵ

//������
uint16_t receive_cnt;//����ɹ���������֡����
uint8_t confidence;
uint16_t distance,noise,reftof;
uint32_t peak,intg;
uint8_t dis;
extern int q;
//

//PID����
float Follow_p=0.15;
float Follow_d=0.4;//0.1
float Follow_error=0;
float Follow_Lasterror=0;
float Follow_out=0;

//�����߼���־λ
int flag2=0;//��ʼ·�̻��ֱ�־λ
int flag3=1;//��3��Ȧ����־λ
int flag4=0;//��3�⿪ʼ������־λ
int flag5=0;//��·�ڱ�־λ
int flag6=1;//�����ⶨ����ѭ�����̱�־λ��û�в��ã�
int flag7=1;//�����ʱ�������ͣ����־������·�����α�־λ
int stop=0; //ͣ����־λ
int start=0;//��ʼ���پ�����ֱ�־λ
int t1=0;   //��������5s

//������
float fAcc[3], fGyro[3], fAngle[3];



void LED_Init()
{
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
			while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
      {	
      }
		  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_7);
	    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7, 0);
}

void BEEP_Init()
{
			 SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
			 while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOG))
       {
       }
			 GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_1);
		   GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, GPIO_PIN_1);
}


int main(void)
{     
	//����ϵͳʱ��  �õ��δ�ʱ����Ҫ���ã���ÿ�ʼ����һ��
	//SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |	SYSCTL_OSC_MAIN);  
	ROM_FPUEnable();//ʹ�ܸ��㵥Ԫ
	ROM_FPULazyStackingEnable();//�����ӳٶ�ջ,�����ж���Ӧ�ӳ� 
	SysCtlClockSet( SYSCTL_SYSDIV_4|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN|SYSCTL_USE_PLL);
	initTime();	        //һЩ��ʱ�����ĳ�ʼ������
	uint16_t delaycnt = 0;
	LED_GPIO_Config();	//��ʼ������
	BLEUART_Init();     //������ʼ��
	OLED_Init();		    //OLED��ʼ��
	Motor_Init();       //�����ʼ��
	PWM_Init();	        //PWM��ʼ��
	Encoder_Init();     //��������ʼ��
	PID_Init();         //PID������ʼ��
	BEEP_Init();	      //������
  	LED_Init();         //LED��ʼ��
	Red_Sensor_Init();  //�Ҷȴ�������ʼ��
	ROM_UART3_Init();		//��ʼ���������ڼ�����
	BLEUART_Init();     //������ʼ��
	ButtonsInit();	    //��ʼ�������ⲿ�ж�
	Timer_init();       //��ʱ����ʼ��10ms��һ��	
  motor=0;            //���ģʽ
	main_speed=50;      //���������ٶ�
	while(1)
	{
	    OLED_ShowString(4,0,"Di:",6);			/* ��ʾ���� */
		  OLED_ShowNum(14,0,distance,5,6);
		  OLED_ShowFloatNum(10,7,PWM1,2,2,6);
		  OLED_ShowFloatNum(10,14,PWM2,2,2,6);
			OLED_ShowFloatNum(10,21,Speed1,2,2,6);
		  OLED_ShowFloatNum(10,28,Speed2,2,2,6);
		  OLED_ShowFloatNum(10,35,s,2,2,6);		
		  OLED_ShowNum(10,42,  motor,2,6);		
//		RS_Pin[4]=Read_RS_RM;
//		RS_Pin[5]=Read_RS_R ;
//	  RS_Pin[2]=Read_RS_M1 ;
//		RS_Pin[3]=Read_RS_M2;
//		RS_Pin[0]=Read_RS_L;
//		RS_Pin[1]=Read_RS_LM;
//		OLED_ShowNum(0,0,RS_Pin[0],2,6);
//		OLED_ShowNum(0,7,RS_Pin[1],2,6);
//		OLED_ShowNum(0,14,RS_Pin[2],2,6);
//		OLED_ShowNum(0,21,RS_Pin[3],2,6);
//		OLED_ShowNum(0,28,RS_Pin[4],2,6);
//		OLED_ShowNum(0,35,RS_Pin[5],2,6);
		  OLED_Update();	
	}
}
uint16_t t=0;
void TIMER0A_Handler(void)				//ϵͳ�����жϺ���  ���Լ����庯������Ҫ�ú�������TimerIntRegister�����������¶�ʱ����ʼ����
{
	//�ڴ���������������
	motor = BLE_GetNumber(); //��ȡָ��
	if(last_motor==0&&motor!=0)
		   motors=motor;
   	last_motor=motor;

	if(motors==1)  //��һ��
	{
      if(motor==5)
		  {
			   t1++;
     	   Motor_SetPwm1(0);
	       Motor_SetPwm2(0);
				 if(t1>=500)
				 GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, GPIO_PIN_1);
		     else
			 	 GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, 0);	
		  }
			RS_Pin[4]=Read_RS_RM;
			RS_Pin[5]=Read_RS_R ;
			RS_Pin[2]=Read_RS_M1 ;
			RS_Pin[3]=Read_RS_M2;
			RS_Pin[0]=Read_RS_L;
			RS_Pin[1]=Read_RS_LM;
			Sensor=RS_Pin[0]*6+RS_Pin[1]*4+RS_Pin[2]*2+RS_Pin[3]*(-2)+RS_Pin[4]*(-4)+RS_Pin[5]*(-6);
      if( RS_Pin[2]&&RS_Pin[3]&&	(RS_Pin[4]||RS_Pin[1]))
			{
					flag2=1;
					s1=0;
					s2=0;
					s=0;
					Sensor=0;
			}
	    s=(s1+s2)*0.5;
	    if(s>0.2 && s<0.4 &&	flag2) 
		     Sensor=0;
	    if(s>0.4)
		     flag2=0;
			error1=0-Sensor;
			out= kp*error1+kd*(error1-error2);
			error2=error1;
			LastSensor=Sensor;
			Follow_error=200-distance;
			Follow_out=Follow_p*Follow_error+Follow_d*(Follow_error-Follow_Lasterror);
			Follow_Lasterror=Follow_error;
			if(Follow_out>=50)
				Follow_out=50;
			else if(Follow_out<=-50)
				Follow_out=-50;
			PWM1=-Follow_out+out;
			PWM2=-Follow_out-out;
			if(distance>450)
			{
			PWM1=27+out;
			PWM2=27-out;
			}
			if(PWM1>=90)
				PWM1=90;
			else if(PWM1<=-90)
				PWM1=-90;
			if(PWM2>=90)
				PWM2=90;
			else if(PWM2<=-90)
				PWM2=-90;
			if(stop)
			{
			Motor_SetPwm1(0);
			Motor_SetPwm2(0);
			}
			else
			{
			Motor_SetPwm1(PWM1);
			Motor_SetPwm2(PWM2);
			}	
	}
	else if(motors==2)  //�ڶ���
  {   
		  if(motor==5)
		  {
			   t1++;
     	   Motor_SetPwm1(0);
	       Motor_SetPwm2(0);
				 if(t1>=500)
				 GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, GPIO_PIN_1);
				 else
				 GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, 0);
		  }
			RS_Pin[4]=Read_RS_RM;
			RS_Pin[5]=Read_RS_R ;
			RS_Pin[2]=Read_RS_M1 ;
			RS_Pin[3]=Read_RS_M2;
			RS_Pin[0]=Read_RS_L;
			RS_Pin[1]=Read_RS_LM;
			Sensor=RS_Pin[0]*6+RS_Pin[1]*4+RS_Pin[2]*2+RS_Pin[3]*(-2)+RS_Pin[4]*(-4)+RS_Pin[5]*(-6);
			if( RS_Pin[2]&&RS_Pin[3]&&	(RS_Pin[4]||RS_Pin[1]))
			{
				 flag2=1;
				 s1=0;
				 s2=0;
				 s=0;
				 Sensor=0;
			}
				s=(s1+s2)*0.5;
		  if(s>0.2 && s<0.4 &&	flag2)
				 Sensor=0;
			if(s>0.4)
				 flag2=0;
			error1=0-Sensor;
			out= kp*error1+kd*(error1-error2);
			error2=error1;
			LastSensor=Sensor;
			Follow_error=200-distance;
			Follow_out=Follow_p*Follow_error+Follow_d*(Follow_error-Follow_Lasterror);
			Follow_Lasterror=Follow_error;
			if(Follow_out>=50)
				Follow_out=50;
			else if(Follow_out<=-50)
				Follow_out=-50;
			PWM1=-Follow_out+out;
			PWM2=-Follow_out-out;
			if(distance>350)
			{
			PWM1=37+out;
			PWM2=37-out;
			}
			if(PWM1>=90)
				PWM1=90;
			else if(PWM1<=-90)
				PWM1=-90;
			if(PWM2>=90)
				PWM2=90;
			else if(PWM2<=-90)
				PWM2=-90;
			if(stop)
			{
			Motor_SetPwm1(0);
			Motor_SetPwm2(0);
			}
			else
			{
			Motor_SetPwm1(PWM1);
			Motor_SetPwm2(PWM2);
			}	

	}
	else if(motors==3)//������
	{
		  if(motor==5)
		 {
				t1++;
				Motor_SetPwm1(0);
				Motor_SetPwm2(0);
				if(t1>=500)
				GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, GPIO_PIN_1);
				else
				GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, 0);
		 }
      if(flag3==1) //��һȦ
	   {
				RS_Pin[4]=Read_RS_RM;
				RS_Pin[5]=Read_RS_R ;
				RS_Pin[2]=Read_RS_M1 ;
				RS_Pin[3]=Read_RS_M2;
				RS_Pin[0]=Read_RS_L;
				RS_Pin[1]=Read_RS_LM;
				Sensor=RS_Pin[0]*6+RS_Pin[1]*4+RS_Pin[2]*2+RS_Pin[3]*(-2)+RS_Pin[4]*(-4)+RS_Pin[5]*(-6);
				if( RS_Pin[2]&&RS_Pin[3]&&(RS_Pin[4]||RS_Pin[1]))
				 {
						 flag2=1;
						 s1=0;
						 s2=0;
						 s=0;
						 Sensor=0;
			   }
	 	   s=(s1+s2)*0.5;
	     if(s>0.2 && s<0.4 &&	flag2)
	      {
		         Sensor=0;
		         flag5=1;
	      }
	     if(s>0.4)
		     flag2=0;
       if(flag5&& (RS_Pin[2]&&RS_Pin[3]&&	(RS_Pin[4]||RS_Pin[1])))
	      {
		     flag3=2;
		     flag5=0;
	      }
				error1=0-Sensor;
				out= kp*error1+kd*(error1-error2);
				error2=error1;
				LastSensor=Sensor;
				Follow_error=200-distance;
				Follow_out=Follow_p*Follow_error+Follow_d*(Follow_error-Follow_Lasterror);
				Follow_Lasterror=Follow_error;
				if(Follow_out>=50)
					Follow_out=50;
				else if(Follow_out<=-50)
					Follow_out=-50;
				PWM1=-Follow_out+out;
				PWM2=-Follow_out-out;
				if(distance>400)
				{
				PWM1=35+out;
				PWM2=35-out;
				}
				if(PWM1>=90)
					PWM1=90;
				else if(PWM1<=-90)
					PWM1=-90;
				if(PWM2>=90)
					PWM2=90;
				else if(PWM2<=-90)
					PWM2=-90;
				if(stop)
				{
				Motor_SetPwm1(0);
				Motor_SetPwm2(0);
				}
				else
				{
				Motor_SetPwm1(PWM1);
				Motor_SetPwm2(PWM2);
				}	 
		 }
		  else if(flag3==2)//�ڶ�Ȧ
	   {
				RS_Pin[4]=Read_RS_RM;
				RS_Pin[5]=Read_RS_R ;
				RS_Pin[2]=Read_RS_M1 ;
				RS_Pin[3]=Read_RS_M2;
				RS_Pin[0]=Read_RS_L;
				RS_Pin[1]=Read_RS_LM;
				Sensor=RS_Pin[0]*6+RS_Pin[1]*4+RS_Pin[2]*2+RS_Pin[3]*(-2)+RS_Pin[4]*(-4)+RS_Pin[5]*(-6);
        if( RS_Pin[2]&&RS_Pin[3]&&	(RS_Pin[4]||RS_Pin[1]))
	       {
						 flag2=1;
						 s1=0;
						 s2=0;
						 s=0;
						 Sensor=0;
						 start=1;
	       }
				if(start)
				s=(s1+s2)*0.5;
	      if(s>0.27 && s<0.40 &&	flag2)
	        {
						 Sensor=6;
						 flag4=0;
						 flag5 =1;
						 t=0;
	        }
	      else if(s>0.40 &&	flag2)
		         flag2=0;
	      if(s<1.7&&s>0.4)	 
			    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7, GPIO_PIN_7);
	      else 
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7, 0);
				t++;
				error1=0-Sensor;
				out= kp*error1+kd*(error1-error2);
				error2=error1;
				LastSensor=Sensor;
	      if(flag4)
	       {
						Follow_error=200-distance;
						Follow_out=Follow_p*Follow_error+Follow_d*(Follow_error-Follow_Lasterror);
						Follow_Lasterror=Follow_error;
						if(Follow_out>=50)
							Follow_out=50;
						else if(Follow_out<=-50)
							Follow_out=-50;
						PWM1=-Follow_out+out;//����
						PWM2=-Follow_out-out;//����
						if(distance>400)
						{
						PWM1=30+out;
						PWM2=30-out;	
						}
	       }
	      else
	       { 
						if(flag5&& (RS_Pin[2]&&RS_Pin[3]&&	(RS_Pin[4]||RS_Pin[1])))
					  {
						 flag3=3;
						 flag5=0;
					  }
						PWM1=32+out;
						PWM2=32-out; 
	       }
			if(PWM1>=90)
				PWM1=90;
			else if(PWM1<=-90)
				PWM1=-90;
			if(PWM2>=90)
				PWM2=90;
			else if(PWM2<=-90)
				PWM2=-90;
			if(stop)
			{
			Motor_SetPwm1(0);
			Motor_SetPwm2(0);
			}
			else
			{
			Motor_SetPwm1(PWM1);
			Motor_SetPwm2(PWM2);
			}	 		 
	  }
	   else if(flag3==3)//����Ȧ
	 {
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7, 0);
			RS_Pin[4]=Read_RS_RM;
			RS_Pin[5]=Read_RS_R ;
			RS_Pin[2]=Read_RS_M1 ;
			RS_Pin[3]=Read_RS_M2;
			RS_Pin[0]=Read_RS_L;
			RS_Pin[1]=Read_RS_LM;
			Sensor=RS_Pin[0]*6+RS_Pin[1]*4+RS_Pin[2]*2+RS_Pin[3]*(-2)+RS_Pin[4]*(-4)+RS_Pin[5]*(-6);
			if( RS_Pin[2]&&RS_Pin[3]&&	(RS_Pin[4]||RS_Pin[1]))
			 {
				 flag2=1;
				 s1=0;
				 s2=0;
				 s=0;
				 Sensor=0;
			 }
	  	s=(s1+s2)*0.5;
			if(s>0.2 && s<0.4 &&	flag2)
			 {
				 t=0;
				 Sensor=0;
			 }
	    t++;
	   if(s>0.4)
		  flag2=0;
	  error1=0-Sensor;
	  out= kp*error1+kd*(error1-error2);
	  error2=error1;
		LastSensor=Sensor;
		Follow_error=200-distance;
		Follow_out=Follow_p*Follow_error+Follow_d*(Follow_error-Follow_Lasterror);
		Follow_Lasterror=Follow_error;
		if(Follow_out>=50)
			Follow_out=50;
		else if(Follow_out<=-50)
			Follow_out=-50;
		PWM1=-Follow_out+out;//����
	  PWM2=-Follow_out-out;//����
		if(distance>400)
		{
			if(t<200)
			{
				 PWM1=22+out;
	       PWM2=22-out;
			}
			else
			{
		     PWM1=30+out;//����
	       PWM2=30-out;//��
			}
		}
			if(PWM1>=90)
				PWM1=90;
			else if(PWM1<=-90)
				PWM1=-90;
			if(PWM2>=90)
				PWM2=90;
			else if(PWM2<=-90)
				PWM2=-90;
			if(stop)
			{
			Motor_SetPwm1(0);
			Motor_SetPwm2(0);
			}
			else
			{
			Motor_SetPwm1(PWM1);
			Motor_SetPwm2(PWM2);
			}
	  }

	}
	else if(motors==6) //������
	{
//		    if(motor==5)
//		{
//			     t1++;
//     	Motor_SetPwm1(0);
//	  Motor_SetPwm2(0);
//		 if(t1>=500)
//		 {
//			 			  GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, GPIO_PIN_1);
//		 }
//		 else
//		 {
//			 		  GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, 0);
//		 }
//			
//		}
//		s=(s1+s2)*0.5;

//		if(s>0.45&&(flag6==1))
//		{	
//			s1=0;
//		  s2=0;
//			s=0;
//			flag6=2;
//			main_speed=Speed_Low;
//			kp=90;
//      kd=30;
//			
//		}
//		else if(s>0.65&&flag6==2)
//		{	s1=0;
//		  s2=0;
//			main_speed=Speed_Fast;
//			s=0;
//			flag6=3;
//			kp=10;
//      kd=5;
//			
//		}
//		else if(s>0.58&&flag6==3)
//		{	s1=0;
//		  s2=0;
//			main_speed=Speed_Low;
//			s=0;
//			flag6=4;
//			kp=90;
//      kd=30;
//			
//		}
//		else if(s>0.65&&flag6==4)
//		{ s1=0;
//		  s2=0;
//			main_speed=Speed_Fast;
//			s=0;
//			flag6=5;
//			kp=10;
//      kd=5;
//			//stop=1;
//		}
//		else if(s>1.15&&flag6==5)
//		{ s1=0;
//		  s2=0;
//			main_speed=Speed_Low;
//			s=0;
//			flag6=6;
//			kp=90;
//      kd=30;
//			//stop=1;
//		}
//		else if(s>0.60&&flag6==6)
//		{		    s1=0;
//		    s2=0;
//			main_speed=Speed_Fast;
//			s=0;
//			flag6=7;
//			kp=10;
//      kd=5;
//			//stop=1;
//		}
//		else if(s>0.58&&flag6==7)
//		{
//					    s1=0;
//		    s2=0;
//			main_speed=Speed_Low;
//			s=0;
//			flag6=8;
//			kp=90;
//      kd=30;
//			//stop=1;
//		}
//		else if(s>0.58&&flag6==8)
//		{
//					    s1=0;
//		    s2=0;
//			main_speed=Speed_Fast;
//			s=0;
//			flag6=9;
//			kp=10;
//      kd=5;
//			
//		}
//		else if(s>0.68&&flag6==9)
//		{
//					    s1=0;
//		    s2=0;
//			main_speed=0;
//			//stop=1;
//		}

	   timer0_cont++;
     if(motor==5)
		  {
			     t1++;
     	   Motor_SetPwm1(0);
	       Motor_SetPwm2(0);
		     if(t1>=500)
			 	  GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, GPIO_PIN_1);
		     else
			 		GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, 0);
	   	}
			RS_Pin[4]=Read_RS_RM;
			RS_Pin[5]=Read_RS_R ;
			RS_Pin[2]=Read_RS_M1 ;
			RS_Pin[3]=Read_RS_M2;
			RS_Pin[0]=Read_RS_L;
			RS_Pin[1]=Read_RS_LM;
			Sensor=RS_Pin[0]*6+RS_Pin[1]*4+RS_Pin[2]*2+RS_Pin[3]*(-2)+RS_Pin[4]*(-4)+RS_Pin[5]*(-6);
			if( RS_Pin[2]&&RS_Pin[3]&&	(RS_Pin[4]||RS_Pin[1])&&flag7)
				{
					flag2=1;
					s1=0;
					s2=0;
					s=0;
					Sensor=0;
				}
	    s=(s1+s2)*0.5;
			if(s>0.2 && s<0.35 &&	flag2)
					 Sensor=0;
			if(s>0.35&&flag2)
			 {
				 flag2=0;
				 flag7=0;
			 }
			kp=80;
	    kd=25;
			error1=0-Sensor;
			out= kp*error1+kd*(error1-error2);
			error2=error1;
			LastSensor=Sensor;
			if(	timer0_cont>=600)
			 {
					Follow_error=200-distance;
					Follow_p=0.15;
					Follow_out=Follow_p*Follow_error+Follow_d*(Follow_error-Follow_Lasterror);
					Follow_Lasterror=Follow_error;
					if(Follow_out>=60)
						  Follow_out=60;
					else if(Follow_out<=-60)
							Follow_out=-60;
					PWM1=-Follow_out+out;//����
					PWM2=-Follow_out-out;//����
				  if(distance>450)
		       {
			       if(timer0_cont>=900)
			         {
									 PWM1=47+out;
									 PWM2=47-out;
			         }
						 else
							 {
								 PWM1=32+out;
								 PWM2=32-out;
							 }
		       }
     	}
	   else
			{
					PWM1=30+out;
					PWM2=30-out;	
			}
			if(PWM1>=90)
				PWM1=90;
			else if(PWM1<=-90)
				PWM1=-90;
			if(PWM2>=90)
				PWM2=90;
			else if(PWM2<=-90)
				PWM2=-90;
			if(stop)
			{
			Motor_SetPwm1(0);
			Motor_SetPwm2(0);
			}
			else
			{
			Motor_SetPwm1(PWM1);
			Motor_SetPwm2(PWM2);
			}	
	}
  else if(motors==5) //����
	 {
	 
	 }
  	ROM_TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);     //�����ʱ���ж�Դ  ��һ������Ϊ�����Ķ�ʱ�� �� �ڶ�������Ϊ��ʱ����ģʽ��TimerIntEnable���������ĵڶ�������
}




uint32_t zhouqi=0;
uint32_t freq1=0;
uint32_t freq2=0;
uint32_t timer3_flag=0;
uint32_t timer4_flag=0;
uint32_t Speed1_capture_1=0,Speed1_capture_2=0,Speed1_capture_3=0;
uint32_t Speed2_capture_1=0,Speed2_capture_2=0,Speed2_capture_3=0;
//uint32_t up_count=0,down_count=0;
void TIMER3_WID_IRQHandler(void)
{
	static uint32_t time3_count=0;
	//static uint32_t time3_count1=0;
  uint32_t status=TimerIntStatus( WTIMER3_BASE,  true);
	TimerIntClear( WTIMER3_BASE,  status);
  //��һ�ν��ж������ڼ�⵽�������أ�Ȼ�󽫼�ʱֵ��ȡ���������ؼ���Ϊ�½���
	
	time3_count++;
    //1s��ӡһ�Σ����ǲ�֪��Ϊʲô��UARTprintf��ӡһ�ξͲ���ӡ�ˣ��һ��ڸĽ����������벶���ǿ���û�����

		if(timer3_flag==0)
		{
			TimerControlEvent( TIMER3_BASE,  TIMER_A,
																 TIMER_EVENT_POS_EDGE);
//		TimerControlEvent( TIMER3_BASE,  TIMER_A,
//																 TIMER_EVENT_NEG_EDGE);
			Speed1_capture_1=TimerValueGet( TIMER3_BASE,  TIMER_A);
			timer3_flag=1;
		}
			//�ڶ��ν��ж�����Ϊ��⵽���½��أ�Ȼ�󽫼�ʱֵ��ȡ����ʱ���Ѿ�����˸ߵ�ƽ����
			//���Լ����ռ�ձȣ��������ؼ���Ϊ������
		else if(timer3_flag==1)
		{
			TimerControlEvent( TIMER3_BASE,  TIMER_A,
																 TIMER_EVENT_POS_EDGE);
			Speed1_capture_2=TimerValueGet( TIMER3_BASE,TIMER_A);	
			timer3_flag=0;

					/* ____   ___ capture_1�൱�ڼ�⵽��һ�������ؼǵ�����
						 |  |   |   capture_2�൱�ڼ�⵽��һ���½��ؼǵ���
						_|  |___|   ����capture_2��capture_1֮�伴Ϊ�ߵ�ƽ��
					capture_3�൱�ڼ�⵽�ڶ��������ؼǵ���������capture_3��capture_1֮��Ϊ������*/
					/*  /|   /| ����Ҫ��ռ�ձȺ�Ƶ�ʣ���Ϊ���õ��Ƕ�ʱ��A�����ԼӼ�����
						 / |  / | ��ʱ��A�ǵ����ȵ����Ȼ���ٴ�0��ʼ����������ͨ����ߵ�ƽ�͵͵�ƽʱ��������
						/  | /  | ���capture_1��capture_2���ڵ�һ���������ڵ������׶Σ���1��2�Ĳ���Ǹߵ�ƽ
					 /   |/   | ���1��2�ֱ������������ڵ������׶Σ��Ǹߵ�ƽ��Ҫͨ��0xffff-capture_1+capture_2��á�
					���capture_2��capture_3���ڵ�һ���������ڵ������׶Σ���2��3�Ĳ���ǵ͵�ƽ��
					����ֱ������������������׶Σ��͵�ƽ��Ҫͨ��0xffff-capture_2+capture_3�����*/
		
					//Ƶ������Ƶ�����ڼ��ɵõ�

			freq1 = (Speed1_capture_2-Speed1_capture_1)*100;//(up_count+down_count)*100;
			Speed1 = freq1*2*3.1416*0.024/137;
			s1+=Speed1/100;
			s3+=Speed1/100;
					//ռ�ձ�Ϊ�ߵ�ƽռ���ڵı�ֵ���ɵõ�
	//		duty=up_count*100/(up_count+down_count);
		 }
	if(time3_count%2==0)
	{
		//Serial_SendByte(3,0x33);
		//Serial_SendNumber(3,Speed1*100,4);
		time3_count=0;
	}

}


void TIMER4_WID_IRQHandler(void)
{
	static uint32_t time4_count=0;
	//static uint32_t time4_count1=0;
  uint32_t status=TimerIntStatus( WTIMER4_BASE,  true);
	TimerIntClear( WTIMER4_BASE,  status);
  //��һ�ν��ж������ڼ�⵽�������أ�Ȼ�󽫼�ʱֵ��ȡ���������ؼ���Ϊ�½���
	
	time4_count++;
		if(timer4_flag==0)
		{
			TimerControlEvent( TIMER4_BASE,  TIMER_A,
																 TIMER_EVENT_POS_EDGE);
			Speed2_capture_1=TimerValueGet( TIMER4_BASE,  TIMER_A);
			timer4_flag=1;
		}
		//�ڶ��ν��ж�����Ϊ��⵽���أ�Ȼ�󽫼�ʱֵ��ȡ����ʱ���Ѿ�����˸ߵ�ƽ����
		else if(timer4_flag==1)
		{
			TimerControlEvent( TIMER4_BASE,  TIMER_A,
																 TIMER_EVENT_POS_EDGE);
			Speed2_capture_2=TimerValueGet( TIMER4_BASE,TIMER_A);	
			timer4_flag=0;
			//Ƶ������Ƶ�����ڼ��ɵõ�

			freq2 = (Speed2_capture_2-Speed2_capture_1)*100;//(up_count+down_count)*100;
			Speed2 = freq2*2*3.1416*0.024/137;
						s2+=Speed2/100;
			s5+=Speed2/100;
		 }
	if(time4_count%2==0)
	{

		time4_count=0;
	}

}


//void ROM_UART3IntHandler(void)
//{

//	
//  uint32_t ui32IntStatus;

//	
//  ui32IntStatus = UARTIntStatus(UART3_BASE, true);
//	
//  ROM_UARTIntClear(UART3_BASE, ui32IntStatus);//������жϱ�־λ
//	//�˺������ж��Ƿ���ܵ��������з�����
//	//�����жϽ���if��ifִ�����˳��ж�
//	if (!ROM_UARTCharsAvail(UART3_BASE))
//	{
//		return;
//	}
//	//�����жϽ���while
//	while (ROM_UARTCharsAvail(UART3_BASE))
//	{
//			  RxBuffer[0]=ROM_UARTCharGetNonBlocking(UART3_BASE);
//        //printf("%c\n",re_buf);
//        RxLine++;                      //ÿ���յ�һ�����ݣ�����ص����ݳ��ȼ�1
//        DataBuff[RxLine-1]=RxBuffer[0];  //��ÿ�ν��յ������ݱ��浽��������
//        if(RxBuffer[0]== 0x21)            //���ս�����־λ��������ݿ����Զ��壬����ʵ����������ֻ��ʾ��ʹ�ã���һ����0x21
//        {
//						USART_PID_Adjust(1); // ���ݽ����Ͳ�����ֵ����
//            printf("RXLen=%d\r\n", RxLine);
//            for (int i = 0; i < RxLine; i++)
//                printf("UART DataBuff[%d] = %c\r\n", i, DataBuff[i]);
//					
//						//������
//            printf("%d\r\n", PID_Speed1_Kp);
//						printf("%d\r\n", PID_Speed1_Ki);
//						printf("%d\r\n", PID_Speed1_Kd);
//            memset(DataBuff, 0, sizeof(DataBuff)); // ��ջ�������
//            RxLine = 0; // ��ս��ճ���
//        }		
//			
//		}
//	
//	//�����жϺ������ݿ�������������
//	
//}

