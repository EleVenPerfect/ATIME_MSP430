/*************************************
库功能：RS485库
库要求：程序已经载入：
        #include <msp430x14x.h>
        #include "atime_uart.h"
应用函数：
修改历史：
         ‘修改人’   ‘修改内容’  ‘修改时间’
	    空		  空		 空
最后修改时间：2016-02-09
作者： ATIME	版权所有
实例程序：
常见错误解释：
*************************************/

#ifndef _ATIME_MSP430_RS485_C_ 
#define _ATIME_MSP430_RS485_C_

#include "atime_uart.c"

/*************************************
库全局变量组
*************************************/
#define UART0_RS485_ON                  //定义开启RS485的接口（UART0_RS485_ON，UART0_RS485_OFF）
#define UART0_RS485_DIRPORT     5       //定义DIR线端口
#define UART0_RS485_DIRBIT      2       //定义DIR线引脚

#define UART1_RS485_OFF                 //定义开启RS485的接口（UART1_RS485_ON，UART1_RS485_OFF）
#define UART1_RS485_DIRPORT     5       //定义DIR线端口
#define UART1_RS485_DIRBIT      2       //定义DIR线引脚


/************************************
代码区段：UART0相关函数
***************************************/
#ifdef UART0_RS485_ON

/************************************
函数功能：初始化串口0
传递参数：空
返回值：空
***************************************/
void rs4850_init(void)
{

}


/************************************
函数功能：串口0发送数据
传递参数：
        ch ：待发送数据；
返回值：空
***************************************/
void rs4850_sendchar(unsigned char ch)
{

}


/************************************
函数功能：串口发送字符串
传递参数：
        ch ：待发送数据；
返回值：空
***************************************/
void rs4850_send(unsigned char ch[])
{

}



#endif


/************************************
代码区段：UART1相关函数
***************************************/
#ifdef UART1_RS485_ON
/************************************
函数功能：初始化串口1
传递参数：空
返回值：空
***************************************/
void rs4851_init(void)
{

}


/************************************
函数功能：串口1发送数据
传递参数：
        ch ：待发送数据；
返回值：空
***************************************/
void rs4851_sendchar(unsigned char ch)
{

}


/************************************
函数功能：串口1发送字符串
传递参数：
        ch ：待发送数据；
返回值：空
***************************************/
void rs4851_send(unsigned char ch[])
{

}



#endif



/************************************
代码区段：公共函数
***************************************/

/************************************
函数功能：初始化串口
传递参数：空
返回值：空
***************************************/
void rs485_init(void)
{
#ifdef UART1_RS485_ON
    rs4851_init();
#endif
#ifdef UART0_RS485_ON
    rs4850_init();
#endif
}


/************************************
函数功能：串口发送数据
传递参数：
        num：串口号（0,1）；
        ch ：待发送数据；
返回值：
      0xff：错误
      0：正常
***************************************/
unsigned char rs485_sendchar( unsigned char ch, unsigned char num)
{
    if(num==0)
    {
    #ifdef UART0_RS485_ON
        rs4850_sendchar(ch);
        return (0);
    #endif
    }
    else 
    if(num==1)
    {
    #ifdef UART1_RS485_ON
        rs4851_sendchar(ch);
        return (0);
    #endif
    }
    return (0xff);
}


/************************************
函数功能：串口发送字符串
传递参数：
        num：串口号（0,1）；
        ch ：待发送数据；
返回值：空
***************************************/
unsigned char rs485_send( unsigned char ch[], unsigned char num)
{
    unsigned int i =0;
    unsigned char err=0;
    
    if(num==0)
    {
    #ifdef UART0_RS485_ON
        for( i=0; ch[i]!=0x0; i++)
          err +=rs485_sendchar( ch[i], num);
        return (err);
    #endif
    }
    else 
    if(num==1)
    {
    #ifdef UART1_RS485_ON
        for( i=0; ch[i]!=0x0; i++)
          err +=rs485_sendchar( ch[i], num);
        return (err);
    #endif
    }
    return (0xff);
}


#endif