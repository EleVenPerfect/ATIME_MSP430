/************************************
库功能：AM2320库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
应用函数：
        void max6675_init()
        unsigned int max6675_read()
修改历史：
	‘修改人’   ‘修改内容’  ‘修改时间’
	  空          空	  空
最后修改时间：2016-05-07
作者： ATIME	版权所有
实例程序：
    unsigned int  rdo    = 0;  //存储热电偶温度
    unsigned char rdo_h  = 0;  //存储热电偶温度高位
    unsigned char rdo_l  = 0;  //存储热电偶温度低位
    max6675_init();
    rdo = max6675_read();
    if(max6675_status==on)
    {
        rdo_h = rdo/100%100;
        rdo_l = rdo%100;
    }
    else
    {
        rdo = 9999;
        rdo_h = 99;
        rdo_l = 99;
    }

常见问题解释：
1.如果数据始终是0，可能是没有运行对应的函数。
2.如果返回999，则可能链接线，或没接上拉电阻。
注意：本程序没有加入CRC校验，只是检测温度、湿度数据指令是否正确。当错误时返回-999.
***************************************/

#ifndef _ATIME_MSP430_AM2320_H_ 
#define _ATIME_MSP430_AM2320_H_

#include "atime_max6675.c"

/************************************
函数功能：SPI接口硬件初始化
传递参数：空
返回值：空
***************************************/
void max6675_init();

/************************************
函数功能：SPI接口读取数据
传递参数：空
返回值：温度值
***************************************/
unsigned int max6675_read();

#endif