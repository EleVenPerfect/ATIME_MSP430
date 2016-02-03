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
常见错误解释：
***************************************/


#ifndef _ATIME_MSP430_SPI_HARDWARE_H_ 
#define _ATIME_MSP430_SPI_HARDWARE_H_


/************************************
库全局变量组
***************************************/
/*SPI串口0设置*/
#define SPI0_OFF                       //SPI串口0开关（SPI0_ON，SPI0_OFF）
#define SPI0_BAUD              14400   //波特率设置（）
#define SPI0_RECEIVE           1       //SPI串口0接收中断使能（1EN，0DIS）
#define SPI0_SEND              0       //SPI串口0发送中断使能（1EN，0DIS）
#define SPI0_MODE              1       //SPI模式设置（1~4）
#define SPI0_PIN               4       //SPI模式设置（3~4）
#define SPI0_CHAR_LENGTH       8       //数据位位数设置（8,7）

/*SPI串口1设置*/
#define SPI1_OFF                       //SPI串口1开关（SPI1_ON，SPI1_OFF）
#define SPI1_BAUD              14400   //波特率设置（）
#define SPI1_RECEIVE           1       //SPI串口1接收中断使能（1EN，0DIS）
#define SPI1_SEND              0       //SPI串口1发送中断使能（1EN，0DIS）
#define SPI1_MODE              0       //SPI模式设置（1~4）
#define SPI1_PIN               4       //SPI模式设置（3~4）
#define SPI1_CHAR_LENGTH       8       //数据位位数设置（8,7）
/************************************
工作模式说明：
模式0：CPHA=0;CPOL=0;
模式1：CPHA=0;CPOL=1;
模式2：CPH1=1;CPOL=0;
模式3：CPHA=1;CPOL=1;
CPHA:时钟相位设置；
     0表示每周期第一个时钟沿采样，
     1表示每周期第二个时钟沿采样。
CPOL:时钟信号设置；
     0表示闲时为0，
     1表示闲时为1.
***************************************/

/************************************
代码区段：SPI0相关函数
***************************************/
#ifdef SPI0_ON
/************************************
函数功能：初始化SPI接口1
传递参数：空
返回值：空
***************************************/
unsigned char spi0_init(void)
{

    return(0);                  //设置成功
}

#endif

/************************************
代码区段：SPI1相关函数
***************************************/
#ifdef SPI1_ON
/************************************
函数功能：初始化SPI接口2
传递参数：空
返回值：空
***************************************/
unsigned char spi1_init(void)
{

    return(0);                  //设置成功
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
unsigned char spi_sendchar( unsigned char ch, unsigned char num)
{
    if(num==0)
    {
    #ifdef SPI0_ON
        spi0_sendchar(ch);
        return (0);
    #endif
    }
    else 
    if(num==1)
    {
    #ifdef SPI1_ON
        spi1_sendchar(ch);
        return (0);
    #endif
    }
    return (0xff);
}


/************************************
函数功能：SPI接收数据
传递参数：
        num：SPI口号（0,1）；
        ch ：待发送数据；
返回值：
      0xff：错误
      0：正常
***************************************/
需要判断忙！！！UART也需要！！


#endif