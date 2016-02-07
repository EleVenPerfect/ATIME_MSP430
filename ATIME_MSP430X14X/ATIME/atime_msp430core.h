/*************************************
库功能：MSP430核心功能
库要求：程序已经载入：
        #include <msp430x14x.h>
应用函数：
        watchdog_close();
        basic_clock_init();
修改历史：
         ‘修改人’   ‘修改内容’  ‘修改时间’
	    空		  空		 空
最后修改时间：2016-01-22
作者： ATIME	版权所有
实例程序：
      #include <msp430x14x.h>
      #include "atime_msp430core.h"		    //MSP430核心库
      
      void main(void)
      { 
        
          watchdog_close();					//关闭看门狗
          basic_clock_init();				//系统时钟初始化
              
          while(1);
      }

常见错误解释：
1.Error[Pe011]: unrecognized preprocessing directive:检查#include拼写。
2.软件仿真时，弹出：a word access on odd address 0x19;
  表示有整型常量指定存储在flash奇地址上了。应该地址改成偶地址。
  但是本函数中的地址为寄存器地址，所以不应该更改，而是用char型地址保存，详见ioreg_trans函数。
*************************************/

#ifndef _ATIME_MSP430_CORE_H_ 
#define _ATIME_MSP430_CORE_H_

#include "atime_msp430core.c"


/*************************************
函数功能：关闭看门狗
传递参数：空
返回值：空
*************************************/
void watchdog_close(void);


/*************************************
函数功能：软件延时(近似)
传递参数：空
返回值：空
*************************************/
void wait_ms(unsigned int i);


/*************************************
函数功能：软件延时(近似)
传递参数：空
返回值：空
*************************************/
void wait_5us(unsigned int i);


/*************************************
函数功能：系统时钟控制
basic clock 设置，以高频晶振为MCLK/SMCLK时钟源。MCLK不分频，SMCLK8分频。
传递参数：空
返回值：空
*************************************/
void basic_clock_init(void);


/************************************
函数功能：打开总中断
传递参数：空
返回值：空
***************************************/
void interrupt_switch(enum msp430_switch a);


/************************************
函数功能：寄存器转换
          返回制定的IO端口寄存器地址；
传递参数：
        port：端口号（1~6）；
        func：功能（0~4）；
          0:PxIN;
          1:PxOUT;
          2:PxDIR;
          3:PxSEL;
返回值：地址
特别注意：
返回值为地址，调试时本来使用unsigned int 型，仿真出现a word access on odd address 0x19弹窗，
问题出现在：有整型常量指定存储在flash奇地址上了。地址改成偶地址。
实际I/O地址即为奇数地址，尝试改为char型，可以通过。
***************************************/
unsigned char* ioreg_trans( unsigned char port, unsigned char func);



#endif
