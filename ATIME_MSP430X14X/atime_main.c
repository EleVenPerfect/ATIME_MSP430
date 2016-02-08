#include <msp430x14x.h>
#include "atime_msp430core.h"		//MSP430核心库
#include "atime_lcd5110_printf.h"
#include "atime_flash.c"


void main(void)
{
    int a;
    
    watchdog_close();			//关闭看门狗
    wait_ms(50);
    basic_clock_init();			//系统时钟初始化
    lcd5110_init();
    printf("Power By ATIME");
    flash_init();
/*    
    FlashInit();
    FlashWriteChar(InfoB,0x25);
    a=FlashReadChar (InfoB);        //InfoB在H文件中有宏定义
    FlashWriteWord(InfoB+2,0x5669);
    a = FlashReadWord(InfoB+2);
    FlashErase(InfoB);
*/    
    while(1);
}