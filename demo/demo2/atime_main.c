#include <msp430x14x.h>
#include "atime_msp430core.h"		//MSP430核心库
#include "atime_oled4_12864_printf.h"
#include "atime_uart0_scanf.h"
#include "atime_interrupt.c"
#include "atime_timer_a.h"
void main(void)
{
    unsigned char a[20];
    watchdog_close();			    //关闭看门狗
    basic_clock_init();			    //系统时钟初始化
    uart_init();                            //串口初始化
    interrupt_switch(on);                   //开总中断
    oled4_12864_init();
    printf("ATIME!\r\n");

    while(1)
    {
	scanf("%s",a);
	printf("%s\r\n",a);
    }
}