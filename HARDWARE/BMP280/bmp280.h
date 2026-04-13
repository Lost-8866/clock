#ifndef __BMP280_H
#define __BMP280_H
#include "sys.h"
#include "stdbool.h"

/*****************辰哥单片机设计******************
											STM32
 * 文件			:	BMP280传感器h文件                   
 * 版本			: V1.0
 * 日期			: 2024.8.20
 * MCU			:	STM32F103C8T6
 * 接口			:	见代码			
 * IP账号		:	辰哥单片机设计（同BILIBILI|抖音|快手|小红书|CSDN|公众号|视频号等）
 * 作者			:	辰哥 
 * 工作室		: 异方辰电子工作室
 * 讲解视频	:	https://www.bilibili.com/video/BV1QJWsezEKS/?share_source=copy_web
 * 官方网站	:	www.yfcdz.cn

**********************BEGIN***********************/

//==============================================BMP280硬件接口==================================================
#define		BMP280_IIC_CLK				RCC_APB2Periph_GPIOB
#define		BMP280_IIC_PORT				GPIOB
#define		BMP280_IIC_SCL_PIN		GPIO_Pin_6
#define		BMP280_IIC_SDA_PIN		GPIO_Pin_7

#define 	BMP280_IIC_SCL				PBout(6)
#define 	BMP280_IIC_SDA				PBout(7)
#define 	BMP280_READ_SDA   		PBin(7)  	//输入SDA 

//=============================================================================================================


#define BMP280_ADDR						(0xEC)
#define BMP280_DEFAULT_CHIP_ID			(0x58)

#define BMP280_CHIP_ID					(0xD0)                                 /* Chip ID Register */
#define BMP280_RST_REG					(0xE0)                                 /* Softreset Register */
#define BMP280_STAT_REG					(0xF3)                                 /* Status Register */
#define BMP280_CTRL_MEAS_REG			(0xF4)                                 /* Ctrl Measure Register */
#define BMP280_CONFIG_REG				(0xF5)                                 /* Configuration Register */
#define BMP280_PRESSURE_MSB_REG			(0xF7)                                 /* Pressure MSB Register */
#define BMP280_PRESSURE_LSB_REG			(0xF8)                                 /* Pressure LSB Register */
#define BMP280_PRESSURE_XLSB_REG		(0xF9)                                 /* Pressure XLSB Register */
#define BMP280_TEMPERATURE_MSB_REG		(0xFA)                                 /* Temperature MSB Reg */
#define BMP280_TEMPERATURE_LSB_REG		(0xFB)                                 /* Temperature LSB Reg */
#define BMP280_TEMPERATURE_XLSB_REG		(0xFC)                                 /* Temperature XLSB Reg */

#define BMP280_SLEEP_MODE				(0x00)
#define BMP280_FORCED_MODE				(0x01)
#define BMP280_NORMAL_MODE				(0x03)

#define BMP280_TEMPERATURE_CALIB_DIG_T1_LSB_REG             (0x88)
#define BMP280_PRESSURE_TEMPERATURE_CALIB_DATA_LENGTH       (24)
#define BMP280_DATA_FRAME_SIZE			(6)

#define BMP280_OVERSAMP_SKIPPED			(0x00)
#define BMP280_OVERSAMP_1X				(0x01)
#define BMP280_OVERSAMP_2X				(0x02)
#define BMP280_OVERSAMP_4X				(0x03)
#define BMP280_OVERSAMP_8X				(0x04)
#define BMP280_OVERSAMP_16X				(0x05)

//IIC所有操作函数
void BMP_IIC_Init(void);                                                           //初始化IIC的IO口				 
void BMP_IIC_Start(void);				                                           //发送IIC开始信号
void BMP_IIC_Stop(void);	  			                                           //发送IIC停止信号
void BMP_IIC_Send_Byte(u8 txd);			                                           //IIC发送一个字节
u8 BMP_IIC_Read_Byte(unsigned char ack);                                           //IIC读取一个字节
u8 BMP_IIC_Wait_Ack(void); 				                                           //IIC等待ACK信号
void BMP_IIC_Ack(void);					                                           //IIC发送ACK信号
void BMP_IIC_NAck(void);				                                           //IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	
			 
u8 iicDevReadByte(u8 devaddr,u8 addr);	                                       /*读一字节*/
void iicDevWriteByte(u8 devaddr,u8 addr,u8 data);	                           /*写一字节*/
void iicDevRead(u8 devaddr,u8 addr,u8 len,u8 *rbuf);                           /*连续读取多个字节*/
void iicDevWrite(u8 devaddr,u8 addr,u8 len,u8 *wbuf);                          /*连续写入多个字节*/


bool BMP280Init(void);
void BMP280GetData(float* pressure, float* temperature, float* asl);





#endif



