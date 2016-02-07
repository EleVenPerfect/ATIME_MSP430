/************************************
库功能：MSP430内部ADC库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
应用函数：
        adc12_init( int chs, enum msp430_switch repeat)
        adc12_vref( unsigned char num, unsigned char refp, unsigned char refn)
        adc12_read(unsigned char chs)
        adc12_start(void)
        adc12_stop(void)
        adc12_ieset( unsigned char i, enum msp430_switch a)
        adc12_temper( unsigned char refp)
修改历史：
        ‘修改人’   ‘修改内容’  ‘修改时间’
	空	    空		空
最后修改时间：2016-01-28
作者： ATIME	版权所有
实例程序：
1.重复模式例程（repeat=on）:
      #include <msp430x14x.h>
      #include "atime_msp430core.h"		//MSP430核心库
      #include "atime_lcd1602.h"
      #include "atime_adc12.h"
      #include "atime_interrupt.c"

      void main(void)
      {

          watchdog_close();			//关闭看门狗
          basic_clock_init();			//系统时钟初始化
          lcd1602_init( rightmove, cursornotdisplay);
          adc12_init(0x0f,on);                //初始化ADC12，0~3通道自动重复
          adc12_vref( 0, 2, 0);               //设置ADC12通道0转换参考电压为2.5V
          interrupt_switch(on);               //打开总中断
          adc12_start();                      //触发ADC12
          while(1)
          {
              adc12_start();                  //触发ADC12
              wait_ms(220);
              printint1602( result1, 0, 1);
              printint1602( result2, 1, 1);
              printint1602( result3, 0, 7);
              printint1602( result4, 1, 7);
          }
      }
      中断服务函数：
      unsigned int result1=123;
      unsigned int result2=456;
      unsigned int result3=789;
      unsigned int result4=910;
      #pragma vector=ADC_VECTOR
      __interrupt void ADC_12_ISR(void)
      {
          static int i=0;
          i++;
          if(i>31)                 //设置转换次数
          {
              adc12_stop();
              i=0;
          }
          result1 =adc12_read(0);     //读取对应通道数据
          result2 =adc12_read(1);
          result3 =ADC12MEM2;
          result4 =ADC12MEM3;
      }
2.非重复模式（repeat=off）:
      #include <msp430x14x.h>
      #include "atime_msp430core.h"		//MSP430核心库
      #include "atime_lcd1602.h"
      #include "atime_adc12.h"
      #include "atime_interrupt.c"

      void main(void)
      {

          watchdog_close();			//关闭看门狗
          basic_clock_init();			//系统时钟初始化
          lcd1602_init( rightmove, cursornotdisplay);
          adc12_init(0x0f,off);                //初始化ADC12，关闭自动重复
          adc12_vref( 0, 2, 0);               //设置ADC12通道0转换参考电压为2.5V
          interrupt_switch(on);               //打开总中断
          while(1)
          {
              adc12_start();                  //触发ADC12
              printint1602( result1, 0, 1);
              printint1602( result2, 1, 1);
              printint1602( result3, 0, 7);
              printint1602( result4, 1, 7);
              wait_ms(220);
          }
      }
      中断服务函数：
      unsigned int result1=123;
      unsigned int result2=456;
      unsigned int result3=789;
      unsigned int result4=910;
      #pragma vector=ADC_VECTOR
      __interrupt void ADC_12_ISR(void)
      {
          result1 =ADC12MEM0;     //读取对应通道数据
          result2 =ADC12MEM1;
          result3 =adc12_read(2);
          result4 =adc12_read(3);
      }
3.内部温度传感器采集
      #include <msp430x14x.h>
      #include "atime_msp430core.h"		//MSP430核心库
      #include "atime_lcd1602.h"
      #include "atime_adc12.h"

      int result1=1;
      void main(void)
      {
          watchdog_close();			//关闭看门狗
          basic_clock_init();			//系统时钟初始化
          lcd1602_init( rightmove, cursornotdisplay);
          adc12_init( 0x401, off);            //初始化ADC12
          adc12_vref( 10, 1, 0);              //设置ADC12通道10转换参考电压为1.5V
          adc12_ieset( 10, off);              //关闭通道10中断，采用查询方式读取
          while(1)
          {
              adc12_start();                  //触发ADC12
              printint1602(ADC12MEM10,1,1);   //显示实际读取结果
              result1 =adc12_temper(1);
              printint1602(result1,0,1);      //显示转换后结果
              wait_ms(400);
          }
      }

常见错误解释：
1.repeat模式开启后，ADC中断内部需要设置逻辑停止ADC转换。
因为ADC转换完成后自动开始新的转换，导致程序一直在中断服务函数中。
2.默认ADC12只打开最后一个通道的中断允许，因此进入中断后必须读取这一通道的结果，
读取某一通道结果硬件自动将该通道对应中断标志位复位。
3.采样通道：
|          0~7           |   8   |      9      |        10        |      11~15       |
|A0 A1 A2 A3 A4 A5 A6 A7 |VeREF+ |VREF./VeREF. |Temperature sensor| (AVCC – AVSS) / 2|
4.内部温度传感器如果不使用可以将ADC12_TEMPER置0以减小程序体积。
***************************************/

#ifndef _ATIME_MSP430_ADC12_C_ 
#define _ATIME_MSP430_ADC12_C_


/************************************
库全局变量组
***************************************/
#define ADC12_CLK_DIV   0x1             //ADC12时钟分频（1~8）
#define ADC12_CLK_SOU   0x0             //ADC12时钟选择（0ADC12OSC，1ACLK，2MCLK，3SMCLK）
#define ADC12_SHS_SOU   0x0             //采样保持时钟源（0ADC12SC，1Timer_AOUT1，2Timer_BOUT0,3Timer_BOUT1）
#define ADC12_TEMPER    1               //内部温度传感器开关（0OFF，1ON）

/************************************
函数功能：等待忙
传递参数：空
返回值：0：正常返回；0xff：超时返回
***************************************/
unsigned char adc12_wait( unsigned int j)
{
    unsigned int i=0;
    while((ADC12CTL1&0x01)==1)
    {
        i++;
        if(i>j)
            return 0xff;
    }
    return 0;
}


/************************************
函数功能：ADC12初始化
传递参数：
        chs:通道编码；
          *chs格式为int型数据，0x0000~0xffff，
          *将此值转换为二进制即为16位，对应16个通道。
          *软件自动扫描打开的起始通道和终止通道，
          *起始通道和终止通道中间的设置0或1均可。
        repeat:重复转换；
返回值：
        0:正确；
        0xff：通道错误；
        0xfe: 等待超时；
注：1.默认正参考电压3.3V，负参考电压0V;
    2.采样保持时间默认为1024个ADC12CLK;
    3.repeat模式开启后，ADC中断内部需要设置逻辑停止ADC转换。
    因为ADC转换完成后自动开始新的转换，导致程序一直在中断服务函数中。
***************************************/
unsigned char adc12_init( int chs, enum msp430_switch repeat)
{
    char low, high;
    unsigned int temp;
    int i, j;
    
    if(adc12_wait(1000))           //如果ADC忙，则等待
        return 0xfe;
    
    ADC12CTL0 =ADC12ON+MSC;

    j =0x0001;
    for( i=0; i<16; i++,j=j<<1 )
    {
        if(chs&j)
        {    
            low =i;
            break;
        }
    }
    j =0x8000;
    for( i=15; i>=0; i--,j=j>>1 )
    {
        if(chs&j)
        {    
            high =i;
            break;
        }
    }/////////扫描开启的通道
    for( i=low; i<=high; i++)
    {
        P6SEL |=(0x01<<i);
    }/////////通道接口设置

    if(repeat==on)
    {
        ADC12CTL1 |=0x04;
        if((high-low)==0)
        {
            ADC12CTL1 &=~(0x02);
            ADC12CTL1 |=SHP;
        }//单通道重复采集
        else
        {
            ADC12CTL1 |=0x02+SHP;
        }//多通道重复采集
    }
    else
    {
        ADC12CTL1 &=(~0x04);
        if((high-low)==0)
        {
            ADC12CTL1 &=(~0x02);
            ADC12CTL1 |=SHP;
        }//单通道单次采集
        else
        {
            ADC12CTL1 |=0x02+SHP;
        }//多通道单次采集
    }    
    
    temp =(unsigned int)low;
    ADC12CTL1 &=0xfff;
    ADC12CTL1 |=(temp<<12);//设置开始标志
    
    ADC12CTL1 |=( (unsigned int)(ADC12_CLK_DIV-0x1)<<5 );//ADC12分频设置
    ADC12CTL1 |=( (unsigned int)ADC12_CLK_SOU )<<3;      //ADC12频率源设置
    ADC12CTL1 |=( (unsigned int)ADC12_SHS_SOU )<<10;     //ADC12频率源设置
    
    ADC12CTL0 |=SHT0_15 + SHT1_15;/************************
    采样保持时间默认为1024个ADC12CLK，如需更改在此处添加代码。
    ******************************************************/
    
    for( i=0; i<16; i++)
    {
        *(char *)(ADC12MCTL0_ + i) |=i; 
    }                           //默认正参考VCC，负参考VSS
    if(low!=high)
    switch(high)
    {
        case 0 :ADC12MCTL0 |=0x80; break;
        case 1 :ADC12MCTL1 |=0x80; break;
        case 2 :ADC12MCTL2 |=0x80; break;
        case 3 :ADC12MCTL3 |=0x80; break;
        case 4 :ADC12MCTL4 |=0x80; break;    
        case 5 :ADC12MCTL5 |=0x80; break;    
        case 6 :ADC12MCTL6 |=0x80; break;    
        case 7 :ADC12MCTL7 |=0x80; break;
        case 8 :ADC12MCTL8 |=0x80; break;
        case 9 :ADC12MCTL9 |=0x80; break;
        case 10:ADC12MCTL10|=0x80; break;
        case 11:ADC12MCTL11|=0x80; break;
        case 12:ADC12MCTL12|=0x80; break;
        case 13:ADC12MCTL13|=0x80; break;
        case 14:ADC12MCTL14|=0x80; break;
        case 15:ADC12MCTL15|=0x80; break;
        default : return 0xff;
    }
    
    ADC12IE |=(0x01<<high);         //默认打开最后一个的中断
    
    return (0);
}


/************************************
函数功能：开始AD12转换
传递参数：空
返回值：空
***************************************/
void adc12_start(void)
{
    while((ADC12CTL1&0x01)==1);
    ADC12CTL0 |=ENC;
    ADC12CTL0 |=ADC12SC; 
}


/************************************
函数功能：参考电压设置
传递参数：
        num :通道选择
        refp:
            1:1.5V;
            2:2.5V;
            3:3.3V;
            4:外部正参考；
        refn:
            0：0V；
            1：外部负参考
返回值：空
注意：内部1.5V、2.5V参考电压不能同时使用。
***************************************/
void adc12_vref( unsigned char num, unsigned char refp, unsigned char refn)
{
    unsigned char i=0;
    
    ADC12CTL0 &=~ENC;           //关闭ENC后才可以进行设置
    if(refn==1)
        i |=0x40;
    if(refp==4)
        i |=0x20;
    if(refp==1||refp==2)
    {
        i |=0x10;
        ADC12CTL0 |=REFON;
        if(refp==2)
            ADC12CTL0 |=REF2_5V;
    }
    
    switch(num)
    {
        case 0 :ADC12MCTL0 |=i; break;
        case 1 :ADC12MCTL1 |=i; break;
        case 2 :ADC12MCTL2 |=i; break;
        case 3 :ADC12MCTL3 |=i; break;
        case 4 :ADC12MCTL4 |=i; break;
        case 5 :ADC12MCTL5 |=i; break;
        case 6 :ADC12MCTL6 |=i; break;
        case 7 :ADC12MCTL7 |=i; break;
        case 8 :ADC12MCTL8 |=i; break;
        case 9 :ADC12MCTL9 |=i; break;
        case 10:ADC12MCTL10|=i; break;
        case 11:ADC12MCTL11|=i; break;
        case 12:ADC12MCTL12|=i; break;
        case 13:ADC12MCTL13|=i; break;
        case 14:ADC12MCTL14|=i; break;
        case 15:ADC12MCTL15|=i; break; 
    }
}


/************************************
函数功能：停止ADC转换
传递参数：空
返回值：空
***************************************/
void adc12_stop(void)
{
    ADC12CTL0 &=~ENC;
    ADC12IFG &=0x00;
}


/************************************
函数功能：停止ADC转换
传递参数：
        i:通道；
        a：选择开关；
返回值：空
***************************************/
void adc12_ieset( unsigned char i, enum msp430_switch a)
{
    switch(a)
    {
        case on :ADC12IE |=0x01<<i; break;
        case off:ADC12IE &=~(0x01<<i); break;
    }
}


/************************************
函数功能：读取对应通道数据
传递参数：空
返回值：采样值
注：也可以直接读取对应的ADC12MEM0~ADC12MEM15
***************************************/
unsigned int adc12_read(unsigned char chs)
{
    return *(unsigned int*)(ADC12MEM0_+chs*2); 
}

#if ADC12_TEMPER==1
/************************************
函数功能：读取温度值
传递参数：参考值
返回值：温度值
***************************************/
int adc12_temper( unsigned char refp)
{
    int a;
    long kk;
    kk =ADC12MEM10;
    if(refp==1)
        kk =((kk-2692)*423);
    if(refp==2)
        kk =((kk-1617)*704);
    if(refp==3)
        kk =((kk-1224)*930)+18000;//校准温度值
    kk =kk/4096;
    a =kk%10+(kk/10%10)*10;
    if(kk<0)
        a =a*(-1);
    return a;
}

#endif


#endif