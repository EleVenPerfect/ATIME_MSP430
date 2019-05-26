#include <msp430x14x.h>
#include "atime_msp430core.h"		//MSP430核心库
#include "atime_lcd5110_printf.h"
#include "atime_bme280.h"

void main(void)
{
    watchdog_close();			//关闭看门狗
    basic_clock_init();			//系统时钟初始化
    lcd5110_init();                     //液晶屏幕初始化

    while(1)
    {
        
    }
}