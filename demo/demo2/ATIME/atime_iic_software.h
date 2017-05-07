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

#include "atime_iic_software.c"


/************************************
函数功能：IIC接口开始
传递参数：空
返回值：空
***************************************/
void iic_start_s();


/************************************
函数功能：IIC接口结束
传递参数：空
返回值：空
***************************************/
void iic_stop_s();


/************************************
函数功能：IIC接口接收ACKNOWLEDGE
传递参数：空
返回值：ACK
***************************************/
unsigned char iic_getack_s();


/************************************
函数功能：IIC接口发送ACKNOWLEDGE
传递参数：空
返回值：空
***************************************/
void iic_setack_s();


/************************************
函数功能：IIC接口读byte
传递参数：空
返回值：读取数据
注：接收数据低位先接收
***************************************/
unsigned char iic_readbyte_s();


/************************************
函数功能：IIC接口写byte
传递参数：发送数据
返回值：空
注：发送数据高位先发送
***************************************/
void iic_writebyte_s(unsigned char data);


/************************************
函数功能：IIC接口写n字节
传递参数：data[]：数据；n：数据个数
返回值：空
注：发送数据高位先发送
***************************************/
void iic_write(unsigned char data[],unsigned char n);


/************************************
函数功能：IIC接口读n字节
传递参数：data[]：数据；n：数据个数
返回值：空
注：发送数据高位先发送
***************************************/
void iic_read(unsigned char wdata[],unsigned char wn,unsigned char rdata[],unsigned char rn);



#endif
