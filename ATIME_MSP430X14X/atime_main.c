#include <msp430x14x.h>
#include "atime_msp430core.h"		//MSP430核心库
#include "atime_lcd5110_printf.h"
#include "atime_timer_a.h"


void main(void)
{
    unsigned int i=1;
    watchdog_close();			//关闭看门狗
    wait_ms(50);
    basic_clock_init();			//系统时钟初始化
    lcd5110_init();
    printf("Power By ATIME");

    timera_pwm_init1(1000);
    while(1)
    {
        i++;
        if(i>9999)
            i=0;
        timera_pwm_init2(0,i);
        timera_pwm_init2(1,10000-i);
    }
}