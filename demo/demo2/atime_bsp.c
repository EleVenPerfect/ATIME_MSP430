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

unsigned int mubiao_num = 10;           //预设目标数量
unsigned int dangqian_num = 0;          //记录当前数量

unsigned char dangqian_num2 = 1;        //临时变量
unsigned char bjdj_stop = 0;            //步进电机运行标志
unsigned int bjdj_speed = 5;           //步进电机转速调节

void Init_MAX7219();
void bsp_init()
{
    //数码管接口初始化
    PxyDIRz( 6, 0, 1); //P60 - DIN
    PxyDIRz( 6, 1, 1); //P61 - CS
    PxyDIRz( 6, 2, 1); //P62 - CLK
    PxyOUTz(6,0,0);
    PxyOUTz(6,1,0);
    PxyOUTz(6,2,0);
    Init_MAX7219();
    
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
    
    //蜂鸣器接口初始化
    PxyDIRz( 6, 7, 1);
    PxyOUTz(6,7,1);
    
    
    //按键接口初始化
    PxyDIRz( 1, 0, 0);
    PxyDIRz( 1, 1, 0);
    PxyIEz( 1, 0, 1);
    PxyIEz( 1, 1, 1);
    PxyIESz( 1, 0, 0);
    PxyIESz( 1, 1, 0);
    PxyIFGz(1,0,0);
    PxyIFGz(1,1,0);
    
    //红外对射模块初始化-上升沿触发中断
    PxyDIRz( 1, 2, 0);
    PxyIEz( 1, 2, 1);
    PxyIESz( 1, 2, 0);
    PxyIFGz( 1, 2, 0);
    
    
    // P24置为低电平
    PxyDIRz( 2, 4, 0);
    
}

unsigned char phasecw[4] ={0x01,0x02,0x04,0x08};//正转 电机导通相序 D-C-B-A
void bjdj_run()//步进电机运转
{
    if(bjdj_stop==0)
    {
        for(unsigned int j=0;j<20;j++)
        for(unsigned char i=0;i<4;i++)
        {
            PxOUT(3)=phasecw[i]>>1;
            wait_ms(bjdj_speed);//转速调节
        }
    }
}


void fengmingqi_run()//蜂鸣器报警,继电器工作
{
    for(int i=0; i<500; i++)
    {
        wait_ms(3);
        PxyOUTz(6,7,1);  //蜂鸣器响一声
        wait_ms(3);
        PxyOUTz(6,7,0);
        wait_ms(3);
        PxyOUTz(6,7,1);  //蜂鸣器响一声
        wait_ms(3);
        PxyOUTz(6,7,0);
        wait_ms(3);
        PxyOUTz(6,7,1);  //蜂鸣器响一声
        wait_ms(3);
        PxyOUTz(6,7,0);
    }
}

void Write_Max7219_byte(unsigned char DATA)         
{
    unsigned char i;    
    PxyOUTz(6,1,0);		
    for(i=8;i>=1;i--)
    {		  
        PxyOUTz(6,2,0);
        PxyOUTz(6,0,(DATA&0x80)>>7);
        DATA=DATA<<1;
        PxyOUTz(6,2,1);
   }                                 
}


void Write_Max7219(unsigned char address,unsigned char dat)
{ 
     PxyOUTz(6,1,0);
     Write_Max7219_byte(address);           //写入地址，即数码管编号
     Write_Max7219_byte(dat);               //写入数据，即数码管显示数字 
     PxyOUTz(6,1,1);                        
}

void Init_MAX7219(void)
{
     Write_Max7219(0x09, 0xff);       //译码方式：BCD码
     Write_Max7219(0x0a, 0x03);       //亮度
     Write_Max7219(0x0b, 0x07);       //扫描界限；4个数码管显示
     Write_Max7219(0x0c, 0x01);       //掉电模式：0，普通模式：1
     Write_Max7219(0x0f, 0x01);       //显示测试：1；测试结束，正常显示：0
}


void number_update()//更新待显示数字左边四位为目标数字，右侧为当前数字
{
    Write_Max7219(1,dangqian_num%10);
    Write_Max7219(2,dangqian_num/10%10);
    Write_Max7219(3,dangqian_num/100%10);
    Write_Max7219(4,dangqian_num/1000%10);
    
    Write_Max7219(5,mubiao_num%10);
    Write_Max7219(6,mubiao_num/10%10);
    Write_Max7219(7,mubiao_num/100%10);
    Write_Max7219(8,mubiao_num/1000%10);
}


#endif