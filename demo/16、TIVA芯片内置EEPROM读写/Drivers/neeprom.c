#include "headfile.h"
#include "neeprom.h"

#define WP_FLASH_BASE PARAMETER_TABLE_STARTADDR
FLIGHT_PARAMETER Trackless_Params;


/***************************************
函数名:	void EEPROM_Init(void)
说明:	单片机自带2K容量EEPROM初始化
入口:	无
出口:	无
备注:	无
作者:	无名创新
***************************************/
void EEPROM_Init(void)
{
  /* EEPROM SETTINGS */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0); // EEPROM activate
  EEPROMInit(); // EEPROM start
	//EEPROMMassErase();
}

/***************************************
函数名:	void ReadFlashParameterALL(FLIGHT_PARAMETER *WriteData)
说明:	读取EEPEROM参数表中的所有参数
入口:	FLIGHT_PARAMETER *WriteData-参数表数据结构体
出口:	无
备注:	无
作者:	无名创新
***************************************/
void ReadFlashParameterALL(FLIGHT_PARAMETER *WriteData)
{
  EEPROMRead((uint32_t *)(&WriteData->Parameter_Table),WP_FLASH_BASE,FLIGHT_PARAMETER_TABLE_NUM*4);
}

/***************************************
函数名:	void ReadFlashParameterOne(uint16_t index,float *ReadData)
说明:	读取EEPEROM参数表中的单个浮点型参数
入口:	uint16_t index-参数索引号,
			float *ReadData-用于存储待读取的参数
出口:	无
备注:	无
作者:	无名创新
***************************************/
void ReadFlashParameterOne(uint16_t index,float *ReadData)
{
  EEPROMRead((uint32_t *)(ReadData),WP_FLASH_BASE+4*index,4);
}

/***************************************
函数名:	void ReadFlashParameterThree(uint16_t index,float *ReadData1,float *ReadData2,float *ReadData3)
说明:	读取EEPEROM参数表中的三个浮点型参数
入口:	uint16_t index-参数索引号,
			float *ReadData1-用于存储待读取的参数1,
			float *ReadData2-用于存储待读取的参数2,
			float *ReadData3-用于存储待读取的参数3
出口:	无
备注:	无
作者:	无名创新
***************************************/
void ReadFlashParameterThree(uint16_t index,float *ReadData1,float *ReadData2,float *ReadData3)
{
  EEPROMRead((uint32_t *)(ReadData1),WP_FLASH_BASE+4*index,4);;
  EEPROMRead((uint32_t *)(ReadData2),WP_FLASH_BASE+4*index+4,4);
  EEPROMRead((uint32_t *)(ReadData3),WP_FLASH_BASE+4*index+8,4);
}


/***************************************
函数名:	void WriteFlashParameter(uint16_t index,float WriteData,FLIGHT_PARAMETER *Table)
说明: 向EEPROM某一地址内写入单个浮点型参数
入口:	uint16_t index-参数索引号,
			float WriteData-待写入的参数,
			FLIGHT_PARAMETER *Table-参数表结构体
出口:	无
备注:	无
作者:	无名创新
***************************************/
void WriteFlashParameter(uint16_t index,float WriteData,FLIGHT_PARAMETER *Table)
{
  ReadFlashParameterALL(Table);//先把片区内的所有数据都都出来值
  Table->Parameter_Table[index]=WriteData;//将需要更改的字段赋新
  EEPROMProgram((uint32_t *)(&Table->Parameter_Table[index]),WP_FLASH_BASE+4*index,4);
}


/***************************************
函数名:	void WriteFlashParameter_Two(uint16_t index,float WriteData1,float WriteData2,FLIGHT_PARAMETER *Table)
说明: 向EEPROM某一地址内写入两个浮点型参数
入口:	uint16_t index-参数索引号,
			float WriteData1-待写入的参数1,
			float WriteData2-待写入的参数2,
			FLIGHT_PARAMETER *Table-参数表结构体
出口:	无
备注:	无
作者:	无名创新
***************************************/
void WriteFlashParameter_Two(uint16_t index,float WriteData1,float WriteData2,FLIGHT_PARAMETER *Table)
{
  ReadFlashParameterALL(Table);//先把片区内的所有数据都都出来
  Table->Parameter_Table[index]=WriteData1;//将需要更改的字段赋新值
  Table->Parameter_Table[index+1]=WriteData2;//将需要更改的字段赋新值
  EEPROMProgram((uint32_t *)(&Table->Parameter_Table[index]),WP_FLASH_BASE+4*index,8);
}


/***************************************
函数名:	void WriteFlashParameter_Three(uint16_t index,float WriteData1,float WriteData2,float WriteData3,FLIGHT_PARAMETER *Table)
说明: 向EEPROM某一地址内写入三个浮点型参数
入口:	uint16_t index-参数索引号,
			float WriteData1-待写入的参数1,
			float WriteData2-待写入的参数2,
			float WriteData3-待写入的参数3,
			FLIGHT_PARAMETER *Table-参数表结构体
出口:	无
备注:	无
作者:	无名创新
***************************************/
void WriteFlashParameter_Three(uint16_t index,float WriteData1,float WriteData2,float WriteData3,FLIGHT_PARAMETER *Table)
{
  ReadFlashParameterALL(Table);//先把片区内的所有数据都都出来
  Table->Parameter_Table[index]=WriteData1;//将需要更改的字段赋新值
  Table->Parameter_Table[index+1]=WriteData2;//将需要更改的字段赋新值
  Table->Parameter_Table[index+2]=WriteData3;//将需要更改的字段赋新值
  EEPROMProgram((uint32_t *)(&Table->Parameter_Table[index]),WP_FLASH_BASE+4*index,12);
}


