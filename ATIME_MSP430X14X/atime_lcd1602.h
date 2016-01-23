/*************************************
库功能：并行驱动LCD1602库
库要求：程序已经载入：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
应用函数：
		lcd1602_init(enum lcdint_ac a,enum lcdint_cursor b)
		print1602(unsigned char db[], unsigned int y, unsigned int x)
		printchar1602(unsigned char db, unsigned int y, unsigned int x)
		set1602(enum set a )
		bcd_trans_char(unsigned char a[], unsigned char b[],unsigned char j)
修改历史：
         ‘修改人’   ‘修改内容’  ‘修改时间’
	    空		  空		 空
最后修改时间：2016-01-23
作者： ATIME	版权所有
实例程序：
      #include <msp430x14x.h>
      #include "atime_msp430core.h"                   //MSP430核心库
      #include "atime_lcd1602.h"

      void main(void)
      { 
          unsigned char db[10]={"A T I M E"};

          watchdog_close();				//关闭看门狗
          basic_clock_init();				//系统时钟初始化
          lcd1602_init( rightmove, cursornotdisplay);   //液晶屏幕初始化
          
          print1602( db, 0, 1);                        //可以引用数组
          print1602( "EleVenPerfect", 1, 1);           //可以引用字符串
          printchar1602( 'O', 0, 12);
          printchar1602( 'K', 0, 13);
          wait_ms( 1000);
          
          while(set1602(right1602))
            wait_ms( 1200);
      }


常见错误解释：
*************************************/

#ifndef _ATIME_MSP430_LCD1602_H_ 
#define _ATIME_MSP430_LCD1602_H_


/*************************************
库全局变量组
*************************************/
#define LCD1602_DATA       4     //定义lcd1602的8位数据口

#define LCD1602_RS_PORT    5     //将RS位定义;端口号
#define LCD1602_RS_BIT     5     //将RS位定义;端口位

#define LCD1602_RW_PORT    5     //将RW位定义;端口号
#define LCD1602_RW_BIT     6     //将RW位定义;端口位

#define LCD1602_EN_PORT    5     //将EN位定义;端口号
#define LCD1602_EN_BIT     7     //将EN位定义;端口位

enum set		{clear1602, left1602, right1602 };				//枚举:设定参数，见105行(set1602)注解
enum lcdint_ac		{leftmove, rightmove, left_move, right_move };                  //枚举:初始化AC移动方向，见129行(lcd1602_init)注解
enum lcdint_cursor	{notdisplay, cursornotdisplay, cursorflash, cursornotflash };	//枚举:初始化光标状态，见129行(lcd1602_init)注解


/************************************
函数功能：写数据1bit
传递参数：db 数据指令内容
返回值：空
***************************************/
void writedata(char db)
{
	//while(mang()==1);
	PxyOUTz(LCD1602_RS_PORT,LCD1602_RS_BIT,1);
	PxyOUTz(LCD1602_RW_PORT,LCD1602_RW_BIT,0);
	PxyOUTz(LCD1602_EN_PORT,LCD1602_EN_BIT,0);
	PxOUT(LCD1602_DATA) =db;
	delay_ms( 2);
	PxyOUTz(LCD1602_EN_PORT,LCD1602_EN_BIT,1);
	delay_ms( 5);
	PxyOUTz(LCD1602_EN_PORT,LCD1602_EN_BIT,0);
}


/************************************
函数功能：写指令1bit
传递参数：db  数据指令内容
返回值：空
***************************************/
void writezhi(unsigned char db)
{
	//while(mang()==1);
	PxyOUTz(LCD1602_RS_PORT,LCD1602_RS_BIT,0);
	PxyOUTz(LCD1602_RW_PORT,LCD1602_RW_BIT,0);
	PxyOUTz(LCD1602_EN_PORT,LCD1602_EN_BIT,0);
	PxOUT(LCD1602_DATA) =db;
	delay_ms( 2);
	PxyOUTz(LCD1602_EN_PORT,LCD1602_EN_BIT,1);
	delay_ms( 5);
	PxyOUTz(LCD1602_EN_PORT,LCD1602_EN_BIT,0);
}


/************************************
函数功能：设置1602
传递参数：a:
			clear1602: 清屏
			left1602 : 左移
			right1602: 右移

返回值：1：成功；：失败0；
***************************************/
unsigned char set1602(enum set a )
{
	switch(a)
	{
		case clear1602 : writezhi(0x01); break;
		case left1602  : writezhi(0x18); break;
		case right1602 : writezhi(0x1C); break;
		default        :return (0);
	}
	//EX0 =1;
	return (1);
}



/************************************
函数功能：并行初始化lcd1602
传递参数：
      a:光标移动设置 ，详见a的注释
      b:开关光标显示 ，详见b的注释
返回值：1：成功；：失败0；
***************************************/
unsigned char lcd1602_init(enum lcdint_ac a,enum lcdint_cursor b)
{
	/*设置I/O端口方向和功能*/
    PxSEL(LCD1602_DATA) =0x00;
    PxySELz(LCD1602_RS_PORT,LCD1602_RS_BIT,0);
    PxySELz(LCD1602_RW_PORT,LCD1602_RW_BIT,0);
    PxySELz(LCD1602_EN_PORT,LCD1602_EN_BIT,0);
        
    PxDIR(LCD1602_DATA) =0xff;
    PxyDIRz(LCD1602_RS_PORT,LCD1602_RS_BIT,1);
    PxyDIRz(LCD1602_RW_PORT,LCD1602_RW_BIT,1);
    PxyDIRz(LCD1602_EN_PORT,LCD1602_EN_BIT,1);
        
    /*设置1602*/
    writezhi( 0x01);
	delay_ms( 15);
	PxyOUTz(LCD1602_RS_PORT,LCD1602_RS_BIT,0);
	PxyOUTz(LCD1602_EN_PORT,LCD1602_EN_BIT,0);
	PxOUT(LCD1602_DATA) =0X38;
	delay_ms( 2);
	PxyOUTz(LCD1602_EN_PORT,LCD1602_EN_BIT,1);
	delay_ms( 5);
	PxyOUTz(LCD1602_EN_PORT,LCD1602_EN_BIT,0);
	PxOUT(LCD1602_DATA) =0X38;
	delay_ms( 2);
	PxyOUTz(LCD1602_EN_PORT,LCD1602_EN_BIT,1);
	delay_ms( 5);
	PxyOUTz(LCD1602_EN_PORT,LCD1602_EN_BIT,0);
	PxOUT(LCD1602_DATA) =0X38;
	delay_ms( 2);
	PxyOUTz(LCD1602_EN_PORT,LCD1602_EN_BIT,1);
	delay_ms( 5);
	writezhi( 0x01);
	
	switch(a)
	{
		case rightmove  : writezhi(0x06); break;	  //画面不动，读写后AC自动+1
		case leftmove   : writezhi(0x04); break;	  //画面不动，读写后AC自动-1
		case right_move : writezhi(0x07); break;	  //画面向左平移1次，读写后AC自动+1
		case left_move  : writezhi(0x05); break;	  //画面向右平移1次，读写后AC自动-1
		default 		: return (0);             // 错误
	}
	switch(b)
	{
		case cursornotflash   : writezhi(0x0e); break;	  //画面显示，光标显示，光标不闪动
		case cursorflash      : writezhi(0x0d); break;	  //画面显示，光标显示，光标闪动
		case cursornotdisplay : writezhi(0x0c); break;	  //画面显示，光标不显示，光标不闪动
		case notdisplay       : writezhi(0x08); break;	  //画面不显示，光标不显示，光标不闪动
		default               : return (0);               // 错误
	}
	set1602(clear1602);
	return (1);
}


/************************************
函数功能：测试字符串长度
传递参数：字符串指针
返回值：字符串长度
***************************************/
unsigned int str_long(unsigned char db[])
{
	unsigned int i;
	for( i =0; db[i]!=0; i++) ;
	return (i);
}


/************************************
函数功能：显示数据、字母
传递参数：db[] : 显示的数据 
		  x:	 横向坐标
		  y:	 纵向坐标
		  坐标系左上角为（0,0）点向右向下为正
返回值：1：成功；：失败0；
特别注意：此函数只可以显示字符串、字符串数组，不可以显示单个字符
***************************************/
unsigned char print1602(unsigned char db[], unsigned int y, unsigned int x)
{
	unsigned int i; 
	writezhi( 0x80 + 0x40*y + x );
	for( i=0; i< str_long(db); i++)		 //修改了判断字符串长度方式	由shrlen改为自定义的str_long
	{
		writedata( db[i]);
	}
	return (1);
}


/************************************
函数功能：设置自定义字符
传递参数：
		  i:设置的自定义字符标号
		  a[]:定义字符的数组数据
返回值：空
特别注意：i的取值：0~7；
显示自定义字符方法：
		用printchar1602函数，数据位写之前定义的字符标号即可
		如：printchar1602(0,2,1);
***************************************/
void setpic1602(unsigned char i, unsigned char a[])
{
	unsigned j;
	writezhi( (0x40+0x08*i) );
	delay_ms( 5);
	for( j=0; j<8; j++)
	{	
		writedata( a[j]);
		delay_ms( 2);	
	}				
}
		

/************************************
函数功能：显示数据、字母
传递参数：db : 显示的数据 
		  x:	 横向坐标
		  y:	 纵向坐标
		  坐标系左上角为（1,1）点向右向下为正
返回值：1：成功；：失败0；
特别注意：此函数只可以显示单个字符
***************************************/
unsigned char printchar1602(unsigned char db, unsigned int y, unsigned int x)
{
	writezhi( 0x80 + 0x40*y + x );
	writedata( db);
	return (1);
}


/************************************
函数功能：1位BCD码转换3位可显示数字
传递参数：a[]，BCD数据;b[]，可显示数据
返回值：空
***************************************/
void bcd_trans_char(unsigned char a[], unsigned char b[],unsigned char j)
{
	unsigned int i;
	for( i=0; i<j; i++)
	{
		b[3*i]	 =a[i]/100      + 0x30;
		b[3*i+1] =(a[i]%100)/10 + 0x30;
		b[3*i+2] =(a[i]%10)     + 0x30;
	}
}



#endif
