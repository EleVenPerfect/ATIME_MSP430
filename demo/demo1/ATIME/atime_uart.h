/************************************
库功能：串口函数库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
应用函数：
          uart_init(void)
          uart_sendchar( unsigned char ch, unsigned char num)
          uart_send( unsigned char ch[], unsigned char num)    
修改历史：
		   	 ‘修改人’   ‘修改内容’  ‘修改时间’
				空			空			空
最后修改时间：2016-01-25
作者： ATIME	版权所有
实例程序：
      #include <msp430x14x.h>
      #include "atime_msp430core.h"		//MSP430核心库
      #include "atime_uart.h"
      #include "atime_interrupt.c"

      void main(void)
      {

          watchdog_close();			//关闭看门狗
          basic_clock_init();			//系统时钟初始化
          uart_init();                          //串口初始化
          interrupt_switch(on);                 //开总中断
          
          while(1)
          {
              uart_send( "ATIME!\n", 0);
              uart_sendchar( '\n', 0);
              wait_ms(500);
          }
      }
      中断服务函数：
      #pragma vector=UART0RX_VECTOR
      __interrupt void UART0_RX_ISR(void)
      {
          unsigned char data=0;
          data =U0RXBUF;                      //接收到的数据存起来
          uart0_sendchar(data);               //将接收到的数据再发送出去
      }


常见错误解释：
Error[e46]: Undefined external "uart1_sendchar" referred in main  串口1设置没有打开
解决：#define UART1_ON   
***************************************/


#ifndef _ATIME_MSP430_UART_H_ 
#define _ATIME_MSP430_UART_H_

#include "atime_uart.c"


/************************************
代码区段：UART0相关函数
***************************************/
#ifdef UART0_ON

/************************************
函数功能：初始化串口0
传递参数：空
返回值：空
***************************************/
void uart0_init(void);


/************************************
函数功能：串口0发送数据
传递参数：
        ch ：待发送数据；
返回值：空
***************************************/
void uart0_sendchar(unsigned char ch);


/************************************
函数功能：串口发送字符串
传递参数：
        ch ：待发送数据；
返回值：空
***************************************/
void uart0_send(unsigned char ch[]);



#endif


/************************************
代码区段：UART1相关函数
***************************************/
#ifdef UART1_ON

/************************************
函数功能：初始化串口1
传递参数：空
返回值：空
***************************************/
void uart1_init(void);


/************************************
函数功能：串口1发送数据
传递参数：
        ch ：待发送数据；
返回值：空
***************************************/
void uart1_sendchar(unsigned char ch);


/************************************
函数功能：串口1发送字符串
传递参数：
        ch ：待发送数据；
返回值：空
***************************************/
void uart1_send(unsigned char ch[]);



#endif



/************************************
代码区段：公共函数
***************************************/

/************************************
函数功能：初始化串口
传递参数：空
返回值：空
***************************************/
void uart_init(void);


/************************************
函数功能：串口发送数据
传递参数：
        num：串口号（0,1）；
        ch ：待发送数据；
返回值：
      0xff：错误
      0：正常
***************************************/
unsigned char uart_sendchar( unsigned char ch, unsigned char num);


/************************************
函数功能：串口发送字符串
传递参数：
        num：串口号（0,1）；
        ch ：待发送数据；
返回值：空
***************************************/
unsigned char uart_send( unsigned char ch[], unsigned char num);



#endif