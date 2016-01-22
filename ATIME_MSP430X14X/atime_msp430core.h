/*************************************
库功能：MSP430核心功能
库要求：程序已经载入：
        #include <msp430x14x.h>
应用函数：
        watchdog_close();
        basic_clock_init();
修改历史：
         ‘修改人’   ‘修改内容’  ‘修改时间’
	    空		  空		 空
最后修改时间：2016-01-22
作者： ATIME	版权所有
实例程序：
      #include <msp430x14x.h>
      #include "atime_msp430core.h"		    //MSP430核心库
      
      void main(void)
      { 
        
          watchdog_close();					//关闭看门狗
          basic_clock_init();				//系统时钟初始化
              
          while(1);
      }

常见错误解释：
*************************************/

#ifndef _ATIME_MSP430_CORE_H_ 
#define _ATIME_MSP430_CORE_H_


/*************************************
库全局变量组
*************************************/
#define XIN   ((double)32768)        //外部低频晶振32.768KHZ
#define XT2IN ((double)8000000)      //外部高频晶振8MHZ
#define MSP430_DIVM 1                //MCLK分频比
#define MSP430_DIVS 1                //SMCLK分频比

enum msp430_switch { on, off};       //枚举逻辑


/*************************************
函数功能：准确延时函数
传递参数：x
返回值：空
*************************************/
#define delay_us(x) __delay_cycles((long)(XT2IN*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(XT2IN*(double)x/1000.0)) 


/*************************************
函数功能：关闭看门狗
传递参数：空
返回值：空
*************************************/
void watchdog_close(void)
{
     WDTCTL =WDTPW + WDTHOLD;
}


/*************************************
函数功能：软件延时(近似)
传递参数：空
返回值：空
*************************************/
void wait_ms(unsigned int i)
{
     int j=0;
     i =i/MSP430_DIVM;
     for( j=0; j<i; j++)
     {
         delay_us(990);
     }
}


/*************************************
函数功能：系统时钟控制
basic clock 设置，以高频晶振为MCLK/SMCLK时钟源。均不分频。
传递参数：空
返回值：空
*************************************/
void basic_clock_init(void)
{
    unsigned char i;  
  
    BCSCTL1 &= ~XT2OFF;                 //打开XT2高频晶体振荡器
    
    do
    {
        IFG1 &= ~OFIFG;                //清除晶振失败标志
        for( i =0xFF; i>0; i--);       //延时等待8MHz晶体起振
 
    }while((IFG1 & OFIFG));            //晶振失效标志仍然存在
 
    switch(MSP430_DIVM)
    {
        case 1 : i=SELM_2 + SELS + DIVM_0; break;
        case 2 : i=SELM_2 + SELS + DIVM_1; break;
        case 4 : i=SELM_2 + SELS + DIVM_2; break;
        case 8 : i=SELM_2 + SELS + DIVM_3; break;
        default: i=SELM_2 + SELS + DIVM_0; break;
    }
    switch(MSP430_DIVS)
    {
        case 1 : i=i + DIVS_0; break;
        case 2 : i=i + DIVS_1; break;
        case 4 : i=i + DIVS_2; break;
        case 8 : i=i + DIVS_3; break;
        default: i=i + DIVS_0; break;
    }
      
    BCSCTL2 =i;//MCLK和SMCLK选择高频晶振
}



#endif
