/************************************
库功能：BME280库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
应用函数：

修改历史：
	‘修改人’   ‘修改内容’  ‘修改时间’
	  空          空	  空
最后修改时间：2016-05-07
作者： ATIME	版权所有
实例程序：


常见问题解释：
注意：
***************************************/

#ifndef _ATIME_MSP430_BME280_C_ 
#define _ATIME_MSP430_BME280_C_


/************************************
库全局变量组
***************************************/
#define BMP280_SCL_PORT        3       //定义SCL总线IO端口
#define BMP280_SCL_BIT         3       //定义SCL总线IO引脚
#define BMP280_SDA_PORT        3       //定义SDA总线IO端口
#define BMP280_SDA_BIT         2       //定义SDA总线IO引脚
//CSB接VCC保证一直处于IIC状态
#define BMP280_ADDR         0xec
#define BMP280_TEMP_ADDR    0xfa
#define BMP280_PRESS_ADDR   0xf7

/************************************
函数功能：
传递参数：空
返回值：空
***************************************/


#endif