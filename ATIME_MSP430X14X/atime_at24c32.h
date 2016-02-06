/************************************
库功能：AT24C32库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
        #include "atime_iic_software.h"
应用函数：
        at24c32_writebyte( unsigned int addr, unsigned char data)
        at24c32_readbyte( unsigned int addr)
        at24c32_writepage( unsigned int addr,unsigned int n, unsigned char rdata[])
        at24c32_readpage( unsigned int addr, unsigned int n, unsigned char rdata[])
修改历史：
	‘修改人’   ‘修改内容’  ‘修改时间’
	  空	      空	  空
最后修改时间：2016-02-06
作者： ATIME	版权所有
实例程序：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"		//MSP430核心库
        #include "atime_lcd5110_printf.h"
        #include "atime_iic_software.h"
        #include "atime_at24c32.h"

        void main(void)
        {
            unsigned char i,temp;
            watchdog_close();			//关闭看门狗
            wait_ms(50);
            basic_clock_init();			//系统时钟初始化
            lcd5110_init();                     //液晶屏幕初始化
            
            for(i=0;i<32;i++)
                at24c32_buf[i]=i;

            at24c32_writepage( 0, 32, at24c32_buf);
            
            for(i=0;i<32;i++)
                at24c32_buf[i]=0;
            
            at24c32_readpage( 0, 32, at24c32_buf);
            i =0;
            while(1)
            {
                temp =at24c32_buf[i];
                i++;
                if(i>31)
                    i=0;
                printf("%02X",temp);//以16进制显示
                wait_ms(200);
                lcd5110_clear_all();
            }
        }
常见错误解释：
***************************************/

#ifndef _ATIME_AT24C32_H_ 
#define _ATIME_AT24C32_H_


/************************************
库全局变量组
***************************************/
#define AT24C32_ADDR            0xA0            //定义器件地址
unsigned char at24c32_buf[32]={0};              //读取一次最多连续读32个

/************************************
函数功能：写32字节数据到地址
传递参数：addr：地址；data：待发送数组
返回值：空
***************************************/
void at24c32_writebyte( unsigned int addr, unsigned char data)
{
    delay_ms(1);
    iic_start_s();
    delay_us(30);
    iic_writebyte_s(AT24C32_ADDR&0xfe);  
    delay_us(30);
    iic_getack_s();
    delay_us(30);
    iic_writebyte_s(addr/256);  
    delay_us(30);
    iic_getack_s();
    delay_us(30);
    iic_writebyte_s(addr%256);  
    delay_us(30);
    iic_getack_s();
    delay_us(30);
    
    iic_writebyte_s(data);  
    delay_us(30);
    iic_getack_s();
    delay_us(30);
    iic_stop_s();
}


/************************************
函数功能：读32字节数据到地址
传递参数：addr：地址
返回值：读取的数据
***************************************/
unsigned char at24c32_readbyte( unsigned int addr)
{
    unsigned char wdata[3],rdata[1];
    delay_ms(1);
    wdata[0] =(AT24C32_ADDR&0xfe);
    wdata[1] =addr/256;
    wdata[2] =addr%256;
    iic_read(wdata,3,rdata,1);
    return rdata[0];
    /*
    iic_start_s();
    delay_us(100);
    for( i=0; i<3; i++)
    {
        iic_writebyte_s(wdata[i]); 
        delay_us(100);
        iic_getack_s();
        delay_us(100);
    }
    iic_start_s();
    delay_us(100);
    wdata[0] |=0x01;
    iic_writebyte_s(wdata[0]);
    delay_us(100);
    iic_getack_s();	
    delay_us(100);
    for( i=0; i<32; i++)
    {
        data[i] =iic_readbyte_s();
        delay_us(100);
        iic_setack_s();
        delay_us(100);
    }
    data[0] =iic_readbyte_s();
    delay_us(100);
    iic_stop_s();
    */
}


/************************************
函数功能：写32字节数据到地址
传递参数：
      addr ：地址；
      n    ：要写的字符个数；
      rdata：待写数据数组；
返回值：空
***************************************/
void at24c32_writepage( unsigned int addr,unsigned int n, unsigned char rdata[])
{
    unsigned char i;
    delay_ms(3);
    iic_start_s();
    delay_us(30);
    iic_writebyte_s(AT24C32_ADDR&0xfe);  
    delay_us(30);
    iic_getack_s();
    delay_us(30);
    iic_writebyte_s(addr/256);  
    delay_us(30);
    iic_getack_s();
    delay_us(30);
    iic_writebyte_s(addr%256);  
    delay_us(30);
    iic_getack_s();
    delay_us(30);
    for( i=0; i<n; i++)
    {
        iic_writebyte_s(rdata[i]);  
        delay_us(30);
        iic_getack_s();
        delay_us(30);
    }
    iic_stop_s();
}


/************************************
函数功能：读32字节数据到地址
传递参数：addr：地址
返回值：读取的数据
注：读取一次最多连续读32个
***************************************/
void at24c32_readpage( unsigned int addr, unsigned int n, unsigned char rdata[])
{
    unsigned char wdata[3];
    delay_ms(1);
    wdata[0] =(AT24C32_ADDR&0xfe);
    wdata[1] =addr/256;
    wdata[2] =addr%256;
    iic_read(wdata,3,rdata,n);
}
    
    
#endif