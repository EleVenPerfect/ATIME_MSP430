/************************************
库功能：MSP430定时器B库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
应用函数：
        timerb_pwm_init1(unsigned long freq)
        timerb_pwm_init2( unsigned char channel, unsigned int duty)
修改历史：
	‘修改人’   ‘修改内容’  ‘修改时间’
	  空	      空	  空
最后修改时间：2016-02-10
作者： ATIME	版权所有
实例程序：
      #include <msp430x14x.h>
      #include "atime_msp430core.h"		//MSP430核心库
      #include "atime_timer_b.h"


      void main(void)
      {
          unsigned int i=1;
          watchdog_close();			//关闭看门狗
          wait_ms(50);
          basic_clock_init();			//系统时钟初始化

          timerb_pwm_init1(1000);
          while(1)
          {
              i+=1000;
              if(i>9999)
                  i=0;
              timerb_pwm_init2(4,i);
              timerb_pwm_init2(5,10000-i);
              wait_ms(500);
          }
      }

注：
1.目前只写了PWM模式的函数，其他功能尚未添加，如有需要欢迎联系atime@atime.org.cn。
2.timera_pwm_init1要先设置好后再调用timera_pwm_init2，才可以保证占空比正确设置。
***************************************/

#ifndef _ATIME_MSP430_TIMER_B_H_ 
#define _ATIME_MSP430_ATMER_B_H_

#include "atime_timer_b.c"


/************************************
函数功能：TIMER_B的PWM频率设置与初始化
传递参数：定时器频率设置2~65kHz
返回值：空
***************************************/
void timerb_pwm_init1(unsigned long freq);


/************************************
函数功能：PWM输出通道及占空比配置
传递参数：
    channel - PWM输出通道
      |__0--TB1--P4.1
      |__1--TB2--P4.2
      |__------------
      |__5--TB2--P4.6
    duty - PWM输出占空比
      |__0~10000--占空比0.00%~100.00%
返回值：空
注意：timerb_pwm_init1要先设置好后再调用timerb_pwm_init2，才可以保证占空比正确设置。
***************************************/
void timerb_pwm_init2( unsigned char channel, unsigned int duty);
                       

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