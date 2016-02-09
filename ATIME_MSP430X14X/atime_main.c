#include <msp430x14x.h>
#include "atime_msp430core.h"		//MSP430核心库
#include "atime_lcd5110_printf.h"
#include "atime_rs485.h"
#include "atime_interrupt.c"


void main(void)
{
    //unsigned char data=0;
    
    watchdog_close();			//关闭看门狗
    wait_ms(50);
    basic_clock_init();			//系统时钟初始化
    lcd5110_init();
    printf("Power By ATIME");
    rs485_init();
    interrupt_switch(on);  
    while(1);
}