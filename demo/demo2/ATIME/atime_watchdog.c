/*************************************
库功能：MSP430 看门狗定时器函数库
库要求：程序已经载入：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
        #include "atime_watchdog.h"
应用函数：
        void watchdog_init( unsigned char clks)
        void watchdog_timer_init( unsigned char clks)
        void watchdog_timer_interrupt(enum msp430_switch a)
修改历史：
         ‘修改人’   ‘修改内容’  ‘修改时间’
	    空	        空          空
最后修改时间：2017-05-07
作者： ATIME	版权所有
实例程序：
1.看门狗模式：
#include <msp430x14x.h>
        #include "atime_msp430core.h"		//MSP430核心库
        #include "atime_uart0_stdio.h"
        #include "atime_watchdog.h"
        #include "atime_interrupt.c"

        void main(void)
        {
            watchdog_init(0);			    //关闭看门狗
            basic_clock_init();			    //系统时钟初始化
            uart_init();                            //串口初始化
            interrupt_switch(on);                   //开总中断

            printf("ATIME\r\n");
            while(1);
        }
注意：程序正常运行会每秒打印一次ATIME，这是因为看门狗定时器溢出后触复位。

2.定时器模式：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"		//MSP430核心库
        #include "atime_uart0_stdio.h"
        #include "atime_watchdog.h"
        #include "atime_interrupt.c"

        void main(void)
        {
            watchdog_timer_init(0);	            //关闭看门狗
            basic_clock_init();			    //系统时钟初始化
            uart_init();                            //串口初始化
            watchdog_timer_interrupt(on);           //开看门狗中断
            interrupt_switch(on);                   //开总中断

            while(1);
        }

        中断部分代码：
        #pragma vector=WDT_VECTOR
        __interrupt void WDT_ISR(void)
        {
            printf("ATIME\r\n");
            watchdog_timer_interrupt(off);
        }
注意：程序会打印一次ATIME，说明进入了中断，中断中关闭了中断使能，因此只进入一次。
*************************************/

#ifndef _ATIME_MSP430_WATCHDOG_TIMER_C_
#define _ATIME_MSP430_WATCHDOG_TIMER_C_

/*************************************
库全局变量组
*************************************/


/************************************
函数功能：设置WATCHDOG_TIMER在看门狗模式
传递参数：clks：时钟源选择与定时器选择
        0：ACLK-1000ms
        1：ACLK-250ms
        2:ACLK-16ms
        3.ACLK-1.9ms
        4.SMCLK-32ms
        5.SMCLK-8ms
        6.SMCLK-0.5ms
        7.SMCLK-0.064ms
返回值：空
注意：每次喂狗还是用这个函数即可。
***************************************/
void watchdog_init( unsigned char clks)
{
    switch(clks)
    {
        case 0:WDTCTL = WDT_ARST_1000;break;
        case 1:WDTCTL = WDT_ARST_250 ;break;
        case 2:WDTCTL = WDT_ARST_16  ;break;
        case 3:WDTCTL = WDT_ARST_1_9 ;break;
        case 4:WDTCTL = WDT_MRST_32  ;break;
        case 5:WDTCTL = WDT_MRST_8   ;break;
        case 6:WDTCTL = WDT_MRST_0_5 ;break;
        case 7:WDTCTL = WDT_MRST_0_064;break;
        default:WDTCTL = WDT_ARST_1000;
    }
}


/************************************
函数功能：设置WATCHDOG_TIMER在看定时器模式
传递参数：clks：时钟源选择与定时器选择
        0：ACLK-1000ms
        1：ACLK-250ms
        2:ACLK-16ms
        3.ACLK-1.9ms
        4.SMCLK-32ms
        5.SMCLK-8ms
        6.SMCLK-0.5ms
        7.SMCLK-0.064ms
返回值：空
注意：设置完成后会自动溢出后重新计时。
***************************************/
void watchdog_timer_init( unsigned char clks)
{
    switch(clks)
    {
        case 0:WDTCTL = WDT_ADLY_1000;break;
        case 1:WDTCTL = WDT_ADLY_250 ;break;
        case 2:WDTCTL = WDT_ADLY_16  ;break;
        case 3:WDTCTL = WDT_ADLY_1_9 ;break;
        case 4:WDTCTL = WDT_MDLY_32  ;break;
        case 5:WDTCTL = WDT_MDLY_8   ;break;
        case 6:WDTCTL = WDT_MDLY_0_5 ;break;
        case 7:WDTCTL = WDT_MDLY_0_064;break;
        default:WDTCTL = WDT_ADLY_1000;
    }
}


/************************************
函数功能：设置WATCHDOG_TIMER中断
传递参数：a:开关
返回值：空
***************************************/
void watchdog_timer_interrupt(enum msp430_switch a)
{
    if(a==on)
        IE1 |= WDTIE;
    else
        IE1 &= ~WDTIE;
}



#endif