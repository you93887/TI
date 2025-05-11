#include "include.h"

#include "VOFA.h"


 #ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif





//������
#define ACC_UPDATE		0x01
#define GYRO_UPDATE		0x02
#define ANGLE_UPDATE	0x04
#define MAG_UPDATE		0x08
#define READ_UPDATE		0x80
float fAcc[3], fGyro[3], fAngle[3];
static volatile char s_cDataUpdate = 0, s_cCmd = 0xff;

uint8_t RxBu = 0;
uint8_t RxBuffer[1];//���ڽ��ջ���
uint16_t RxLine = 0;//ָ���
uint8_t DataBuff[200];//ָ������


float Speed1; //�����ٶ�
float Speed2; //�����ٶ�
float s1;     //���־���
float s2;     //���־���
float s;    //����
float s3;   //���پ���
float s5;   //�������־���
float s4;   //�������־���
extern float PWM1;
extern float PWM2;


uint16_t timer0_cont=0;
uint8_t i=0;
int Wtimer0A_cnt = 0;


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


//���ٶȻ�
float Angle_p=0.15;
float Angle_d=0.1;
float Angle_error=0;
float Angle_lasterror=0;
float Angle_out=0;
float Angle_target=30;
float Angle_actual=0;
int   IMU_start=0;
float Angle=0;


//�ǶȻ�
float IMU_p=1.5;
float IMU_d=0;
float IMU_error=0;
float IMU_lasterror=0;
float IMU_out=0;
float IMU_target=30;
float IMU_actual=0;
//��������
uint8_t key1_num = 0;			//����1
uint8_t key2_num = 0;			//����2
uint8_t key3_num = 0;			//����3
uint8_t key4_num = 0;			//����4

//�����߼���־λ
extern PID PID_Motor1_Speed;
extern PID PID_Motor2_Speed;
extern PID PID_RedSensor_Speed;

//openmvѲ��
float Line_p=3,Line_i=0,Line_d=0,Line_err=0,Line_lasterr=0,Line_out=0;


int center_pos=0;
int Crossing_flag=0;
uint16_t Card=0;
int stop_flag=0;
int Encoder_flag=0;
float s=0;
int Cards[10]={0};
uint8_t Run_flag=0;
int timeing=0;
int crosscnt=0;
uint16_t Card_Num=0;
uint16_t Card_Num2=0;
int start=0;
int start1=0;
int send_flag=0;
int flag6=0;
int flag7=0;
int flag8=0;
int flag9=0;
int LED_flag=0;
uint8_t once_flag=0;
uint8_t cnt=0;
int Time_flag=0;
int flag11=0;
int j=0;
int flag_uart=1;
int flag_uart1=0;
int a=10,b=20,c=30,d=40,e=50,f=60;
int a1,b1,c1,d1,e1,f1;
int a_flag,b_flag,c_flag,d_flag,e_flag,f_flag;
int arr[6]={0};
int Add,Inc,Add10,Inc10;
int change=0;
void menu()
{
	if(a_flag)
	{
		if(Add&&a>=0)
		{
			a++;
     change=1;			
		}
		else if(Inc&&a>=0)
		{
			a--;
			change=1;	
		}
		else if(Add10&&a>=0)
		{
			a=a+10;
			change=1;
		}
		else if(Inc10&&a>=0)
		{
			a=a-10;
			change=1;
		}
		else;
		Add=0;
		Inc=0;
    Add10=0;
		Inc10=0;
	}
	else if(b_flag)
	{
				if(Add&&b>=0)
		{
			b++;
			change=1;	
		}
		else if(Inc&&b>=0)
		{
			b--;
			change=1;	
		}
		else if(Add10&&b>=0)
		{
			b=b+10;
			change=1;
		}
		else if(Inc10&&b>=0)
		{
			b=b-10;
			change=1;
		}
		else;
				Add=0;
		Inc=0;
    Add10=0;
		Inc10=0;

	}
	else if(c_flag)
	{
			if(Add&&c>=0)
		{
			c++;
			change=1;	
		}
		else if(Inc&&c>=0)
		{
			c--;
			change=1;	
		}
		else if(Add10&&c>=0)
		{
			c=c+10;
			change=1;
		}
		else if(Inc10&&c>=0)
		{
			c=c-10;
			change=1;
		}
		else;
			Add=0;
		Inc=0;	
    Add10=0;
		Inc10=0;
		
	}
	else if(d_flag)
	{
						if(Add&&d>=0)
		{
			d++;
			change=1;	
		}
		else if(Inc&&d>=0)
		{
			d--;
			change=1;	
		}
		else if(Add10&&d>=0)
		{
			d=d+10;
			change=1;
		}
		else if(Inc10&&d>=0)
		{
			d=d-10;
			change=1;
		}
		else;
				Add=0;
		Inc=0;
    Add10=0;
		Inc10=0;
		
	}
	else if(e_flag)
	{
			if(Add&&e>=0)
		{
			e++;
			change=1;	
		}
		else if(Inc&&e>=0)
		{
			e--;
			change=1;	
		}
		else if(Add10&&e>=0)
		{
			e=e+10;
			change=1;
		}
		else if(Inc10&&e>=0)
		{
			e=e-10;
			change=1;
		}
		else;
		Add=0;
		Inc=0;
    Add10=0;
		Inc10=0;
	}
	else if(f_flag)
	{
						if(Add&&f>=0)
		{
			f++;
			change=1;	
		}
		else if(Inc&&f>=0)
		{
			f--;
			change=1;	
		}
		else if(Add10&&f>=0)
		{
			f=f+10;
			change=1;
		}
		else if(Inc10&&f>=0)
		{
			f=f-10;
			change=1;
		}
		else;
				Add=0;
		Inc=0;
    Add10=0;
		Inc10=0;
	}


}

int main(void)
{     
	//����ϵͳʱ��  �õ��δ�ʱ����Ҫ���ã���ÿ�ʼ����һ��
	//SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |	SYSCTL_OSC_MAIN);  
	ROM_FPUEnable();//ʹ�ܸ��㵥Ԫ
	ROM_FPULazyStackingEnable();//�����ӳٶ�ջ,�����ж���Ӧ�ӳ� 
	SysCtlClockSet( SYSCTL_SYSDIV_4|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN|SYSCTL_USE_PLL);
	initTime();	        //һЩ��ʱ�����ĳ�ʼ������
	LED_GPIO_Config();	//��ʼ������
  LED_Init();
	BLEUART_Init();     //������ʼ��
	OLED_Init();		    //OLED��ʼ��
	Motor_Init();       //�����ʼ��
	PWM_Init();	        //PWM��ʼ��
//	Encoder_Init();     //��������ʼ��
	PID_Init();         //PID������ʼ��
	ButtonsInit();	    //��ʼ�������ⲿ�ж�
	MPU9050_Init();     //�����ǳ�ʼ��
  KEY_Init();         //�������س�ʼ��
  ROM_UART5_Init(); 
  ROM_UART6_Init();  
	EEPROM_Init();//EEPROM��ʼ��
  Timer1_init();
  Timer_init();	
 	EEPROMRead((uint32_t *)(&flag11),0,4); 
	EEPROMRead((uint32_t *)(&Line_p),4,4); 

	while(1)
	{
			    SetDuty(PWM0_BASE,PWM_OUT_2,70);
		if(change)
		{
		Serial_SendByte(6,0x0d);
		Serial_SendByte(6,a);
		Serial_SendByte(6,b);
		Serial_SendByte(6,c);
		Serial_SendByte(6,d);
		Serial_SendByte(6,e);
		Serial_SendByte(6,f);	
    Serial_SendByte(6,0x2c);
		change=0;
		}
		printf("openmv����.t0.txt=\"%d\"\xff\xff\xff",flag11);
		printf("PID����.t0.txt=\"%f\"\xff\xff\xff",Line_p);
		printf("��ɫ��ֵ.t0.txt=\"%d\"\xff\xff\xff",a);
		printf("��ɫ��ֵ.t1.txt=\"%d\"\xff\xff\xff",b);
		printf("��ɫ��ֵ.t2.txt=\"%d\"\xff\xff\xff",c);
		printf("��ɫ��ֵ.t3.txt=\"%d\"\xff\xff\xff",d);
		printf("��ɫ��ֵ.t4.txt=\"%d\"\xff\xff\xff",e);
		printf("��ɫ��ֵ.t5.txt=\"%d\"\xff\xff\xff",f);
				if ( ReadKey1 == 0)
			{

				key1_num ++;
				key2_num = 0;
				key3_num = 0;
				key4_num = 0;
				LED_flag=0;
				Run_flag=4;
			  GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0);
	      GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0);
			}
			else if( ReadKey2 == 0)
			{

			//	EEPROMProgram((uint32_t *)(&flag11),0,4);
				key2_num ++;
				key1_num = 0;
				key3_num = 0;
				key4_num = 0;
				flag6=0;
				flag7=0;
				LED_flag=0;
				crosscnt=0;
			  GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0);
	      GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0);
			}
			else if( ReadKey3 == 0)
			{
				key3_num ++;
				key1_num = 0;
				key2_num = 0;
				key4_num = 0;
			}
			else if( ReadKey4 == 0)
			{
				key4_num ++;
				key1_num = 0;
				key2_num = 0;
				key3_num = 0;
			}
			else;
			
			if(	key3_num)
			{
					flag11++;
				EEPROMProgram((uint32_t *)(&flag11),0,4);
			}
//			flag11++;
       OLED_ShowNum(0,0,a,3,6);
       OLED_ShowNum(0,7,b,3,6);
			 OLED_ShowNum(0,14,c,3,6);
       OLED_ShowNum(0,21,d,3,6);
       OLED_ShowNum(0,28,e,3,6);			
       OLED_ShowNum(0,35,f,3,6);

	//		OLED_ShowNum(0,10,key1_num,3,8);
			OLED_Update();
			if(start1!=2)
				Serial_SendByte(1,Card);
			if(send_flag==1)
			{
				Serial_SendByte(1,0x10);
				Serial_SendByte(1,0x10);
				Serial_SendByte(1,0x10);
				send_flag=0;
			}
	}
}

float abss(float x,float y)
{
	if(x>y)
		return x-y;
	else
		return y-x;
	
}

float IMU_pid(float Target)
{
	  IMU_target=Target;
		IMU_error=IMU_target-Angle;
		IMU_out=IMU_p*IMU_error+IMU_d*(IMU_error-IMU_lasterror);
		IMU_lasterror=IMU_error;
				if(IMU_out>500)
		   IMU_out=500;
		else if(IMU_out<-500)
		   IMU_out=-500;
		Angle_target=IMU_out;
    Angle_actual=fGyro[2];
		Angle_error=Angle_target-Angle_actual;
		Angle_out+=Angle_p*Angle_error+Angle_d*(Angle_error-Angle_lasterror);
		Angle_lasterror=Angle_error;
		if(Angle_out>600)
		   Angle_out=600;
		else if(Angle_out<-600)
		   Angle_out=-600;
		return Angle_out;
}


void TIMER0A_Handler(void)				//ϵͳ�����жϺ���  ���Լ����庯������Ҫ�ú�������TimerIntRegister�����������¶�ʱ����ʼ����
{
	//�ڴ��������������
  ROM_TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);     //�����ʱ���ж�Դ  ��һ������Ϊ�����Ķ�ʱ�� �� �ڶ�������Ϊ��ʱ����ģʽ��TimerIntEnable���������ĵڶ�������
  		menu();
	if(Encoder_flag)
	 s=(s1+s2)*0.5;
	if(Time_flag==1)
	 timeing+=1;
		
	if(key1_num)//�ж˲���
	{
			if(Cards[2]!=0&&flag6==0&&crosscnt==0)
			{
						if(Cards[2]==3)
							Card=4;
						else if(Cards[2]==4)
							Card=3;
						flag6=1;
			}
      if(crosscnt==0&&Object==4)
			{
          Run_flag=0;
					if(Crossing_flag==2)
					{
						crosscnt=1;
						flag6=0;
					}
			}
			else if(crosscnt==1)
			{  
					Time_flag=1;
					if(flag6==0)
					 {
							if(flag7==0&&Crossing_flag==1)
							 {
									 timeing=0;
									 flag7=1;
									 Run_flag=4;
							 }
							 if(timeing>1000&&flag7)
							 {
									Card_Num=Cards[2];

									if(Card_Num==0)
										Run_flag=4;
									else
									{
										Run_flag=0;
										flag6=1;
									}
								}
					  }
						if(Card_Num!=0&&(Crossing_flag==2)&&(timeing>=800))
						{
								 crosscnt=2;
								 Time_flag=0;
								 timeing=0;
							 if(Card_Num==Card)
								 Run_flag=1;
							 else
								 Run_flag=2;
						}			
			}
      else if(crosscnt==2)
			{

					if(start==1)
					{		      	
							Time_flag=1;			
							if(flag6==0)
							{
								 timeing=0;
								 flag6=1;
							}
							if(timeing>2000)
							{
								Run_flag=3;
								flag9=1;
								start=0;
								GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0);
							}
					}
					else if(Run_flag==4)
							GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_PIN_6);
					if((Crossing_flag==2)&&(timeing>=800))
					{
							 crosscnt=3;
							 Time_flag=0;
							 timeing=0;
						flag9=0;
						LED_flag=1;
					}		
			}				
			if(center_pos==255&&flag9==0&&flag11)
			{
				Run_flag=4;
				if(LED_flag==1)
				 GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, GPIO_PIN_6);
			}
			
			if(Run_flag==1)//��ת90��
			{
					IMU_start=1;
					if(abss(Angle,90)<10)
					{
						IMU_start=0;	
						Run_flag=0;
						Angle=0;
						if(crosscnt==3||crosscnt==2)
						flag11=1;
					}
					else
					{
					 Motor_SetPwm1(-100);
					 Motor_SetPwm2(300);
					}
			}
			else if(Run_flag==2)//��ת90��
			{
					IMU_start=1;
					if(abss(Angle,-90)<5)
					{
						IMU_start=0;	
						Run_flag=0;
						Angle=0;
						if(crosscnt==3||crosscnt==2)
					  flag11=1;
					}
					else
					{
					 Motor_SetPwm1(300);
					 Motor_SetPwm2(-100);
					}
			}
			else if(Run_flag==3)//ת180��
			{
					IMU_start=1;
					if(Angle<-165&&center_pos>30&&center_pos<90)
					{
						IMU_start=0;
						Run_flag=0;
					}
					else
					{
					 Motor_SetPwm1(300);
					 Motor_SetPwm2(-300);
					}
			}
			else if(Run_flag==4)//ͣ��
			{
					 Motor_SetPwm1(0);
					 Motor_SetPwm2(0);
				
			}
			else//����Ѳ��
			{
				if(center_pos==255)
					Line_err=0;
				else
					Line_err=64-center_pos;
					Line_out=Line_p*Line_err+Line_d*(Line_err-Line_lasterr);
					Line_lasterr=Line_err;
					Motor_SetPwm1(200-Line_out);
					Motor_SetPwm2(200+Line_out);		
			}
		}

	
	else if(key2_num) //Զ�˲���
	{
		    if(crosscnt==0)
			{
				if(Cards[2]!=0&&flag6==0)
				{
					Card=Cards[2];
					flag6=1;
				}
				if(start1==2)
				{
					Run_flag=0;
					if(Crossing_flag==2)
					{
						crosscnt=1;
						flag6=0;
						Run_flag=0;
					}
				}
				else
					Run_flag=4;
			}
			else if(crosscnt==1)
			{    
				  Time_flag=1;
				  if((Crossing_flag==2)&&(timeing>=800))
					{
						   crosscnt=2;
							 Time_flag=0;
						   timeing=0;
					}			
			}
      else if(crosscnt==2)
			{
        Time_flag=1;
				if(flag6==0)
				{
						if(flag7==0&&Crossing_flag==1)
						 {
								 timeing=0;
								 flag7=1;
								 Run_flag=4;
						 }
						 if(timeing>1000&&flag7)
						 {
								Card_Num=Cards[2];
								Card_Num2=Cards[3];
								if(Card_Num==0||Card_Num2==0)
									Run_flag=4;
								else
								{
									Encoder_flag=0;
									Run_flag=0;
									flag6=1;
								}
							}
					}
				  if(Card_Num!=0&&Card_Num2!=0&&(Crossing_flag==2)&&(timeing>=800))
					{
						   crosscnt=3;
							 Time_flag=0;
						   timeing=0;
					     flag6=0;
						   flag7=0;
						 if(Card_Num==Card||Card_Num2==Card)
						 {
							 Run_flag=1;
							 Card_Num=0;
						 }
						 else
						 {
							 Run_flag=2;
							 Card_Num=0;
						 }
					}			
			}	
      else if(crosscnt==3)
      {
          Time_flag=1;
					if(flag6==0)
					{
							if(flag7==0&&Crossing_flag==1)
							 {
									 timeing=0;
									 flag7=1;
									 Run_flag=4;
							 }
							 if(timeing>1000&&flag7)
							 {
									Card_Num=Cards[2];
									if(Card_Num==0)
										Run_flag=4;
									else
									{
										Run_flag=0;
										flag6=1;
									}
								}
				  }
				  if(Card_Num!=0&&(Crossing_flag==2)&&(timeing>=800))
					{
						   crosscnt=4;
							 Time_flag=0;
						   timeing=0;
						   if(Card_Num==Card)
							   Run_flag=1;
						   else
							   Run_flag=2;
					}			
       }	
			if(center_pos==255&&flag11)
			{
         Run_flag=4;
	       GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, GPIO_PIN_6);
			}
			
			if(Run_flag==1)//��ת90��
			{
					IMU_start=1;
					if(Angle>75&&center_pos!=255)
					{
						IMU_start=0;	
						Run_flag=0;
						Angle=0;
						send_flag=1;
						if(crosscnt==4)
							flag11=1;
					}
					else
					{
					 Motor_SetPwm1(-100);
					 Motor_SetPwm2(380);
					}
				
			}
			else if(Run_flag==2)//��ת90��
			{
					IMU_start=1;
					if(Angle<-75&&center_pos!=255)
					{
						IMU_start=0;	
						Run_flag=0;
						Angle=0;
						send_flag=1;
						if(crosscnt==4)
						flag11=1;
					}
					else
					{
						Motor_SetPwm1(380);
						Motor_SetPwm2(-100);
					}
				
			}
			else if(Run_flag==3&&center_pos!=255)//ת180��
			{
					 if(once_flag==2)
	           Run_flag=4;
					 else
					 {
							IMU_start=1;
							if(abss(Angle,180)<5)
							{
								IMU_start=0;
								Run_flag=0;
								Angle=0;
							}
							else
							{
						   Motor_SetPwm1(-200);
						   Motor_SetPwm2(200);
							}
						}
			}
			else if(Run_flag==4)//ͣ��
			{
					 Motor_SetPwm1(0);
					 Motor_SetPwm2(0);
				
			}
			else if(Run_flag==5)
			{

				if(s>0.1)
				{
					 Motor_SetPwm1(0);
					 Motor_SetPwm2(0);
				}
				else
				{
					 Motor_SetPwm1(-110);
					 Motor_SetPwm2(-110);
				}
				
			}
			else//����Ѳ��
			{
				if(center_pos==255)
					Line_err=0;
				else
					Line_err=64-center_pos;
					Line_out=Line_p*Line_err+Line_d*(Line_err-Line_lasterr);
					Line_lasterr=Line_err;
					Motor_SetPwm1(185-Line_out);
					Motor_SetPwm2(185+Line_out);		
			}

	}
	else if(key3_num)
	{
		
	}
	else if(key4_num)
	{

	}
	else;
	
}


void TIMER1A_Handler()  //�����ǽ��ٶȻ���
{
	  ROM_TimerIntClear(TIMER1_BASE,TIMER_TIMA_TIMEOUT);  
	  if(IMU_start==1)
		Angle+=fGyro[2]*0.1;
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
		if(timer3_flag==0)
		{
			TimerControlEvent( TIMER3_BASE,  TIMER_A, TIMER_EVENT_POS_EDGE);
			Speed1_capture_1=TimerValueGet( TIMER3_BASE,  TIMER_A);
			timer3_flag=1;
		}
			//�ڶ��ν��ж�����Ϊ��⵽���½��أ�Ȼ�󽫼�ʱֵ��ȡ����ʱ���Ѿ�����˸ߵ�ƽ����
			//���Լ����ռ�ձȣ��������ؼ���Ϊ������
		else if(timer3_flag==1)
		{
			TimerControlEvent( TIMER3_BASE,  TIMER_A, TIMER_EVENT_POS_EDGE);
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
			Speed2 = freq1*2*3.1416*0.024/137;
			s2+=Speed2/100;
		  s5+=Speed2/100;
				
	//		duty=up_count*100/(up_count+down_count);//ռ�ձ�Ϊ�ߵ�ƽռ���ڵı�ֵ���ɵõ�
		 }
	if(time3_count%2==0)
	{
		time3_count=0;
	}

}


void TIMER4_WID_IRQHandler(void)
{
	static uint32_t time4_count=0;
  uint32_t status=TimerIntStatus( WTIMER4_BASE,  true);
	TimerIntClear( WTIMER4_BASE,  status);  //��һ�ν��ж������ڼ�⵽�������أ�Ȼ�󽫼�ʱֵ��ȡ���������ؼ���Ϊ�½���
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
		TimerControlEvent( TIMER4_BASE,  TIMER_A,TIMER_EVENT_POS_EDGE);											 
		Speed2_capture_2=TimerValueGet( TIMER4_BASE,TIMER_A);	
		timer4_flag=0;
		//Ƶ������Ƶ�����ڼ��ɵõ�
		freq2 = (Speed2_capture_2-Speed2_capture_1)*100;//(up_count+down_count)*100;
		Speed1 = freq2*2*3.1416*0.024/137;
		s1+=Speed1/100;
	  s3+=Speed1/100;	

	 }
	if(time4_count%2==0)
	{
		time4_count=0;
	}
}


void ROM_UART5IntHandler(void)
{
    uint32_t ui32IntStatus;
	  uint8_t ui8RxBuffer;
    ui32IntStatus = UARTIntStatus(UART5_BASE, true);
    ROM_UARTIntClear(UART5_BASE, ui32IntStatus);//������жϱ�־λ
	//�˺������ж��Ƿ���ܵ��������з�����
	//�����жϽ���if��ifִ�����˳��ж�
	if ( !ROM_UARTCharsAvail(UART5_BASE))
	{
		
		return;
	}
	//�����жϽ���while
	while (ROM_UARTCharsAvail(UART5_BASE))
	{
    
		//�˺�����ָ���Ľ���FIFO�л�ȡһ���ַ�
		ui8RxBuffer = (uint8_t) ROM_UARTCharGetNonBlocking(UART5_BASE);
		if(ui8RxBuffer==0x01)
		{
			a_flag=1;
			b_flag=c_flag=d_flag=e_flag=f_flag=0;			
		}
		else if(ui8RxBuffer==0x02)
		{
			b_flag=1;
			a_flag=c_flag=d_flag=e_flag=f_flag=0;			
		}
		else if(ui8RxBuffer==0x03)
		{
			c_flag=1;
		  b_flag=a_flag=d_flag=e_flag=f_flag=0;
		}
		else if(ui8RxBuffer==0x04)
		{
			d_flag=1;
			b_flag=c_flag=a_flag=e_flag=f_flag=0;
		}
		else if(ui8RxBuffer==0x05)
		{
			e_flag=1;
						b_flag=c_flag=d_flag=a_flag=f_flag=0;
		}
		else if(ui8RxBuffer==0x06)
		{
			f_flag=1;
						b_flag=c_flag=d_flag=e_flag=a_flag=0;
		}
		else if(ui8RxBuffer==0x0a)
			Add=1;
		else if(ui8RxBuffer==0x0b)
			Inc=1;
		else if(ui8RxBuffer==0x0c)
			Add10=1;
		else if(ui8RxBuffer==0x0d)
			Inc10=1;

	//�����жϺ������ݿ������������
	}
}





void ROM_UART6IntHandler(void)
{
    uint32_t ui32IntStatus;
	  uint8_t ui8RxBuffer;
    ui32IntStatus = UARTIntStatus(UART6_BASE, true);
    ROM_UARTIntClear(UART6_BASE, ui32IntStatus);//������жϱ�־λ
	//�˺������ж��Ƿ���ܵ��������з�����
	//�����жϽ���if��ifִ�����˳��ж�
	  if ( !ROM_UARTCharsAvail(UART6_BASE))
	  {
		
		return;
	  }
	//�����жϽ���while
	  while (ROM_UARTCharsAvail(UART6_BASE))
	  {
		ui8RxBuffer = (uint8_t) ROM_UARTCharGetNonBlocking(UART6_BASE);		//�˺�����ָ���Ľ���FIFO�л�ȡһ���ַ�
		if(ui8RxBuffer==0xc3&&flag_uart==0)
			flag_uart=1;
		else if(ui8RxBuffer==0xc3&&flag_uart==1)
			flag_uart=2;
		else if(flag_uart==2)
		{
			a1=ui8RxBuffer;
			flag_uart=3;
		}
		else if(flag_uart==3)
		{
			b1=ui8RxBuffer;
			flag_uart=4;
		}
		else if(flag_uart==4)
		{
			c1=ui8RxBuffer;
			flag_uart=5;
		}
		else if(flag_uart==5)
		{
			d1=ui8RxBuffer;
			flag_uart=6;
		}
		else if(flag_uart==6)
		{
			e1=ui8RxBuffer;
			flag_uart=7;
		}
		else if(flag_uart==7)
		{
			f1=ui8RxBuffer;
			flag_uart=8;
		}
		else if(ui8RxBuffer==0xc4&&flag_uart==8)
			flag_uart=0;
	  }
	//�����жϺ������ݿ������������
	
}


void ROM_UART1IntHandler(void)
{
	static uint8_t RxState = 0;		//�����ʾ��ǰ״̬��״̬�ľ�̬����
	static uint8_t pRxPacket = 0;	//�����ʾ��ǰ��������λ�õľ�̬����
	uint8_t rxBuf;
  uint32_t ui32IntStatus;
  ui32IntStatus = UARTIntStatus(UART1_BASE, true);
	
  ROM_UARTIntClear(UART1_BASE, ui32IntStatus);//������жϱ�־λ
	//�˺������ж��Ƿ���ܵ��������з�����
	//�����жϽ���if��ifִ�����˳��ж�
	if (!ROM_UARTCharsAvail(UART1_BASE))
	{
		return;
	}
	//�����жϽ���while
	while (ROM_UARTCharsAvail(UART1_BASE))
	{
		rxBuf = ROM_UARTCharGetNonBlocking(UART1_BASE);
		if(rxBuf==2)
			start1=2;
    start=rxBuf;
	}
	
}











