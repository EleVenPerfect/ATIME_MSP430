/*************************************
库功能：串口0 stdio接口库
库要求：程序已经载入：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
        #include <stdio.h>(可不写)
应用函数：
        scanf("",&)
修改历史：
         ‘修改人’   ‘修改内容’  ‘修改时间’
	    空	        空          空
最后修改时间：2017-05-07
作者： ATIME	版权所有
实例程序：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"		//MSP430核心库
        #include "atime_uart0_stdio.h"
        #include "atime_interrupt.c"

        void main(void)
        {
            unsigned char a[SCANF_TEMP_SIZE+2];
            watchdog_close();			    //关闭看门狗
            basic_clock_init();			    //系统时钟初始化
            uart_init();                            //串口初始化
            interrupt_switch(on);                   //开总中断

            while(1)
            {
                printf("ATIME!\r\n");
                wait_ms(500);
                scanf("%s",a);
                printf("%s\r\n",a);
                wait_ms(500);
            }
        }

说明：
本stdio移植的是串口0接口实现了printf和scanf函数。
常见错误解释：
Error[Pe020]: identifier "UART0RX_FLAG" is undefined D:\ATIME_MSP430\demo\demo2\ATIME\atime_uart.c 398
需要注意文件的载入顺序，atime_uart0_scanf.h要比atime_uart0_printf.h先载入。
这是因为atime_uart0_scanf.c中有UART0RX_FLAG全局变量定义，而atime_uart.c中条件编译设置成需要编译uart0_getchar()函数，
同时atime_uart0_printf.c中也会编译atime_uart.c，如果atime_uart0_printf的库放在前面就会在编译atime_uart0_printf过程中编译了atime_uart.c。
*************************************/

#ifndef _ATIME_MSP430_UART0_STDIO_C_
#define _ATIME_MSP430_UART0_STDIO_C_


#include "atime_uart0_scanf.h"
#include "atime_uart0_printf.h"


#endif
