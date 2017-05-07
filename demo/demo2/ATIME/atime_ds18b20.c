/*************************************
库功能：DS18B20驱动库
库要求：程序已经载入：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
应用函数：
        ds18b20_init(void)
        ds18b20_read(void)
        ds18b20_translate( float i, unsigned char r[8])
修改历史：
         ‘修改人’   ‘修改内容’  ‘修改时间’
	    空		  空		 空
最后修改时间：2016-01-25
作者： ATIME	版权所有
实例程序：
      #include <msp430x14x.h>
      #include "atime_msp430core.h"		//MSP430核心库
      #include "atime_lcd1602.h"
      #include "atime_ds18b20.h"

      void main(void)
      {
          float i;
          unsigned char j;
          unsigned char a[8];

          watchdog_close();			//关闭看门狗
          basic_clock_init();			//系统时钟初始化
          lcd1602_init( rightmove, cursornotdisplay);
          
          //P1DIR |= BIT2;//器件的vcc端可以接I/O供电
          //P1OUT |= BIT2;//器件的vcc端可以接I/O供电
          
          while(1)
          {
              j =ds18b20_init();
              if(j ==0xff)
              {
                  i =ds18b20_read();
                  ds18b20_translate( i, a);
                  print1602( a, 0, 3);
                  printchar1602( 'C', 0, 10);
                  wait_ms(100);
              }
              else
              {
                  print1602("Please Cheak",0,2);
                  wait_ms(750);
                  set1602(clear1602);
                  wait_ms(700);
              }
          }
      }

常见错误解释：
*************************************/

#ifndef _ATIME_MSP430_DS18B20_C_ 
#define _ATIME_MSP430_DS18B20_C_


/*************************************
库全局变量组
*************************************/
#define DS18B20_PORT    1       //定义通讯接口的端口组
#define DS18B20_BIT     0       //定义通讯接口的端口位


/************************************
函数功能：初始化ds18b20
传递参数：空
返回值：
    0   ：成功
    0xff：错误
***************************************/
unsigned char ds18b20_init(void)
{
    unsigned char i;
    PxyDIRz(DS18B20_PORT,DS18B20_BIT,1);
    PxyOUTz(DS18B20_PORT,DS18B20_BIT,0);
    delay_us(500);				//延时500uS(480-960)
    PxyOUTz(DS18B20_PORT,DS18B20_BIT,1);
    PxyDIRz(DS18B20_PORT,DS18B20_BIT,0);
    delay_us(80);				//延时80uS
    i =PxyINz(DS18B20_PORT,DS18B20_BIT);
    delay_us(500);				//延时500uS(保持>480uS)
          
    if(i) 
        return 0x00;
    else 
        return 0xff;
}


/************************************
函数功能：读1字节
传递参数：空
返回值：都去的数据
***************************************/
unsigned char ds18b20_read_byte(void) 
{
    unsigned char i;
    unsigned char value = 0;
    for( i=8; i>=1; i--) 
    {
	value >>=1;
	PxyDIRz(DS18B20_PORT,DS18B20_BIT,1);
	PxyOUTz(DS18B20_PORT,DS18B20_BIT,0);
	delay_us(4);			        //*延时4uS	
	PxyOUTz(DS18B20_PORT,DS18B20_BIT,1);
	PxyDIRz(DS18B20_PORT,DS18B20_BIT,0);
	delay_us(10);			        //*延时10uS	
	if(PxyINz(DS18B20_PORT,DS18B20_BIT)) 
	{
            value |=0x80;		
	}
        delay_us(60);			        //*延时60uS	
    }
    return (value);
}


/************************************
函数功能：写一字节数据
传递参数：要写的数据
返回值：空
***************************************/
void ds18b20_write_byte(unsigned char value) 
{
  unsigned char i;
  for( i=8; i!=0; i--) 
  {
    PxyDIRz(DS18B20_PORT,DS18B20_BIT,1);
    PxyOUTz(DS18B20_PORT,DS18B20_BIT,0);
    delay_us(4);			  //延时4uS
    if(value&0x01) 
    {
        PxyOUTz(DS18B20_PORT,DS18B20_BIT,1);		
    }
    delay_us(80);			  //延时80uS
    PxyOUTz(DS18B20_PORT,DS18B20_BIT,1);  //位结束
    value >>=1;	
  }
}


/************************************
函数功能：DS18B20读取温度
传递参数：空
返回值：温度值
***************************************/
float ds18b20_read(void) 
{
    float temper;
    int i;
    unsigned char buf[9];
	
    ds18b20_init();
    ds18b20_write_byte(0xCC);	          //勿略地址
    ds18b20_write_byte(0x44);	          //启动转换
    delay_us(80);
	
    ds18b20_init();
    ds18b20_write_byte(0xCC);	          //勿略地址
    ds18b20_write_byte(0xBE);	          //读取温度
    for( i=0; i<9; i++) 
    {
        buf[i] =ds18b20_read_byte();	
    }
    
    if(buf[1] >248)
    {
	buf[1] =~buf[1];
	buf[0] =~buf[0];
	temper =(-0.0625)*(buf[0]+1+256.0*buf[1]);
    }
    else
    temper =0.0625*(buf[0]+256.0*buf[1]);

    return (temper);
}


/************************************
函数功能：优化显示数字
		  本函数可以去除数字前面无意义的零。
传递参数：
	a[8]:转换后的数组存储位置
返回值：空
注：没有通过返回值返回结果，
    而是利用函数调用数组是用原内存空间这一规则。
***************************************/
void ds18b20_zero_better(unsigned char a[8])
{
    int i;
    i =100*(a[1]-48)+10*(a[2]-48)+(a[3]-48);   //减48：char型转int型计算
    if(i>=100);
    else
    if(i>=10)
    {
        a[1] =a[0];
        a[0] =' ';	
    }
    else
    if(i>=0)
    {
        a[2] =a[0];
        a[1] =' ';
        a[0] =' ';
    }
}


/************************************
函数功能：浮点转字符数组
传递参数：
		i   :待转换浮点数
		r[8]:转换后的数组存储位置
返回值：空
注：没有通过返回值返回结果，
    而是利用函数调用数组是用原内存空间这一规则。
***************************************/
void ds18b20_translate( float i, unsigned char r[8])
{
    unsigned int j;
    if(i >0.0)
        r[0] =' ';
    else 
    {
        r[0] ='-';
        i =(-1)*i;
    }
    i =i*100;
    j =(int)i;
    r[1] =j/10000+48;
    r[2] =(j%10000)/1000+48;
    r[3] =(j%1000)/100+48;
    r[4] ='.';
    r[5] =(j%100)/10+48;
    r[6] =j%10+48;
    r[7] =0x00;	

    ds18b20_zero_better(r);
}



#endif
