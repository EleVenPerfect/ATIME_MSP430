#include <msp430x14x.h>
#include "atime_msp430core.h"		//MSP430核心库
#include "atime_lcd5110_printf.h"
//#include "atime_iic_software.h"
#include "atime_am2320.c"

void main(void)
{
    watchdog_close();			//关闭看门狗
    wait_ms(50);
    basic_clock_init();			//系统时钟初始化
    lcd5110_init();                     //液晶屏幕初始化


    while(1)
    {
        wait_ms(2500);
        as2320_1_read();
        printf("%d\n",am2320_w);//以16进制显示
        printf("%d\n",am2320_s);//以16进制显示

        printf_setadd(0,0);
    } 
}