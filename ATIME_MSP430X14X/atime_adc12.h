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
#define ADC12_CLK_DIV   0x1             //ADC12时钟分频（1~8）
#define ADC12_CLK_SOU   0x2             //ADC12时钟选择（0ADC12OSC，1ACLK，2MCLK，3SMCLK）
#define ADC12_SHS_SOU   0x0             //采样保持时钟源（0ADC12SC，1Timer_AOUT1，2Timer_BOUT0,3Timer_BOUT1）


/************************************
函数功能：ADC12初始化
传递参数：
        chs:通道编码；
        repeat:重复转换；
返回值：
        0:正确；
        0xff：错误；
注：默认正参考电压3.3V，负参考电压0V。
***************************************/
unsigned char adc12_init( int chs, enum msp430_switch repeat)
{
    char low, high;
    unsigned int temp;
    int i, j;
    
    while((ADC12CTL1&0x01)==1);           //如果ADC忙，则等待
    
    ADC12CTL0 =0x00;

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
            ADC12CTL0 |=MSC;
        }//单通道重复采集
        else
        {
            ADC12CTL1 |=0x02+SHP;
            ADC12CTL0 |=MSC;
        }//多通道重复采集
    }
    else
    {
        ADC12CTL1 &=(~0x04);
        if((high-low)==0)
        {
            ADC12CTL1 &=(~0x02+SHP);
            ADC12CTL0 &=~MSC;
        }//单通道单次采集
        else
        {
            ADC12CTL1 |=0x02+SHP;
            ADC12CTL0 &=~MSC;
        }//多通道单次采集
    }    
    
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
    
    ADC12CTL0 |=ADC12ON;
    
    return (0);
}

/************************************
函数功能：开始AD12转换
传递参数：空
返回值：空
***************************************/
void adc12_start(void)
{
    ADC12CTL0 |=ENC;
    ADC12CTL0 |= ADC12SC;
    delay_us(20);
    ADC12CTL0 &= ~ADC12SC;
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