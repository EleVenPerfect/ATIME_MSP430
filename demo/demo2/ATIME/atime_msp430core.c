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
1.Error[Pe011]: unrecognized preprocessing directive:检查#include拼写。
2.软件仿真时，弹出：a word access on odd address 0x19;
  表示有整型常量指定存储在flash奇地址上了。应该地址改成偶地址。
  但是本函数中的地址为寄存器地址，所以不应该更改，而是用char型地址保存，详见ioreg_trans函数。
3.Warning[Pa050]: non-native end of line sequence detected (this diagnostic is only issued once) 可能是某个文件最后不是以回车结尾。
*************************************/

#ifndef _ATIME_MSP430_CORE_C_ 
#define _ATIME_MSP430_CORE_C_


/*************************************
库全局变量组
*************************************/
#define XIN   ((double)32768)        //外部低频晶振32.768KHZ
#define XT2IN ((double)8000000)      //外部高频晶振8MHZ
#define MSP430_DIVM 1                //MCLK分频比
#define MSP430_DIVS 8                //SMCLK分频比

enum msp430_switch { on, off};       //枚举逻辑


/*************************************
I/O接口设置组，切勿修改！
*************************************/
#define PxIN(x)         PxINTEMP(x)
#define PxINTEMP(x)     P##x##IN
#define PxOUT(x)        PxOUTTEMP(x)
#define PxOUTTEMP(x)    P##x##OUT
#define PxDIR(x)        PxDIRTEMP(x)
#define PxDIRTEMP(x)    P##x##DIR
#define PxSEL(x)        PxSELTEMP(x)
#define PxSELTEMP(x)    P##x##SEL

#define PxIFG(x)        PxIFGTEMP(x)
#define PxIFGTEMP(x)    P##x##IFG
#define PxIES(x)        PxIESTEMP(x)
#define PxIESTEMP(x)    P##x##IES
#define PxIE(x)         PxIETEMP(x)
#define PxIETEMP(x)     P##x##IE


#define PxyOUTz(x,y,z)        PxyOUTzTEMP(x,y,z)
#define PxyOUTzTEMP(x,y,z)    (z>0)?((P##x##OUT)|=(0x01<<y)) :((P##x##OUT) &=(~(0x01<<y)))
#define PxyDIRz(x,y,z)        PxyDIRzTEMP(x,y,z)
#define PxyDIRzTEMP(x,y,z)    (z>0)?((P##x##DIR)|=(0x01<<y)) :((P##x##DIR) &=(~(0x01<<y)))
#define PxySELz(x,y,z)        PxySELzTEMP(x,y,z)
#define PxySELzTEMP(x,y,z)    (z>0)?((P##x##SEL)|=(0x01<<y)) :((P##x##SEL) &=(~(0x01<<y)))

#define PxyIFGz(x,y,z)        PxyIFGzTEMP(x,y,z)
#define PxyIFGzTEMP(x,y,z)    (z>0)?((P##x##IFG)|=(0x01<<y)) :((P##x##IFG) &=(~(0x01<<y)))
#define PxyIESz(x,y,z)        PxyIESzTEMP(x,y,z)
#define PxyIESzTEMP(x,y,z)    (z>0)?((P##x##IES)|=(0x01<<y)) :((P##x##IES) &=(~(0x01<<y)))
#define PxyIEz(x,y,z)         PxyIEzTEMP(x,y,z)
#define PxyIEzTEMP(x,y,z)     (z>0)?((P##x##IE)|=(0x01<<y)) :((P##x##IE) &=(~(0x01<<y)))

#define PxyINz(x,y)           PxyINzTEMP(x,y)
#define PxyINzTEMP(x,y)       (P##x##IN)&(0x01<<y)

#define PxyIFG(x,y)           PxyIFGTEMP(x,y)
#define PxyIFGTEMP(x,y)       (P##x##IFG)&(0x01<<y)

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
函数功能：软件延时(近似)
传递参数：空
返回值：空
*************************************/
void wait_5us(unsigned int i)
{
     int j=0;
     i =i/MSP430_DIVM;
     for( j=0; j<i; j++)
     {
         delay_us(4);
     }
}


/*************************************
函数功能：系统时钟控制
basic clock 设置，以高频晶振为MCLK/SMCLK时钟源。MCLK不分频，SMCLK8分频。
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
        delay_us(200);                 //延时等待8MHz晶体起振
 
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


/************************************
函数功能：打开总中断
传递参数：空
返回值：空
***************************************/
void interrupt_switch(enum msp430_switch a)
{
    if(a==on)
        _EINT();
    else
        _DINT();
}


/************************************
函数功能：寄存器转换
          返回制定的IO端口寄存器地址；
传递参数：
        port：端口号（1~6）；
        func：功能（0~4）；
          0:PxIN;
          1:PxOUT;
          2:PxDIR;
          3:PxSEL;
返回值：地址
特别注意：
返回值为地址，调试时本来使用unsigned int 型，仿真出现a word access on odd address 0x19弹窗，
问题出现在：有整型常量指定存储在flash奇地址上了。地址改成偶地址。
实际I/O地址即为奇数地址，尝试改为char型，可以通过。
***************************************/
unsigned char* ioreg_trans( unsigned char port, unsigned char func)
{
    unsigned char i;
    if(port==2||port==1)
        if(func==3)
            func+=0x03;
    func *=1u;
    switch(port)
    {
        case 1:i=0x0020u;i+=func;break;
        case 2:i=0x0028u;i+=func;break;
        case 3:i=0x0018u;i+=func;break;
        case 4:i=0x001Cu;i+=func;break;
        case 5:i=0x0030u;i+=func;break;
        case 6:i=0x0034u;i+=func;break;
    }
    return (unsigned char*)i;
}



#endif
