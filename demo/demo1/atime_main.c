#include <msp430x14x.h>
#include "atime_msp430core.h"		//MSP430核心库
//#include "atime_lcd5110_printf.h"
#include "atime_lcd_hmi.c"
#include "atime_iic_software.h"
#include "atime_ds1307.c"
#include "atime_am2320_1.c"
#include "atime_am2320_2.c"
#include "atime_am2320_3.c"

void main(void)
{
    unsigned char i=0;
    
    watchdog_close();			//关闭看门狗
    wait_ms(50);                        //等待不稳定状态
    basic_clock_init();			//系统时钟初始化
    //lcd5110_init();                     //液晶屏幕初始化
    uart_init();                        //串口初始化
    interrupt_switch(on);               //开总中断
    lcd_order("rest");
    lcd_order("rest");                  //第一次发指令需要写两遍，保证指令正确发出
    
    ds1307_write(ds1307_data);

    while(1)
    {
        ds1307_read(ds1307_data);
        lcd_num_update(6,bcd_to_int(ds1307_data[2]));
        lcd_num_update(7,bcd_to_int(ds1307_data[1]));
        lcd_num_update(8,bcd_to_int(ds1307_data[0]));
        
        wait_ms(450);
        i++;
        if(i>=5)
        {
            i = 0;
            am2320_1_read();
            am2320_2_read();
            am2320_3_read();
        }
        
        //printf("%d \n",am2320_1_w);      //显示温度数据*10
        //printf("%d \n",am2320_1_s);      //显示湿度数据*10
        //printf("%d \n",am2320_2_w);      //显示温度数据*10
        //printf("%d \n",am2320_2_s);      //显示湿度数据*10
        //printf("%d \n",am2320_3_w);      //显示温度数据*10
        //printf("%d \n",am2320_3_s);      //显示湿度数据*10
        
        //printf_setadd(0,0);        

        lcd_num_update(0,am2320_1_w);
        lcd_num_update(1,am2320_1_s);
        lcd_num_update(2,am2320_2_w);
        lcd_num_update(3,am2320_2_s);
        lcd_num_update(4,am2320_3_w);
        lcd_num_update(5,am2320_3_s);
    } 
}
