#include <stdint.h>
#include <stdbool.h>

#include "I2C.h"

#include "hw_memmap.h"
#include "gpio.h"
#include "pin_map.h"
#include "myiic.h"
#include "sysctl.h"
/***********************************************************
@函数名：Init_I2C
@入口参数：无
@出口参数：无
功能描述：TM4C硬件I2C初始化
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void Init_I2C(void) {
  SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1); // Enable I2C1 peripheral
  SysCtlDelay(2); // Insert a few cycles after enabling the peripheral to allow the clock to be fully activated
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // Enable GPIOA peripheral
  SysCtlDelay(2); // Insert a few cycles after enabling the peripheral to allow the clock to be fully activated
  
  // Use alternate function
  GPIOPinConfigure(GPIO_PA6_I2C1SCL);
  GPIOPinConfigure(GPIO_PA7_I2C1SDA);
  
  GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6); // Use pin with I2C SCL peripheral
  GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7); // Use pin with I2C peripheral
	
  I2CMasterInitExpClk(I2C1_BASE, 400*100000,true); // Enable and set frequency to 400 kHz  100
  //I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(),true);          
  SysCtlDelay(2); // Insert a few cycles after enabling the I2C to allow the clock to be fully activated
}

/***********************************************************
@函数名：i2cWriteData
@入口参数：uint8_t addr, uint8_t regAddr, uint8_t *data, uint8_t length
@出口参数：无
功能描述：I2C写数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void i2cWriteData(uint8_t addr, uint8_t regAddr, uint8_t *data, uint8_t length) {
  I2CMasterSlaveAddrSet(I2C1_BASE, addr, false); // Set to write mode
  
  I2CMasterDataPut(I2C1_BASE, regAddr); // Place address into data register
  I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START); // Send start condition
  while (I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
  
  for (uint8_t i = 0; i < length - 1; i++) {
    I2CMasterDataPut(I2C1_BASE, data[i]); // Place data into data register
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT); // Send continues condition
    while (I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
  }
  
  I2CMasterDataPut(I2C1_BASE, data[length - 1]); // Place data into data register
  I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH); // Send finish condition
  while (I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
}

/***********************************************************
@函数名：i2cRead
@入口参数：uint8_t addr, uint8_t regAddr
@出口参数：无
功能描述：I2C读数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
uint8_t i2cRead(uint8_t addr, uint8_t regAddr) {
  I2CMasterSlaveAddrSet(I2C1_BASE, addr, false); // Set to write mode
  
  I2CMasterDataPut(I2C1_BASE, regAddr); // Place address into data register
  I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND); // Send data
  while (I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
  
  I2CMasterSlaveAddrSet(I2C1_BASE, addr, true); // Set to read mode
  
  I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE); // Tell master to read data
  while (I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
  return I2CMasterDataGet(I2C1_BASE); // Read data
}

/***********************************************************
@函数名：i2cWrite
@入口参数：uint8_t addr, uint8_t regAddr, uint8_t data
@出口参数：无
功能描述：I2C写数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void i2cWrite(uint8_t addr, uint8_t regAddr, uint8_t data) {
  i2cWriteData(addr, regAddr, &data, 1);
}


/***********************************************************
@函数名：i2cReadData
@入口参数：uint8_t addr, uint8_t regAddr, uint8_t *data,
uint8_t length
@出口参数：无
功能描述：I2C读数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void i2cReadData(uint8_t addr, uint8_t regAddr, uint8_t *data, uint8_t length) {
  I2CMasterSlaveAddrSet(I2C1_BASE, addr, false); // Set to write mode
  I2CMasterDataPut(I2C1_BASE, regAddr); // Place address into data register
  I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND); // Send data
  while (I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done 
  I2CMasterSlaveAddrSet(I2C1_BASE, addr, true); // Set to read mode
  I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START); // Send start condition
  while (I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
  data[0] = I2CMasterDataGet(I2C1_BASE); // Place data into data register 
  for (uint8_t i = 1; i < length - 1; i++) 
	{
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT); // Send continues condition
    while (I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
    data[i] = I2CMasterDataGet(I2C1_BASE); // Place data into data register
  }
  I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH); // Send finish condition
  while (I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
  data[length - 1] = I2CMasterDataGet(I2C1_BASE); // Place data into data register
}


void Single_WriteI2C(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)
{
  i2cWrite(SlaveAddress,REG_Address,REG_data);
}	

unsigned char Single_ReadI2C(unsigned char SlaveAddress,unsigned char REG_Address)
{
  return i2cRead(SlaveAddress,REG_Address);
}

short int Double_ReadI2C(unsigned char SlaveAddress,unsigned char REG_Address)
{
  unsigned char msb , lsb ;
  msb = i2cRead(SlaveAddress,REG_Address);
  lsb = i2cRead(SlaveAddress,REG_Address+1);
  return ( ((short int)msb) << 8 | lsb) ;
}

////////////////////////
/***********************************************************
@函数名：Init_I2C2
@入口参数：无
@出口参数：无
功能描述：TM4C硬件I2C初始化
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void Init_I2C0(void) {
  SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0); // Enable I2C0 peripheral
  SysCtlDelay(2); // Insert a few cycles after enabling the peripheral to allow the clock to be fully activated
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); // Enable GPIOB peripheral
  SysCtlDelay(2); // Insert a few cycles after enabling the peripheral to allow the clock to be fully activated
  
  // Use alternate function
  GPIOPinConfigure(GPIO_PB2_I2C0SCL);
  GPIOPinConfigure(GPIO_PB3_I2C0SDA);
  
  GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2); // Use pin with I2C SCL peripheral
  GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3); // Use pin with I2C peripheral
  
  //I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(),true); // Enable and set frequency to 400 kHz
  I2CMasterInitExpClk(I2C0_BASE, 800*100000,true);//3200
  //I2CMasterInitExpClk(I2C0_BASE, 400*100000,true);
	SysCtlDelay(2); // Insert a few cycles after enabling the I2C to allow the clock to be fully activated
}

/***********************************************************
@函数名：i2cWriteData
@入口参数：uint8_t addr, uint8_t regAddr, uint8_t *data, uint8_t length
@出口参数：无
功能描述：I2C写数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/

void i2c0WriteData(uint8_t addr, uint8_t regAddr, uint8_t *data, uint8_t length) 
{
	int16_t i2c0_try_cnt=0;
  I2CMasterSlaveAddrSet(I2C0_BASE, addr, false); // Set to write mode  
  I2CMasterDataPut(I2C0_BASE, regAddr); // Place address into data register
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START); // Send start condition 
	
	i2c0_try_cnt=500;
	while(I2CMasterBusy(I2C0_BASE)&&i2c0_try_cnt--) ; // Wait until transfer is done
  
	for (uint8_t i = 0; i < length - 1; i++) 
	{
    I2CMasterDataPut(I2C0_BASE, data[i]); // Place data into data register
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT); // Send continues condition
    i2c0_try_cnt=500;
		while(I2CMasterBusy(I2C0_BASE)&&i2c0_try_cnt--); // Wait until transfer is done
  }
  
  I2CMasterDataPut(I2C0_BASE, data[length - 1]); // Place data into data register
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH); // Send finish condition
	
	i2c0_try_cnt=500;
  while(I2CMasterBusy(I2C0_BASE)&&i2c0_try_cnt--);// Wait until transfer is done
}

/***********************************************************
@函数名：i2cRead
@入口参数：uint8_t addr, uint8_t regAddr
@出口参数：无
功能描述：I2C读数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
uint8_t i2c0Read(uint8_t addr, uint8_t regAddr) {
  int16_t i2c0_try_cnt=0;
	I2CMasterSlaveAddrSet(I2C0_BASE, addr, false); // Set to write mode
  
  I2CMasterDataPut(I2C0_BASE, regAddr); // Place address into data register
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND); // Send data
	
	i2c0_try_cnt=500;
  while (I2CMasterBusy(I2C0_BASE)&&i2c0_try_cnt--); // Wait until transfer is done
  
  I2CMasterSlaveAddrSet(I2C0_BASE, addr, true); // Set to read mode
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE); // Tell master to read data
  
	i2c0_try_cnt=500;
	while(I2CMasterBusy(I2C0_BASE)&&i2c0_try_cnt--);  // Wait until transfer is done
	
  return I2CMasterDataGet(I2C0_BASE); // Read data
}

/***********************************************************
@函数名：i2cWrite
@入口参数：uint8_t addr, uint8_t regAddr, uint8_t data
@出口参数：无
功能描述：I2C写数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void i2c0Write(uint8_t addr, uint8_t regAddr, uint8_t data) {
  i2c0WriteData(addr, regAddr, &data, 1);
}


/***********************************************************
@函数名：i2cReadData
@入口参数：uint8_t addr, uint8_t regAddr, uint8_t *data,
uint8_t length
@出口参数：无
功能描述：I2C读数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void i2c0ReadData(uint8_t addr, uint8_t regAddr, uint8_t *data, uint8_t length) {
	int16_t i2c0_try_cnt=0;
  I2CMasterSlaveAddrSet(I2C0_BASE, addr, false); // Set to write mode
  I2CMasterDataPut(I2C0_BASE, regAddr); // Place address into data register
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND); // Send data
	
	i2c0_try_cnt=500;
  while (I2CMasterBusy(I2C0_BASE)&&i2c0_try_cnt--); // Wait until transfer is done 
	
	
  I2CMasterSlaveAddrSet(I2C0_BASE, addr, true); // Set to read mode
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START); // Send start condition
	
	i2c0_try_cnt=500;
  while (I2CMasterBusy(I2C0_BASE)&&i2c0_try_cnt--); // Wait until transfer is done
	
	
  data[0] = I2CMasterDataGet(I2C0_BASE); // Place data into data register 
  for (uint8_t i = 1; i < length - 1; i++) 
	{
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT); // Send continues condition
		
		i2c0_try_cnt=500;
    while (I2CMasterBusy(I2C0_BASE)&&i2c0_try_cnt--); // Wait until transfer is done
		
    data[i] = I2CMasterDataGet(I2C0_BASE); // Place data into data register
  }
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH); // Send finish condition
	
	i2c0_try_cnt=500;
  while (I2CMasterBusy(I2C0_BASE)&&i2c0_try_cnt--); // Wait until transfer is done

  data[length - 1] = I2CMasterDataGet(I2C0_BASE); // Place data into data register
}
