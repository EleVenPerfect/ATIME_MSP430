/************************************
库功能：AM2320库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
        #include "atime_iic_software.h"
应用函数：
修改历史：
	‘修改人’   ‘修改内容’  ‘修改时间’
	  空          空	  空
最后修改时间：2016-02-05
作者： ATIME	版权所有
实例程序：

常见错误解释：
***************************************/

#ifndef _ATIME_MSP430_IIC_AM2320_C_ 
#define _ATIME_MSP430_IIC_AM2320_C_


/************************************
库全局变量组
***************************************/
<<<<<<< HEAD
int am2320_w = 0;                       //温度数据
int am2320_s = 0;                       //湿度数据

#define AM2320_1_SDA_PORT        3       //定义SDA总线IO端口
#define AM2320_1_SDA_BIT         1       //定义SDA总线IO引脚
#define AM2320_1_SCL_PORT        3       //定义SCL总线IO端口
#define AM2320_1_SCL_BIT         0       //定义SCL总线IO引脚

/************************************
函数功能：IIC接口开始
传递参数：空
返回值：空
***************************************/
void am2320_1_iic_start_s()
{
    PxyDIRz(AM2320_1_SCL_PORT,AM2320_1_SCL_BIT,1);
    PxyDIRz(AM2320_1_SDA_PORT,AM2320_1_SDA_BIT,1);
    
    PxyOUTz(AM2320_1_SCL_PORT,AM2320_1_SCL_BIT,1);
    PxyOUTz(AM2320_1_SDA_PORT,AM2320_1_SDA_BIT,1);
    delay_us(50);
    
    PxyOUTz(AM2320_1_SDA_PORT,AM2320_1_SDA_BIT,0);
    delay_us(50);
    PxyOUTz(AM2320_1_SCL_PORT,AM2320_1_SCL_BIT,0);
    delay_us(10);
}

/************************************
函数功能：IIC接口结束
传递参数：空
返回值：空
***************************************/
void am2320_1_iic_stop_s()
{
    PxyDIRz(AM2320_1_SCL_PORT,AM2320_1_SCL_BIT,1);
    PxyDIRz(AM2320_1_SDA_PORT,AM2320_1_SDA_BIT,1);
    delay_us(15);
    PxyOUTz(AM2320_1_SCL_PORT,AM2320_1_SCL_BIT,1);
    PxyOUTz(AM2320_1_SDA_PORT,AM2320_1_SDA_BIT,0);
    delay_us(15);
    PxyOUTz(AM2320_1_SDA_PORT,AM2320_1_SDA_BIT,1);
    delay_us(30);
}


/************************************
函数功能：IIC接口接收ACKNOWLEDGE
传递参数：空
返回值：ACK
***************************************/
unsigned char am2320_1_iic_getack_s()
{
    unsigned char ack=1;
    PxyDIRz(AM2320_1_SCL_PORT,AM2320_1_SCL_BIT,1);
    PxyDIRz(AM2320_1_SDA_PORT,AM2320_1_SDA_BIT,0);
    
    PxyOUTz(AM2320_1_SCL_PORT,AM2320_1_SCL_BIT,0);
    delay_us(10);
    PxyOUTz(AM2320_1_SCL_PORT,AM2320_1_SCL_BIT,1);
    delay_us(10);
    ack =PxyINz(AM2320_1_SDA_PORT,AM2320_1_SDA_BIT);
    PxyOUTz(AM2320_1_SCL_PORT,AM2320_1_SCL_BIT,0);
    return ack;
}
/************************************
函数功能：IIC接口发送ACKNOWLEDGE
传递参数：空
返回值：空
***************************************/
void am2320_1_iic_setack_s()
{
    PxyDIRz(AM2320_1_SCL_PORT,AM2320_1_SCL_BIT,1);
    PxyDIRz(AM2320_1_SDA_PORT,AM2320_1_SDA_BIT,1);
    
    PxyOUTz(AM2320_1_SCL_PORT,AM2320_1_SCL_BIT,0);
    PxyOUTz(AM2320_1_SDA_PORT,AM2320_1_SDA_BIT,0);
    delay_us(10);
    PxyOUTz(AM2320_1_SCL_PORT,AM2320_1_SCL_BIT,1);
    delay_us(30);
    PxyOUTz(AM2320_1_SCL_PORT,AM2320_1_SCL_BIT,0);
    PxyOUTz(AM2320_1_SDA_PORT,AM2320_1_SDA_BIT,0);
    delay_us(10);
}

/************************************
函数功能：IIC接口读byte
传递参数：空
返回值：读取数据
注：接收数据低位先接收
***************************************/
unsigned char am2320_1_iic_readbyte_s()
{
    unsigned char i,data=0,temp;
    PxyDIRz(AM2320_1_SCL_PORT,AM2320_1_SCL_BIT,1);
    PxyDIRz(AM2320_1_SDA_PORT,AM2320_1_SDA_BIT,0);
    for( i=0; i<8; i++)
    {
        PxyOUTz(AM2320_1_SCL_PORT,AM2320_1_SCL_BIT,1);
	delay_us(20);
        data <<=1;
        temp =PxyINz(AM2320_1_SDA_PORT,AM2320_1_SDA_BIT);
        if(temp)
            data |=0x01;
        else
            data &=~(0x01);
        PxyOUTz(AM2320_1_SCL_PORT,AM2320_1_SCL_BIT,0);
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
void am2320_1_iic_writebyte_s(unsigned char data)
{
    unsigned char i;
    PxyDIRz(AM2320_1_SCL_PORT,AM2320_1_SCL_BIT,1);
    PxyDIRz(AM2320_1_SDA_PORT,AM2320_1_SDA_BIT,1);
    
    for( i=0; i<8; i++)
    {
        if(data&0x80)
            PxyOUTz(AM2320_1_SDA_PORT,AM2320_1_SDA_BIT,1);
        else
            PxyOUTz(AM2320_1_SDA_PORT,AM2320_1_SDA_BIT,0);
		delay_us(20);
        PxyOUTz(AM2320_1_SCL_PORT,AM2320_1_SCL_BIT,1);
        delay_us(40);
        PxyOUTz(AM2320_1_SCL_PORT,AM2320_1_SCL_BIT,0);
        delay_us(15);
        data <<=1;
        PxyOUTz(AM2320_1_SDA_PORT,AM2320_1_SDA_BIT,0);
    }
}

=======
unsigned char am2320_data[8]={0x0};
>>>>>>> parent of d805e50... am2320


/************************************
函数功能：IIC接口读数据
传递参数：数据存储数组
返回值：空
***************************************/
void as2320_1_waken_iic(void)
{
    am2320_1_iic_start_s();
    am2320_1_iic_writebyte_s(0xB8);
    am2320_1_iic_getack_s();
    wait_ms(2);
    am2320_1_iic_start_s();
}

/************************************
函数功能：IIC接口读数据
传递参数：数据存储数组
返回值：空
***************************************/
void as2320_1_read_iic(void)
{
<<<<<<< HEAD
    unsigned char am2320_data[8]={0};
    am2320_1_iic_start_s();
    am2320_1_iic_writebyte_s(0xB8|0x01);
    am2320_1_iic_getack_s();
    wait_ms(1);
    am2320_data[0] = am2320_1_iic_readbyte_s();
    am2320_1_iic_setack_s();
    am2320_data[1] = am2320_1_iic_readbyte_s();
    am2320_1_iic_setack_s();
    am2320_data[2] = am2320_1_iic_readbyte_s();
    am2320_1_iic_setack_s();
    am2320_data[3] = am2320_1_iic_readbyte_s();
    am2320_1_iic_setack_s();
    am2320_data[4] = am2320_1_iic_readbyte_s();
    am2320_1_iic_setack_s();
    am2320_data[5] = am2320_1_iic_readbyte_s();
    am2320_1_iic_setack_s();
    am2320_data[6] = am2320_1_iic_readbyte_s();
    am2320_1_iic_setack_s();
    am2320_data[7] = am2320_1_iic_readbyte_s();
    am2320_1_iic_start_s();
    if( am2320_data[0]==0x03&&am2320_data[1]==0x04 )
    {

         am2320_s = am2320_data[2]*256+am2320_data[3];
         am2320_w = am2320_data[4]*256+am2320_data[5];
    }
    else
    {
        am2320_s = -999;
        am2320_w = -999;
    } 
=======
    iic_start_s();
    iic_writebyte_s(0xB8|0x01);
    iic_getack_s();
    wait_ms(1);
    am2320_data[0] = iic_readbyte_s();
    iic_setack_s();
    am2320_data[1] = iic_readbyte_s();
    iic_setack_s();
    am2320_data[2] = iic_readbyte_s();
    iic_setack_s();
    am2320_data[3] = iic_readbyte_s();
    iic_setack_s();
    am2320_data[4] = iic_readbyte_s();
    iic_setack_s();
    am2320_data[5] = iic_readbyte_s();
    iic_setack_s();
    am2320_data[6] = iic_readbyte_s();
    iic_setack_s();
    am2320_data[7] = iic_readbyte_s();
    iic_stop_s();
>>>>>>> parent of d805e50... am2320
}


/************************************
函数功能：IIC接口写数据
传递参数：数据存储数组
返回值：空
***************************************/
void am2320_1_write_iic(void)
{
    am2320_1_iic_start_s();
    am2320_1_iic_writebyte_s(0xB8);
    am2320_1_iic_getack_s();
    am2320_1_iic_writebyte_s(0x03);
    am2320_1_iic_getack_s();
    am2320_1_iic_writebyte_s(0x00);
    am2320_1_iic_getack_s();
    am2320_1_iic_writebyte_s(0x04);
    am2320_1_iic_getack_s();
    am2320_1_iic_start_s();
}


/************************************
函数功能：AM2320读数据
传递参数：数据存储数组
返回值：空
***************************************/
void as2320_1_read()
{
<<<<<<< HEAD
    as2320_1_waken_iic();
    am2320_1_write_iic();
    wait_ms(3);
    as2320_1_read_iic();
=======
    as2320_waken_iic();
    as2320_read_iic();
    wait_ms(3);
    am2320_write_iic();
>>>>>>> parent of d805e50... am2320
}


#endif