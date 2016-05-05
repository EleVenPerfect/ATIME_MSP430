#include <msp430x14x.h>
#include "atime_msp430core.h"		//MSP430核心库
#include "atime_lcd5110_printf.h"
#include "atime_iic_software.h"
#include "atime_ds1307.h"
#include "atime_am2320.c"

void main(void)
{
    unsigned char i,j;
    watchdog_close();			//关闭看门狗
    wait_ms(50);
    basic_clock_init();			//系统时钟初始化
    lcd5110_init();         //液晶屏幕初始化
    
    ds1307_write(ds1307_data);
    while(1)
    {
    ds1307_read(ds1307_data);
        j++;
        for( i=0; i<8; i++)
            printf("  %02X   ",ds1307_data[i]);//以16进制显示
         
        printf_setadd(0,0);
        wait_ms(400);
    } 
}