#include <msp430x14x.h>
#include "atime_msp430core.h"		//MSP430核心库
#include "atime_uart0_stdio.h"
#include "atime_timer_a.h"
#include "atime_timer_b.h"
#include "atime_bsp.c"
#include "atime_interrupt.c"

unsigned int mubiao_num = 10;
unsigned int dangqian_num = 0;


void main(void)
{
    watchdog_close();                   //关闭看门狗
    basic_clock_init();	                //系统时钟初始化
    uart_init();                        //串口初始化
    bsp_init();                         //板级初始化
    interrupt_switch(on);               //开总中断
    
    while(1)
    {
        //步进电机旋转
        bjdj_run();
        
        //报警与继电器模块逻辑
        if( mubiao_num==dangqian_num )
        {
            baojing_run();
        }
        
        //显示数字更新
        number_update();
        wait_ms(10);
    }
}
