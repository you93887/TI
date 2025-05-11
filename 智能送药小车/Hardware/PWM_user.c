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
	//配置PWM时钟（设置USEPWMDIV分频器）
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);																									//PWM时钟 16M

	//使能时钟
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);			//使能PWM模块0时钟			//使能PWM模块1时钟	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);		//使能GPIOF时钟																		
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);	
	//使能引脚复用PWM功能
	GPIOPinTypePWM(GPIO_PORTH_BASE,GPIO_PIN_0);
	GPIOPinTypePWM(GPIO_PORTH_BASE,GPIO_PIN_1);
	GPIOPinTypePWM(GPIO_PORTB_BASE,GPIO_PIN_4);
	GPIOPinTypePWM(GPIO_PORTB_BASE,GPIO_PIN_5);
	//PWM信号分配
	GPIOPinConfigure(GPIO_PH0_M0PWM0);					//PF2->PWM模块1信号6																							
	GPIOPinConfigure(GPIO_PH1_M0PWM1);					//PF3->PWM模块1信号7																								
	GPIOPinConfigure(GPIO_PB4_M0PWM2);					//PF2->PWM模块1信号6																							
	GPIOPinConfigure(GPIO_PB5_M0PWM3);
	//配置PWM发生器
	//模块1->发生器3->上下计数，不同步
	PWMGenConfigure(PWM0_BASE,PWM_GEN_0,PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);	
	PWMGenConfigure(PWM0_BASE,PWM_GEN_1,PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);	
	//配置PWM周期
	PWMGenPeriodSet(PWM0_BASE,PWM_GEN_0,64000);			//64*10^3/16/10^6=4ms																			
	PWMGenPeriodSet(PWM0_BASE,PWM_GEN_1,64000);		
	//配置PWM占空比
	//PWMPulseWidthSet(PWM1_BASE,PWM_OUT_6,PWMGenPeriodGet(PWM1_BASE, PWM_GEN_3)*0.01);		//比较值为四分之一总计数值，25%
	//PWMPulseWidthSet(PWM1_BASE,PWM_OUT_7,PWMGenPeriodGet(PWM1_BASE, PWM_GEN_3)*0.01);		//比较值为四分之三总计数值，75%

	//使能PWM模块1输出
	PWMOutputState(PWM0_BASE,PWM_OUT_0_BIT,true);
	PWMOutputState(PWM0_BASE,PWM_OUT_1_BIT,true);
	PWMOutputState(PWM0_BASE,PWM_OUT_2_BIT,true);
	PWMOutputState(PWM0_BASE,PWM_OUT_3_BIT,true);
	//使能PWM发生器
	PWMGenEnable(PWM0_BASE,PWM_GEN_0);
	PWMGenEnable(PWM0_BASE,PWM_GEN_1);

}

/******************************************************************************************************************
*函数名: SetCycle(uint32_t ui32Base,uint32_t ui32PWMOut,uint32_t cycle)
*描	 述： 设置PWM的周期
*输	 入：PWM模块编号、信号编号、周期（ms）
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
		
	uint32_t Period = 16000 * cycle;
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
