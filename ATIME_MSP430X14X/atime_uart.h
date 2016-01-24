/************************************
库功能：串口函数库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
应用函数：
修改历史：
		   	 ‘修改人’   ‘修改内容’  ‘修改时间’
				空			空			空
最后修改时间：2014-01-29
作者： ATIME	版权所有
实例程序：
常见错误解释：

***************************************/


#ifndef _ATIME_MSP430_UART_H_ 
#define _ATIME_MSP430_UART_H_


/************************************
库全局变量组
***************************************/
/*串口0设置*/
#define UART0_ON                        //串口0开关（UART0_ON，UART0_OFF）
#define UART0_RECEIVE           1       //串口0接收使能（1EN，0DIS）
#define UART0_SEND              1       //串口0发送使能（1EN，0DIS）
#define UART0_MODE              1       //串口0功能设置（1UART，0SPI）
#define UART0_BAUD              9600    //波特率设置（）
#define UART0_PARITY            0       //奇偶校验位设置（0NONE,1ODD,2EVEN）
#define UART0_CHAR_LENGTH       8       //数据位位数设置（8,7）
#define UART0_STOP_BIT          1       //停止位设置（1,2）

/*串口1设置*/
#define UART1_OFF                       //串口1开关（UART1_ON，UART1_OFF）
#define UART1_RECEIVE           EN      //串口1接收使能（EN，DIS）
#define UART1_SEND              EN      //串口1发送使能（EN，DIS）
#define UART1_MODE              UART    //串口1功能设置（UART，SPI）
#define UART1_BAUD              9600    //波特率设置（）
#define UART1_PARITY            NONE    //奇偶校验位设置（NONE,ODD,EVEN）
#define UART1_CHAR_LENGTH       8       //数据位位数设置（8,7）
#define UART1_STOP_BIT          1       //停止位设置（1,2）



/////////代码区段：UART0相关函数/////////////
#ifdef UART0_ON

/************************************
函数功能：波特率设置
传递参数：
      sysclk:系统时钟频率；
      baud  :波特率；
返回值：空
注：参考博客地址：http://www.cnblogs.com/Engin/archive/2011/07/06/2098990.html
***************************************/
int abs_abs_abs(int i);
void set_baud_clock0( long sysclk, long baud)
{

    unsigned int kk;
    char i,mSum;
    int txEr0,txEr1;
    
    mSum =0;
    txEr0 =0;
    txEr1 =0;
    
    kk =(unsigned int)sysclk/baud;
    U0BR1 =kk >> 8;         //高8位
    U0BR0 =kk & 0xff;       //低8位
    
     for( i=0; i<8; i++)
    {
        txEr0 =(int) 100 * baud * ((i + 1) * kk + mSum) / sysclk - 100 * (i + 1);
        txEr1 =(int) 100 * baud * ((i + 1) * kk + mSum + 1) / sysclk - 100 * (i + 1);
        if(abs_abs_abs(txEr1) < abs_abs_abs(txEr0))
        {
            mSum++;
            U0MCTL |= (1<<i);
        }
    }
}


/************************************
函数功能：初始化串口0
传递参数：空
返回值：空
***************************************/
void uart0_init(void)
{

    U0CTL |= SWRST;             //初始置1
    
    U0TCTL |= SSEL1;		    //波特率发生器时钟频率SMCLK(1MHz)
    
    set_baud_clock0(XT2IN/MSP430_DIVS,UART0_BAUD);//设置串口0波特率寄存器

    #if UART0_PARITY==2
      U0CTL |=PENA;
      U0CTL |=PEV;
    #endif
    #if UART0_PARITY==1
      U0CTL |=PENA;
      U0CTL &=(~PEV);
    #endif                //奇偶校验位设置

    #if UART0_STOP_BIT==2
      U0CTL |=SPB;
    #endif                //停止位设置
      
    #if UART0_CHAR_LENGTH==8
      U0CTL |=CHAR;
    #endif                //数据位长度设置

    #if UART0_MODE==0
      U0CTL |=SYNC;
    #endif
    #if UART0_MODE==1
      U0CTL &=~SYNC;
    #endif                //USART模式设置
  
  
    P3SEL |= 0x30;              //设置I/O端口模式
    P3DIR |= 0x10;
	
    U0ME |= UTXE0 + URXE0;      //发送接收使能
	
    U0CTL &= (~SWRST);		//软件清零SWRST位
	
    U0IE |= URXIE0 + UTXIE0;	//发送接收中断使能
}


/************************************
函数功能：串口0发送数据
传递参数：
        ch ：待发送数据；
返回值：空
***************************************/
void uart0_sendchar(unsigned char ch)
{
    //while(!(IFG1&UTXIFG0));          //发送寄存器空的时候发送数据
    U0TXBUF =ch;
}


/************************************
函数功能：串口发送字符串
传递参数：
        ch ：待发送数据；
返回值：空
***************************************/
void uart0_send(unsigned char ch[])
{
    unsigned int i =0;
    for( i=0; ch[i]!=0x0; i++)
      uart0_sendchar(ch[i]);
}



#endif


/////////代码区段：UART2相关函数/////////////
#ifdef UART1_ON



#endif



/////////代码区段：公共函数/////////////

/************************************
函数功能：取绝对值
传递参数：值
返回值：绝对值
***************************************/
int abs_abs_abs(int i)
{
    return (i>=0?i:(-i));
}


/************************************
函数功能：初始化串口
传递参数：空
返回值：空
***************************************/
void uart_init(void)
{
#ifdef UART1_ON
    uart1_init();
#endif
#ifdef UART0_ON
    uart0_init();
#endif
}


/************************************
函数功能：串口发送数据
传递参数：
        num：串口号（0,1）；
        ch ：待发送数据；
返回值：空
***************************************/
void uart_sendchar( unsigned char num, unsigned char ch)
{

}


/************************************
函数功能：串口发送字符串
传递参数：
        num：串口号（0,1）；
        ch ：待发送数据；
返回值：空
***************************************/
void uart_send( unsigned char num, unsigned char ch[])
{
    unsigned int i =0;
    for( i=0; ch[i]!=0x0; i++)
      uart_sendchar( num, ch[i]);
}




#endif