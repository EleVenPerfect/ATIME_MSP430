/************************************
库功能：MSP430定时器A库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
应用函数：
        timera_pwm_init1(unsigned long freq)
        timera_pwm_init2( unsigned char channel, unsigned int duty)
修改历史：
	‘修改人’   ‘修改内容’  ‘修改时间’
	  空	      空	  空
最后修改时间：2016-02-09
作者： ATIME	版权所有
实例程序：
注：
1.目前只写了PWM模式的函数，其他功能尚未添加，如有需要欢迎联系atime@atime.org.cn。
2.timera_pwm_init1要先设置好后再调用timera_pwm_init2，才可以保证占空比正确设置。
***************************************/

#ifndef _ATIME_MSP430_TIMER_A_C_ 
#define _ATIME_MSP430_ATMER_A_C_


/************************************
库全局变量组
***************************************/


/************************************
函数功能：TIMER_A的PWM频率设置与初始化
传递参数：定时器频率设置
返回值：空
***************************************/
void timera_pwm_init1(unsigned long freq)
{
    unsigned long mod =0;
    unsigned long modtemp =0;
    unsigned char ps =0;                //分频系数
	
    TACTL |=TACLR;                      //清除定时器A

    if((modtemp=(unsigned long)(XT2IN/MSP430_DIVS)/(freq*8))<0xFFFF)
    {
        ps =3;
        mod =modtemp;
	if((modtemp=(unsigned long)(XT2IN/MSP430_DIVS)/(freq*4))<0xFFFF)
	{
            ps =2;
            mod =modtemp;
            if((modtemp=(unsigned long)(XT2IN/MSP430_DIVS)/(freq*2))<0xFFFF)
            {
                ps =1;
		mod =modtemp;
		if((modtemp=(unsigned long)(XT2IN/MSP430_DIVS)/freq)<0xFFFF)
		{
                    ps =0;
		    mod =modtemp;
		}
            }
	 }
    }
    TACTL =0;                           //关闭定时器
    TACTL =TASSEL1 + (ps<<6) + MC_1;
    TACCR0 =mod;
}


/************************************
函数功能：PWM输出通道及占空比配置
传递参数：
    channel - PWM输出通道
      |__0--TA1
      |__1--TA2
    duty - PWM输出占空比
      |__0~10000--占空比0.00%~100.00%
返回值：空
注意：timera_pwm_init1要先设置好后再调用timera_pwm_init2，才可以保证占空比正确设置。
***************************************/
void timera_pwm_init2( unsigned char channel, unsigned int duty)
{
    switch(channel)
    {
          case 0:
          {
              TACCTL1 =OUTMOD_7;
              TACCR1 =(unsigned int)((duty*TACCR0)/10000);
              P1SEL |=(0x01<<5);
              P1DIR |=(0x01<<5);
          }
          break;
          case 1:
          {
              TACCTL2 =OUTMOD_7;
              TACCR2 =(unsigned int)((duty*TACCR0)/10000);
              P1SEL |=(0x01<<6);
              P1DIR |=(0x01<<6);
          }
          break;
    }
}


/************************************
函数功能：
传递参数：空
返回值：空
***************************************/
/************************************
函数功能：
传递参数：空
返回值：空
***************************************/


#endif