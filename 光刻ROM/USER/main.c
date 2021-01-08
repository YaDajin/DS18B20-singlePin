#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "ds18b20.h" 

int main(void)
{
    u8 i=0;
    extern u8 rom[8];       //���rom
    u8 RCR_Result=1;        //У��Ľ��
    
    delay_init();	    	//��ʱ������ʼ��	  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart_init(115200);	 	//���ڳ�ʼ��Ϊ 115200
    while(DS18B20_Init())
    {
        printf("device is error\r\n");
        delay_ms(200);
    }
    printf("device is yes\r\n");
    read_rom();
    for(i=0;i<8;i++)		 
    {
         printf ("[0x%x] ",(rom[i]/16)<<4|(rom[i]%16));     //��ʮ�����Ƶĸ�ʽ��ӡ����λ����
    }                                               	 //Ӧ��ע���������a����λ������ʾΪ10������b����λ������ʾΪ11
    RCR_Result=CRC8(rom);                              //�ж϶�ȡ�Ĺ��ROM�Ƿ���ȷ
    if (RCR_Result==0)
        printf(">>RCR OK\n");
    else 
        printf(">>RCR error\n");
    while(1);
}
