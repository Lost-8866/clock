 
#ifndef __DS3231_H__
#define __DS3231_H__

#include "stm32f10x.h"
#include "sys.h"

//==============================================DS3231??????==================================================
#define     DS3231_IIC_CLK             RCC_APB2Periph_GPIOB
#define     DS3231_IIC_PORT            GPIOB
#define     DS3231_IIC_SCL_PIN         GPIO_Pin_6
#define     DS3231_IIC_SDA_PIN         GPIO_Pin_7

#define     DS3231_IIC_SCL             PBout(6)
#define     DS3231_IIC_SDA             PBout(7)
#define     DS3231_READ_SDA            PBin(7)   //????SDA 

//=============================================================================================================

//DS3231?????????
#define DS3231_WriteAddress 0xD0    //????��??? 
#define DS3231_ReadAddress  0xD1    //?????????
#define DS3231_SECOND       0x00    //??
#define DS3231_MINUTE       0x01    //??
#define DS3231_HOUR         0x02    //?
#define DS3231_WEEK         0x03    //????
#define DS3231_DAY          0x04    //??
#define DS3231_MONTH        0x05    //??
#define DS3231_YEAR         0x06    //??
//????1            
#define DS3231_SALARM1ECOND 0x07    //??
#define DS3231_ALARM1MINUTE 0x08    //??
#define DS3231_ALARM1HOUR   0x09    //?
#define DS3231_ALARM1WEEK   0x0A    //????/??
//????2
#define DS3231_ALARM2MINUTE 0x0b    //??
#define DS3231_ALARM2HOUR   0x0c    //?
#define DS3231_ALARM2WEEK   0x0d    //????/??

#define DS3231_CONTROL      0x0e    //????????
#define DS3231_STATUS       0x0f    //???????
#define BSY                 2       //?
#define OSF                 7       //?????????
#define DS3231_XTAL         0x10    //????????????
#define DS3231_TEMPERATUREH 0x11    //????????????(8��)
#define DS3231_TEMPERATUREL 0x12    //????????????(??2��) 

typedef struct 
{
	u8 hour;
	u8 min;
	u8 sec;            
	u8 year;
	u8 month;
	u8 date;
	u8 week;
	u8 temperature;
}_calendar_obj;                     
extern _calendar_obj calendar;    //????????

//IIC???��???????
void DS3231_IIC_Init(void);                                                         //?????IIC??IO??
void DS3231_IIC_Start(void);                                                         //????IIC??????
void DS3231_IIC_Stop(void);                                                          //????IIC?????
void DS3231_IIC_Send_Byte(u8 txd);                                                   //IIC??????????
void DS3231_IIC_SDA_OUT(void);                                                       //SDA???????????
void DS3231_IIC_SDA_IN(void);                                                        //SDA??????????
u8 DS3231_IIC_Read_Byte(unsigned char ack);                                          //IIC?????????
u8 DS3231_IIC_Wait_Ack(void);                                                        //IIC???ACK???
void DS3231_IIC_Ack(void);                                                           //IIC????ACK???
void DS3231_IIC_NAck(void);                                                          //IIC??????ACK???

//DS3231???��???????
u8 DS3231_I2C_Read_Data(u8 read_addr);                                                //???DS3231???
void DS3231_I2C_Write_Byte(u8 address,u8 data);                                       //��??DS3231???

void DS3231_SetTime(u8 yea,u8 mon,u8 da,u8 we,u8 hou,u8 min,u8 sec);
void get_show_time(void);
void get_show_Temperature(void);

#endif


