/* Copyright (c)  2019-2025 Wuhan Nameless Innovation Technology Co.,Ltd. All rights reserved.*/
/*----------------------------------------------------------------------------------------------------------------------/
                开源并不等于免费，先驱者的历史已经证明，在国内这个环境下，毫无收益的开源，单靠坊间个人爱好者，自发地参与项
								目完善的方式行不通，好的开源项目需要请专职人员做好售后服务、手把手教学、统计用户反馈需求、在实践中完成对产
								品的一次次完善与迭代升级。经过综合考虑，无名飞控开源代码中，程序仅保留公司正版激活功能代码，版本激活无实际
								功能，属于公司产品常规出厂操作，不影响客户学习，其余代码全部开放给客户学习，客户移植和二次开发代码请保留代
								码版权。
-----------------------------------------------------------------------------------------------------------------------/
*               本程序只供购买者学习使用，版权著作权属于无名科创团队，无名科创团队将飞控程序源码提供给购买者，
*               购买者要为无名科创团队提供保护，未经作者许可，不得将源代码提供给他人，不得将源代码放到网上供他人免费下载， 
*               更不能以此销售牟利，如发现上述行为，无名科创团队将诉之以法律解决！！！
-----------------------------------------------------------------------------------------------------------------------
*                                                 为什么选择无名创新？
*                                         感动人心价格厚道，最靠谱的开源飞控；
*                                         国内业界良心之作，最精致的售后服务；
*                                         追求极致用户体验，高效进阶学习之路；
*                                         萌新不再孤单求索，合理把握开源尺度；
*                                         响应国家扶贫号召，促进教育体制公平；
*                                         新时代奋斗最出彩，建人类命运共同体。 
-----------------------------------------------------------------------------------------------------------------------
*               生命不息、奋斗不止；前人栽树，后人乘凉！！！
*               开源不易，且学且珍惜，祝早日逆袭、进阶成功！！！
*               学习优秀者，简历可推荐到DJI、ZEROTECH、XAG、AEE、GDU、AUTEL、EWATT、HIGH GREAT等公司就业
*               求职简历请发送：15671678205@163.com，需备注求职意向单位、岗位、待遇等
*               无名科创开源飞控QQ群：2号群465082224、1号群540707961（人员已满）
*               CSDN博客：http://blog.csdn.net/u011992534
*               优酷ID：NamelessCotrun无名小哥
*               B站教学视频：https://space.bilibili.com/67803559/#/video
*               客户使用心得、改进意见征集贴：http://www.openedv.com/forum.php?mod=viewthread&tid=234214&extra=page=1
*               淘宝店铺：https://shop348646912.taobao.com/?spm=2013.1.1000126.2.5ce78a88ht1sO2
*               百度贴吧:无名科创开源飞控
*               公司官网:www.nameless.tech
*               无名创新国内首款TI开源飞控设计初衷、知乎专栏:https://zhuanlan.zhihu.com/p/54471146
*               修改日期:2020/03/24                   
*               版本：躺赢者PRO——CarryPilot_V3.0.1
*               版权所有，盗版必究。
*               Copyright(C) 2019-2025 武汉无名创新科技有限公司 
*               All rights reserved
-----------------------------------------------------------------------------------------------------------------------
*               重要提示：
*               正常淘宝咸鱼转手的飞控、赠送朋友、传给学弟的都可以进售后群学习交流，
*               不得直接在网上销售无名创新资料，无名创新代码有声明版权，他人不得将
*               资料代码传网上供他人下载，不得以谋利为目的销售资料代码，发现有此操
*               作者，公司会提前告知，请1天内及时处理，否则你的学校、单位、姓名、电
*               话、地址信息会被贴出在公司官网、官方微信公众平台、官方技术博客、知乎
*               专栏以及淘宝店铺首页予以公示公告，此种所作所为，会成为个人污点，影响
*               升学、找工作、社会声誉、很快就很在无人机界出名，后果很严重。
*               因此行为给公司造成重大损失者，会以法律途径解决，感谢您的合作，谢谢！！！
----------------------------------------------------------------------------------------------------------------------*/
#include "Headfile.h"
#include "PPM.h"

uint8_t ppm_update_flag=0;
void PortFIntHandler(void);
/***********************************************************
@函数名：PPM_Init
@入口参数：无
@出口参数：无
功能描述：PPM初始化
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void PPM_Init(void)
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//使能GPIO外设
  GPIOIntRegister(GPIO_PORTC_BASE, PortFIntHandler);//GPIO注册中断
  GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_3);//PC3作为中断输入源
  GPIOPadConfigSet(GPIO_PORTC_BASE,GPIO_PIN_3,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);//配置PC3为上拉
  GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_3 , GPIO_FALLING_EDGE);//中断触发类型为下降沿触发
  GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_3);//使能PC3中断
  IntPrioritySet(INT_GPIOC,USER_INT0);
}


uint16_t PPM_Databuf[8]={0};
static uint16 PPM_buf[8]={0}; 
/***********************************************************
@函数名：PortFIntHandler
@入口参数：无
@出口参数：无
功能描述：PPM中断函数
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void PortFIntHandler(void)//Port3中断程序
{
	static uint32_t last_ppm_time=0,now_ppm_time=0;
	static uint8_t ppm_ready=0,ppm_sample_cnt=0;;
	static uint16_t ppm_time_delta=0;	
  GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_3);//清除中断标志	
  last_ppm_time=now_ppm_time;//系统运行时间获取，单位us
  now_ppm_time=micros();//单位us	
  ppm_time_delta=now_ppm_time-last_ppm_time;//获取时间间隔
  if(ppm_ready==1)//PPM解析开始
  {
    if(ppm_time_delta>=800&&ppm_time_delta<=2200)
    {
      ppm_sample_cnt++;     
      PPM_buf[ppm_sample_cnt-1]=ppm_time_delta;//对应通道写入缓冲区
      if(ppm_sample_cnt>=8)//单次解析结束
      {
        memcpy(PPM_Databuf,PPM_buf,ppm_sample_cnt*sizeof(uint16));
        ppm_ready=0;
      }
    }
    else
    {
      if(ppm_time_delta>=2500)//帧结束电平至少2ms=2000us，由于部分老版本遥控器、
        //接收机输出PPM信号不标准，当出现解析异常时，尝试改小此值，该情况仅出现一例：使用天地飞老版本遥控器
      {
        memcpy( PPM_Databuf,PPM_buf,ppm_sample_cnt*sizeof(uint16));
        ppm_ready = 1;
        ppm_sample_cnt=0;
				ppm_update_flag=1;
      }
      else  ppm_ready=0;
    }
  }
  else if(ppm_time_delta>=2500)//帧结束电平至少2ms=2000us
  {			
    ppm_ready=1;
    ppm_sample_cnt=0;
		ppm_update_flag=1;
  }
}
