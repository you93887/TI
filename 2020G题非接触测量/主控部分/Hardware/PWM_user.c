/***************************************************
 * @brief   关于TM4C123GH6PZ芯片的PWM初始化
 *          以及对应的调节占空比函数，和调节周期函数
 *
 *			PWM0_0   PH0						PWM1_0   PD0, PG2
 *			PWM0_1   PH1						PWM1_1   PD1, PG3
 *			PWM0_2   PB4, PH2					PWM1_2   PA6, PE4, PG4
 *			PWM0_3   PB5, PH3					PWM1_3   PA7, PE5, PG5
 *			PWM0_4   PE4, PG4, PH4				PWM1_4   PF0
 *			PWM0_5   PE5, PG5, PH5				PWM1_5   PF1
 *			PWM0_6   PC4, PD0, PG5, PH5			PWM1_6   PF2
 *			PWM0_7   PC5, PD1, PG5, PH5			PWM1_7   PF3
 *
 *          默认是PWM的第一列，不复用其他引脚
 *
 * @date    2024/6/21
 * @author  qihang chen
****************************************************/

#include "include.h"

/******************************************************************************************************************
*函数名: PWM_Init()
*描	 述：PWM初始化函数
*输	 入：无
	关于发生器是PWM0，1对应发生器0即PWM_GEN_0， 
	2，3对应PWM_GEN_1  ......
	PWM周期 = PWMGenPeriodSet第三个参数 / PWM时钟 （默认16M）
	对于占空比用PWMPulseWidthSet函数，用PWMGenPeriodGet获取 PWM 发生器模块的周期，然后* 0~1之间的值
******************************************************************************************************************/
void PWM_Init(void)
{
	//配置PWM时钟（设置USEPWMDIV分频器） 16分频 即40M/16 = 2.5M
	SysCtlPWMClockSet(SYSCTL_PWMDIV_16);																									//PWM时钟 16M

	//使能时钟
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);			//使能PWM模块0时钟																		
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);		//使能GPIOH时钟																		
	
	//使能引脚复用PWM功能
	GPIOPinTypePWM(GPIO_PORTH_BASE,GPIO_PIN_1);
	GPIOPinTypePWM(GPIO_PORTH_BASE,GPIO_PIN_2);
	//PWM信号分配
	GPIOPinConfigure(GPIO_PH1_M0PWM1);					//PH1->PWM模块0信号1																																									
	GPIOPinConfigure(GPIO_PH2_M0PWM2);					//PH2->PWM模块0信号2
	
	//配置PWM发生器
	//模块0->发生器0->上下计数，不同步
	//模块0->发生器1->上下计数，不同步
	PWMGenConfigure(PWM0_BASE,PWM_GEN_0,PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);	
	PWMGenConfigure(PWM0_BASE,PWM_GEN_1,PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);	
	
	//配置PWM周期
	PWMGenPeriodSet(PWM0_BASE,PWM_GEN_0,10000);			// 2.5*10^6 / x	 HZ	  											
	PWMGenPeriodSet(PWM0_BASE,PWM_GEN_1,10000);			// 2.5*10^6 / x  HZ     
	
	//配置PWM占空比
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_1,PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*0.5);		//50%
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_2,PWMGenPeriodGet(PWM0_BASE, PWM_GEN_1)*0.5);		//50%
	
	//使能PWM模块0输出
	PWMOutputState(PWM0_BASE,PWM_OUT_1_BIT,true);
	PWMOutputState(PWM0_BASE,PWM_OUT_2_BIT,true);
	
	//使能PWM发生器
	PWMGenEnable(PWM0_BASE,PWM_GEN_0);
	PWMGenEnable(PWM0_BASE,PWM_GEN_1);
}

/******************************************************************************************************************
*函数名: SetCycle(uint32_t ui32Base,uint32_t ui32PWMOut,uint32_t cycle)
*描	 述： 设置PWM的频率
*输	 入：PWM模块编号、信号编号、频率（HZ）
******************************************************************************************************************/
void SetCycle(uint32_t ui32Base,uint32_t ui32PWMOut,uint32_t cycle)
{
	uint32_t ui32Gen;
	uint32_t ui32OutBits;
	
	switch(ui32PWMOut)
	{
		case PWM_OUT_0:	ui32Gen=PWM_GEN_0,ui32OutBits=PWM_OUT_0_BIT;	break;
		case PWM_OUT_1:	ui32Gen=PWM_GEN_0,ui32OutBits=PWM_OUT_1_BIT;	break;
		case PWM_OUT_2:	ui32Gen=PWM_GEN_1,ui32OutBits=PWM_OUT_2_BIT;	break;
		case PWM_OUT_3:	ui32Gen=PWM_GEN_1,ui32OutBits=PWM_OUT_3_BIT;	break;
		case PWM_OUT_4:	ui32Gen=PWM_GEN_2,ui32OutBits=PWM_OUT_4_BIT;	break;
		case PWM_OUT_5:	ui32Gen=PWM_GEN_2,ui32OutBits=PWM_OUT_5_BIT;	break;
		case PWM_OUT_6:	ui32Gen=PWM_GEN_3,ui32OutBits=PWM_OUT_6_BIT;	break;
		case PWM_OUT_7:	ui32Gen=PWM_GEN_3,ui32OutBits=PWM_OUT_7_BIT;	break;
	}
		
	uint32_t Period = 5000000 / cycle;
    //配置PWM周期
    PWMGenPeriodSet(ui32Base,ui32Gen,Period);
    PWMOutputState(ui32Base, ui32OutBits, true);
    //使能发生器模块
    PWMGenEnable(ui32Base, ui32Gen);
}

/******************************************************************************************************************
*函数名: SetDuty(uint32_t ui32Base,uint32_t ui32PWMOut,float duty)
*描	 述：PWM调节占空比函数
*输	 入：PWM模块编号、信号编号、占空比
******************************************************************************************************************/
void SetDuty(uint32_t ui32Base,uint32_t ui32PWMOut,float duty)
{
	uint32_t ui32Gen;
	uint32_t ui32OutBits;
	
	switch(ui32PWMOut)
	{
		case PWM_OUT_0:	ui32Gen=PWM_GEN_0,ui32OutBits=PWM_OUT_0_BIT;	break;
		case PWM_OUT_1:	ui32Gen=PWM_GEN_0,ui32OutBits=PWM_OUT_1_BIT;	break;
		case PWM_OUT_2:	ui32Gen=PWM_GEN_1,ui32OutBits=PWM_OUT_2_BIT;	break;
		case PWM_OUT_3:	ui32Gen=PWM_GEN_1,ui32OutBits=PWM_OUT_3_BIT;	break;
		case PWM_OUT_4:	ui32Gen=PWM_GEN_2,ui32OutBits=PWM_OUT_4_BIT;	break;
		case PWM_OUT_5:	ui32Gen=PWM_GEN_2,ui32OutBits=PWM_OUT_5_BIT;	break;
		case PWM_OUT_6:	ui32Gen=PWM_GEN_3,ui32OutBits=PWM_OUT_6_BIT;	break;
		case PWM_OUT_7:	ui32Gen=PWM_GEN_3,ui32OutBits=PWM_OUT_7_BIT;	break;
	}
	
    //配置占空比
    PWMPulseWidthSet(ui32Base, ui32PWMOut, PWMGenPeriodGet(ui32Base, ui32Gen)*duty);
    PWMOutputState(ui32Base, ui32OutBits, true);
    //使能发生器模块
    PWMGenEnable(ui32Base, ui32Gen);
}