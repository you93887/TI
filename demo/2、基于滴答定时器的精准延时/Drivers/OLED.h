/* Copyright (c)  2018-2025 Wuhan Nameless Innovation Technology Co.,Ltd. All rights reserved.*/
/*----------------------------------------------------------------------------------------------------------------------/
*               本程序只供购买者学习使用，版权著作权属于无名科创团队，无名科创团队将飞控程序源码提供给购买者，
*               购买者要为无名科创团队提供保护，未经作者许可，不得将源代码提供给他人，不得将源代码放到网上供他人免费下载， 
*               更不能以此销售牟利，如发现上述行为，无名科创团队将诉之以法律解决！！！
-----------------------------------------------------------------------------------------------------------------------/
*               生命不息、奋斗不止；前人栽树，后人乘凉！！！
*               开源不易，且学且珍惜，祝早日逆袭、进阶成功！！！
*               学习优秀者，简历可推荐到DJI、ZEROTECH、XAG、AEE、GDU、AUTEL、EWATT、HIGH GREAT等公司就业
*               求职简历请发送：15671678205@163.com，需备注求职意向单位、岗位、待遇等
*               无名科创开源飞控QQ群：540707961
*               CSDN博客：http://blog.csdn.net/u011992534
*               优酷ID：NamelessCotrun无名小哥
*               B站教学视频：https://space.bilibili.com/67803559/#/video
*               客户使用心得、改进意见征集贴：http://www.openedv.com/forum.php?mod=viewthread&tid=234214&extra=page=1
*               淘宝店铺：https://shop348646912.taobao.com/?spm=2013.1.1000126.2.5ce78a88ht1sO2
*               百度贴吧:无名科创开源飞控
*               公司官网:www.nameless.tech
*               修改日期:2019/4/12
*               版本：躺赢者——CarryPilot_V1.0
*               版权所有，盗版必究。
*               Copyright(C) 2017-2025 武汉无名创新科技有限公司 
*               All rights reserved
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
#ifndef __OLED_H
#define __OLED_H


//#define  OLED_WORK_MODE_I2C//OLED使用I2C模式驱动，当采用SPI模式时，注释本行即可


#define LCD_TFT_ENABLE 0

#define Bit_SET   1
#define Bit_RESET 0
#define SYSCTL_PERIPH_GPIO_OLED SYSCTL_PERIPH_GPIOA
#define GPIO_OLED   GPIO_PORTA_BASE 
#define LCD_DCout(data)		  GPIOPinWrite(GPIO_OLED, GPIO_PIN_2, data ? GPIO_PIN_2 : 0)//DC
#define LCD_RSTout(data)		GPIOPinWrite(GPIO_OLED, GPIO_PIN_3, data ? GPIO_PIN_3 : 0)//RST
#define LCD_SDAout(data)		GPIOPinWrite(GPIO_OLED, GPIO_PIN_4, data ? GPIO_PIN_4 : 0)//SDA
#define LCD_SCLout(data)		GPIOPinWrite(GPIO_OLED, GPIO_PIN_5, data ? GPIO_PIN_5 : 0)//SCL


#define READ_SDA   GPIOPinRead(GPIO_OLED,GPIO_PIN_4)	//SDA 
#define IIC_SDA_H  GPIOPinWrite(GPIO_OLED,GPIO_PIN_4,GPIO_PIN_4)//SDA 
#define IIC_SDA_L  GPIOPinWrite(GPIO_OLED,GPIO_PIN_4,0)//SDA
#define IIC_SCL_H  GPIOPinWrite(GPIO_OLED,GPIO_PIN_5,GPIO_PIN_5)//SCL
#define IIC_SCL_L  GPIOPinWrite(GPIO_OLED,GPIO_PIN_5,0)//SCL

void LCD_WrDat(unsigned char dat);
void LCD_WrCmd(unsigned char cmd);
void OLED_WrCmd(unsigned char IIC_Data);
void OLED_WrDat(unsigned char IIC_Data);
void LCD_Set_Pos(unsigned char x, unsigned char y);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void LCD_P6x8Char(unsigned char x,unsigned char  y,unsigned char ch);
void LCD_P6x8Str(unsigned char x,unsigned char  y,unsigned char ch[]);
void write_6_8_number(unsigned char x,unsigned char  y,float number);
void LCD_P8x16Char(unsigned char x,unsigned char  y,unsigned char ch);
void LCD_P8x16Str(unsigned char x,unsigned char  y,unsigned char ch[]);
void write_8_16_number(unsigned char x,unsigned char  y,float number);
void LCD_clear_L(unsigned char x,unsigned char y);
void OLED_Init(void);
void OLED_Init_Fast(void);
void Draw_Logo(void);
void Draw_Logo1(void);
void LCD_CLS(void);

void display_6_8_number(unsigned char x,unsigned char y, float number);
void display_6_8_string(unsigned char x,unsigned char  y,unsigned char ch[]);
	
#endif



