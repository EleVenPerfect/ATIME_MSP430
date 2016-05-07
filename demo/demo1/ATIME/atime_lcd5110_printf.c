/*************************************
库功能：NOKIA5110液晶printf接口库
库要求：程序已经载入：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
        #include <stdio.h>(可不写)
        #include "ascii_8_6.h"//ASCII字库(可不写)
        #include "atime_lcd5110.h"(可不写)
应用函数：
        printf("")
        printf_setadd( unsigned char y, unsigned char x)
修改历史：
         ‘修改人’   ‘修改内容’  ‘修改时间’
	    空	        空          空
最后修改时间：2016-02-04
作者： ATIME	版权所有
实例程序：
      #include <msp430x14x.h>
      #include "atime_msp430core.h"		//MSP430核心库
      #include "atime_lcd5110_printf.h"

      void main(void)
      {
          int x =1993;
          float y =3.1415;
          
          watchdog_close();			//关闭看门狗
          basic_clock_init();			//系统时钟初始化
          lcd5110_init();                     //液晶屏幕初始化

          printf("The number is:%d;and:%f\n",x,y);
          printf("Float:%f",y);
          printf("\nPower By ATIME");
          
          while(1);
      }

说明：写完一个字符光标自动加一，自动换行，最后一行写完后光标自动回到第一行。
      本行写完之后，光标自动换到下一行第一列。
      因字库限制目前只支持ASCII码（32~127）内的字符，不支持中文。
*************************************/

#ifndef _ATIME_MSP430_LCD5110_PRINTF_C_ 
#define _ATIME_MSP430_LCD5110_PRINTF_C_

#ifndef _ATIME_MSP430_LCD5110_H_ 
#include "atime_lcd5110.h"
#endif

#include <stdio.h>
/*************************************
库全局变量组
*************************************/
unsigned char lcd5110_x =0;
unsigned char lcd5110_y =0;

/************************************
函数功能：串口写入数据
传递参数：空
返回值：空
***************************************/
int putchar_uart(int ch)
{
    return ch;
}


/************************************
函数功能：屏幕写入数据
传递参数：空
返回值：空
***************************************/
int putchar_5110(int ch)
{
    if(ch=='\n')
    {
        lcd5110_y++;
        lcd5110_x =0;
        //lcd5110_clear_row(lcd5110_y);
        return ch;
    }
    printchar5110( (unsigned char)ch, lcd5110_y, lcd5110_x*6);
    lcd5110_x++;
    if(lcd5110_x==14)
    {
        lcd5110_x =0;
        lcd5110_y++;
        if(lcd5110_y==6)
            lcd5110_y =0;
        //lcd5110_clear_row(lcd5110_y);
    }

    return ch;
}


/************************************
函数功能：写入数据
传递参数：空
返回值：空
***************************************/
int putchar(int ch)
{
    putchar_uart(ch);
    putchar_5110(ch);
    return ch;
}


/************************************
函数功能：设置液晶显示地址
传递参数：y：纵坐标
          x：横坐标
返回值：空
注：本函数为了校准坐标
***************************************/
void printf_setadd( unsigned char y, unsigned char x)
{
    lcd5110_x =x;
    lcd5110_y =y;
}

#endif
