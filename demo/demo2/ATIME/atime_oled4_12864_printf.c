/*************************************
库功能：四线OLED 12864 printf接口库
库要求：程序已经载入：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
        #include "atime_oled4_12864_printf.h"
        #include <stdio.h>(可不写)
        #include "atime_oled4_12864.h"(可不写)
应用函数：
        printf("")
        void printf_setadd( unsigned char y, unsigned char x)
修改历史：
         ‘修改人’   ‘修改内容’  ‘修改时间’
	    空	        空          空
最后修改时间：2017-05-08
作者： ATIME	版权所有
实例程序：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"		//MSP430核心库
        #include "atime_oled4_12864_printf.h"
        #include "atime_uart0_scanf.h"
        #include "atime_interrupt.c"

        void main(void)
        {
            unsigned char a[20];
            watchdog_close();			    //关闭看门狗
            basic_clock_init();			    //系统时钟初始化
            uart_init();                            //串口初始化
            interrupt_switch(on);                   //开总中断
            oled4_12864_init();
            printf("ATIME!\r\n");

            while(1)
            {
                scanf("%s",a);
                printf("%s\r\n",a);
            }
        }

说明：写完一个字符光标自动加一，自动换行，最后一行写完后光标自动回到第一行。
      本行写完之后，光标自动换到下一行第一列。
      因字库限制目前只支持ASCII码（32~127）内的字符，不支持中文。
*************************************/

#ifndef _ATIME_MSP430_OLED4_12864_PRINTF_C_
#define _ATIME_MSP430_OLED4_12864_PRINTF_C_

#ifndef _ATIME_MSP430_OLED4_12864_H_
#include "atime_oled4_12864.h"
#endif

#include <stdio.h>
/*************************************
库全局变量组
*************************************/
unsigned char oled4_12864_x =0;
unsigned char oled4_12864_y =0;

#define OLED4_12864_PRINTF_FRONT_SIZE   8       //设置8或16，选择字体大小,默认16
#define OLED_SCANFINBACKSP              0x08    //ASCII  <--  (退格键)
#define OLED_SCANFINDELETE              0x7F    //ASCII <DEL> (DEL 键)

/************************************
函数功能：屏幕写入数据
传递参数：空
返回值：空
***************************************/
int putchar_oled4_12864(int ch)
{

    if(ch == '\r')
    {
        oled4_12864_x =0;
        oled4_12864_addr( oled4_12864_y, oled4_12864_x);
        //oled4_12864_clear_row(oled4_12864_y);
        return ch;
    }
    if(ch == '\n')
    {
        oled4_12864_y++;
        if(OLED4_12864_PRINTF_FRONT_SIZE != 8)
           oled4_12864_y++;
        if(oled4_12864_y>=8)
        {
            oled4_12864_y = 0;
            //oled4_12864_clear_all();
        }
        oled4_12864_addr( oled4_12864_y, oled4_12864_x);
        //oled4_12864_clear_row(oled4_12864_y);
        return ch;
    }
    if(ch == OLED_SCANFINBACKSP||ch == OLED_SCANFINDELETE)
    {
        if(OLED4_12864_PRINTF_FRONT_SIZE == 8)
        {
            if(oled4_12864_x == 0)
            {
                if(oled4_12864_y ==0 )
                    oled4_12864_y = 8;
                oled4_12864_y--;
                oled4_12864_x = 126;
            }
            oled4_12864_x -= 6;
            printchar_oled4_12864_8_6( ' ', oled4_12864_y, oled4_12864_x);

            return ch;
        }
        else
        {
            if(oled4_12864_x == 0)
            {
                if(oled4_12864_y ==0 )
                    oled4_12864_y = 8;
                oled4_12864_y -= 2;
                oled4_12864_x = 120;
            }
            oled4_12864_x -= 8;
            printchar_oled4_12864_16_8( ' ', oled4_12864_y, oled4_12864_x);
            return ch;
        }
    }

    if(OLED4_12864_PRINTF_FRONT_SIZE == 8)
    {
        printchar_oled4_12864_8_6( ch, oled4_12864_y, oled4_12864_x);
        oled4_12864_x += 6;
        if(oled4_12864_x > 122)
        {
            oled4_12864_x =0;
            oled4_12864_y++;
            if(oled4_12864_y>=8)
            {
                oled4_12864_y = 0;
                //oled4_12864_clear_all();
                oled4_12864_addr( oled4_12864_y, oled4_12864_x);
            }
            //oled4_12864_clear_row(oled4_12864_y);
        }
    }
    else
    {
        printchar_oled4_12864_16_8( (unsigned char)ch, oled4_12864_y, oled4_12864_x);
        oled4_12864_x += 8;
        if(oled4_12864_x > 119)
        {
            oled4_12864_x =0;
            oled4_12864_y += 2;
            if(oled4_12864_y>=8)
            {
                oled4_12864_y = 0;
                //oled4_12864_clear_all();
                oled4_12864_addr( oled4_12864_y, oled4_12864_x);
            }
            //oled4_12864_clear_row(oled4_12864_y);
        }
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
    putchar_oled4_12864(ch);
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
    oled4_12864_x =x;
    oled4_12864_y =y;
}

#endif