/*************************************
库功能：NOKIA5110液晶驱动库
库要求：程序已经载入：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
        #include "ascii_8_6.h"//ASCII字库
应用函数：
        lcd5110_init(void)
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

#ifndef _ATIME_MSP430_LCD5110_H_ 
#define _ATIME_MSP430_LCD5110_H_

#ifndef _ASCII_8_6_H_
#include "ascii_8_6.h"
#endif

#include "atime_lcd5110.c"


/************************************
函数功能：写入1byte数据或指令
传递参数：
        dat：数据或指令；
        cmd：1数据，0指令；
返回值：空
***************************************/
void lcd5110_send_byte(unsigned char dat, unsigned char cmd);


/************************************
函数功能：设置显示地址
传递参数：空
返回值：空
***************************************/
void lcd5110_add( unsigned char y, unsigned char x);


/************************************
函数功能：引脚初始化
          设置对应引脚为IO输出模式
传递参数：空
返回值：空
***************************************/
void lcd5110_port_init(void);


/************************************
函数功能：LCD5110清屏
传递参数：空
返回值：空
***************************************/
void lcd5110_clear_all(void);


/************************************
函数功能：LCD5110行清屏
传递参数：a：行号（0~5）
返回值：空
***************************************/
void lcd5110_clear_row(unsigned char a);


/************************************
函数功能：LCD5110初始化
传递参数：空
返回值：空
***************************************/
void lcd5110_init(void);


/************************************
函数功能：显示一个char字符
传递参数：
        d  ：数据；
        y  ：纵坐标（0~5）
        x  ：横坐标（0~83）
返回值：空
***************************************/
void printchar5110( unsigned char d, unsigned char y, unsigned char x);


/************************************
函数功能：显示字符串
传递参数：
        d[]：数据；
        y  ：纵坐标（0~5）
        x  ：横坐标（0~83）
返回值：空
***************************************/
void print5110( unsigned char d[], unsigned char y, unsigned char x);



#endif
