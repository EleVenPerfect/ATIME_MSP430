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

#ifndef _ATIME_MSP430_OLED4_12864_H_
#define _ATIME_MSP430_OLED4_12864_H_

#include "atime_oled4_12864.c"

/************************************
函数功能：写入1byte数据或指令
传递参数：
        dat：数据或指令；
        cmd：1数据，0指令；
返回值：空
***************************************/
void oled4_12864_send_byte( unsigned char dat, unsigned char cmd);


/************************************
函数功能：设置位置
传递参数：
        x:横坐标位置(0~127)
        y:纵坐标位置(0~7)
返回值：空
***************************************/
void oled4_12864_addr(unsigned char x, unsigned char y);


/************************************
函数功能：设置位置
传递参数：
        a:on开；off关
返回值：空
***************************************/
void oled4_12864_display( enum msp430_switch a);


/************************************
函数功能：清屏
传递参数：空
返回值：空
***************************************/
void oled4_12864_clear_all(void);


/************************************
函数功能：引脚初始化
          设置对应引脚为IO输出模式
传递参数：空
返回值：空
***************************************/
void oled4_12864_port_init(void);


/************************************
函数功能：OLED初始化
传递参数：空
返回值：空
***************************************/
void oled4_12864_init(void);


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
void printchar_oled4_12864_8_6( unsigned char d, unsigned char y, unsigned char x);


/************************************
函数功能：显示8*6字符串
传递参数：
        d[]：数据；
        y  ：纵坐标（0~7）
        x  ：横坐标（0~127）
返回值：空
***************************************/
void print_oled4_12864_8_6( unsigned char d[], unsigned char y, unsigned char x);


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
void printchar_oled4_12864_16_8( unsigned char d, unsigned char y, unsigned char x);


/************************************
函数功能：OLED显示一个16*8点阵字符串
传递参数：空
返回值：空
***************************************/
void print_oled4_12864_16_8( unsigned char d[], unsigned char y, unsigned char x);

#endif


#endif
