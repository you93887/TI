#ifndef __MYIIC_H
#define __MYIIC_H


void Init_I2C(void);
void i2cWrite(uint8_t addr, uint8_t regAddr, uint8_t data) ;
uint8_t i2cRead(uint8_t addr, uint8_t regAddr);
void i2cReadData(uint8_t addr, uint8_t regAddr, uint8_t *data, uint8_t length);

void Init_I2C0(void);
void i2c0Write(uint8_t addr, uint8_t regAddr, uint8_t data) ;
uint8_t i2c0Read(uint8_t addr, uint8_t regAddr);
void i2c0ReadData(uint8_t addr, uint8_t regAddr, uint8_t *data, uint8_t length);

void Single_WriteI2C(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);
unsigned char Single_ReadI2C(unsigned char SlaveAddress,unsigned char REG_Address);
short int Double_ReadI2C(unsigned char SlaveAddress,unsigned char REG_Address);
#endif
















