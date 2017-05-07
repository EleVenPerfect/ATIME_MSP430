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