#include "include.h"

/*定义共同体变量类型*/
typedef union
{
    float Fdata;         //以浮点数形式读取变量
    uint32_t Adata;      //以32位无符号形式读取变量
    
}Vofa_Type;


extern uint8_t DataBuff[];

extern uint8_t PID_Speed1_Kp;
extern uint8_t PID_Speed1_Ki;
extern uint8_t PID_Speed1_Kd;

extern uint8_t PID_Speed_Kp;
extern uint8_t PID_Speed_Ki;
extern uint8_t PID_Speed_Kd;

/**
  * 函数功能：将浮点数拆分成4个字节
  * 入口参数：Fdata：需要操作的浮点数
  * 入口参数：ArrayByte：数组地址
  * 返 回 值：无
  */
void Float_to_Byte(float Fdata,  uint8_t *ArrayByte)
{
    
    Vofa_Type Vofa;                   //定义Vofa_Type类型的Vofa变量
    
    Vofa.Fdata = Fdata;               //把需要操作的浮点数复制到共同体的Fdata变量中
    ArrayByte[0] = Vofa.Adata;        //0-7位移到数组元素0
    ArrayByte[1] = Vofa.Adata >> 8;   //8-15位移动到数组元素1
    ArrayByte[2] = Vofa.Adata >> 16;  //16-23位移动到数组元素2
    ArrayByte[3] = Vofa.Adata >> 24;  //24-31位移动到数组元素3 
    
}


/**
  * 函数功能：串口发送数据到VOFA+上位机
  * 入口参数：无
  * 返 回 值：无
  */
void JustFloat(void)
{
    
    float a = 250;                                  //定义浮点数a变量
    float b = 160;                                  //定义浮点数b变量
    uint8_t Byte[4];                               //定义保存浮点数转换后的数组变量
    uint8_t Tail[4] = {0x00, 0x00, 0x80, 0x7f};    //定义包尾数组
    
    Float_to_Byte(a , Byte);                       //转换第一个浮点数
    Serial_SendArray(3,Byte, 4);                     //发送第一组数据
 
    Float_to_Byte(b, Byte);                        //转换第二个浮点数
    Serial_SendArray(3,Byte, 4);                     //发送第二组数据
 
    Serial_SendArray(3,Tail, 4);                     //发送包尾 
    
}



float Get_Data(void)
{
    uint8_t data_Start_Num = 0; // 记录数据位开始的地方
    uint8_t data_End_Num = 0; // 记录数据位结束的地方
    uint8_t data_Num = 0; // 记录数据位数
    uint8_t minus_Flag = 0; // 判断是不是负数
    float data_return = 0; // 解析得到的数据
    for(uint8_t i=0;i<200;i++) // 查找等号和感叹号的位置
    {
        if(DataBuff[i] == '=') data_Start_Num = i + 1; // +1是直接定位到数据起始位
        if(DataBuff[i] == '!')
        {
            data_End_Num = i - 1;
            break;
        }
    }
    if(DataBuff[data_Start_Num] == '-') // 如果是负数
    {
        data_Start_Num += 1; // 后移一位到数据位
        minus_Flag = 1; // 负数flag
    }
    data_Num = data_End_Num - data_Start_Num + 1;
    if(data_Num == 4) // 数据共4位
    {
        data_return = (DataBuff[data_Start_Num]-48)  + (DataBuff[data_Start_Num+2]-48)*0.1f +
                (DataBuff[data_Start_Num+3]-48)*0.01f;
    }
    else if(data_Num == 5) // 数据共5位
    {
        data_return = (DataBuff[data_Start_Num]-48)*10 + (DataBuff[data_Start_Num+1]-48) + (DataBuff[data_Start_Num+3]-48)*0.1f +
                (DataBuff[data_Start_Num+4]-48)*0.01f;
    }
    else if(data_Num == 6) // 数据共6位
    {
        data_return = (DataBuff[data_Start_Num]-48)*100 + (DataBuff[data_Start_Num+1]-48)*10 + (DataBuff[data_Start_Num+2]-48) +
                (DataBuff[data_Start_Num+4]-48)*0.1f + (DataBuff[data_Start_Num+5]-48)*0.01f;
    }
    if(minus_Flag == 1)  data_return = -data_return;
//    printf("data=%.2f\r\n",data_return);
    return data_return;
}




void USART_PID_Adjust(uint8_t Motor_n)
{
    float data_Get = Get_Data(); // 存放接收到的数据
//    printf("data=%.2f\r\n",data_Get);
    if(Motor_n == 1)//左边电机
    {
        if(DataBuff[0]=='P' && DataBuff[1]=='1') // 速度环P
				{
					PID_Speed1_Kp = (uint8_t)data_Get;
				}
				else if(DataBuff[0]=='I' && DataBuff[1]=='1') // 速度环I
				{
					PID_Speed1_Ki = (uint8_t)data_Get;
				}
				else if(DataBuff[0]=='D' && DataBuff[1]=='1') // 速度环D
				{
					PID_Speed1_Kd = (uint8_t)data_Get;
				}
				else if(DataBuff[0]=='P' && DataBuff[1]=='2') // 位置环P
				{
					//PID_Kp = (uint8_t)data_Get;
				}
				else if(DataBuff[0]=='I' && DataBuff[1]=='2') // 位置环I
				{
					//PID_Ki = (uint8_t)data_Get;
				}
				else if(DataBuff[0]=='D' && DataBuff[1]=='2') // 位置环D
				{
					//PID_Kd = (uint8_t)data_Get;
				}
				
				
//        else if(DataBuff[0]=='I' && DataBuff[1]=='1') // 位置环I
//            pid_l_position.ki = data_Get;
//        else if(DataBuff[0]=='D' && DataBuff[1]=='1') // 位置环D
//            pid_l_position.kd = data_Get;
//        else if(DataBuff[0]=='P' && DataBuff[1]=='2') // 速度环P
//            pid_l_speed.kp = data_Get;
//        else if(DataBuff[0]=='I' && DataBuff[1]=='2') // 速度环I
//            pid_l_speed.ki = data_Get;
//        else if(DataBuff[0]=='D' && DataBuff[1]=='2') // 速度环D
//            pid_l_speed.kd = data_Get;
//        else if((DataBuff[0]=='S' && DataBuff[1]=='p') && DataBuff[2]=='e') //目标速度
//            L_Target_Speed = data_Get;
//        else if((DataBuff[0]=='P' && DataBuff[1]=='o') && DataBuff[2]=='s') //目标位置
//            L_Target_Position = data_Get;
    }
		
//    else if(Motor_n == 0) // 右边电机
//    {
//        if(DataBuff[0]=='P' && DataBuff[1]=='1') // 位置环P
//            pid_r_position.kp = data_Get;
//        else if(DataBuff[0]=='I' && DataBuff[1]=='1') // 位置环I
//            pid_r_position.ki = data_Get;
//        else if(DataBuff[0]=='D' && DataBuff[1]=='1') // 位置环D
//            pid_r_position.kd = data_Get;
//        else if(DataBuff[0]=='P' && DataBuff[1]=='2') // 速度环P
//            pid_r_speed.kp = data_Get;
//        else if(DataBuff[0]=='I' && DataBuff[1]=='2') // 速度环I
//            pid_r_speed.ki = data_Get;
//        else if(DataBuff[0]=='D' && DataBuff[1]=='2') // 速度环D
//            pid_r_speed.kd = data_Get;
//        else if((DataBuff[0]=='S' && DataBuff[1]=='p') && DataBuff[2]=='e') //目标速度
//            R_Target_Speed = data_Get;
//        else if((DataBuff[0]=='P' && DataBuff[1]=='o') && DataBuff[2]=='s') //目标位置
//            R_Target_Position = data_Get;
//    }
}

