/*************************************
库功能：MSP430内部FLASH操作库
库要求：程序已经载入：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
应用函数：
        flash_init()
        flash_erase_segment(unsigned int addr)
        flash_writechar( unsigned int addr, unsigned char data)
        flash_writeword( unsigned int addr, unsigned int data)
        unsigned char flash_readchar(unsigned int addr)
        unsigned int flash_readword(unsigned int addr)
修改历史：
         ‘修改人’   ‘修改内容’  ‘修改时间’
	    空	        空          空
最后修改时间：2016-01-29
作者： ATIME	版权所有
实例程序：
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
	  
注意：
1.本库函数目前只实现了块擦出，但仍要注意不要把存储程序的flash块擦出了。
2.flash写入前需要擦除，写入后再写入需要再擦出，否则发生错误。
3.块地址详见数据手册。
4.flash写入时比较耗电，注意电源是否充足，有些开发板可能会出现液晶屏变暗甚至不显示。
*************************************/

#ifndef _ATIME_MSP430_FLASH_H_ 
#define _ATIME_MSP430_FLASH_H_

#include "atime_flash.c"


/************************************
函数功能：初始化flash控制器
传递参数：空
返回值：空
注：默认设置300k左右的时钟，使用mclk时钟源；
***************************************/
void flash_init();


/************************************
函数功能：等待flash控制器
传递参数：空
返回值：空Flash忙则等待，直到flash空闲。
***************************************/
void flash_wait();


/************************************
函数功能：flash块擦除
传递参数：addr：待擦出块地址
返回值：空
***************************************/
void flash_erase_segment(unsigned int addr);


/************************************
函数功能：flash写1byte（8bit）
传递参数：
    addr：地址；
    data：待写入数据
返回值：空
***************************************/
void flash_writechar( unsigned int addr, unsigned char data);


/************************************
函数功能：flash写1个字（16bit）
传递参数：
addr：待写入地址；
data：待写入字；
返回值：空
***************************************/
void flash_writeword( unsigned int addr, unsigned int data);


/************************************
函数功能：读1byte数据（8bit）
传递参数：addr：待读取地址
返回值：读取的数据
***************************************/
unsigned char flash_readchar(unsigned int addr);


/************************************
函数功能：读取一个字（16bit）
传递参数：待读取字的地址
返回值：读取的数据
***************************************/
unsigned int flash_readword(unsigned int addr);



#endif