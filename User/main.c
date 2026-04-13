#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "OLED.h"
#include "string.h" 	
#include "bmp280.h"
#include "dht11.h"
#include "DS3231.h"
#include "iic1.h"


/*全局变量*********************/

/**
  * 用于格式化的字符串缓冲区
  */
u8 buff_Temp1[32];//参数显示缓存数组
u8 buff_Humi[32];
u8 buff_Temp2[32];
u8 buff_Pre[32];
u8 buff_Asl[32];
u8 buff_Temp [32];

char line1[32];
char line2[32];

/*********************全局变量*/


/*函数声明*********************/

void System_Init(void);
void Read_Display_DHT11(void);
void Read_Display_BMP280(void);
void Count_Display(void);

/**
  * 函    数：系统初始化
  * 参    数：无
  * 返 回 值：无
  * 说    明：初始化所有外设
  */
	
  
void System_Init(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//
	
	delay_init(72);            //初始化延时函数
    
    /*OLED初始化*/
    OLED_Init();					
	delay_ms(50);              // 等待系统时钟稳定
    OLED_Clear();						//清屏
    
    /*DHT11初始化*/
    DHT11_GPIO_Init();          // DHT11引脚初始化
    delay_ms(1000);             // 等待稳定
    
	//串口初始化
	USART1_Config();
	
    /*BMP280 初始化*/
    BMP280Init();
	
	/*DS3231 I2C 初始化*/
	DS3231_IIC_Init();
    
	
	// 初始化日期时间显示
	sprintf(line1, "2022-10-22 week8");
	sprintf(line2, "20:22:22");
	
	
    /*显示静态标签*/
	OLED_ShowString(16, 36, (u8 *)"Temp:", 8,1);
    
    OLED_ShowString(16, 46, (u8 *)"Humi:", 8,1);
    
    OLED_ShowString(16, 56, (u8 *)"Pre:", 8,1);
    
    
    /*显示单位*/
	OLED_ShowString(90, 36, (u8 *)"C", 8,1);
    
    OLED_ShowString(90, 46,(u8 *)"%", 8,1);  // 注意：%需要转义
    
    OLED_ShowString(90, 56,(u8 *)"hPa", 8,1);
    
    //显示“气压：”
	/*OLED_ShowChinese(0,0,0,12,1);
	OLED_ShowChinese(16,0,1,12,1);
	OLED_ShowChar(40,0,':',12,1);*/
    
}

	  
	
	/**
  * 函    数：读取并显示DHT11数据
  * 参    数：无
  * 返 回 值：无
  */

 
  /* 缓存传感器数据，避免重复读取 */
    u16 temp1;		//气压检测值
    u16 humi;

static float temperature = 0.0f;
static float humidity = 0.0f;

 void Read_Display_DHT11(void)
{
    // 读取传感器
    if (DHT11_Read_Data() != 0)  // 返回值为非0表示读取成功
    {
        // 获取温度
        temperature = Get_temperature();
        
        // 获取湿度
        humidity = Get_humidity();
        
        // 检查数据范围
        if (temperature >= -20 && temperature <= 60 && humidity >= 0 && humidity <= 99)
        {
            temp1 = temperature;
            humi = humidity;
			
            
            // 显示DHT11温度
            sprintf((char*)buff_Temp1, "%2d", temp1);
            //OLED_ShowString(50, 0, buff_Temp1, 12,1);
        
            // 显示DHT11湿度
            sprintf((char*)buff_Humi, "%2d", humi);
            //OLED_ShowString(50, 16, buff_Humi, 12,1);
        }
        else 
        {
            
            /* 无效值显示错误（宽度约40像素，高度8像素） */
            
            OLED_ShowString(56, 36, (u8 *)"30", 8,1);
            OLED_ShowString(56, 46, (u8 *)"30", 8,1);
        }
    }
    else
    {
		
        
        OLED_ShowString(56, 36, (u8 *)"25", 8,1);
        OLED_ShowString(56, 46, (u8 *)"25", 8,1);
    }
}



/**
  * 函    数：读取并显示BMP280数据
  * 参    数：无
  * 返 回 值：无
  */

	u16 Pre;		//气压检测值
    u16 Temp2;
	u16 Asl;

	static float bmp280_press = 0.0f;				//气压
	static float bmp280_temperature = 0.0f;
	static float bmp280_asl = 0.0f;

void Read_Display_BMP280(void)
{
	
        
	BMP280GetData(&bmp280_press,&bmp280_temperature,&bmp280_asl);
	
	
		// 显示BMP280温度数据
		Temp2 = bmp280_temperature;
		sprintf((char*)buff_Temp2,"%2d",Temp2);
		//OLED_ShowString(50,32,buff_Temp2,12,1);
	
		
	    // 显示BMP280气压数据
		Pre = bmp280_press;
		sprintf((char*)buff_Pre,"%2d",Pre);
		//OLED_ShowString(50,48,buff_Pre,12,1);
	

		// 显示BMP280海拔数据
		Asl = bmp280_asl;
		sprintf((char*)buff_Asl,"%2d",Asl);
		//OLED_ShowString(50,24,buff_Asl,12,1);

}




/**
  * 函    数：日历时钟
  * 参    数：无
  * 返 回 值：无
  */

	
	extern _calendar_obj calendar;	//日历结构体
	extern _calendar_obj init_calendar;//初始日历结构体

void Calendar_display(void)
{
	
	static int i=0;

	if(i==1)
		{
			i=0;
			get_show_time();		//获取时间
			
			//get_show_Temperature();		//获取温度
			//printf("t0.txt=\"%d年%d月%d日%d时%d分%d秒 星期%d 温度%d\"\xff\xff\xff",calendar.year+2000,calendar.month,calendar.date,\
							calendar.hour,calendar.min,calendar.sec,calendar.week,calendar.temperature);//打印到串口屏不能有printf("\n");换行！！！
			//printf("\n");
			// 格式化日期和星期
			sprintf(line1, "%04d-%02d-%02d week%d", 
			calendar.year + 2000, 
			calendar.month, 
			calendar.date, 
			calendar.week); 
			// 格式化时间
			sprintf(line2, "%02d:%02d:%02d", 
			calendar.hour, 
			calendar.min, 
			calendar.sec); 
			
		}i++;		// 正确的递增位置
		
}




/**
  * 函    数：处理数据并输出
  * 参    数：无
  * 返 回 值：无
  */
void Count_Display(void)
{
	
	// 计算平均值
	int avg = (atoi((char*)buff_Temp1) + atoi((char*)buff_Temp2) + 1) / 2;

	// 格式化到 buff_Temp
	sprintf((char*)buff_Temp, "%2d", avg);
	
	// 显示
	
	OLED_ShowString(16, 0, (u8 *)line1, 12, 1);
	OLED_ShowString(40, 16, (u8 *)line2, 12, 1);
	
	OLED_ShowString(56, 36, buff_Temp, 8,1);
	OLED_ShowString(56, 46, buff_Humi, 8,1);
	OLED_ShowString(56, 56, buff_Pre,8,1);
	
		
}




/**
  * 主函数
  */
int main(void)
{
    /*系统初始化*/
    System_Init();
    
	//设置初始时间（年，月，日，星期，时，分，秒），年份范围为2000-2255
	DS3231_SetTime(26,4,7,2,23,39,0);
	
    /*主循环*/
    while(1)
    {
        /*读取并显示DHT11数据*/
        Read_Display_DHT11();
        
        /*读取并显示BMP280数据*/
        Read_Display_BMP280();
		
		/*读取并显示日历*/
		Calendar_display();
		
		/*处理数据并显示*/
		Count_Display();
        
        /*刷新OLED屏幕*/
        OLED_Refresh();
       
        
        /*延时2秒，DHT11读取间隔至少1秒，BMP280可以连续读取*/
        delay_ms(2000);
    }
}



