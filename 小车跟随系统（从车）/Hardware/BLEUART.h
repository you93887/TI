#ifndef __BLEUART_H
#define __BLEUART_H


#define Serial_Getlength 4      //�������ݳ���

void BLEUART_Init(void);        //�������ڳ�ʼ��
uint32_t Serial_NumberChange(uint8_t *Array, uint8_t Length);//�����ڽ��յ��ַ�ת��Ϊ����
void BLE_Send(uint32_t number,uint8_t BLE_Sendlength);//�����������ݷ���
uint32_t BLE_GetNumber();       //�����������ڽ��յ�����

#endif
