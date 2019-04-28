/*************************************
库功能：OLED 4线128*64驱动库
库要求：程序已经载入：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
        #include "atime_oled4_12864.h"
应用函数：
        void printchar_oled4_12864_8_6( unsigned char d, unsigned char y, unsigned char x)
        void print_oled4_12864_8_6( unsigned char d[], unsigned char y, unsigned char x)
        void printchar_oled4_12864_16_8( unsigned char d, unsigned char y, unsigned char x)
        void print_oled4_12864_16_8( unsigned char d[], unsigned char y, unsigned char x)
        void oled4_12864_display( enum msp430_switch a)
        void oled4_12864_init(void)
        void oled4_12864_clear_all(void)
修改历史：
         ‘修改人’   ‘修改内容’  ‘修改时间’
	    空	        空          空
最后修改时间：2017-05-08
作者： ATIME	版权所有
实例程序：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"		//MSP430核心库
        #include "atime_uart0_stdio.h"
        #include "atime_oled4_12864.h"
        #include "atime_interrupt.c"

        void main(void)
        {
            watchdog_close();			    //关闭看门狗
            basic_clock_init();			    //系统时钟初始化
            uart_init();                            //串口初始化
            interrupt_switch(on);                   //开总中断
            oled4_12864_init();
            printchar_oled4_12864_16_8('@',0,0);
            print_oled4_12864_16_8("ATIME!ATIME!ATIME!",2,4);
            while(1)
            {
                oled4_12864_display(on);
                wait_ms(900);
                oled4_12864_display(off);
                wait_ms(600);
            }
        }

常见错误解释：
Error[Pe011]: unrecognized preprocessing directive:检查#include拼写。
Error[e46]: Undefined external "printchar_oled4_12864_16_8" referred in atime_main：
            OLED_16_8设置错误，需要设为1开启。
说明：8*6的字库采用ASCII字库，取值范围（32~127），屏幕上一个字占8行6列.
      16*8的字库采用ASCII字库，取值范围（32~126），屏幕上一个字占8行6列。
      若无必要，可以关闭字库，以节省空间占用。
*************************************/

#ifndef _ATIME_MSP430_OLED4_12864_C_
#define _ATIME_MSP430_OLED4_12864_C_


/*************************************
库全局变量组
*************************************/
#define OLED4_12864_RST_PORT        4       //定义RST位端口
#define OLED4_12864_RST_BIT         5       //定义RST位引脚
#define OLED4_12864_DC_PORT         4       //定义DC位端口
#define OLED4_12864_DC_BIT          4       //定义DC位引脚
#define OLED4_12864_SDA_PORT        4       //定义DSA位端口
#define OLED4_12864_SDA_BIT         6       //定义DSA位引脚
#define OLED4_12864_SCL_PORT        4       //定义SCLK位端口
#define OLED4_12864_SCL_BIT         7       //定义SCLK位引脚

#define OLED_FONT_8_6               1       //0关闭，1开启，字库设置
#define OLED_FONT_16_8              1       //0关闭，1开启，字库设置



/************************************
函数功能：写入1byte数据或指令
传递参数：
        dat：数据或指令；
        cmd：1数据，0指令；
返回值：空
***************************************/
void oled4_12864_send_byte( unsigned char dat, unsigned char cmd)
{
    unsigned char i;
    if(cmd)
        PxyOUTz(OLED4_12864_DC_PORT,OLED4_12864_DC_BIT,1);
    else
        PxyOUTz(OLED4_12864_DC_PORT,OLED4_12864_DC_BIT,0);

    for(i=0;i<8;i++)
    {
        PxyOUTz(OLED4_12864_SCL_PORT,OLED4_12864_SCL_BIT,0);

        if(dat&0x80)
            PxyOUTz(OLED4_12864_SDA_PORT,OLED4_12864_SDA_BIT,1);
        else
            PxyOUTz(OLED4_12864_SDA_PORT,OLED4_12864_SDA_BIT,0);

	PxyOUTz(OLED4_12864_SCL_PORT,OLED4_12864_SCL_BIT,1);
	dat<<=1;
    }

    PxyOUTz(OLED4_12864_DC_PORT,OLED4_12864_DC_BIT,1);
}


/************************************
函数功能：设置位置
传递参数：
        x:横坐标位置(0~127)
        y:纵坐标位置(0~7)
返回值：空
***************************************/
void oled4_12864_addr(unsigned char x, unsigned char y)
{
    x--;
    oled4_12864_send_byte(0xb0+y,0);
    oled4_12864_send_byte(((x&0xf0)>>4)|0x10,0);
    oled4_12864_send_byte((x&0x0f)|0x01,0);
}


/************************************
函数功能：设置位置
传递参数：
        a:on开；off关
返回值：空
***************************************/
void oled4_12864_display( enum msp430_switch a)
{
    if(a == on)
    {
	oled4_12864_send_byte(0X8D,0);
	oled4_12864_send_byte(0X14,0);
	oled4_12864_send_byte(0XAF,0);
    }
    else
    {
	oled4_12864_send_byte(0X8D,0);
	oled4_12864_send_byte(0X10,0);
	oled4_12864_send_byte(0XAE,0);
    }
}


/************************************
函数功能：清屏
传递参数：空
返回值：空
***************************************/
void oled4_12864_clear_all(void)
{
    unsigned int i,j;

    for(i=0;i<8;i++)
    {
        oled4_12864_send_byte(0xb0+i,0);
        oled4_12864_send_byte(0x00,0);
        oled4_12864_send_byte(0x10,0);
        for( j=0; j<128; j++)
            oled4_12864_send_byte(0x0,1);
    }
}


/************************************
函数功能：引脚初始化
          设置对应引脚为IO输出模式
传递参数：空
返回值：空
***************************************/
void oled4_12864_port_init(void)
{
    PxyDIRz(OLED4_12864_RST_PORT,OLED4_12864_RST_BIT,1);
    PxyDIRz(OLED4_12864_DC_PORT,OLED4_12864_DC_BIT,1);
    PxyDIRz(OLED4_12864_SDA_PORT,OLED4_12864_SDA_BIT,1);
    PxyDIRz(OLED4_12864_SCL_PORT,OLED4_12864_SCL_BIT,1);

    PxySELz(OLED4_12864_RST_PORT,OLED4_12864_RST_BIT,0);
    PxySELz(OLED4_12864_DC_PORT,OLED4_12864_DC_BIT,0);
    PxySELz(OLED4_12864_SDA_PORT,OLED4_12864_SDA_BIT,0);
    PxySELz(OLED4_12864_SCL_PORT,OLED4_12864_SCL_BIT,0);
}


/************************************
函数功能：OLED初始化
传递参数：空
返回值：空
***************************************/
void oled4_12864_init(void)
{
    oled4_12864_port_init();

    PxyOUTz(OLED4_12864_RST_PORT,OLED4_12864_RST_BIT,1);
    wait_ms(100);
    PxyOUTz(OLED4_12864_RST_PORT,OLED4_12864_RST_BIT,0);
    wait_ms(100);
    PxyOUTz(OLED4_12864_RST_PORT,OLED4_12864_RST_BIT,1);

    oled4_12864_send_byte(0xAE,0);//--turn off oled panel
    oled4_12864_send_byte(0x00,0);//---set low column address
    oled4_12864_send_byte(0x10,0);//---set high column address
    oled4_12864_send_byte(0x40,0);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    oled4_12864_send_byte(0x81,0);//--set contrast control register
    oled4_12864_send_byte(0xCF,0); // Set SEG Output Current Brightness
    oled4_12864_send_byte(0xA1,0);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    oled4_12864_send_byte(0xC8,0);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    oled4_12864_send_byte(0xA6,0);//--set normal display
    oled4_12864_send_byte(0xA8,0);//--set multiplex ratio(1 to 64)
    oled4_12864_send_byte(0x3f,0);//--1/64 duty
    oled4_12864_send_byte(0xD3,0);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    oled4_12864_send_byte(0x00,0);//-not offset
    oled4_12864_send_byte(0xd5,0);//--set display clock divide ratio/oscillator frequency
    oled4_12864_send_byte(0x80,0);//--set divide ratio, Set Clock as 100 Frames/Sec
    oled4_12864_send_byte(0xD9,0);//--set pre-charge period
    oled4_12864_send_byte(0xF1,0);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    oled4_12864_send_byte(0xDA,0);//--set com pins hardware configuration
    oled4_12864_send_byte(0x12,0);
    oled4_12864_send_byte(0xDB,0);//--set vcomh
    oled4_12864_send_byte(0x40,0);//Set VCOM Deselect Level
    oled4_12864_send_byte(0x20,0);//-Set Page Addressing Mode (0x00/0x01/0x02)
    oled4_12864_send_byte(0x02,0);//
    oled4_12864_send_byte(0x8D,0);//--set Charge Pump enable/disable
    oled4_12864_send_byte(0x14,0);//--set(0x10) disable
    oled4_12864_send_byte(0xA4,0);// Disable Entire Display On (0xa4/0xa5)
    oled4_12864_send_byte(0xA6,0);// Disable Inverse Display On (0xa6/a7)
    oled4_12864_send_byte(0xAF,0);//--turn on oled panel

    oled4_12864_send_byte(0xAF,0); /*display ON*/

    oled4_12864_clear_all();
    oled4_12864_addr( 0, 0);
}


/************************************
代码区段：开启8*6显示
***************************************/
#if OLED_FONT_8_6==1

#ifndef _ASCII_8_6_H_
#include "ascii_8_6.h"
#endif

/************************************
函数功能：OLED显示一个8*6点阵字符
传递参数：空
返回值：空
***************************************/
void printchar_oled4_12864_8_6( unsigned char d, unsigned char y, unsigned char x)
{
    unsigned char i;
    d -=32;
    oled4_12864_addr(x,y);
    for( i=0; i<=5; i++)
    {
        oled4_12864_send_byte(ascii_8_6[d][i],1);
    }
}


/************************************
函数功能：显示8*6字符串
传递参数：
        d[]：数据；
        y  ：纵坐标（0~7）
        x  ：横坐标（0~127）
返回值：空
***************************************/
void print_oled4_12864_8_6( unsigned char d[], unsigned char y, unsigned char x)
{
    unsigned char i,j,k;

    oled4_12864_addr( x, y);
    k = x;
    for( j=0; d[j]!='\0'; j++)
    {
        for( i=0; i<=5; i++)
        {
            if(x==0)
            {
                oled4_12864_send_byte(0x00,0);
                oled4_12864_send_byte(0x10,0);
                x++;
            }
            oled4_12864_send_byte(ascii_8_6[d[j]-32][i],1);
        }
        k += 6;
        if(k > 122)
        {
            y++;
            x = 0;
            k = 0;
            oled4_12864_addr( x, y);
        }
    }                    //前32个字符未取用
}

#endif


/************************************
代码区段：开启16*8显示
***************************************/
#if OLED_FONT_16_8==1

#ifndef _ASCII_16_8_H_
#include "ascii_16_8.h"
#endif                  //ASCII码16*8点阵

/************************************
函数功能：OLED显示一个16*8点阵字符
传递参数：空
返回值：空
***************************************/
void printchar_oled4_12864_16_8( unsigned char d, unsigned char y, unsigned char x)
{
    unsigned char i;
    d -=32;
    oled4_12864_addr(x,y);
    for( i=0; i<8; i++)
    {
        if(x==0)
        {
            oled4_12864_send_byte(0x00,0);
            oled4_12864_send_byte(0x10,0);
            x++;
        }
        oled4_12864_send_byte(ascii_16_8[d*16+i],1);
    }
    oled4_12864_addr(--x,y+1);
    for( i=0; i<8; i++)
    {
        if(x==0)
        {
            oled4_12864_send_byte(0x00,0);
            oled4_12864_send_byte(0x10,0);
            x++;
        }
        oled4_12864_send_byte(ascii_16_8[d*16+i+8],1);
    }

}


/************************************
函数功能：OLED显示一个16*8点阵字符串
传递参数：空
返回值：空
***************************************/
void print_oled4_12864_16_8( unsigned char d[], unsigned char y, unsigned char x)
{
    unsigned char j;

    for( j=0; d[j]!='\0'; j++)
    {
        printchar_oled4_12864_16_8( d[j], y, x);
        x += 8;
        if(x > 119)
        {
            y += 2;
            x = 0;
        }
    }                    //前32个字符未取用
}

#endif


#endif
