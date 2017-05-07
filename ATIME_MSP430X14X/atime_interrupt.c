/************************************
库功能：中断服务函数控制库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
应用函数：
修改历史：
		   	 ‘修改人’   ‘修改内容’  ‘修改时间’
				空			空			空
最后修改时间：2016-01-25
作者： ATIME	版权所有
实例程序：
常见错误解释：
1.Error[e46]: Undefined external "uart1_sendchar" referred in main  串口1设置没有打开
解决：#define UART1_ON
2.Warning[Pe223]: function "uart0_sendchar" declared implicitly
解决：函数库载入顺序错误，需要将被调用的函数库放在这个控制库前面载入。
***************************************/
#ifndef _MSP430_INTERRUPT_H_
#define _MSP430_INTERRUPT_H_
/************************************
函数功能：串口0接收中断服务函数
***************************************/
#pragma vector=UART0RX_VECTOR
__interrupt void USART0_RX_ISR(void)
{

#ifdef _ATIME_MSP430_UART0_SCANF_C_
UART0RX_FLAG = 1;           //重要：此宏设置内的代码是使用于scanf函数底层设置，如果使用scanf请务必不要删除这段
#endif                      //同时，如果没有使用scanf，此段代码将会自动不进入编译，也不建议删除这段代码

}


/************************************
函数功能：串口0发送中断服务函数
***************************************/
#pragma vector=UART0TX_VECTOR
__interrupt void USART0_TX_ISR(void)
{

}


/************************************
函数功能：串口1接收中断服务函数
***************************************/
#pragma vector=UART1RX_VECTOR
__interrupt void USART1_RX_ISR(void)
{

}


/************************************
函数功能：串口1发送中断服务函数
***************************************/
#pragma vector=UART1TX_VECTOR
__interrupt void USART1_TX_ISR(void)
{

}


/************************************
函数功能：ADC12中断服务函数
***************************************/
#pragma vector=ADC_VECTOR
__interrupt void ADC_12_ISR(void)
{

}


/************************************
函数功能：看门狗定时器定时模式中断服务函数
注意：只有在定时模式时才会进入中断，
      在看门狗模式会触发复位。
***************************************/
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{

}



#endif
