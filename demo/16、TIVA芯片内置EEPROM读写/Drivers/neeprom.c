#include "headfile.h"
#include "neeprom.h"

#define WP_FLASH_BASE PARAMETER_TABLE_STARTADDR
FLIGHT_PARAMETER Trackless_Params;


/***************************************
������:	void EEPROM_Init(void)
˵��:	��Ƭ���Դ�2K����EEPROM��ʼ��
���:	��
����:	��
��ע:	��
����:	��������
***************************************/
void EEPROM_Init(void)
{
  /* EEPROM SETTINGS */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0); // EEPROM activate
  EEPROMInit(); // EEPROM start
	//EEPROMMassErase();
}

/***************************************
������:	void ReadFlashParameterALL(FLIGHT_PARAMETER *WriteData)
˵��:	��ȡEEPEROM�������е����в���
���:	FLIGHT_PARAMETER *WriteData-���������ݽṹ��
����:	��
��ע:	��
����:	��������
***************************************/
void ReadFlashParameterALL(FLIGHT_PARAMETER *WriteData)
{
  EEPROMRead((uint32_t *)(&WriteData->Parameter_Table),WP_FLASH_BASE,FLIGHT_PARAMETER_TABLE_NUM*4);
}

/***************************************
������:	void ReadFlashParameterOne(uint16_t index,float *ReadData)
˵��:	��ȡEEPEROM�������еĵ��������Ͳ���
���:	uint16_t index-����������,
			float *ReadData-���ڴ洢����ȡ�Ĳ���
����:	��
��ע:	��
����:	��������
***************************************/
void ReadFlashParameterOne(uint16_t index,float *ReadData)
{
  EEPROMRead((uint32_t *)(ReadData),WP_FLASH_BASE+4*index,4);
}

/***************************************
������:	void ReadFlashParameterThree(uint16_t index,float *ReadData1,float *ReadData2,float *ReadData3)
˵��:	��ȡEEPEROM�������е����������Ͳ���
���:	uint16_t index-����������,
			float *ReadData1-���ڴ洢����ȡ�Ĳ���1,
			float *ReadData2-���ڴ洢����ȡ�Ĳ���2,
			float *ReadData3-���ڴ洢����ȡ�Ĳ���3
����:	��
��ע:	��
����:	��������
***************************************/
void ReadFlashParameterThree(uint16_t index,float *ReadData1,float *ReadData2,float *ReadData3)
{
  EEPROMRead((uint32_t *)(ReadData1),WP_FLASH_BASE+4*index,4);;
  EEPROMRead((uint32_t *)(ReadData2),WP_FLASH_BASE+4*index+4,4);
  EEPROMRead((uint32_t *)(ReadData3),WP_FLASH_BASE+4*index+8,4);
}


/***************************************
������:	void WriteFlashParameter(uint16_t index,float WriteData,FLIGHT_PARAMETER *Table)
˵��: ��EEPROMĳһ��ַ��д�뵥�������Ͳ���
���:	uint16_t index-����������,
			float WriteData-��д��Ĳ���,
			FLIGHT_PARAMETER *Table-������ṹ��
����:	��
��ע:	��
����:	��������
***************************************/
void WriteFlashParameter(uint16_t index,float WriteData,FLIGHT_PARAMETER *Table)
{
  ReadFlashParameterALL(Table);//�Ȱ�Ƭ���ڵ��������ݶ�������ֵ
  Table->Parameter_Table[index]=WriteData;//����Ҫ���ĵ��ֶθ���
  EEPROMProgram((uint32_t *)(&Table->Parameter_Table[index]),WP_FLASH_BASE+4*index,4);
}


/***************************************
������:	void WriteFlashParameter_Two(uint16_t index,float WriteData1,float WriteData2,FLIGHT_PARAMETER *Table)
˵��: ��EEPROMĳһ��ַ��д�����������Ͳ���
���:	uint16_t index-����������,
			float WriteData1-��д��Ĳ���1,
			float WriteData2-��д��Ĳ���2,
			FLIGHT_PARAMETER *Table-������ṹ��
����:	��
��ע:	��
����:	��������
***************************************/
void WriteFlashParameter_Two(uint16_t index,float WriteData1,float WriteData2,FLIGHT_PARAMETER *Table)
{
  ReadFlashParameterALL(Table);//�Ȱ�Ƭ���ڵ��������ݶ�������
  Table->Parameter_Table[index]=WriteData1;//����Ҫ���ĵ��ֶθ���ֵ
  Table->Parameter_Table[index+1]=WriteData2;//����Ҫ���ĵ��ֶθ���ֵ
  EEPROMProgram((uint32_t *)(&Table->Parameter_Table[index]),WP_FLASH_BASE+4*index,8);
}


/***************************************
������:	void WriteFlashParameter_Three(uint16_t index,float WriteData1,float WriteData2,float WriteData3,FLIGHT_PARAMETER *Table)
˵��: ��EEPROMĳһ��ַ��д�����������Ͳ���
���:	uint16_t index-����������,
			float WriteData1-��д��Ĳ���1,
			float WriteData2-��д��Ĳ���2,
			float WriteData3-��д��Ĳ���3,
			FLIGHT_PARAMETER *Table-������ṹ��
����:	��
��ע:	��
����:	��������
***************************************/
void WriteFlashParameter_Three(uint16_t index,float WriteData1,float WriteData2,float WriteData3,FLIGHT_PARAMETER *Table)
{
  ReadFlashParameterALL(Table);//�Ȱ�Ƭ���ڵ��������ݶ�������
  Table->Parameter_Table[index]=WriteData1;//����Ҫ���ĵ��ֶθ���ֵ
  Table->Parameter_Table[index+1]=WriteData2;//����Ҫ���ĵ��ֶθ���ֵ
  Table->Parameter_Table[index+2]=WriteData3;//����Ҫ���ĵ��ֶθ���ֵ
  EEPROMProgram((uint32_t *)(&Table->Parameter_Table[index]),WP_FLASH_BASE+4*index,12);
}


