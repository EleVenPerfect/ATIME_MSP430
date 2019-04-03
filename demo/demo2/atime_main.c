#include <msp430x14x.h>
#include "atime_msp430core.h"		//MSP430核心库
#include "atime_uart0_stdio.h"
#include "atime_bsp.c"
#include "atime_interrupt.c"




void main(void)
{
    unsigned int kk = 0;
    watchdog_close();                   //关闭看门狗
    basic_clock_init();	                //系统时钟初始化
    wait_ms(500);
    uart_init();                        //串口初始化
    bsp_init();                         //板级初始化
    
    wait_ms(500);
    Init_MAX7219();
    wait_ms(2000);
    Write_Max7219(0x0f, 0x00);       //显示测试：1；测试结束，正常显示：0
    number_update();
    
    interrupt_switch(on);               //开总中断
                
    while(1)
    {   
        number_update();
        //报警与继电器模块逻辑
        if( dangqian_num >= mubiao_num )
            ;//fengmingqi_run();//蜂鸣器响

        bjdj_run();
        PxyOUTz(2,3,1); 
        kk++;
        if(kk>=2)
        {
            kk = 0;
            number_update();
        }

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
        
        bjdj_run();
        PxyOUTz(2,3,0);
    }
}
