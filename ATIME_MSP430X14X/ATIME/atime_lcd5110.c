/*************************************
库功能：NOKIA5110液晶驱动库
库要求：程序已经载入：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
        #include "ascii_8_6.h"//ASCII字库
应用函数：
        lcd5110_port_init(void)
        print5110( unsigned char d[], unsigned char y, unsigned char x)
        printchar5110( unsigned char d, unsigned char y, unsigned char x)
修改历史：
         ‘修改人’   ‘修改内容’  ‘修改时间’
	    空	        空          空
最后修改时间：2016-01-29
作者： ATIME	版权所有
实例程序：
      #include <msp430x14x.h>
      #include "atime_msp430core.h"		//MSP430核心库
      #include "ascii_8_6.h"                    //ASCII字库
      #include "atime_lcd5110.h"

      void main(void)
      {
          watchdog_close();			//关闭看门狗
          basic_clock_init();			//系统时钟初始化
          lcd5110_init();
          printchar5110('A',0,15);
          while(1)
          {
              print5110("ATIME!",1,50);
              wait_ms(700);
              lcd5110_clear_row(1);
              wait_ms(400);
              print5110("ELEVENPERFECT!",2,42);//当前行不够后自动换行到下一行
              wait_ms(700);
              lcd5110_clear_row(2);
              lcd5110_clear_row(3);
              wait_ms(400);
          }
      }

常见错误解释：
Error[Pe011]: unrecognized preprocessing directive:检查#include拼写。
说明：字库采用6*8的ASCII字库，取值范围（32~127），屏幕上一个字占8行6列。
*************************************/

#ifndef _ATIME_MSP430_LCD5110_C_ 
#define _ATIME_MSP430_LCD5110_C_

#ifndef _ASCII_8_6_H_
#include "ascii_8_6.h"
#endif

/*************************************
库全局变量组
*************************************/
#define LCD5110_RST_PORT        4       //定义RST位端口
#define LCD5110_RST_BIT         1       //定义RST位引脚
#define LCD5110_CE_PORT         4       //定义CE位端口
#define LCD5110_CE_BIT          0       //定义CE位引脚
#define LCD5110_DC_PORT         5       //定义DC位端口
#define LCD5110_DC_BIT          7       //定义DC位引脚
#define LCD5110_DIN_PORT        5       //定义DIN位端口
#define LCD5110_DIN_BIT         6       //定义DIN位引脚
#define LCD5110_CLK_PORT        5       //定义CLK位端口
#define LCD5110_CLK_BIT         5       //定义CLK位引脚

#define LCD5110_CONTRAST        4       //设置液晶对比度（0~7）数字越小越黑


/************************************
函数功能：写入1byte数据或指令
传递参数：
        dat：数据或指令；
        cmd：1数据，0指令；
返回值：空
***************************************/
void lcd5110_send_byte(unsigned char dat, unsigned char cmd)
{
    unsigned char i;
    PxyOUTz(LCD5110_CE_PORT,LCD5110_CE_BIT,0);
    if(cmd==0)                                          //1数据，0指令
        PxyOUTz(LCD5110_DC_PORT,LCD5110_DC_BIT,0);
    else
        PxyOUTz(LCD5110_DC_PORT,LCD5110_DC_BIT,1);
    for( i=8; i; i--)                                   //8位数据, 先高后低
    {
        if(dat&0x80)
        {
            PxyOUTz(LCD5110_DIN_PORT,LCD5110_DIN_BIT,1);
        }
        else
        {
            PxyOUTz(LCD5110_DIN_PORT,LCD5110_DIN_BIT,0);
        }
        PxyOUTz(LCD5110_CLK_PORT,LCD5110_CLK_BIT,0);
        dat <<=1;
        PxyOUTz(LCD5110_CLK_PORT,LCD5110_CLK_BIT,1);
    }
    PxyOUTz(LCD5110_CE_PORT,LCD5110_CE_BIT,1);          //保持CE线为0，让数据在显示屏上也保持
    PxyOUTz(LCD5110_CE_PORT,LCD5110_CE_BIT,0); 
    PxyOUTz(LCD5110_DC_PORT,LCD5110_DC_BIT,1);
}


/************************************
函数功能：设置显示地址
传递参数：空
返回值：空
***************************************/
void lcd5110_add( unsigned char y, unsigned char x)
{
    lcd5110_send_byte( 0x80|x, 0);
    lcd5110_send_byte( 0x40|y, 0);
}


/************************************
函数功能：引脚初始化
          设置对应引脚为IO输出模式
传递参数：空
返回值：空
***************************************/
void lcd5110_port_init(void)
{
    PxyDIRz(LCD5110_RST_PORT,LCD5110_RST_BIT,1);
    PxyDIRz(LCD5110_CE_PORT,LCD5110_CE_BIT,1);
    PxyDIRz(LCD5110_DC_PORT,LCD5110_DC_BIT,1);
    PxyDIRz(LCD5110_DIN_PORT,LCD5110_DIN_BIT,1);
    PxyDIRz(LCD5110_CLK_PORT,LCD5110_CLK_BIT,1);

    PxySELz(LCD5110_RST_PORT,LCD5110_RST_BIT,0);
    PxySELz(LCD5110_DC_PORT,LCD5110_DC_BIT,0);
    PxySELz(LCD5110_CE_PORT,LCD5110_CE_BIT,0);
    PxySELz(LCD5110_DIN_PORT,LCD5110_DIN_BIT,0);
    PxySELz(LCD5110_CLK_PORT,LCD5110_CLK_BIT,0);
}


/************************************
函数功能：LCD5110清屏
传递参数：空
返回值：空
***************************************/
void lcd5110_clear_all(void)
{
    unsigned int i;
	
    lcd5110_send_byte( 0x80, 0);
    lcd5110_send_byte( 0x40, 0);
	
    for( i=504; i; i--)
        lcd5110_send_byte( 0x00, 1);
}


/************************************
函数功能：LCD5110行清屏
传递参数：a：行号（0~5）
返回值：空
***************************************/
void lcd5110_clear_row(unsigned char a)
{
    unsigned int i;
	
    lcd5110_add( a, 0);
	
    for( i =84; i; i--)
       lcd5110_send_byte( 0x00, 1);
}


/************************************
函数功能：LCD5110初始化
传递参数：空
返回值：空
***************************************/
void lcd5110_init(void)
{
    lcd5110_port_init();
    PxyOUTz(LCD5110_RST_PORT,LCD5110_RST_BIT,0);
    PxyOUTz(LCD5110_RST_PORT,LCD5110_RST_BIT,1);        //复位
    
    lcd5110_send_byte( 0x21, 0);                        //设置水平寻址，扩展指令集
    lcd5110_send_byte( 0x06, 0);                        //设置温度系数2
    lcd5110_send_byte((0x10|(7-(unsigned char)LCD5110_CONTRAST)), 0);  //设置偏置系统1:48
    lcd5110_send_byte( 0xc8, 0);                        //设置电压VLCD = 3.06 + 0.06*Vop,对比度调整
    lcd5110_send_byte( 0x20, 0);		        //工作模式,水平寻址基本指令      
    lcd5110_send_byte( 0x0c, 0);                        //设置LCD普通模式

    lcd5110_send_byte( 0x40, 0);                        //设置起始x地址
    lcd5110_send_byte( 0x80, 0);                        //设置起始y地址

    lcd5110_clear_all();
}


/************************************
函数功能：显示一个char字符
传递参数：
        d  ：数据；
        y  ：纵坐标（0~5）
        x  ：横坐标（0~83）
返回值：空
***************************************/
void printchar5110( unsigned char d, unsigned char y, unsigned char x)
{
    unsigned char i;
    d -=32;                         //前32个字符未取用
    lcd5110_add( y, x);
    for( i=0; i<=5; i++) 
        lcd5110_send_byte(ascii_8_6[d][i],1);
}


/************************************
函数功能：显示字符串
传递参数：
        d[]：数据；
        y  ：纵坐标（0~5）
        x  ：横坐标（0~83）
返回值：空
***************************************/
void print5110( unsigned char d[], unsigned char y, unsigned char x)
{
    unsigned char i,j;
    
    lcd5110_add( y, x);
    for( j=0;d[j]!='\0';j++)
    {
        for( i=0; i<=5; i++) 
            lcd5110_send_byte(ascii_8_6[d[j]-32][i],1);
    }                    //前32个字符未取用
}



#endif
