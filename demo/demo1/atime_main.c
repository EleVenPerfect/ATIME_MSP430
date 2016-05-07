#include <msp430x14x.h>
#include "atime_msp430core.h"		//MSP430核心库
#include "atime_lcd5110_printf.h"
#include "atime_am2320_1.c"
#include "atime_am2320_2.c"

void main(void)
{
    watchdog_close();			//关闭看门狗
    wait_ms(50);                        //等待不稳定状态
    basic_clock_init();			//系统时钟初始化
    lcd5110_init();                     //液晶屏幕初始化

    while(1)
    {
        wait_ms(2500);
        as2320_1_read();
        as2320_2_read();
        printf("%d\n",am2320_1_w);      //显示温度数据*10
        printf("%d\n",am2320_1_s);      //显示湿度数据*10
        printf("%d\n",am2320_2_w);      //显示温度数据*10
        printf("%d\n",am2320_2_s);      //显示湿度数据*10
        
        printf_setadd(0,0);
    } 
}
