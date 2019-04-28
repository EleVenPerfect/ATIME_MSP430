/*************************************
库功能：串口0 printf接口库
库要求：程序已经载入：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
        #include <stdio.h>(可不写)
应用函数：
        printf("")
修改历史：
         ‘修改人’   ‘修改内容’  ‘修改时间’
	    空	        空          空
最后修改时间：2017-05-05
作者： ATIME	版权所有
实例程序：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"		//MSP430核心库
        #include "atime_uart0_printf.h"
        #include "atime_interrupt.c"

        void main(void)
        {
            watchdog_close();			    //关闭看门狗
            basic_clock_init();			    //系统时钟初始化
            uart_init();                    //串口初始化
            interrupt_switch(on);           //开总中断

            while(1)
            {
                printf("ATIME!\r\n");
                wait_ms(500);
                printf("\r\n");
                wait_ms(500);
            }
        }

说明：注意区分\r\n：
      \r是光标返回行开头
      \n是下一行，但是光标位置不会自动回到开头
      因此需要使用\r\n来进行换行操作。
*************************************/

#ifndef _ATIME_MSP430_UART0_PRINTF_C_
#define _ATIME_MSP430_UART0_PRINTF_C_

#include <stdio.h>

#ifndef _ATIME_MSP430_UART0_H_
#include "atime_uart.h"
#endif


/*************************************
库全局变量组
*************************************/


/************************************
函数功能：写入数据
传递参数：空
返回值：空
***************************************/
int putchar( int ch)
{
    uart0_sendchar(ch);
    return ch;
}



#endif
