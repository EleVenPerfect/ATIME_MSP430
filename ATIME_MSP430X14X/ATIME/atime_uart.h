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


/************************************
库全局变量组
***************************************/
/*串口0设置*/
#define UART0_ON                        //串口0开关（UART0_ON，UART0_OFF）
#define UART0_BAUD              9600    //波特率设置（）
#define UART0_RECEIVE           1       //串口0接收中断使能（1EN，0DIS）
#define UART0_SEND              0       //串口0发送中断使能（1EN，0DIS）
#define UART0_PARITY            0       //奇偶校验位设置（0NONE,1ODD,2EVEN）
#define UART0_CHAR_LENGTH       8       //数据位位数设置（8,7）
#define UART0_STOP_BIT          1       //停止位设置（1,2）

/*串口1设置*/
#define UART1_ON                        //串口1开关（UART1_ON，UART1_OFF）
#define UART1_BAUD              9600    //波特率设置（）
#define UART1_RECEIVE           1       //串口1接收中断使能（1EN，0DIS）
#define UART1_SEND              0       //串口1发送中断使能（1EN，0DIS）
#define UART1_PARITY            0       //奇偶校验位设置（0NONE,1ODD,2EVEN）
#define UART1_CHAR_LENGTH       8       //数据位位数设置（8,7）
#define UART1_STOP_BIT          1       //停止位设置（1,2）
/************************************
代码区段：UART0相关函数
***************************************/
#ifdef UART0_ON

/************************************
波特率设置组，切勿修改
***************************************/
#define UART0_BAUD_SETTING   (double)((double)(XT2IN/MSP430_DIVS)/((double)UART0_BAUD))           //波特率计算公式
#define UART0_BAUD_H         (unsigned char)((unsigned long)UART0_BAUD_SETTING>>8)               //U0BR1高位                                         //提取高位
#define UART0_BAUD_L         (unsigned char)((unsigned long)UART0_BAUD_SETTING)                  //U0BR0低位                                         //低位
#define UART0_BAUD_U0MCTL    (unsigned char)((UART0_BAUD_SETTING-(long)UART0_BAUD_SETTING)*8+0.5) //U0MCTL低位
//注：+0.5含义：编译器数据类型转换时为去整数部分，加0.5可以使它四舍五入 
/************************************
函数功能：初始化串口0
传递参数：空
返回值：空
***************************************/
void uart0_init(void)
{

    unsigned char i=0x01;          //U0CTL设置
    
    U0CTL |=SWRST;                  //初始置1
    
    #if UART0_PARITY==2
      i |=PENA;
      i |=PEV;
    #endif
    #if UART0_PARITY==1
      i |=PENA;
      i &=(~PEV);
    #endif                //奇偶校验位设置

    #if UART0_STOP_BIT==2
      i |=SPB;
    #endif                //停止位设置
      
    #if UART0_CHAR_LENGTH==8
      i |=CHAR;
    #endif                //数据位长度设置

    U0CTL |=i;		  //软件清零SWRST位
    
    
    U0TCTL |=SSEL1;		    //波特率发生器时钟频率SMCLK(1MHz)
    
    U0BR1 =UART0_BAUD_H;            //取串口时钟频率/波特率频率整数
    U0BR0 =UART0_BAUD_L;            //高位存在BR1，低位存在BR0
    switch(UART0_BAUD_U0MCTL)      //小数存在U0MCTL，算法见上方宏定义
    {
        case 1 :U0MCTL =0x04; break;
        case 2 :U0MCTL =0x22; break;
        case 3 :U0MCTL =0x92; break;
        case 4 :U0MCTL =0xaa; break;
        case 5 :U0MCTL =0x6d; break;
        case 6 :U0MCTL =0xdd; break;
        case 7 :U0MCTL =0xf7; break;
        default:U0MCTL =0x00;
    }
    
    P3SEL |= 0x30;              //设置I/O端口模式P3.4-P3.5
    P3DIR |= 0x10;
	
    U0ME |= UTXE0 + URXE0;      //发送接收使能
    
    U0CTL&=~SWRST;              //软件清零SWRST位
    
    #if UART0_RECEIVE==1	
        U0IE |= URXIE0;	        //接收中断使能
    #endif
        
    #if UART0_SEND==1	
        U0IE |= UTXIE0;	        //发送中断使能
    #endif    
}


/************************************
函数功能：串口0发送数据
传递参数：
        ch ：待发送数据；
返回值：空
***************************************/
void uart0_sendchar(unsigned char ch)
{
    while(!(IFG1&UTXIFG0));          //发送寄存器空的时候发送数据
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
    for( i=0; ch[i]!='\0'; i++)
      uart0_sendchar(ch[i]);
}



#endif


/************************************
代码区段：UART1相关函数
***************************************/
#ifdef UART1_ON

/************************************
波特率设置组，切勿修改
***************************************/
#define UART1_BAUD_SETTING   (double)((double)(XT2IN/MSP430_DIVS)/((double)UART1_BAUD))           //波特率计算公式
#define UART1_BAUD_H         (unsigned char)((unsigned long)UART1_BAUD_SETTING>>8)               //U1BR1高位                                         //提取高位
#define UART1_BAUD_L         (unsigned char)((unsigned long)UART1_BAUD_SETTING)                  //U1BR0低位                                         //低位
#define UART1_BAUD_U1MCTL    (unsigned char)((UART1_BAUD_SETTING-(long)UART1_BAUD_SETTING)*8+0.5) //U1MCTL低位
//注：+0.5含义：编译器数据类型转换时为去整数部分，加0.5可以使它四舍五入 
/************************************
函数功能：初始化串口1
传递参数：空
返回值：空
***************************************/
void uart1_init(void)
{

    unsigned char i=0x01;          //U1CTL设置
    
    U1CTL |=SWRST;                  //初始置1
    
    #if UART1_PARITY==2
      i |=PENA;
      i |=PEV;
    #endif
    #if UART1_PARITY==1
      i |=PENA;
      i &=(~PEV);
    #endif                //奇偶校验位设置

    #if UART1_STOP_BIT==2
      i |=SPB;
    #endif                //停止位设置
      
    #if UART1_CHAR_LENGTH==8
      i |=CHAR;
    #endif                //数据位长度设置

    U1CTL |=i;		  //软件清零SWRST位
    
    
    U1TCTL |=SSEL1;		    //波特率发生器时钟频率SMCLK(1MHz)
    
    U1BR1 =UART1_BAUD_H;            //取串口时钟频率/波特率频率整数
    U1BR0 =UART1_BAUD_L;            //高位存在BR1，低位存在BR1
    switch(UART1_BAUD_U1MCTL)      //小数存在U1MCTL，算法见上方宏定义
    {
        case 1 :U1MCTL =0x04; break;
        case 2 :U1MCTL =0x22; break;
        case 3 :U1MCTL =0x92; break;
        case 4 :U1MCTL =0xaa; break;
        case 5 :U1MCTL =0x6d; break;
        case 6 :U1MCTL =0xdd; break;
        case 7 :U1MCTL =0xf7; break;
        default:U1MCTL =0x00;
    }

    P3SEL |= 0xc0;              //设置I/O端口模式P3.6-P3.7
    P3DIR |= 0x40;

    U1ME |= UTXE1 + URXE1;      //发送接收使能
    
    U1CTL&=~SWRST;              //软件清零SWRST位
    
    #if UART1_RECEIVE==1	
        U1IE |= URXIE1;	        //接收中断使能
    #endif
        
    #if UART1_SEND==1	
        U1IE |= UTXIE1;	        //发送中断使能
    #endif    
}


/************************************
函数功能：串口1发送数据
传递参数：
        ch ：待发送数据；
返回值：空
***************************************/
void uart1_sendchar(unsigned char ch)
{
    while(!(IFG2&UTXIFG1));          //发送寄存器空的时候发送数据
    U1TXBUF =ch;
}


/************************************
函数功能：串口1发送字符串
传递参数：
        ch ：待发送数据；
返回值：空
***************************************/
void uart1_send(unsigned char ch[])
{
    unsigned int i =0;
    for( i=0; ch[i]!='\0'; i++)
      uart1_sendchar(ch[i]);
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
返回值：
      0xff：错误
      0：正常
***************************************/
unsigned char uart_sendchar( unsigned char ch, unsigned char num)
{
    if(num==0)
    {
    #ifdef UART0_ON
        uart0_sendchar(ch);
        return (0);
    #endif
    }
    else 
    if(num==1)
    {
    #ifdef UART1_ON
        uart1_sendchar(ch);
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
unsigned char uart_send( unsigned char ch[], unsigned char num)
{
    unsigned int i =0;
    unsigned char err=0;
    
    if(num==0)
    {
    #ifdef UART0_ON
        for( i=0; ch[i]!=0x0; i++)
          err +=uart_sendchar( ch[i], num);
        return (err);
    #endif
    }
    else 
    if(num==1)
    {
    #ifdef UART1_ON
        for( i=0; ch[i]!=0x0; i++)
          err +=uart_sendchar( ch[i], num);
        return (err);
    #endif
    }
    return (0xff);
}



#endif