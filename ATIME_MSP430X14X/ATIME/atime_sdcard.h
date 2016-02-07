
//尚未测试，移植自ATIME51单片机库。
//尚未测试，移植自ATIME51单片机库。
//尚未测试，移植自ATIME51单片机库。


/************************************
库功能：SPI驱动sdcard库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"		//MSP430核心库
        #include "atime_spi_s.h"
        #include "atime_msp430core.h"
应用函数：
修改历史：
		   	 ‘修改人’   ‘修改内容’  ‘修改时间’
				空			空			空
最后修改时间：尚未测试，移植自ATIME51单片机库。
作者： ATIME	版权所有
实例程序：
常见错误解释：
此程序只适用于SD卡而不是SDHC卡，容量小于2G，尚未测试。
***************************************/

#ifndef _ATIME_MSP430_SPI_SDCARD_H_ 
#define _ATIME_MSP430_SPI_SDCARD_H_

#include "atime_sdcard.c"


/************************************
函数功能：发送命令
传递参数：空
返回值：应答
***************************************/
unsigned char sd_write_zhi(unsigned char a[]);


/************************************
函数功能：复位SD卡
传递参数：空
返回值：0:成功;1:失败
***************************************/
unsigned char sd_res(void);


/************************************
函数功能：初始化SD卡
传递参数：空
返回值：0:成功;1:失败
**************************************/
unsigned char sd_init(void);


/************************************
函数功能：向SD卡某地址写入512字节数据
传递参数：add:地址   a[]：待写入数据
返回值：0:成功;1:失败
***************************************/
unsigned char sd_write512(unsigned long add, unsigned char a[]);


/************************************
函数功能：向SD卡某地址读取512字节数据
传递参数：add:地址   a[]：待写入数据
返回值：0:成功;1:失败
***************************************/
unsigned char sd_read512(unsigned long add, unsigned char a[]);



#endif