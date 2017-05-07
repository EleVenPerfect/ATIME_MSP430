/************************************
库功能：串口函数库
库要求：主函数中已定义：
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
应用函数：
修改历史：
		   	 ‘修改人’   ‘修改内容’  ‘修改时间’
				空			空			空
最后修改时间：2016-01-25
作者： ATIME	版权所有
实例程序：

常见错误解释：
Error[e46]: Undefined external "uart1_sendchar" referred in main  串口1设置没有打开
解决：#define UART1_ON   
***************************************/


#ifndef _ATIME_LCD_HMI_C_ 
#define _ATIME_LCD_HMI_C_

#include "atime_uart.h"

/************************************
函数功能：发送指令
传递参数：指令名
返回值：空
注意：第一次发指令需要写两遍，保证指令正确发出
***************************************/
void lcd_order(unsigned char order[])
{
    unsigned char a[4] = {0xff,0xff,0xff,'\0'};
    uart_send( order, 1);
    uart_send( a, 1);
}

/************************************
函数功能：发送指令
传递参数：ch ：通道
          val：数据
返回值：空
注：此函数针对本例程的HMI。且不显示负数符号。
***************************************/
void lcd_num_update(unsigned char ch,int val)
{
    unsigned char shiwei;
    unsigned char gewei;
    unsigned char xiaoshu;
    unsigned char order[] ={'n','0','.','v','a','l','=','2','0',0xff,0xff,0xff,'\0','\0'};
    
    if(val<0)
    {
        val = val*(-1);
        switch(ch)
        {
            case 0:lcd_order("t18.txt=\"-\"");break;
            case 2:lcd_order("t19.txt=\"-\"");break;
            case 4:lcd_order("t20.txt=\"-\"");break;
          default :break;
        }
    }
    else
    if(val>=0)
    {
        switch(ch)
        {
            case 0:lcd_order("t18.txt=\" \"");break;
            case 2:lcd_order("t19.txt=\" \"");break;
            case 4:lcd_order("t20.txt=\" \"");break;
          default :break;
        }
    }
    shiwei = (val%1000)/100 + 0x30;
    gewei  = (val%100)/10 + 0x30;
    xiaoshu = val%10 +0x30;
    
    if(ch < 5)
    {
        order[0] = 'n';
        order[1] = 2*ch+0x30;
        order[2] = '.';
        order[3] = 'v';
        order[4] = 'a';
        order[5] = 'l';
        order[6] = '=';
        order[7] = shiwei;
        order[8] = gewei;
        order[9] = 0xff;
        order[10] = 0xff;
        order[11] = 0xff;
        order[12] = '\0';
        order[13] = '\0';
        uart_send( order, 1);

        order[0] = 'n';
        order[1] = 2*ch+0x31;
        order[2] = '.';
        order[3] = 'v';
        order[4] = 'a';
        order[5] = 'l';
        order[6] = '=';
        order[7] = xiaoshu;
        order[8] = 0xff;
        order[9] = 0xff;
        order[10] = 0xff;
        order[11] = '\0';
        order[12] = '\0';
        order[13] = '\0';
        uart_send( order, 1);
    }
    else
    if(ch == 5)
    {
        order[0] = 'n';
        order[1] = 0x31;
        order[2] = 0x30;
        order[3] = '.';
        order[4] = 'v';
        order[5] = 'a';
        order[6] = 'l';
        order[7] = '=';
        order[8] = shiwei;
        order[9] = gewei;
        order[10] = 0xff;   
        order[11] = 0xff;
        order[12] = 0xff;
        order[13] = '\0';
        uart_send( order, 1);

        order[0] = 'n';
        order[1] = 0x31;
        order[2] = 0x31;
        order[3] = '.';
        order[4] = 'v';
        order[5] = 'a';
        order[6] = 'l';
        order[7] = '=';
        order[8] = xiaoshu;
        order[9] = 0xff;
        order[10] = 0xff;   
        order[11] = 0xff;
        order[12] = '\0';
        order[13] = '\0';
        uart_send( order, 1);
    }
    
    if(ch >5 && ch < 9)
    {
        order[0] = 'n';
        order[1] = 0x31;
        order[2] = ch+0x30-0x04;
        order[3] = '.';
        order[4] = 'v';
        order[5] = 'a';
        order[6] = 'l';
        order[7] = '=';
        order[8] = gewei;
        order[9] = xiaoshu;
        order[10] = 0xff;   
        order[11] = 0xff;
        order[12] = 0xff;
        order[13] = '\0';
        uart_send( order, 1);
    }
}

/************************************
函数功能：BCD转int
传递参数：bcd：BCD数据
返回值：val：int数据
***************************************/
int bcd_to_int(unsigned char bcd)
{
    int a;
    a = bcd%16+(bcd/16)*10;
    return a;
}

#endif