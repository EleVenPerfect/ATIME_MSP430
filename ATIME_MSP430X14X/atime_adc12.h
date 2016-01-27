/************************************
库功能：MSP430内部ADC库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
应用函数：
修改历史：
		   	 ‘修改人’   ‘修改内容’  ‘修改时间’
				空			空			空
最后修改时间：2014-01-29
作者： ATIME	版权所有
实例程序：
常见错误解释：
***************************************/

#ifndef _ATIME_MSP430_ADC12_H_ 
#define _ATIME_MSP430_ADC12_H_


/************************************
库全局变量组
***************************************/
#define ADC12_REF_P     33              //正参考电压（1:1.5，2:2.5，3:3.3，4:Vref+）
#define ADC12_REF_N     0               //负参考电压（0Vss，1Vref/Veref）
#define ADC12_CLK_DIV   0x1             //ADC12时钟分频（1~8）
#define ADC12_CLK_SOU   0x0             //ADC12时钟选择（0ADC12OSC，1ACLK，2MCLK，3SMCLK）
#define ADC12_SHS_SOU   0x0             //采样保持时钟源（0ADC12SC，1Timer_AOUT1，2Timer_BOUT0,3Timer_BOUT1）


/************************************
函数功能：ADC12初始化
传递参数：
        chs:通道编码；
        repeat:重复转换；
返回值：
0:正确；
0xff：错误；
***************************************/
unsigned char adc12_init( int chs, enum msp430_switch repeat)
{
    char low, high;
    unsigned int temp;
    int i, j;
    
    while((ADC12CTL1&0x01)==1);           //如果ADC忙，则等待
    
    ADC12CTL0 =0x00;
    
#if ADC12_REF_P !=3||ADC12_REF_P !=4 
    ADC12CTL0 |=REFON;
#endif
#if ADC12_REF_P ==2
    ADC12CTL0 |=REF2_5V;
#endif   
    
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
    
    
    
    if((high-low)==0)
        ADC12CTL1 &=(~0x02);
    else
        ADC12CTL1 |=0x02;
    if(repeat==on)
        ADC12CTL1 |=0x04;
    else
        ADC12CTL1 &=(~0x04);
    ///////////转换模式设置
    ADC12CTL0 |=MSC;
    ADC12CTL1 |=0x0200;//SHP  
    if(((high-low)==0)&&(repeat==off))
    {
        ADC12CTL0 &=(~MSC);  
        ADC12CTL1 &=(~0x0200);
    }
    //////////采样模式扫描和设置
    
    
    
    
    temp =(unsigned int)low;
    ADC12CTL1 &=0xfff;
    ADC12CTL1 |=(temp<<12);//设置开始标志
    
    ADC12CTL1 |=( (unsigned int)(ADC12_CLK_DIV-0x1)<<5 );//ADC12分频设置
    ADC12CTL1 |=( (unsigned int)ADC12_CLK_SOU )<<3;      //ADC12频率源设置
    ADC12CTL1 |=( (unsigned int)ADC12_SHS_SOU )<<10;     //ADC12频率源设置
    
    ADC12CTL0 |=SHT0_2 + SHT1_2;/************************
    采样保持时间默认为8个ADC12CLK，如需更改在此处添加代码。
    ******************************************************/
  
    for( i=0; i<16; i++)
    {
        j =i;
        
        #if ADC12_REF_N==1
          j |=0x40;
        #endif
        #if ADC12_REF_P==2||ADC12_REF_P==1
          j |=0x10;
        #endif

        *(char *)(ADC12MCTL0_ + i) |=j; 
    }
    //if(high-low!=0)
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
    
    ADC12CTL0 |=ADC12ON+ENC;
    
    return (0);
}

/************************************
函数功能：开始AD12转换
传递参数：空
返回值：空
***************************************/
void adc12_start(void)
{
    ADC12CTL0 |= ENC;
    ADC12CTL0 |= ADC12SC;
    delay_us(20);
    ADC12CTL0 &= ~ADC12SC;
}


/************************************
函数功能：
传递参数：空
返回值：
***************************************/
/************************************
函数功能：
传递参数：空
返回值：
***************************************/
/************************************
函数功能：
传递参数：空
返回值：
***************************************/
/************************************
函数功能：
传递参数：空
返回值：
***************************************/
/************************************
函数功能：
传递参数：空
返回值：
***************************************/
/************************************
函数功能：
传递参数：空
返回值：
***************************************/

#endif