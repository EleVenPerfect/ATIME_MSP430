欢迎来到ATIME MSP430库.

欢迎您来到ATIME_MSP430函数库，目前函数库使用MSP430F149作为核心，函数库包含部分常

用模块的操作函数。该函数库高内聚低耦合，以atime_msp430core.h作为支持，保证其他头

文件的正常使用。 

### 1.通用模块统一保存 时钟控制、延时、总中断控制这些通用的函数

保存在atime_msp430core.c中，减小不必要的重复。 

### 2.简化I/O操作 将P1~P6引脚的寄

存器操作简化为PxyOUTz(port,bit,value)、PxyDIRz(port,bit,value)、PxySELz

(port,bit,value)等，大大简化从其他单片机移植I/O模拟操作到MSP430的过程。详细可见

：atime_msp430core.c 

### 3.配置简单 所有配置信息保存在该函数库的.c文件中的库全局

变量组中，其他部分不需要修改。 

### 4.自动匹配时钟 串口、SPI、PWM等和时钟相关的设

置本库中自动校准，保证不同系统时钟下的通用性。 

### 5.函数名命名统一 函数名命名基

本采用“模块名_init” 函数作为初始化，以“模块名_read/write” 作为操作函数，方便

使用。下面以DS1307为例：如下所示 

`

  #include <msp430x14x.h>
  #include "atime_msp430core.h"                  //MSP430核心库
  #include "atime_lcd5110_printf.h"
  #include "atime_iic_software.h"
  #include "atime_ds1307.h"
  void main(void)
  {
     unsigned char i,j;
      watchdog_close();                          //关闭看门狗
      wait_ms(50);
      basic_clock_init();                        //系统时钟初始化
      lcd5110_init();                            //液晶屏幕初始化
      ds1307_write(ds1307_data);
      while(1)
      {
         ds1307_read(ds1307_data);
          j++;
          for( i=0; i<8; i++)
             printf("  %02X   ",ds1307_data[i]);//以16进制显示
          if(j%2==0)
             printf("AT-");
          else
             printf("IME");
          printf_setadd(0,0);
          wait_ms(400);
      } 
  }


`

### 6.函数文件保存 所有.c文件可以不依赖.h文件单独使用，但以.h方式载入头文件可

以减小代码体积，将不编译不需要的函数。

### 联系作者 欢迎发送电子邮件到

atime@atime.org.cn或登陆我的个人网站atime.org.cn，谢谢！
