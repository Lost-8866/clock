
#ifndef __IIC1_H__
#define __IIC1_H__

#include "stm32f10x.h"
#include "ds3231.h"

///*端口设置*/
#define IIC_SCL         PBout(8)       //SCLK  时钟   
#define IIC_SDA         PBout(9)       //SDA   写数据  
#define READ_SDA        PBin(9)        //SDA   读数据

#define	WriteAddress	DS3231_WriteAddress
#define RReadAddress    DS3231_ReadAddress

void I2C1_Init(void);
void I2C1_Write_Byte(u8 address,u8 data);
u8 I2C1_Read_Data(u8 read_addr);
u8 BCD2HEX(u8 val);
u16 B_BCD(u8 val);


#endif
