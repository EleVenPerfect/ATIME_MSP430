/************************************
库功能：软件SPI库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
应用函数：
修改历史：
		   	 ‘修改人’   ‘修改内容’  ‘修改时间’
				空			空			空
最后修改时间：2016-01-29
作者： ATIME	版权所有
实例程序：
常见错误解释：
***************************************/


#ifndef _ATIME_MSP430_SPI_SOFTWARE_H_ 
#define _ATIME_MSP430_SPI_SOFTWARE_H_


/************************************
库全局变量组
***************************************/
struct spi_s
{
    unsigned char cs_p;         //设备使能信号，port
    unsigned char cs_b;         //设备使能信号，bit
    unsigned char clk_p;        //时钟信号，port
    unsigned char clk_b;        //时钟信号，bit
    unsigned char sdi_p;        //主设备输入，port
    unsigned char sdi_b;        //主设备输入，bit
    unsigned char sdo_p;        //从设备输出，port
    unsigned char sdo_b;        //从设备输出，bit
    unsigned char mode;         //设置工作模式（0~3）
};                               //定义SPI总线信号线数据传输结构

/************************************
工作模式说明：
模式0：CPHA=0;CPOL=0;
模式1：CPHA=0;CPOL=1;
模式2：CPH1=1;CPOL=0;
模式3：CPHA=1;CPOL=0;
CPHA:时钟相位设置；
     0表示每周期第一个时钟沿采样，
     1表示每周期第二个时钟沿采样。
CPOL:时钟信号设置；
     0表示闲时为0，
     1表示闲时为1.
***************************************/

 /**************************************
函数功能：端口初始化
传递参数：struct spi_s a：端口信息
返回值：空
***************************************/
void spi_s_init(struct spi_s a)
{
    unsigned char *add;
    
    add =ioreg_trans(a.cs_p,2);
    *add |=(0x01<<a.cs_b);//设置PxDIR
    add =ioreg_trans(a.cs_p,3);
    *add &=~(0x01<<a.cs_b);//设置PxSEL
    
    add =ioreg_trans(a.clk_p,2);
    *add |=(0x01<<a.clk_b);//设置PxDIR
    add =ioreg_trans(a.clk_p,3);
    *add &=~(0x01<<a.clk_b);//设置PxSEL
    
    add =ioreg_trans(a.sdo_p,2);
    *add |=(0x01<<a.sdo_b);//设置PxDIR
    add =ioreg_trans(a.sdo_p,3);
    *add &=~(0x01<<a.sdo_b);//设置PxSEL
    
    add =ioreg_trans(a.sdi_p,2);
    *add &=~(0x01<<a.sdi_b);//设置PxDIR
    add =ioreg_trans(a.sdi_p,3);
    *add &=~(0x01<<a.sdi_b);//设置PxSEL
}


 /**************************************
函数功能：高低位互换
传递参数：待转换字节
返回值：结果
***************************************/
unsigned char transbit( unsigned char x)
{
	unsigned char i;
	i =(x & 0x7e) | ( ( (x&0x80) >> 7 ) | ( (x&0x01) << 7 ) );
	x =i;
	i =(x & 0xbd) | ( ( (x&0x40) >> 5 ) | ( (x&0x02) << 5 ) );
	x =i;
	i =(x & 0xdb) | ( ( (x&0x20) >> 3 ) | ( (x&0x04) << 3 ) );
	x =i;		
	i =(x & 0xe7) | ( ( (x&0x10) >> 1 ) | ( (x&0x08) << 1 ) );
	x =i;  
	return x;		
}


/************************************
函数功能：SPI写数据0
传递参数：空
返回值：空
***************************************/
unsigned char spi_s_write0( struct spi_s a, unsigned char data)
{
    unsigned char *add,j,i,temp;
    
    add =ioreg_trans(a.cs_p,1); //CS  线PxOUT 
    *add |=(0x01<<a.cs_b);
    *add &=~(0x01<<a.cs_b);
    
    add =ioreg_trans(a.clk_p,1); //CLK  线PxOUT1 
    *add |=(0x01<<a.clk_b);
    
    i =0;
    for( j=0; j<8; j++)
    {
        add =ioreg_trans(a.sdo_p,1);//SDO 线PxOUT
        if(data&0x80)
            *add |=(0x01<<a.sdo_b);//SDO线上数据
        
        add =ioreg_trans(a.clk_p,1); //CLK  线PxOUT0 
        *add &=~(0x01<<a.clk_b);//CLK线下降沿
        
        add =ioreg_trans(a.sdi_p,0); //SDI  线PxIN0 
        temp =*add;//读取 SDI 线数据
        i <<=1;
        i |=((temp>>a.sdi_b)&0x01);
        
        
        add =ioreg_trans(a.clk_p,1); //CLK  线PxOUT0 
        *add |=(0x01<<a.clk_b);//CLK线上升沿
        
        data <<=1;
    }
    
    add =ioreg_trans(a.cs_p,1); //CS  线PxOUT 
    *add |=(0x01<<a.cs_b);
    return (i);
}
/************************************
函数功能：SPI写数据1
传递参数：空
返回值：空
***************************************/
unsigned char spi_s_write1( struct spi_s a, unsigned char data)
{
    unsigned char i;
    

    return (i);
}
/************************************
函数功能：SPI写数据2
传递参数：空
返回值：空
***************************************/
unsigned char spi_s_write2( struct spi_s a, unsigned char data)
{
    unsigned char i;
    

    return (i);
}
/************************************
函数功能：SPI写数据3
传递参数：空
返回值：空
***************************************/
unsigned char spi_s_write3( struct spi_s a, unsigned char data)
{
    unsigned char i;
    

    return (i);
}
/************************************
函数功能：SPI写数据
传递参数：空
返回值：空
***************************************/
unsigned char spi_s_write( struct spi_s a, unsigned char data)
{
    unsigned char i;
    
    switch(a.mode)
    {
        case 0 : i=spi_s_write0( a, data);break;
        case 1 : i=spi_s_write1( a, data);break;
        case 2 : i=spi_s_write2( a, data);break;
        case 3 : i=spi_s_write3( a, data);break;
        default:i=0xff;
    }
    return (i);
}



#endif