/*************************************
库功能：串口0 scanf接口库
库要求：程序已经载入：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
		#include "atime_interrupt.c"//必须载入
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
        #include "atime_uart0_scanf.h"
        #include "atime_uart0_printf.h"
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
常见错误解释：
1.出现Error[e27]: Entry "getchar" in module atime_main ( D:\ATIME_MSP430\demo\demo2\Debug\Obj\atime_main.r43 ) redefined in module
?getchar ( C:\Program Files (x86)\IAR Systems\Embedded Workbench 7.0\430\lib\dlib\dl430fn.r43 )
是因为设置的问题，需要将工程设置里的general option里的Library Configuration里的library设置为：CLIB。
2.Error[Pe020]: identifier "UART0RX_FLAG" is undefined D:\ATIME_MSP430\demo\demo2\ATIME\atime_uart.c 398
需要注意文件的载入顺序，atime_uart0_scanf.h要比atime_uart0_printf.h先载入。
这是因为atime_uart0_scanf.c中有UART0RX_FLAG全局变量定义，而atime_uart.c中条件编译设置成需要编译uart0_getchar()函数，
同时atime_uart0_printf.c中也会编译atime_uart.c，如果atime_uart0_printf的库放在前面就会在编译atime_uart0_printf过程中编译了atime_uart.c。
3.若下载程序后出现无反应，首先检查是否进行了串口初始化，然后检查是否在入了#include "atime_interrupt.c"。
使用注意：
1.VT100终端设置流控（DTR/DSR、RTS/CTS、XON/XOFF）均为关闭状态，终端使用VT100模式。
2.本scanf移植函数不支持光标移动操作，只会记录按键记录，因此Backspace和Delete按键实际是一样的功能。
3.输入缓冲区大小SCANF_TEMP_SIZE建议根据使用情况设置，尽量减小不必要的资源浪费。
4.按Ctrl+C和Ctrl+Z都是取消当前输入，重新输入。
5.输入缓存实际大小是SCANF_TEMP_SIZE+2，后两个用于存储结束标识符。
6.atime_uart0_sacnf使用时，如果没有#include atime_uart0_printf.h则会出现没有输出的情况，这是因为没有载入移植的printf。
*************************************/

#ifndef _ATIME_MSP430_UART0_SCANF_C_
#define _ATIME_MSP430_UART0_SCANF_C_

#include <stdio.h>

/*************************************
库全局变量组
*************************************/
#define SCANF_TEMP_SIZE       200//输入缓冲区大小
unsigned char UART0RX_FLAG = 0;


#ifndef _ATIME_MSP430_UART0_H_  //因为已经定义宏_ATIME_MSP430_UART0_SCANF_C_，
#include "atime_uart.h"         //atime_uart.c中需要已定义变量UART0RX_FLAG
#endif                          //


/*************************************
标准终端设备中，特殊ASCII码定义，请勿修改
*************************************/
#define SCANFINBACKSP 0x08           //ASCII  <--  (退格键)
#define SCANFINDELETE 0x7F           //ASCII <DEL> (DEL 键)
#define SCANFINEOL '\r'              //ASCII <CR>  (回车键)
#define SCANFINSKIP '\3'             //ASCII control-C
#define SCANFINEOF '\x1A'            //ASCII control-Z

#define SCANFOUTDELETE "\x8 \x8"     //VT100 backspace and clear

#define SCANFRETRY      "\r\n^TRY AGAIN\r\n"
#define SCANFTEMPFULL   "^FULL"

/************************************
函数功能：获取数据
传递参数：空
返回值：ch：获取到的数据
***************************************/
int getchar( void)
{
    static char buffer[SCANF_TEMP_SIZE+2];      //字符缓存
    static char ptr;                            //缓存指针
    char ch;

    if(buffer[ptr] != 0x0)                     //如果缓冲区有字符
    {
        ch = buffer[ptr];
        buffer[ptr] = 0x0;                      //清除已发出的缓存内容（没必要）
        ptr++;
        return (ch);                            //则逐个返回字符
    }

    ptr = 0;                                    //发送完毕，缓冲区指针归零，

    while(1)
    {
        ch = uart0_getchar();                   //等待接收一个字符

        if( ch==SCANFINDELETE || ch==SCANFINBACKSP )//==退格或删除键==
        {
            if(ptr)                             //缓冲区有值
            {
                ptr--;                          //从缓冲区移除一个字符
                printf(SCANFOUTDELETE);         //同时显示也删掉一个字符
            }
        }
        else
        if( ch == SCANFINSKIP || ch == SCANFINEOF )//==取消键 Ctrl+C ==
        {
            printf(SCANFRETRY);                 //终端显示跳至下一行
            ptr = 0;
            continue;                          //结束符本次循环
        }
        else
        if(ch == SCANFINEOL)                    //== '\r' 回车==
        {
            buffer[ptr++] = '\n';               //终端换行
            printf("\r\n");                     //光标返回第一列
            buffer[ptr] = 0;                    //末尾添加结束符（NULL）
            ptr = 0;                            //指针清空
            return (buffer[ptr++]);
        }
        else
        if(ptr < SCANF_TEMP_SIZE)               //== 正常字符 ==
        {
           if(ch >= ' ')                        //删除 0x20以下字符
            {
                printf("%c",ch);
                buffer[ptr++] = ch;             //存入缓冲区
            }
        }
        else                                    //缓冲区已满
        {

            printf(SCANFTEMPFULL);              //== 0x07 缓存满提示信息
        }
    }
}




#endif
