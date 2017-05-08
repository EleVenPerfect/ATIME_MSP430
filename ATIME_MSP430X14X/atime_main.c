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
