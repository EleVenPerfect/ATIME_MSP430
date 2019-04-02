#include <msp430x14x.h>
#include "atime_msp430core.h"		//MSP430核心库
#include "atime_uart0_stdio.h"
#include "atime_timer_a.h"
#include "atime_timer_b.h"
#include "atime_bsp.c"
#include "atime_interrupt.c"




void main(void)
{
    watchdog_close();                   //关闭看门狗
    basic_clock_init();	                //系统时钟初始化
    uart_init();                        //串口初始化
    bsp_init();                         //板级初始化
    
    Write_Max7219(0x0f, 0x00);          //显示测试：1；测试结束，正常显示：0
    Write_Max7219(1,0);
    Write_Max7219(2,0);
    Write_Max7219(3,0);
    Write_Max7219(4,0); 
    Write_Max7219(5,0);
    Write_Max7219(6,0);
    Write_Max7219(7,0);
    Write_Max7219(8,0);
    
    interrupt_switch(on);               //开总中断
 
    while(1)
    {    
        //更新显示数字
        number_update();
        //步进电机旋转
        bjdj_run();
        
        //报警与继电器模块逻辑
        if( dangqian_num >= mubiao_num )
            fengmingqi_run();//蜂鸣器响
        
        if( dangqian_num >= mubiao_num+3 )
        {
            PxyOUTz(1,7,1); //继电器关闭
            bjdj_stop = 1;
        }
        else
        {
            PxyOUTz(1,7,0); //继电器打开
            bjdj_stop = 0;
        }
        
        //LED闪动
        PxyOUTz(2,3,1); 
        //wait_ms(300);
        bjdj_run();
        PxyOUTz(2,3,0);
        //wait_ms(300);
    }
}
