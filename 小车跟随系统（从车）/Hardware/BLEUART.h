#ifndef __BLEUART_H
#define __BLEUART_H


#define Serial_Getlength 4      //接收数据长度

void BLEUART_Init(void);        //蓝牙串口初始化
uint32_t Serial_NumberChange(uint8_t *Array, uint8_t Length);//将串口接收的字符转化为数据
void BLE_Send(uint32_t number,uint8_t BLE_Sendlength);//蓝牙串口数据发送
uint32_t BLE_GetNumber();       //返回蓝牙串口接收的数据

#endif
