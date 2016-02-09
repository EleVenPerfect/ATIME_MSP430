#include <msp430x14x.h>
#include "atime_msp430core.h"		//MSP430核心库
#include "atime_lcd5110_printf.h"
#include "atime_timer_a.h"
#include "atime_interrupt.c"


void main(void)
{
    watchdog_close();			//关闭看门狗
    wait_ms(50);
    basic_clock_init();			//系统时钟初始化
    lcd5110_init();
    printf("Power By ATIME");

    //interrupt_switch(on);

    timera_pwm_init1(1000);
    timera_pwm_init2(1,6666);
    
    while(1);
}