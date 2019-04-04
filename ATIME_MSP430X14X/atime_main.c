#include <msp430x14x.h>
#include "atime_msp430core.h"		//MSP430核心库
#include "atime_lcd5110_printf.h"

void main(void)
{
    int x =1993;
    float y =3.1415;
    
    char i =0;
    watchdog_close();			//关闭看门狗
    basic_clock_init();			//系统时钟初始化
    lcd5110_init();                     //液晶屏幕初始化

    printf("The number is:%d;and:%f\n",x,y);
    printf("Float:%f",y);
    
    
    
    while(1)
    {
        i++;
    printf_setadd(0,i%10);
    
    printf("Power By %d",i%10);
    wait_ms(500);
    lcd5110_clear_all();
    }
}