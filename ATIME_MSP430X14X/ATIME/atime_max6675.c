/************************************
库功能：AM2320库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
应用函数：
        void max6675_init()
        unsigned int max6675_read()
修改历史：
	‘修改人’   ‘修改内容’  ‘修改时间’
	  空          空	  空
最后修改时间：2016-05-07
作者： ATIME	版权所有
实例程序：
    unsigned int  rdo    = 0;  //存储热电偶温度
    unsigned char rdo_h  = 0;  //存储热电偶温度高位
    unsigned char rdo_l  = 0;  //存储热电偶温度低位
    max6675_init();
    rdo = max6675_read();
    if(max6675_status==on)
    {
        rdo_h = rdo/100%100;
        rdo_l = rdo%100;
    }
    else
    {
        rdo = 9999;
        rdo_h = 99;
        rdo_l = 99;
    }

常见问题解释：
1.如果数据始终是0，可能是没有运行对应的函数。
2.如果返回999，则可能链接线，或没接上拉电阻。
注意：本程序没有加入CRC校验，只是检测温度、湿度数据指令是否正确。当错误时返回-999.
***************************************/

#ifndef _ATIME_MSP430_MAX7765_C_ 
#define _ATIME_MSP430_MAX6675_C_


/************************************
库全局变量组
***************************************/
#define MAX6675_SCK_PORT    5       //定义通讯接口的端口组
#define MAX6675_SCK_BIT     4       //定义通讯接口的端口位
#define MAX6675_CS_PORT     6       //定义通讯接口的端口组
#define MAX6675_CS_BIT      5       //定义通讯接口的端口位
#define MAX6675_SO_PORT     6       //定义通讯接口的端口组
#define MAX6675_SO_BIT      4       //定义通讯接口的端口位
enum msp430_switch max6675_status = off;//存储热电偶传感器状态，是否在线


/************************************
功能：SPI接口预定义，不要修改此部分
***************************************/
#define MAX6675_CS_SET   PxyOUTz(MAX6675_CS_PORT,MAX6675_CS_BIT,1)
#define MAX6675_CS_CLR   PxyOUTz(MAX6675_CS_PORT,MAX6675_CS_BIT,0)
#define MAX6675_SCK_SET  PxyOUTz(MAX6675_SCK_PORT,MAX6675_SCK_BIT,1)
#define MAX6675_SCK_CLR  PxyOUTz(MAX6675_SCK_PORT,MAX6675_SCK_BIT,0)
#define MAX6675_SO       PxyINz(MAX6675_SO_PORT,MAX6675_SO_BIT)

/************************************
函数功能：SPI接口硬件初始化
传递参数：空
返回值：空
***************************************/
void max6675_init()
{
    PxySELz(MAX6675_CS_PORT,MAX6675_CS_BIT,0);
    PxySELz(MAX6675_SCK_PORT,MAX6675_SCK_BIT,0);
    PxySELz(MAX6675_SO_PORT,MAX6675_SO_BIT,0);
    PxyDIRz(MAX6675_CS_PORT,MAX6675_CS_BIT,1);
    PxyDIRz(MAX6675_SCK_PORT,MAX6675_SCK_BIT,1);
    PxyDIRz(MAX6675_SO_PORT,MAX6675_SO_BIT,0);

}

/************************************
函数功能：SPI接口读取数据
传递参数：空
返回值：温度值
***************************************/
unsigned int max6675_read() //热电偶数据读取，返回温度
{ 
    unsigned char i;
    unsigned int  temp_data; 
    // MAX6675_CS_SET;
   
    MAX6675_CS_CLR;
    _NOP();
    _NOP();
    MAX6675_CS_CLR;
    MAX6675_SCK_CLR;
    for(i=0;i<16;i++)//16位数据读取
    { 
        temp_data<<=1;//向左移一位
        _NOP();
        MAX6675_SCK_SET;//上升
        _NOP(); 
        MAX6675_SCK_CLR;
        if(MAX6675_SO)
        {
           temp_data|=0x01;
        } 
        else
        {
           temp_data|=0x00;
        }
        _NOP();
   }
   MAX6675_CS_SET;
   _NOP();
   _NOP();
   if(temp_data==65528)
       max6675_status = off;
   else
       max6675_status = on;
   temp_data >>=6;
   return temp_data;
}



#endif