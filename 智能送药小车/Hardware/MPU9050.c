#include "include.h"

static uint8_t s_ucWitDataBuff[WIT_DATA_BUFF_SIZE];
static uint32_t s_uiWitDataCnt = 0, s_uiReadRegIndex = 0;
int16_t sReg[REGSIZE];
extern float fAcc[3], fGyro[3], fAngle[3];


static void CopeWitData(uint8_t ucIndex, uint16_t *p_data, uint32_t uiLen)
{
    uint32_t uiReg1 = 0, uiReg2 = 0, uiReg1Len = 0, uiReg2Len = 0;
    uint16_t *p_usReg1Val = p_data;
    uint16_t *p_usReg2Val = p_data+3;
    
    uiReg1Len = 4;
    switch(ucIndex)
    {
        case WIT_ACC:   uiReg1 = AX;    uiReg1Len = 3;  uiReg2 = TEMP;  uiReg2Len = 1;  break;
        case WIT_ANGLE: uiReg1 = Roll;  uiReg1Len = 3;  uiReg2 = VERSION;  uiReg2Len = 1;  break;
        case WIT_TIME:  uiReg1 = YYMM;	break;
        case WIT_GYRO:  uiReg1 = GX;  uiLen = 3;break;
        case WIT_MAGNETIC: uiReg1 = HX;  uiLen = 3;break;
        case WIT_DPORT: uiReg1 = D0Status;  break;
        case WIT_PRESS: uiReg1 = PressureL;  break;
        case WIT_GPS:   uiReg1 = LonL;  break;
        case WIT_VELOCITY: uiReg1 = GPSHeight;  break;
        case WIT_QUATER:    uiReg1 = q0;  break;
        case WIT_GSA:   uiReg1 = SVNUM;  break;
        case WIT_REGVALUE:  uiReg1 = s_uiReadRegIndex;  break;
		default:
			return ;
    }
    if(uiLen == 3)
    {
        uiReg1Len = 3;
        uiReg2Len = 0;
    }
    if(uiReg1Len)
	{
		memcpy(&sReg[uiReg1], p_usReg1Val, uiReg1Len<<1);
	}
    if(uiReg2Len)
	{
		memcpy(&sReg[uiReg2], p_usReg2Val, uiReg2Len<<1);
	}
}


static uint8_t __CaliSum(uint8_t *data, uint32_t len)
{
    uint32_t i;
    uint8_t ucCheck = 0;
    for(i=0; i<len; i++) ucCheck += *(data + i);
    return ucCheck;
}


void WitSerialDataIn(uint8_t ucData)
{
    uint16_t usData[4];
    uint8_t ucSum;


    s_ucWitDataBuff[s_uiWitDataCnt++] = ucData;

	if(s_ucWitDataBuff[0] != 0x55)
	{
		s_uiWitDataCnt--;
		memcpy(s_ucWitDataBuff, &s_ucWitDataBuff[1], s_uiWitDataCnt);
		return ;
	}
	if(s_uiWitDataCnt >= 11)
	{
		ucSum = __CaliSum(s_ucWitDataBuff, 10);
		if(ucSum != s_ucWitDataBuff[10])
		{
			s_uiWitDataCnt--;
			memcpy(s_ucWitDataBuff, &s_ucWitDataBuff[1], s_uiWitDataCnt);
			return ;
		}
		usData[0] = ((uint16_t)s_ucWitDataBuff[3] << 8) | (uint16_t)s_ucWitDataBuff[2];
		usData[1] = ((uint16_t)s_ucWitDataBuff[5] << 8) | (uint16_t)s_ucWitDataBuff[4];
		usData[2] = ((uint16_t)s_ucWitDataBuff[7] << 8) | (uint16_t)s_ucWitDataBuff[6];
		usData[3] = ((uint16_t)s_ucWitDataBuff[9] << 8) | (uint16_t)s_ucWitDataBuff[8];
		CopeWitData(s_ucWitDataBuff[1], usData, 4);
		s_uiWitDataCnt = 0;
	}

    if(s_uiWitDataCnt == WIT_DATA_BUFF_SIZE)s_uiWitDataCnt = 0;
}



void MPU9050_Init()
{
	  ROM_UART3_Init();
}





void ROM_UART3IntHandler(void)
{


  uint32_t ui32IntStatus;

	uint8_t temp_data;
	
    ui32IntStatus = UARTIntStatus(UART3_BASE, true);
	
    ROM_UARTIntClear(UART3_BASE, ui32IntStatus);//先清除中断标志位
	//此函数是判断是否接受到东西，有返回真
	//发送中断进入if，if执行完退出中断
//	if ( !ROM_UARTCharsAvail(UART2_BASE))
//	{
//		//在此后写发送中断内容
//		
//		return;
//	}
	
	//接受中断进入while
	while (ROM_UARTCharsAvail(UART3_BASE))
	{
		//此函数从指定的接收FIFO中获取一个字符

		temp_data = (uint8_t) ROM_UARTCharGetNonBlocking(UART3_BASE);
    WitSerialDataIn(temp_data);
			for(int i = 0; i < 3; i++)
			{
				//fAcc[i] = sReg[AX+i] / 32768.0f * 16.0f;
				//fAngle[i] = sReg[Roll+i] / 32768.0f * 180.0f;
				fGyro[i] = sReg[GX+i] / 32768.0f * 2000.0f;
			}

	}
	//接受中断后续内容可以在下面添加
	
}

