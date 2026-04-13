
#include "ds3231.h"
#include "delay.h"

_calendar_obj calendar;

//?????IIC
void DS3231_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(DS3231_IIC_CLK, ENABLE );
    
    GPIO_InitStructure.GPIO_Pin=DS3231_IIC_SCL_PIN|DS3231_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP ;                            //???????
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(DS3231_IIC_PORT,&GPIO_InitStructure);

    DS3231_IIC_SCL=1;
    DS3231_IIC_SDA=1;

}

//SDA???????????
void DS3231_IIC_SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin=DS3231_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//SDA???????
    GPIO_Init(DS3231_IIC_PORT,&GPIO_InitStructure);

}

//SDA??????????
void DS3231_IIC_SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin=DS3231_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//SCL????????
    GPIO_Init(DS3231_IIC_PORT,&GPIO_InitStructure);
    
}

//????IIC??????
void DS3231_IIC_Start(void)
{
    DS3231_IIC_SDA_OUT();                                                                 //sda?????
    DS3231_IIC_SDA=1;      
    DS3231_IIC_SCL=1;
    delay_us(4);
    DS3231_IIC_SDA=0;                                                                 //START:when CLK is high,DATA change form high to low 
    delay_us(4);
    DS3231_IIC_SCL=0;                                                                 //??I2C???????????????????? 
}

//????IIC?????
void DS3231_IIC_Stop(void)
{
    DS3231_IIC_SDA_OUT();                                                                 //sda?????
    DS3231_IIC_SCL=0;
    DS3231_IIC_SDA=0;                                                                 //STOP:when CLK is high DATA change form low to high
    delay_us(4);
    DS3231_IIC_SCL=1; 
    DS3231_IIC_SDA=1;                                                                 //????I2C??????????
    delay_us(4);                                
}

//????????????
//???????1????????????
//        0???????????
u8 DS3231_IIC_Wait_Ack(void)
{
    u8 ucErrTime=0;
    DS3231_IIC_SDA_IN();                                                                  //SDA?????????  
    DS3231_IIC_SDA=1;delay_us(1);      
    DS3231_IIC_SCL=1;delay_us(1);     
    while(DS3231_READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            DS3231_IIC_Stop();
            return 1;
        }
    }
    DS3231_IIC_SCL=0;                                                                 //??????0      
    return 0;  
}

//????ACK???
void DS3231_IIC_Ack(void)
{
    DS3231_IIC_SCL=0;
    DS3231_IIC_SDA_OUT();
    DS3231_IIC_SDA=0;
    delay_us(2);
    DS3231_IIC_SCL=1;
    delay_us(2);
    DS3231_IIC_SCL=0;
}

//??????ACK???
void DS3231_IIC_NAck(void)
{
    DS3231_IIC_SCL=0;
    DS3231_IIC_SDA_OUT();
    DS3231_IIC_SDA=1;
    delay_us(2);
    DS3231_IIC_SCL=1;
    delay_us(2);
    DS3231_IIC_SCL=0;
}

//IIC??????????
//?????????????
//1???????
//0???????
void DS3231_IIC_Send_Byte(u8 txd)
{
    u8 t;   
    DS3231_IIC_SDA_OUT();      
    DS3231_IIC_SCL=0;                                                                 //????????????????
    for(t=0;t<8;t++)
    {
        DS3231_IIC_SDA=(txd&0x80)>>7;
        txd<<=1;      
        delay_us(2);                                                           //??TEA5767?????????????????
        DS3231_IIC_SCL=1;
        delay_us(2); 
        DS3231_IIC_SCL=0;
        delay_us(2);
    }    
}

//??1??????ack=1???????ACK??ack=0??????nACK   
u8 DS3231_IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    DS3231_IIC_SDA_IN();                                                                  //SDA?????????
    for(i=0;i<8;i++ )
    {
        DS3231_IIC_SCL=0; 
        delay_us(2);
        DS3231_IIC_SCL=1;
        receive<<=1;
        if(DS3231_READ_SDA)receive++;
        delay_us(1); 
    }
    if (!ack)
        DS3231_IIC_NAck();                                                            //????nACK
    else
        DS3231_IIC_Ack();                                                             //????ACK   
    return receive;
}

//???????????????????
//ReadAddr:???????????  
//?????  :??????????
u8 DS3231_I2C_Read_Data(u8 read_addr)
{
    u8 temp=0;          
    DS3231_IIC_Start();  
    DS3231_IIC_Send_Byte(DS3231_WriteAddress);                                                    //????????§Õ????     
    DS3231_IIC_Wait_Ack(); 
    DS3231_IIC_Send_Byte(read_addr);                                                       //???????
    DS3231_IIC_Wait_Ack();

    DS3231_IIC_Start();      
    DS3231_IIC_Send_Byte(DS3231_ReadAddress);                                                  //??????????????                
    DS3231_IIC_Wait_Ack();     
    temp=DS3231_IIC_Read_Byte(0);               
    DS3231_IIC_Stop();                                                                //?????????????    
    return temp;
}

//????????§Õ?????????
//WriteAddr :§Õ????????????    
//DataToWrite:?§Õ???????
void DS3231_I2C_Write_Byte(u8 address,u8 data)
{
    DS3231_IIC_Start();  
    DS3231_IIC_Send_Byte(DS3231_WriteAddress);                                                    //????????§Õ????     
    DS3231_IIC_Wait_Ack();    
    DS3231_IIC_Send_Byte(address);                                                       //???????
    DS3231_IIC_Wait_Ack();       
    DS3231_IIC_Send_Byte(data);                                                       //???????                              
    DS3231_IIC_Wait_Ack();   
    DS3231_IIC_Stop();                                                                 //?????????????     
}

//BCD?HEX
u8 BCD2HEX(u8 val)
{
    u8 temp;  
    temp = val&0xf0;  
    temp = temp>>4;  
    temp = temp*10;  
    temp = temp+(val&0x0f);  
    return temp;  
}

//HEX?BCD
u16 B_BCD(u8 val)
{
    u8 temp;  
    temp = val/10;  
    temp = temp<<4;  
    temp = temp|(val%10);  
    return temp;  
}

void DS3231_SetTime(u8 yea,u8 mon,u8 da,u8 we,u8 hou,u8 min,u8 sec)
{
  u8 temp=0;
  
  temp=B_BCD(yea);
  DS3231_I2C_Write_Byte(0x06,temp);
  
  temp=B_BCD(mon);
  DS3231_I2C_Write_Byte(0x05,temp);
   
  temp=B_BCD(da);
  DS3231_I2C_Write_Byte(0x04,temp);
  
	temp=B_BCD(we);
  DS3231_I2C_Write_Byte(0x03,temp);
	
  temp=B_BCD(hou);
  DS3231_I2C_Write_Byte(0x02,temp);
  
  temp=B_BCD(min);
  DS3231_I2C_Write_Byte(0x01,temp);
  
  temp=B_BCD(sec);
  DS3231_I2C_Write_Byte(0x00,temp);
}


void get_show_time(void)
{
	calendar.year=DS3231_I2C_Read_Data(0x06);  
	calendar.year=BCD2HEX(calendar.year);

	calendar.month=DS3231_I2C_Read_Data(0x05); 
	calendar.month=BCD2HEX(calendar.month);

	calendar.date=DS3231_I2C_Read_Data(0x04);  
	calendar.date=BCD2HEX(calendar.date);

	calendar.week=DS3231_I2C_Read_Data(0x03);  
	calendar.week=BCD2HEX(calendar.week);

	calendar.hour=DS3231_I2C_Read_Data(0x02); 
	calendar.hour&=0x3f;                    
	calendar.hour=BCD2HEX(calendar.hour);

	calendar.min=DS3231_I2C_Read_Data(0x01);
	calendar.min=BCD2HEX(calendar.min);


	calendar.sec=DS3231_I2C_Read_Data(0x00);
	calendar.sec=BCD2HEX(calendar.sec);
}

void get_show_Temperature()
{
	DS3231_I2C_Write_Byte(DS3231_CONTROL,0x20|0x05);
	calendar.temperature=DS3231_I2C_Read_Data(DS3231_TEMPERATUREH);
}
