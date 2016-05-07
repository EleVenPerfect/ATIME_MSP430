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

注意：本程序没有加入CRC校验，只是检测温度、湿度数据指令是否正确。当错误时返回-999.
***************************************/

#ifndef _ATIME_MSP430_IIC_AM2320_C_ 
#define _ATIME_MSP430_IIC_AM2320_C_


/************************************
库全局变量组
***************************************/
int am2320_w = 0;               //温度数据
int am2320_s = 0;               //湿度数据


/************************************
函数功能：IIC接口读数据
传递参数：数据存储数组
返回值：空
***************************************/
void as2320_waken_iic(void)
{
    iic_start_s();
    iic_writebyte_s(0xB8);
    iic_getack_s();
    wait_ms(2);
    iic_stop_s();
}


/************************************
函数功能：IIC接口读数据
传递参数：数据存储数组
返回值：空
***************************************/
void as2320_read_iic(void)
{
    unsigned char am2320_data[8]={0};
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
}


/************************************
函数功能：IIC接口写数据
传递参数：数据存储数组
返回值：空
***************************************/
void am2320_write_iic(void)
{
    iic_start_s();
    iic_writebyte_s(0xB8);
    iic_getack_s();
    iic_writebyte_s(0x03);
    iic_getack_s();
    iic_writebyte_s(0x00);
    iic_getack_s();
    iic_writebyte_s(0x04);
    iic_getack_s();
    iic_stop_s();
}


/************************************
函数功能：AM2320读数据
传递参数：数据存储数组
返回值：空
***************************************/
void as2320_read()
{
    as2320_waken_iic();
    am2320_write_iic();
    wait_ms(3);
    as2320_read_iic();
}


#endif