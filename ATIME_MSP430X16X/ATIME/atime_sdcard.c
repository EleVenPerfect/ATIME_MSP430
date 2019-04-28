
//尚未测试，移植自ATIME51单片机库。
//尚未测试，移植自ATIME51单片机库。
//尚未测试，移植自ATIME51单片机库。


/************************************
库功能：SPI驱动sdcard库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"		//MSP430核心库
        #include "atime_spi_s.h"
        #include "atime_msp430core.h"
应用函数：
修改历史：
		   	 ‘修改人’   ‘修改内容’  ‘修改时间’
				空			空			空
最后修改时间：尚未测试，移植自ATIME51单片机库。
作者： ATIME	版权所有
实例程序：
常见错误解释：
此程序只适用于SD卡而不是SDHC卡，容量小于2G，尚未测试。
***************************************/

#ifndef _ATIME_MSP430_SPI_SDCARD_C_ 
#define _ATIME_MSP430_SPI_SDCARD_C_


/************************************
库全局变量组
***************************************/
struct spi_s sdcard={4,0,4,1,4,2,4,3,3,1};
//分别定义：CS、CLK、SDI、SDO的端口号、端口位

/************************************
函数功能：发送命令
传递参数：空
返回值：应答
***************************************/
unsigned char sd_write_zhi(unsigned char a[])
{
	unsigned char i,temp,*add;
	
        add =ioreg_trans(sdcard.clk_p,1); //CLK  线PxOUT1 
        if((sdcard.mode&0x01)==0x00)//CPOL=0，则平时为低电平
            *add &=~(0x01<<sdcard.clk_b);
        else
            *add |=(0x01<<sdcard.clk_b);
        for(i=0;i<8;i++)
        {
            wait_5us(sdcard.wait);
            spi_s_clk(sdcard);
            wait_5us(sdcard.wait);
            spi_s_clk(sdcard);
        }
	
	for(i=0; i<6; i++)
		spi_s_write(sdcard,a[i]);
	i =0;
	do
	{
	 	temp =spi_s_read(sdcard);
		i++;
	}
	while((temp==0xff)&&(i<100));
	return (temp);
}


/************************************
函数功能：复位SD卡
传递参数：空
返回值：0:成功;1:失败
***************************************/
unsigned char sd_res(void)
{
	unsigned char temp ,i;
	unsigned char zhiling[] ={0x40, 0x00, 0x00, 0x00, 0x00, 0x95};
	
	for(i=0; i<0x0f; i++)
	{
		spi_s_write(sdcard,0xff);
	}
	
	i =0;
	do
	{
		temp =sd_write_zhi(zhiling);
		i++;
		if(i >=100)
		{
			
			return (0x01);
		}
	}
	while(temp!=0x01);
	
	spi_s_write(sdcard,0xff);
	return (0);
}								


/************************************
函数功能：初始化SD卡
传递参数：空
返回值：0:成功;1:失败
**************************************/
unsigned char sd_init(void)
{
	unsigned char i,temp;
	unsigned char zhiling[] ={0x41, 0x00, 0x00, 0x00, 0x00, 0xff};
	
	for(i=0; i<=9; i++)
		spi_s_write(sdcard,0xff);
	
	i =0;
	do
	{
		temp =sd_write_zhi(zhiling);
		i++;
		if(i >=250)
		{
			
			return (0x02);
		}
	}
	while(temp !=0X00);
	
	spi_s_write(sdcard,0xff);
	return (0);
}			   			 ////MMC卡版本
/*
unsigned char sd_init(void)
{
	unsigned char i,temp;
	unsigned char zhiling[] ={0x77, 0x00, 0x00, 0x00, 0x00, 0xff};
	
	i =0;
	do
	{
		temp =sd_write_zhi(zhiling);
		i++;
		if(i >=300)
		{
			
			return (0x02);
		}
	}
	while(temp !=0X01);
	
	spi_s_write(sdcard,0xff);

	zhiling[0] =0x69;
	
	i =0;
	do
	{
		temp =sd_write_zhi(zhiling);
		i++;
		if(i >=300)
		{
			
			return (0x03);		 ///
		}
	}
	while(temp !=0X00);
	
	spi_s_write(sdcard,0xff);
	return (0);				
}						*///不可用版本

/************************************
函数功能：向SD卡某地址写入512字节数据
传递参数：add:地址   a[]：待写入数据
返回值：0:成功;1:失败
***************************************/
unsigned char sd_write512(unsigned long add, unsigned char a[])
{
	unsigned char temp;
	unsigned int i;
	unsigned char zhiling[] ={0x58, 0x00, 0x00, 0x00, 0x00, 0xff}; 
	sd_init();		   //有时可能会需要每次读写都进行初始化
	add <<=9;		   //addr = addr * 512	将块地址（扇区地址）转为字节地址 ［这里就限制了SD卡的最大容量为4G］
	zhiling[1]=((add&0xff000000)>>24); 	
 	zhiling[2]=((add&0x00ff0000)>>16);
 	zhiling[3]=((add&0x0000ff00)>>8);
	
	i =0;
	do
	{
		temp =sd_write_zhi(zhiling);
		i++;
		if(i ==100)
		{
			
			return (temp);
		}
	}
	while(temp !=0);
	for(i=0; i<100; i++)
		spi_s_read(sdcard);			//原来是读，现在改成了写
	spi_s_write(sdcard,0xfe);
	for(i=0; i<512; i++)
		spi_s_write(sdcard,a[i]);
	spi_s_write(sdcard,0xff);
	spi_s_write(sdcard,0xff);
	temp =spi_s_read(sdcard);
	if(temp&0x1f !=0x05)	
	{
		
		return (0x03);
	}
	while(spi_s_read(sdcard)!=0xff);
	
	spi_s_write(sdcard,0xff);
	return (0);
}

/************************************
函数功能：向SD卡某地址读取512字节数据
传递参数：add:地址   a[]：待写入数据
返回值：0:成功;1:失败
***************************************/
unsigned char sd_read512(unsigned long add, unsigned char a[])
{
	unsigned int i;
	unsigned char temp;
	unsigned char zhiling[] ={0x51, 0x00, 0x00, 0x00, 0x00, 0xff};
	sd_init(); 				 //有时可能会需要每次读写都进行初始化
	add <<=9;
	zhiling[1]=((add&0xff000000)>>24); 	
 	zhiling[2]=((add&0x00ff0000)>>16);
 	zhiling[3]=((add&0x0000ff00)>>8);
	
	i =0;
	do
	{
		temp =sd_write_zhi(zhiling);
		i++;
		if(i ==100)
		{
			return (0x04);
		}
	}
	while(temp!=0);
	while(spi_s_read(sdcard)!=0xfe);
	for(i=0; i<512; i++)
		a[i] =spi_s_read(sdcard);
	spi_s_read(sdcard);
	spi_s_read(sdcard);
	
	spi_s_write(sdcard,0xff);
	return (0);
}


#endif