/************************************
库功能：软件IIC模块库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
应用函数：
修改历史：
	‘修改人’   ‘修改内容’  ‘修改时间’
	  空          空	  空
最后修改时间：2016-02-05
作者： ATIME	版权所有
实例程序：
说明：
1.The SDA pin is normally pulled high with an external device. Data on the SDA pin may change only during SCL low time periods (refer to
Data Validity timing diagram). Data changes during SCL high periods will indicate a start
or stop condition as defined below.
2.
***************************************/


#ifndef _ATIME_MSP430_IIC_SOFTWARE_H_ 
#define _ATIME_MSP430_IIC_SOFTWARE_H_


/************************************
库全局变量组
***************************************/
#define SDA_PORT        6       //定义SDA总线IO端口
#define SDA_BIT         1       //定义SDA总线IO引脚
#define SCL_PORT        6       //定义SCL总线IO端口
#define SCL_BIT         0       //定义SCL总线IO引脚
#define IIC_CYC         10      //定义时序的周期

/************************************
函数功能：初始化IIC接口
传递参数：空
返回值：空
***************************************/
void iic_init_s()
{
    
    
    PxySELz(SCL_PORT,SCL_BIT,0);
    PxySELz(SDA_PORT,SDA_BIT,0);
}


/************************************
函数功能：IIC接口开始
传递参数：空
返回值：空
***************************************/
void iic_start_s()
{
    PxyDIRz(SCL_PORT,SCL_BIT,1);
    PxyDIRz(SDA_PORT,SDA_BIT,1);
    
    PxyOUTz(SCL_PORT,SCL_BIT,1);
    PxyOUTz(SDA_PORT,SDA_BIT,1);
    delay_us(10);
    
    PxyOUTz(SDA_PORT,SDA_BIT,0);
    delay_us(10);
    PxyOUTz(SCL_PORT,SCL_BIT,0);
}

/************************************
函数功能：IIC接口结束
传递参数：空
返回值：空
***************************************/
void iic_stop_s()
{
    PxyDIRz(SCL_PORT,SCL_BIT,1);
    PxyDIRz(SDA_PORT,SDA_BIT,1);
    delay_us(8);
    PxyOUTz(SCL_PORT,SCL_BIT,1);
    PxyOUTz(SDA_PORT,SDA_BIT,0);
    PxyOUTz(SDA_PORT,SDA_BIT,1);
}


/************************************
函数功能：IIC接口接收ACKNOWLEDGE
传递参数：空
返回值：空
***************************************/
unsigned char iic_getack_s()
{
    unsigned char ack=1;
    PxyDIRz(SCL_PORT,SCL_BIT,1);
    PxyDIRz(SDA_PORT,SDA_BIT,0);
    
    PxyOUTz(SCL_PORT,SCL_BIT,0);
    delay_us(3);
    PxyOUTz(SCL_PORT,SCL_BIT,1);
    delay_us(3);
    ack =PxyINz(SCL_PORT,SCL_BIT);
    PxyOUTz(SCL_PORT,SCL_BIT,0);
    return ack;
}
/************************************
函数功能：IIC接口发送ACKNOWLEDGE
传递参数：空
返回值：空
***************************************/
void iic_setack_s()
{
    PxyDIRz(SCL_PORT,SCL_BIT,1);
    PxyDIRz(SDA_PORT,SDA_BIT,1);
    
    PxyOUTz(SCL_PORT,SCL_BIT,0);
    PxyOUTz(SDA_PORT,SDA_BIT,0);
    PxyOUTz(SCL_PORT,SCL_BIT,1);
    PxyOUTz(SCL_PORT,SCL_BIT,0);
    PxyOUTz(SDA_PORT,SDA_BIT,0);
}

/************************************
函数功能：IIC接口读byte
传递参数：空
返回值：空
***************************************/
void iic_readchar_s()
{
    unsigned char i,data,temp=0;
    PxyDIRz(SCL_PORT,SCL_BIT,1);
    PxyDIRz(SDA_PORT,SDA_BIT,0);
    for( i=0; i<8; i++)
    {
        PxyOUTz(SCL_PORT,SCL_BIT,1);
        data <<=1;
        temp =PxyINz(SCL_PORT,SCL_BIT);
        if(temp)
            data |=0x01;
        else
            data &=~(0x01);
        PxyOUTz(SCL_PORT,SCL_BIT,0);
        delay_us(10);
    }
}


/************************************
函数功能：IIC接口写byte
传递参数：空
返回值：空
***************************************/
void iic_writechar_s(unsigned char data)
{
    unsigned char i;
    PxyDIRz(SCL_PORT,SCL_BIT,1);
    PxyDIRz(SDA_PORT,SDA_BIT,1);
    
    for( i=0; i<8; i++)
    {
        if(data&0x80)
            PxyOUTz(SDA_PORT,SDA_BIT,1);
        else
            PxyOUTz(SDA_PORT,SDA_BIT,0);
        PxyOUTz(SCL_PORT,SCL_BIT,1);
        delay_us(20);
        PxyOUTz(SCL_PORT,SCL_BIT,0);
        delay_us(15);
    }
}



#endif