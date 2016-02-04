/************************************
库功能：硬件SPI模块库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
应用函数：
修改历史：
		   	 ‘修改人’   ‘修改内容’  ‘修改时间’
				空			空			空
最后修改时间：2016-01-31，尚未测试
作者： ATIME	版权所有
实例程序：
说明：本程序只支持3线主机模式的SPI通信，可以进行多机通信。
***************************************/


#ifndef _ATIME_MSP430_SPI_HARDWARE_H_ 
#define _ATIME_MSP430_SPI_HARDWARE_H_


/************************************
库全局变量组
***************************************/
/*SPI串口0设置*/
#define SPI0_ON                        //SPI串口0开关（SPI0_ON，SPI0_OFF）
#define SPI0_RECEIVE           1       //SPI串口0接收中断使能（1EN，0DIS）
#define SPI0_SEND              1       //SPI串口0发送中断使能（1EN，0DIS）
#define SPI0_MODE              1       //SPI模式设置（0~3）
#define SPI0_CHAR_LENGTH       8       //数据位位数设置（8,7）
#define SPI0_CLK	       2       //定义CLK源（0EXTERNAL UCLK，1ACLK，2SMCLK，3SMCLK）
#define SPI0_DIV               500     //分频系数（整数300~500k）

/*SPI串口1设置*/
#define SPI1_OFF                       //SPI串口0开关（SPI0_ON，SPI0_OFF）
#define SPI1_RECEIVE           1       //SPI串口0接收中断使能（1EN，0DIS）
#define SPI1_SEND              1       //SPI串口0发送中断使能（1EN，0DIS）
#define SPI1_MODE              1       //SPI模式设置（0~3）
#define SPI1_CHAR_LENGTH       8       //数据位位数设置（8,7）
#define SPI1_CLK	       2       //定义CLK源（0EXTERNAL UCLK，1ACLK，2SMCLK，3SMCLK）
#define SPI1_DIV               500     //分频系数（整数300~500k）

/************************************
工作模式说明：
0： 上升沿发送，下降沿接收 正常UCLK
1： 下降沿发送，上升沿接收 正常UCLK
2： 下降沿发送，上升沿接收 延时半个周期UCLK
3： 上升沿发送，下降沿接收 延时半个周期UCLK
CPOL:时钟信号设置；
     0表示闲时为0，
     1表示闲时为1.
***************************************/

/************************************
代码区段：SPI0相关函数
***************************************/
#ifdef SPI0_ON
/************************************
函数功能：初始化SPI接口0
传递参数：空
返回值：空
***************************************/
unsigned char spi0_init(void)
{	
	U0CTL |=SWRST;                 //初始
	U0CTL &=0x01;
	U0CTL |=MM+SYNC;
	#if SPI0_CHAR_LENGTH==8
	U0CTL |=CHAR;
	#endif
	
	U0TCTL =0x00;
	U0TCTL |=(SPI0_MODE<<6)+(SPI0_CLK<<4)+STC;

        if(SPI0_DIV<=2)
		return 0xff;
	U0BR0 =SPI0_DIV/256;
	U0BR1 =SPI0_DIV%256;
	
	U0RCTL =0x00;
	U0MCTL =0x00;
	
	ME1 |=USPIE0;
	UCTL0 &= ~SWRST;
	#if SPI0_SEND==1
	IE1 |=UTXIE0;
	#endif
	#if SPI0_RECEIVE==1
	IE1 |=URXIE0;
	#endif
        return(0);                  //设置成功
}


/************************************
函数功能：SPI接口0发送并接受
传递参数：空
返回值：错误值：0xff；
        正确值：0；
***************************************/
unsigned char spi0_trans(unsigned char a)
{
    unsigned char num=0;
    while(!(U0TCTL&0x01))
    {
        num++;
        if(num>254)
            return 0xff;
    }
    while(!(IFG1&UTXIFG0));
    U0TXBUF =a;
    return U0RXBUF;
}


#endif

/************************************
代码区段：SPI1相关函数
***************************************/
#ifdef SPI1_ON
/************************************
函数功能：初始化SPI接口1
传递参数：空
返回值：空
***************************************/
unsigned char spi1_init(void)
{
	U1CTL |=SWRST;                 //初始
	U1CTL &=0x01;
	U1CTL |=MM+SYNC;
	#if SPI1_CHAR_LENGTH==8
	U1CTL |=CHAR;
	#endif
	
	U1TCTL =0x00;
	U1TCTL |=(SPI1_MODE<<6)+(SPI1_CLK<<4)+STC;
	
	if(SPI1_DIV<=2)
		return 0xff;
	U1BR0 =SPI1_DIV/256;
	U1BR1 =SPI1_DIV%256;
	
	U1RCTL =0x00;
	U1MCTL =0x00;
	
	ME2 |=USPIE1;
	UCTL1 &= ~SWRST;
	#if SPI1_SEND==1
	IE2 |=UTXIE1;
	#endif
	#if SPI1_RECEIVE==1
	IE2 |=URXIE1;
	#endif
        return(0);                  //设置成功
}


/************************************
函数功能：SPI接口1发送读取当前信息
传递参数：空
返回值：错误值：0xff；
        正确值：发送同时读取的值；
***************************************/
unsigned char spi1_trans(unsigned char a)
{
    unsigned char num=0;
    while(!(U1TCTL&0x01))
    {
        num++;
        if(num>254)
            return 0xff;
    }
    while(!(IFG2&UTXIFG1));
    U1TXBUF =a;
    while(!(IFG2&URXIFG1));
    return U1RXBUF;
}


#endif
/************************************
代码区段：公共函数
***************************************/

/************************************
函数功能：初始化SPI接口
传递参数：空
返回值：空
***************************************/
void spi_init(void)
{
#ifdef SPI1_ON
    spi1_init();
#endif
#ifdef SPI0_ON
    spi0_init();
#endif
}


/************************************
函数功能：SPI发送数据
传递参数：
        num：SPI口号（0,1）；
        ch ：待发送数据；
返回值：
      0xff：错误
      0：正常
***************************************/
unsigned char spi_trans( unsigned char ch, unsigned char num)
{
    if(num==0)
    {
    #ifdef SPI0_ON
        spi0_trans(ch);
        return (0);
    #endif
    }
    else 
    if(num==1)
    {
    #ifdef SPI1_ON
        spi1_trans(ch);
        return (0);
    #endif
    }
    return (0xff);
}


#endif