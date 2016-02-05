/************************************
库功能：软件IIC模块库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
应用函数：
        iic_start_s()
        iic_stop_s()
        iic_getack_s()
        iic_setack_s()
        iic_readbyte_s()
        iic_writebyte_s(unsigned char data)
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
#define SDA_PORT        3       //定义SDA总线IO端口
#define SDA_BIT         1       //定义SDA总线IO引脚
#define SCL_PORT        3       //定义SCL总线IO端口
#define SCL_BIT         0       //定义SCL总线IO引脚

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
    delay_us(50);
    
    PxyOUTz(SDA_PORT,SDA_BIT,0);
    delay_us(50);
    PxyOUTz(SCL_PORT,SCL_BIT,0);
    delay_us(10);
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
    delay_us(15);
    PxyOUTz(SCL_PORT,SCL_BIT,1);
    PxyOUTz(SDA_PORT,SDA_BIT,0);
	delay_us(15);
    PxyOUTz(SDA_PORT,SDA_BIT,1);
	delay_us(15);
}


/************************************
函数功能：IIC接口接收ACKNOWLEDGE
传递参数：空
返回值：ACK
***************************************/
unsigned char iic_getack_s()
{
    unsigned char ack=1;
    PxyDIRz(SCL_PORT,SCL_BIT,1);
    PxyDIRz(SDA_PORT,SDA_BIT,0);
    
    PxyOUTz(SCL_PORT,SCL_BIT,0);
    delay_us(10);
    PxyOUTz(SCL_PORT,SCL_BIT,1);
    delay_us(10);
    ack =PxyINz(SDA_PORT,SDA_BIT);
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
	delay_us(10);
    PxyOUTz(SCL_PORT,SCL_BIT,1);
	delay_us(30);
    PxyOUTz(SCL_PORT,SCL_BIT,0);
    PxyOUTz(SDA_PORT,SDA_BIT,0);
	delay_us(10);
}

/************************************
函数功能：IIC接口读byte
传递参数：空
返回值：读取数据
注：接收数据低位先接收
***************************************/
unsigned char iic_readbyte_s()
{
    unsigned char i,data=0,temp;
    PxyDIRz(SCL_PORT,SCL_BIT,1);
    PxyDIRz(SDA_PORT,SDA_BIT,0);
    for( i=0; i<8; i++)
    {
        PxyOUTz(SCL_PORT,SCL_BIT,1);
		delay_us(20);
        data <<=1;
        temp =PxyINz(SDA_PORT,SDA_BIT);
        if(temp)
            data |=0x01;
        else
            data &=~(0x01);
        PxyOUTz(SCL_PORT,SCL_BIT,0);
        delay_us(10);
    }
    return data;
}


/************************************
函数功能：IIC接口写byte
传递参数：发送数据
返回值：空
注：发送数据高位先发送
***************************************/
void iic_writebyte_s(unsigned char data)
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
		delay_us(20);
        PxyOUTz(SCL_PORT,SCL_BIT,1);
        delay_us(40);
        PxyOUTz(SCL_PORT,SCL_BIT,0);
        delay_us(15);
		data <<=1;
    }
}

/************************************
函数功能：IIC接口写n字节
传递参数：data[]：数据；n：数据个数
返回值：空
注：发送数据高位先发送
***************************************/
void iic_write(unsigned char data[],unsigned char n)
{
    unsigned char i;
    iic_start_s();
    data[0] &=~0x01;
    //delay_us(30);
    for( i=0; i<n; i++)
    {
        iic_writebyte_s(data[i]);  
        //delay_us(30);
        iic_getack_s();
        //delay_us(30);
    }
    iic_stop_s();
}


/************************************
函数功能：IIC接口写n字节
传递参数：data[]：数据；n：数据个数
返回值：空
注：发送数据高位先发送
***************************************/
void iic_read(unsigned char wdata[],unsigned char wn,unsigned char rdata[],unsigned char rn)
{
    unsigned char i;
    iic_start_s();
    wdata[0] &=~0x01;
    for( i=0; i<wn; i++)
    {
        iic_writebyte_s(wdata[i]);  
        iic_getack_s();
    }
    iic_start_s();
    wdata[0] |=0x01;
    iic_writebyte_s(wdata[0]);
    iic_getack_s();	
    for( i=0; i<rn-1; i++)
    {
        rdata[i] =iic_readbyte_s();
        iic_setack_s();
    }
    rdata[i] =iic_readbyte_s();
    iic_stop_s();
}



#endif