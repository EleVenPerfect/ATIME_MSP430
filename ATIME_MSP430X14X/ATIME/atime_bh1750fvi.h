/************************************
库功能：bh1750fvi库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
应用函数：
      void bh1750_init();
      void bh1750_read();
修改历史：
	‘修改人’   ‘修改内容’  ‘修改时间’
	  空          空	  空
最后修改时间：2016-05-17
作者： ATIME	版权所有
实例程序：
#include <msp430x14x.h>
#include "atime_msp430core.h"		//MSP430核心库
#include "atime_bh1750fvi.h"
#include "atime_lcd5110_printf.h"

void main(void)
{
    unsigned int gq;
    watchdog_close();			//关闭看门狗
    basic_clock_init();			//系统时钟初始化
    lcd5110_init();                     //液晶屏幕初始化
    bh1750_init();                      //BH1750端口初始化

    while(1)
    {
        gq = bh1750_read();
        printf("%d \n",gq);
        wait_ms(1000);
    }
}

常见问题解释：
***************************************/

#ifndef _ATIME_MSP430_BH1750FVI_H_ 
#define _ATIME_MSP430_BH1750FVI_H_

#include "atime_bh1750fvi.c"

/************************************
函数功能：初始化BH1750，根据需要请参考pdf进行修改
传递参数：空
返回值：空
*************************************/
void bh1750_init();
/************************************
函数功能：读取光强数据
传递参数：空
返回值：gq 光强数据
        光强数据将会保存在gq和BH1750FVI_BUF中
***************************************/
unsigned int bh1750_read();

#endif