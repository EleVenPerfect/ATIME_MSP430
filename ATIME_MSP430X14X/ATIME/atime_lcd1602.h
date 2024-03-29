/*************************************
库功能：并行驱动LCD1602库
库要求：程序已经载入：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
应用函数：
		lcd1602_init(enum lcdint_ac a,enum lcdint_cursor b)
		print1602(unsigned char db[], unsigned int y, unsigned int x)
		printchar1602(unsigned char db, unsigned int y, unsigned int x)
		set1602(enum set a )
		bcd_trans_char(unsigned char a[], unsigned char b[],unsigned char j)
修改历史：
         ‘修改人’   ‘修改内容’  ‘修改时间’
	    空		  空		 空
最后修改时间：2016-01-23
作者： ATIME	版权所有
实例程序：
      #include <msp430x14x.h>
      #include "atime_msp430core.h"                   //MSP430核心库
      #include "atime_lcd1602.h"

      void main(void)
      { 
          unsigned char db[10]={"A T I M E"};

          watchdog_close();				//关闭看门狗
          basic_clock_init();				//系统时钟初始化
          lcd1602_init( rightmove, cursornotdisplay);   //液晶屏幕初始化
          
          print1602( db, 0, 1);                        //可以引用数组
          print1602( "EleVenPerfect", 1, 1);           //可以引用字符串
          printchar1602( 'O', 0, 12);
          printchar1602( 'K', 0, 13);
          wait_ms( 1000);
          
          while(set1602(right1602))
            wait_ms( 1200);
      }


常见错误解释：
*************************************/

#ifndef _ATIME_MSP430_LCD1602_H_ 
#define _ATIME_MSP430_LCD1602_H_

#include "atime_lcd1602.c"


/************************************
函数功能：写数据1bit
传递参数：db 数据指令内容
返回值：空
***************************************/
void writedata(char db);


/************************************
函数功能：写指令1bit
传递参数：db  数据指令内容
返回值：空
***************************************/
void writezhi(unsigned char db);


/************************************
函数功能：设置1602
传递参数：a:
			clear1602: 清屏
			left1602 : 左移
			right1602: 右移

返回值：1：成功；：失败0；
***************************************/
unsigned char set1602(enum set a );


/************************************
函数功能：并行初始化lcd1602
传递参数：
      a:光标移动设置 ，详见a的注释
      b:开关光标显示 ，详见b的注释
返回值：1：成功；：失败0；
***************************************/
unsigned char lcd1602_init(enum lcdint_ac a,enum lcdint_cursor b);


/************************************
函数功能：测试字符串长度
传递参数：字符串指针
返回值：字符串长度
***************************************/
unsigned int str_long(unsigned char db[]);


/************************************
函数功能：显示数据、字母
传递参数：db[] : 显示的数据 
		  x:	 横向坐标
		  y:	 纵向坐标
		  坐标系左上角为（0,0）点向右向下为正
返回值：1：成功；：失败0；
特别注意：此函数只可以显示字符串、字符串数组，不可以显示单个字符
***************************************/
unsigned char print1602(unsigned char db[], unsigned int y, unsigned int x);


/************************************
函数功能：设置自定义字符
传递参数：
		  i:设置的自定义字符标号
		  a[]:定义字符的数组数据
返回值：空
特别注意：i的取值：0~7；
显示自定义字符方法：
		用printchar1602函数，数据位写之前定义的字符标号即可
		如：printchar1602(0,2,1);
***************************************/
void setpic1602(unsigned char i, unsigned char a[]);
		

/************************************
函数功能：显示数据、字母
传递参数：db : 显示的数据 
		  x:	 横向坐标
		  y:	 纵向坐标
		  坐标系左上角为（1,1）点向右向下为正
返回值：1：成功；：失败0；
特别注意：此函数只可以显示单个字符
***************************************/
unsigned char printchar1602(unsigned char db, unsigned int y, unsigned int x);


/************************************
函数功能：显示数据、字母
传递参数：db : 显示的数据 
		  x:	 横向坐标
		  y:	 纵向坐标
		  坐标系左上角为（1,1）点向右向下为正
返回值：1：成功；：失败0；
特别注意：此函数只可以显示单个字符
***************************************/
unsigned char printint1602(unsigned int db, unsigned int y, unsigned int x);


/************************************
函数功能：1位BCD码转换3位可显示数字
传递参数：a[]，BCD数据;b[]，可显示数据
返回值：空
***************************************/
void bcd_trans_char(unsigned char a[], unsigned char b[],unsigned char j);



#endif
