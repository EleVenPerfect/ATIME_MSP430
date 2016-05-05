/************************************
库功能：DS1307库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
        #include "atime_iic_software.h"
应用函数：
        ds1307_read(unsigned char a[])
        ds1307_write(unsigned char a[])
修改历史：
	‘修改人’   ‘修改内容’  ‘修改时间’
	  空          空	  空
最后修改时间：2016-02-05
作者： ATIME	版权所有
实例程序：
      #include <msp430x14x.h>
      #include "atime_msp430core.h"		//MSP430核心库
      #include "atime_lcd5110_printf.h"
      #include "atime_iic_software.h"
      #include "atime_ds1307.h"

      void main(void)
      {
          unsigned char i,j;
          watchdog_close();			//关闭看门狗
          wait_ms(50);
          basic_clock_init();			//系统时钟初始化
          lcd5110_init();                     //液晶屏幕初始化
          
          ds1307_write(ds1307_data);
          while(1)
          {
              ds1307_read(ds1307_data);
              j++;
              for( i=0; i<8; i++)
                  printf("  %02X   ",ds1307_data[i]);//以16进制显示
               
              printf_setadd(0,0);
              wait_ms(400);
          } 
      }
常见错误解释：
***************************************/

#ifndef _ATIME_MSP430_IIC_DS1307_C_ 
#define _ATIME_MSP430_IIC_DS1307_C_


/************************************
库全局变量组
***************************************/
unsigned char ds1307_data[8]={0x00,0x50,0x20,0x05,0x05,0x02,0x16,0x00};
//秒，分，时（24时制），星期，日，月，年，设置
//以BCD码存储，如：15秒，则写0x15，其他同理；
//当秒第七位为1时，时钟暂停，这一位清零后开始运行；
//当分位第六位为1时为12时制，12时值第五位为0AM/1PM区分，这一位清零为24时制；
//ds1307_data第7个数字表示ds1307输出方波设置，置零可以关闭.
/************************************
函数功能：IIC接口开始
传递参数：空
返回值：空
***************************************/
void ds1307_read(unsigned char a[])
{
    unsigned char i;
    iic_start_s();
    iic_writebyte_s(0xd0);
    iic_getack_s();
    iic_writebyte_s(0x00);
    iic_getack_s();
    iic_start_s();
    iic_writebyte_s(0xd1);
    iic_getack_s();
    for( i=0; i<8; i++)
    {
        a[i] =iic_readbyte_s();
        //delay_us(30);
        if(i==7)
            break;
        iic_setack_s();
        //delay_us(30);
    }
    iic_stop_s();
}


/************************************
函数功能：IIC接口写数据
传递参数：空
返回值：空
***************************************/
void ds1307_write(unsigned char a[])
{
    iic_start_s();
    //delay_us(30);
    iic_writebyte_s(0xd0);
    //delay_us(30);
    iic_getack_s();
    //delay_us(30);
    iic_writebyte_s(0x00);
    //delay_us(30);
    iic_getack_s();
    //delay_us(30);
    for(int i=0;i<8;i++)
    {
        iic_writebyte_s(a[i]);
        //delay_us(30);
        iic_getack_s();
        //delay_us(30);
    }
    iic_stop_s();
}

#endif