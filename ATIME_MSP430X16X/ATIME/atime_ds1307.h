/************************************
库功能：DS1307库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
        #include "atime_iic_software.h"
应用函数：
        ds1307_read(unsigned char a[])
        ds1307_write(unsigned char a[])
修改历史：
	‘修改人’   ‘修改内容’  ‘修改时间’
	  空          空	  空
最后修改时间：2016-02-05
作者： ATIME	版权所有
实例程序：
      #include <msp430x14x.h>
      #include "atime_msp430core.h"		//MSP430核心库
      #include "atime_lcd5110_printf.h"
      #include "atime_iic_software.h"
      #include "atime_ds1307.h"

      void main(void)
      {
          unsigned char i,j;
          watchdog_close();			//关闭看门狗
          wait_ms(50);
          basic_clock_init();			//系统时钟初始化
          lcd5110_init();                     //液晶屏幕初始化
          
          ds1307_write(ds1307_data);
          while(1)
          {
              ds1307_read(ds1307_data);
              j++;
              for( i=0; i<8; i++)
                  printf("  %02X   ",ds1307_data[i]);//以16进制显示
              if(j%2==0)
                  printf("AT-");
              else
                  printf("IME");
              printf_setadd(0,0);
              wait_ms(400);
          } 
      }
常见错误解释：
***************************************/

#ifndef _ATIME_MSP430_IIC_DS1307_H_ 
#define _ATIME_MSP430_IIC_DS1307_H_

#include "atime_ds1307.c"


/************************************
函数功能：IIC接口读数据
传递参数：数据存储数组
返回值：空
***************************************/
void ds1307_read(unsigned char a[]);


/************************************
函数功能：IIC接口写数据
传递参数：数据存储数组
返回值：空
***************************************/
void ds1307_write(unsigned char a[]);



#endif