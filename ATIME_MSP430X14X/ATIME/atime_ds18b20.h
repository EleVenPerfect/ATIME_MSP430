/*************************************
库功能：DS18B20驱动库
库要求：程序已经载入：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
应用函数：
        ds18b20_init(void)
        ds18b20_read(void)
        ds18b20_translate( float i, unsigned char r[8])
修改历史：
         ‘修改人’   ‘修改内容’  ‘修改时间’
	    空		  空		 空
最后修改时间：2016-01-25
作者： ATIME	版权所有
实例程序：
      #include <msp430x14x.h>
      #include "atime_msp430core.h"		//MSP430核心库
      #include "atime_lcd1602.h"
      #include "atime_ds18b20.h"

      void main(void)
      {
          float i;
          unsigned char j;
          unsigned char a[8];

          watchdog_close();			//关闭看门狗
          basic_clock_init();			//系统时钟初始化
          lcd1602_init( rightmove, cursornotdisplay);
          
          //P1DIR |= BIT2;//器件的vcc端可以接I/O供电
          //P1OUT |= BIT2;//器件的vcc端可以接I/O供电
          
          while(1)
          {
              j =ds18b20_init();
              if(j ==0xff)
              {
                  i =ds18b20_read();
                  ds18b20_translate( i, a);
                  print1602( a, 0, 3);
                  printchar1602( 'C', 0, 10);
                  wait_ms(100);
              }
              else
              {
                  print1602("Please Cheak",0,2);
                  wait_ms(750);
                  set1602(clear1602);
                  wait_ms(700);
              }
          }
      }

常见错误解释：
*************************************/

#ifndef _ATIME_MSP430_DS18B20_H_ 
#define _ATIME_MSP430_DS18B20_H_

#include "atime_at24c32.c"


/************************************
函数功能：初始化ds18b20
传递参数：空
返回值：
    0   ：成功
    0xff：错误
***************************************/
unsigned char ds18b20_init(void);


/************************************
函数功能：读1字节
传递参数：空
返回值：都去的数据
***************************************/
unsigned char ds18b20_read_byte(void) ;


/************************************
函数功能：写一字节数据
传递参数：要写的数据
返回值：空
***************************************/
void ds18b20_write_byte(unsigned char value) ;


/************************************
函数功能：DS18B20读取温度
传递参数：空
返回值：温度值
***************************************/
float ds18b20_read(void) ;


/************************************
函数功能：优化显示数字
		  本函数可以去除数字前面无意义的零。
传递参数：
	a[8]:转换后的数组存储位置
返回值：空
注：没有通过返回值返回结果，
    而是利用函数调用数组是用原内存空间这一规则。
***************************************/
void ds18b20_zero_better(unsigned char a[8]);


/************************************
函数功能：浮点转字符数组
传递参数：
		i   :待转换浮点数
		r[8]:转换后的数组存储位置
返回值：空
注：没有通过返回值返回结果，
    而是利用函数调用数组是用原内存空间这一规则。
***************************************/
void ds18b20_translate( float i, unsigned char r[8]);



#endif
