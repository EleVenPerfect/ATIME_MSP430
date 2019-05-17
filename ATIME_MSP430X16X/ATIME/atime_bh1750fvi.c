/************************************
库功能：bh1750fvi库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
应用函数：
      void bh1750_init();
      void bh1750_read();
修改历史：
	‘修改人’   ‘修改内容’  ‘修改时间’
	  空          空	  空
最后修改时间：2016-05-17
作者： ATIME	版权所有
实例程序：
#include <msp430x14x.h>
#include "atime_msp430core.h"		//MSP430核心库
#include "atime_bh1750fvi.h"
#include "atime_lcd5110_printf.h"

void main(void)
{
    unsigned int gq;
    watchdog_close();			//关闭看门狗
    basic_clock_init();			//系统时钟初始化
    lcd5110_init();                     //液晶屏幕初始化
    bh1750_init();                      //BH1750端口初始化

    while(1)
    {
        gq = bh1750_read();
        printf("%d \n",gq);
        wait_ms(1000);
    }
}

常见问题解释：
***************************************/

#ifndef _ATIME_MSP430_BH1750FVI_C_ 
#define _ATIME_MSP430_BH1750FVI_C_


/************************************
库全局变量组
***************************************/
#define BH1750FVI_SDA_PORT     6       //定义通讯接口的端口组
#define BH1750FVI_SDA_BIT      6       //定义通讯接口的端口位
#define BH1750FVI_SCL_PORT     6       //定义通讯接口的端口组
#define BH1750FVI_SCL_BIT      7       //定义通讯接口的端口位

#define SlaveAddress 0x46//0x23//定义地址0x46为ADDR=0,0x23为ADDR=1
unsigned char BH1750FVI_BUF[4];

/************************************
无需修改部分
***************************************/
#define SCL_H PxyOUTz(BH1750FVI_SCL_PORT,BH1750FVI_SCL_BIT,1)//P6OUT |= BIT7//此IO在硬件上需要更改
#define SCL_L PxyOUTz(BH1750FVI_SCL_PORT,BH1750FVI_SCL_BIT,0)//P6OUT &= ~BIT7
#define SDA_H PxyOUTz(BH1750FVI_SDA_PORT,BH1750FVI_SDA_BIT,1)//SDA_H P6OUT |= BIT6
#define SDA_L PxyOUTz(BH1750FVI_SDA_PORT,BH1750FVI_SDA_BIT,0)//P6OUT &= ~BIT6

#define SDA_IN  PxyDIRz(BH1750FVI_SDA_PORT,BH1750FVI_SDA_BIT,0)//P6DIR &= ~BIT6   //SDA改成输入模式
#define SDA_OUT PxyDIRz(BH1750FVI_SDA_PORT,BH1750FVI_SDA_BIT,1)//P6DIR |= BIT6    //SDA变回输出模式
#define SDA_VAL PxyINz(BH1750FVI_SDA_PORT,BH1750FVI_SDA_BIT)//P6IN&BIT6        //SDA的位值

/*******************************
函数功能：延时5微秒
传递参数：空
返回值：空
********************************/
void Delay5us(void)
{
   unsigned char i;
   
    for(i = 0;i < 15;i++);
        _NOP();
}
/*******************************
函数功能：延时5毫秒
传递参数：空
返回值：空
********************************/
void Delay5ms(void)
{
   unsigned int i=40000;
   while (i != 0)
   {
       i--;
   }
}

/*******************************
函数功能：延时
传递参数：空
返回值：空
********************************/
void delay(unsigned int x)
{
    unsigned char i;
    while(x--)
        for(i=0;i<125;i++);
}

/*******************************
函数功能：IIC起始信号
传递参数：空
返回值：空
********************************/
void BH1750_Start()
{
    SDA_H;                    //拉高数据线
    SCL_H;                    //拉高时钟线
    Delay5us();                 //延时
    SDA_L;                    //产生下降沿
    Delay5us();                 //延时
    SCL_L;                    //拉低时钟线
}

/*******************************
函数功能：IIC停止信号
传递参数：空
返回值：空
********************************/
void BH1750_Stop()
{
    SDA_L;                    //拉低数据线
    SCL_H;                    //拉高时钟线
    Delay5us();                 //延时
    SDA_H;                    //产生上升沿
    Delay5us();                 //延时
}

/*******************************
函数功能：发送应答
传递参数：空
返回值：空
********************************/
void BH1750_SendACK(void)
{
    SDA_L;                  //写应答信号
    _NOP();_NOP();
    SCL_H;                    //拉高时钟线
    Delay5us();                  //延时
    SCL_L;                    //拉低时钟线
    _NOP();_NOP();
    SDA_H;
    Delay5us();                 //延时
}

/*******************************
函数功能：无应答
传递参数：空
返回值：空
********************************/
void mnack(void)
{
    SDA_H;
    _NOP(); _NOP();
    SCL_H;
    Delay5us();
    SCL_L;
    _NOP(); _NOP();
    SDA_L;  
    Delay5us();      
}

/************************************
函数功能：发送应答信号
传递参数：空
返回值：ack (0:ACK 1:NAK)
**************************************/
unsigned char BH1750_RecvACK()
{
    unsigned char ack;
    SDA_H;
    _NOP(); _NOP();
    SCL_H;
    _NOP(); _NOP();
    SDA_IN;
    _NOP(); _NOP();
    ack = SDA_VAL;   //读入SDA数值
    SCL_L;
    Delay5us();
    SDA_OUT;
    if(ack)    
        return 0;
    else            
        return  1;
}

/*******************************
函数功能：写1
传递参数：空
返回值：空
********************************/
void write1(void)
{
    SDA_H;
    //Delay5us();
    SCL_H;
    Delay5us();
    SCL_L;
    Delay5us();
}

/*******************************
函数功能：写0
传递参数：空
返回值：空
********************************/
void write0(void)
{
    SDA_L;
    // Delay5us();
    SCL_H;
    Delay5us();
    SCL_L;
    Delay5us();
}

/*******************************
函数功能：发送1字节
传递参数：空
返回值：空
********************************/
void BH1750_SendByte(unsigned char dat)
{
   unsigned char i;
   
   for (i = 0; i < 8; i++)
   {        //8位计数器
      if(dat&0x80)
        write1();
      else
        write0();
      dat <<= 1;              //移出数据的最高位
   }
   BH1750_RecvACK();
}

/*******************************
函数功能：获取数据
传递参数：空
返回值：空
********************************/
unsigned char BH1750_RecvByte()
{
    unsigned char i;
    unsigned char dat = 0,flag;

    SDA_H;                    //使能内部上拉,准备读取数据,
    for (i = 0; i < 8; i++)
    {       //8位计数器
       //SDA_H;
       //Delay5us();
       SCL_H;
       SDA_IN;
       Delay5us();
       flag=SDA_VAL;
       dat <<= 1;
       if(flag)
         dat|=0x01;
       SDA_OUT;
       SCL_L;
       Delay5us();
  }
  return dat;
}

/*******************************
函数功能：单字节写
传递参数：空
返回值：空
********************************/
void Single_Write_BH1750(unsigned char REG_Address)
{
    BH1750_Start();                  //起始信号
    BH1750_SendByte(SlaveAddress);   //发送设备地址+写信号
    BH1750_SendByte(REG_Address);    //内部寄存器地址，请参考中文pdf22页
    //  BH1750_SendByte(REG_data);       //内部寄存器数据，请参考中文pdf22页
    BH1750_Stop();                   //发送停止信号
}


/************************************
函数功能：初始化BH1750，根据需要请参考pdf进行修改
传递参数：空
返回值：空
*************************************/
void bh1750_init()
{
    PxyDIRz(BH1750FVI_SCL_PORT,BH1750FVI_SCL_BIT,1);
    Single_Write_BH1750(0x01);  
}

/*******************************
函数功能：单字节读取
传递参数：空
返回值：空
********************************/
/*
unsigned char Single_Read_BH1750(unsigned char REG_Address)
{  
unsigned char REG_data;
BH1750_Start();                          //起始信号
BH1750_SendByte(SlaveAddress);           //发送设备地址+写信号
BH1750_SendByte(REG_Address);            //发送存储单元地址，从0开始
BH1750_Start();                          //起始信号
BH1750_SendByte(SlaveAddress+1);         //发送设备地址+读信号
REG_data=BH1750_RecvByte();              //读出寄存器数据
BH1750_SendACK(1);  
BH1750_Stop();                           //停止信号
   return REG_data;
}
*/

/*******************************
函数功能：连续读出BH1750内部数据
传递参数：空
返回值：空
********************************/
void Multiple_Read_BH1750(void)
{  
    unsigned char i;
    BH1750_Start();                          //起始信号
    BH1750_SendByte(0x47);         //发送设备地址+读信号
    for (i = 0; i < 3; i++)
    {                      //连续读取6个地址数据，存储中BUF
        BH1750FVI_BUF[i] = BH1750_RecvByte();          //BUF[0]存储0x32地址中的数据
        if (i == 3) 
        {
           mnack();                //最后一个数据需要回NOACK
        }
        else 
        {
           BH1750_SendACK();                //回应ACK
        }
    }
    BH1750_Stop();                          //停止信号
    Delay5ms();
}
/************************************
函数功能：读取光强数据
传递参数：空
返回值：gq光强数据
        光强数据将会保存在gq和BH1750FVI_BUF中
***************************************/
unsigned int bh1750_read()
{
    unsigned int gq;
    Single_Write_BH1750(0x01);   // power on
    Single_Write_BH1750(0x10);   // H- resolution mode
    delay(180);              //延时180ms
    Multiple_Read_BH1750();       //连续读出数据，存储在BUF中
    unsigned int h = (((unsigned int)BH1750FVI_BUF[0])<<8);
    unsigned int l = ((unsigned int)BH1750FVI_BUF[1]);
    gq = (unsigned int)(((float)h+(float)l)/1.2);
    //senser_lux_now = (unsigned int)((BUF[1]<<8+BUF[0])/1.2);
    //bh1750_set_mode(0x20);
    return gq;
}

#endif