##1.文件头
	/************************************
	 库功能：
	 库要求：
	 应用函数：
	 修改历史：
	   ‘修改人’   ‘修改内容’  ‘修改时间’
		空			空			空
	 最后修改时间：
	 作者： ATIME	版权所有
	 实例程序：
	 常见错误解释：
	 ***************************************/

##2.防重载
	表示整个库文件所需的一切全局变量及配置。
	define命名规则：
	_ATIME_控制器名_器件模块名_功能名_H_
	
									 //空一行
	#ifndef _ATIME_MSP430_SPI_H_ 
	#define _ATIME_MSP430_SPI_H_
	
								     //空两行
								     
	#endif		                     //endif前面三行空格    
##3.库函数变量组
	全局变量命名规则：
	不提供给用户的采用：器件模块名_功能名  //防止重名
	提供给用户的：尽量简化
	全局变量大写。
	/************************************
	库全局变量组
	***************************************/
									 //不空行
	sbit SPI_CS   =P1^4;
	sbit SPI_MOSI =P1^5;
	sbit SPI_SCLK =P1^6;
	sbit SPI_MISO =P1^7;
								     //空两行

##4.函数头
	函数命名采用：主体_功能(_其他)的方式，全部小写
	/************************************
	函数功能：SPI初始化
	传递参数：
			a：SPI启动、关闭选择
			b：SPI速度选择：1/4	；1/16；1/64；1/128	四种
					对应： 	 0  ； 1  ；  2	；  3
	返回值：0：失败
		   1：成功
	***************************************/
	int subject_action ( unsigned char a, int b)
	{
		unsigned char name;   //第一部分为局部变量定义，后空一行	
			
		//function;           //第二部分为函数功能实现，后空一行
		
		return (b);           //第三部分为函数返回值，采用()括住
	}
	
	
							  //空三行
	例如:
	void stc51_waitms(unsigned int n)
	{
		int i;
		
		while(n--)
		{
			for(i=0; i<75; i++)
			{
				;
			}	
		}
	}
	
##5.	功能实现
####1.变量赋值
	尽量使用 &= <<= 等符号，左侧空格
	b =0xff;                     //等号左侧加空格
	
####2.for循环
	for( i=0; i<8; i++)          //每个变量开头一个空格，括号内部赋值比较不空格
	{
		function;                //第三行写代码
	}
	
	for(i =0; db[i] !=0; i++) ;  //空循环内容在分号前加空格
	
####3.switch选择
	switch括号内不空格
	switch(a)
	{
		case clear1602 : function1;  break;  //冒号后加空格，break前加空格
		case left1602  : function2;  break;
		case right1602 : function3;  break;
		default        : return (0);         //冒号保持对齐
	}
####4.if语句、while循环
	if、while做判断时括号内判断符号两边空格；
	while单一变量不判断时不空格。
	
	if(name == 0x12)
	{
		function;
	}
	
####5.表达式与函数调用
	加减法、判断符号两边空格，乘除法两边不空格，函数调用括号前空格；
	数组方括号内表达式不空格；
	writezhi (0x80 + 0x40*y + x);
	
####6.括号内变量空格
	括号内只有一个变量名时不加空格，有多个变量名时每一个变量名前面个加一个空格；
	数组方括号内表达式不空格；
	如：
	switch(a)
	for( i=0; i<8; i++) 
	int subject_action ( unsigned char a, int b)

####7.语义空行
	不同语义部分加一个空行区分。

####8.条件编译部分
	函数中的全局条件编译部分可以顶格，不使用缩进。