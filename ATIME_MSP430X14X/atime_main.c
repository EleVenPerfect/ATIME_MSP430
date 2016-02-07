#include <msp430x14x.h>
#include "atime_msp430core.h"		//MSP430核心库


void main(void)
{
    watchdog_close();			//关闭看门狗
    wait_ms(50);
    basic_clock_init();			//系统时钟初始化
	
    while(1);
}