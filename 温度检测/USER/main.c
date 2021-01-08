#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "ds18b20.h" 

/*三个DS18B20的光刻ROM_MINI*/
u8 rom1[8]={0x28,0x6a,0x17,0x42,0xa,0x0,0x0,0xb2};  //rom1
u8 rom2[8]={0x28,0x6e,0xad,0xc5,0x8,0x0,0x0,0x8c};  //rom2
u8 rom3[8]={0x28,0xea,0x24,0x42,0xa,0x0,0x0,0x92};  //rom3

float dat=25.5;
int c;
u32 value;

int main(void)
{
    short temperature[3]; 
    float T[3];         //存放数值
    float TT;           //温度平均值
    vu8 key;

    delay_init();	    	//延时函数初始化	  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
    uart_init(115200);	 	//串口初始化为 115200
    LED_Init();		  		//初始化与LED连接的硬件接口
    LCD_Init();			   	//初始化LCD  
    KEY_Init();
    
    //POINT_COLOR=RED;		//设置字体为红色 
    LCD_ShowString(16,16+24*0,200,24,24,"DS18B20-TEST:");
    LCD_ShowString(16,16+24*1,200,24,24,"State:");
    LCD_ShowString(16,32+24*2,200,24,24,"Value:   .  C");
    LCD_ShowString(16,48+24*3,200,24,24,"Tem1:    .  C");
    LCD_ShowString(16,48+24*4,200,24,24,"Tem2:    .  C");
    LCD_ShowString(16,48+24*5,200,24,24,"Tem3:    .  C");
    LCD_ShowString(16,48+24*6,200,24,24,"AVRT:    .  C");
    
    while(1)
    {
        while(DS18B20_Init())
        {
            LCD_ShowString(176,16+24*0,200,24,24,"NO ");
            delay_ms(200);
            LCD_Fill(176,16+24*0,224,16+24*1,WHITE);
            delay_ms(200);
        }
        LCD_ShowString(176,16,200,24,24,"YES");
        /*阈值设定与显示*/
        key=KEY_Scan(1);
        if(key)
        {
            switch(key)
            {
                case KEY1_PRES:
                    dat+=0.5;break;
                case KEY0_PRES:
                    dat-=0.5;break;
                default:
                    break;
            }
        }   
        value=(u32)(dat*10);
        LCD_ShowNum(16+16*4+8,32+24*2,(value/10),3,24);
        LCD_ShowNum(16+16*7+8,32+24*2,(value%10),1,24);
        
        /*温度采集*/
        temperature[0]=DS18B20_Get_Temp(rom1);
        temperature[1]=DS18B20_Get_Temp(rom2);
        temperature[2]=DS18B20_Get_Temp(rom3);
        T[0]=(float)temperature[0]/10;
        T[1]=(float)temperature[0]/10;
        T[2]=(float)temperature[0]/10;
        
        /*显示温度*/
        if(temperature[0]<0)//显示rom1
        {
            LCD_ShowChar(16+16*4,48+24*3,'-',24,0);			        //显示负号
            temperature[0]=-temperature[0];					        //转为正数
        }else 
            LCD_ShowChar(16+16*4,48+24*3,'+',24,0);			        //去掉负号
        LCD_ShowNum(16+16*5,48+24*3,temperature[0]/10,2,24);	//显示正数部分	    
        LCD_ShowNum(16+16*8-8,48+24*3,temperature[0]%10,1,24);	//显示小数部分
        if(temperature[1]<0)//下面是rom2
        {
            LCD_ShowChar(16+16*4,48+24*4,'-',24,0);			        //显示负号
            temperature[1]=-temperature[1];					        //转为正数
        }else 
            LCD_ShowChar(16+16*4,48+24*4,'+',24,0);			        //去掉负号
        LCD_ShowNum(16+16*5,48+24*4,temperature[1]/10,2,24);	//显示正数部分	    
        LCD_ShowNum(16+16*8-8,48+24*4,temperature[1]%10,1,24);	//显示小数部分 		   
        if(temperature[2]<0)//显示rom3
        {
            LCD_ShowChar(16+16*4,32+24*5,'-',24,0);			        //显示负号
            temperature[2]=-temperature[2];					        //转为正数
        }else 
            LCD_ShowChar(16+16*4,48+24*5,'+',24,0);			        //去掉正号
        LCD_ShowNum(16+16*5,48+24*5,temperature[2]/10,2,24);	//显示正数部分	    
        LCD_ShowNum(16+16*8-8,48+24*5,temperature[2]%10,1,24);	//显示小数部分
        
        /*求温度平均值*/
        TT=(T[0]+T[1]+T[2])/3;
        if(TT<0){
            LCD_ShowChar(16+16*4,48+24*6,'-',24,0);			        //显示负号
            TT=-TT;
        }
        else{
            LCD_ShowChar(16+16*4,48+24*6,'+',24,0);			        //去掉正号
        }
        LCD_ShowNum(16+16*5,48+24*6,(int)(TT*10)/10,2,24);	    //显示正数部分	    
        LCD_ShowNum(16+16*8-8,48+24*6,(int)(TT*10)%10,1,24);	//显示小数部分 
        /*状态显示*/
        if(TT>=dat)
        {
            LED0=0;
            LCD_ShowString(16+16*5,16+24*1,100,24,24,"danger");
        }else{
            LED0=1;
            LCD_ShowString(16+16*5,16+24*1,100,24,24,"safe  ");
        }
    }
}

