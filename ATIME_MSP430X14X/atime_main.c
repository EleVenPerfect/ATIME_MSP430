#include <msp430x14x.h>
#include "atime_msp430core.h"		//MSP430核心库
#include "atime_lcd5110_printf.h"
#include "atime_flash.h"


void main(void)
{
    unsigned int a =0;
    
    watchdog_close();			//关闭看门狗
    wait_ms(50);
    basic_clock_init();			//系统时钟初始化
    lcd5110_init();
    printf("Power By ATIME");

    flash_init();                       //初始化flash
    
    flash_erase_segment(FLASH_INFO_A);  //写flash前先把该块擦除
    flash_writeword( FLASH_INFO_A, 0xaabb);//写一个word
    a =flash_readword(FLASH_INFO_A);    //读一个word
    printf("%02X\n",a);
    flash_erase_segment(FLASH_INFO_A);  //写flash前先把该块擦除
    a =flash_readword(FLASH_INFO_A);    //验证擦除数据
    printf("%02X\n",a);
    
    flash_writechar( FLASH_INFO_A, 0xab);//写一个byte
    a =flash_readword(FLASH_INFO_A);     //读一个word
    printf("%02X\n",a);
    flash_erase_segment(FLASH_INFO_A);   //写flash前先把该块擦除
    a =flash_readchar(FLASH_INFO_A);     //验证擦除数据
    printf("%02X\n",a);
    
    while(1);
}