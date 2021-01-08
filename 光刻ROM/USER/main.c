#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "ds18b20.h" 

int main(void)
{
    u8 i=0;
    extern u8 rom[8];       //存放rom
    u8 RCR_Result=1;        //校验的结果
    
    delay_init();	    	//延时函数初始化	  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
    uart_init(115200);	 	//串口初始化为 115200
    while(DS18B20_Init())
    {
        printf("device is error\r\n");
        delay_ms(200);
    }
    printf("device is yes\r\n");
    read_rom();
    for(i=0;i<8;i++)		 
    {
         printf ("[0x%x] ",(rom[i]/16)<<4|(rom[i]%16));     //以十六进制的格式打印到上位机中
    }                                               	 //应当注意的是数字a在上位机中显示为10，数字b在上位机中显示为11
    RCR_Result=CRC8(rom);                              //判断读取的光刻ROM是否正确
    if (RCR_Result==0)
        printf(">>RCR OK\n");
    else 
        printf(">>RCR error\n");
    while(1);
}
