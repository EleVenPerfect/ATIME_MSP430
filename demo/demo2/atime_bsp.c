/************************************
库功能：板级支持库
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
注意：本程序没有加入CRC校验，只是检测温度、湿度数据指令是否正确。当错误时返回-999.
***************************************/

#ifndef _ATIME_MSP430_BSP_C_ 
#define _ATIME_MSP430_BSP_C_

void bsp_init()
{
    //数码管接口初始化
    PxyDIRz( 6, 0, 1); //P60 - DIN
    PxyDIRz( 6, 1, 1); //P61 - CS
    PxyDIRz( 6, 2, 1); //P62 - CLK
    PxyOUTz(6,0,0);
    PxyOUTz(6,1,0);
    PxyOUTz(6,2,0);
    //步进电机接口初始化
    PxyDIRz( 3, 1, 1);
    PxyDIRz( 3, 2, 1);
    PxyDIRz( 3, 3, 1);
    PxyDIRz( 3, 4, 1);
    PxyOUTz(3,1,0);
    PxyOUTz(3,2,0);
    PxyOUTz(3,3,0);
    PxyOUTz(3,4,0);
    
    //继电器接口初始化
    PxyDIRz( 1, 7, 1);
    PxyOUTz(1,7,0);//位1时跳动
    
    //LED接口初始化
    PxyDIRz( 2, 1, 1);
    PxyDIRz( 2, 2, 1);
    PxyDIRz( 2, 3, 1);
    PxyDIRz( 2, 0, 1);
    PxyOUTz(2,1,1);
    PxyOUTz(2,2,1);
    PxyOUTz(2,3,1);
    PxyOUTz(2,0,1);
    
    //LED接口初始化
    PxyDIRz( 2, 0, 1);
    PxyDIRz( 2, 1, 1);
    PxyDIRz( 2, 2, 1);
    PxyDIRz( 2, 3, 1);
    
    //蜂鸣器接口初始化
    PxyDIRz( 6, 7, 1);
    PxyOUTz(6,7,1);
    //PxyOUTz(6,7,0);
    
    //按键接口初始化
    PxyDIRz( 1, 0, 0);
    PxyDIRz( 1, 1, 0);
    PxyIEz( 1, 0, 1);
    PxyIEz( 1, 1, 1);
    PxyIESz( 1, 0, 1);
    PxyIESz( 1, 1, 1);
    PxyIFGz(1,0,0);
    PxyIFGz(1,1,0);
    PxyIFGz(1,2,0);
    
    //红外对射模块初始化-上升沿触发中断
    PxyDIRz( 1, 2, 0);
    PxyIEz( 1, 2, 1);
    PxyIESz( 1, 2, 0);
    PxyIFGz( 1, 2, 0);
    
    
    // P24置为低电平
    PxyDIRz( 2, 4, 0);
    
}

void bjdj_run()//步进电机运转
{
    
}

void baojing_run()//蜂鸣器报警,继电器工作
{
    PxyOUTz(1,7,1);  //继电器跳动
    
    for(int i=0; i<500; i++)
    {
        PxyOUTz(6,7,1);  //蜂鸣器响一声
        PxyOUTz(6,7,0);
        PxyOUTz(6,7,1);  //蜂鸣器响一声
        PxyOUTz(6,7,0);
        PxyOUTz(6,7,1);  //蜂鸣器响一声
        PxyOUTz(6,7,0);
    }
}



void number_update()//更新待显示数字左边四位为目标数字，右侧为当前数字
{
}



#endif