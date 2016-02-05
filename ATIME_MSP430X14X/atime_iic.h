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
说明：本程序只支持3线主机模式的SPI通信，可以进行多机通信。
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
void iic_init()
{
    PxyDIRz(SCL_PORT,SCL_BIT,0);
    PxyDIRz(SDA_PORT,SDA_BIT,0);
    
    PxySELz(SCL_PORT,SCL_BIT,0);
    PxySELz(SDA_PORT,SDA_BIT,0);
}



#endif