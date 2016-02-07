##1：添加include目录
###1.右键单击工程
###2.在option中选择C/C++ Compiler
###3.选择preprocessor
###4.在additional include directories:里添加$PROJ_DIR$\ATIME
###5.完成

##2：使用相关
###所有全局变量、宏定义等相关配置都在.c文件中。
###所有文件都是以.h/.c文件配套使用，include "atime_xxx.h"后自动会加载对应的.c文件，不需要其他操作；
###使用.h文件可以将不使用的函数排除编译，减小代码体积;
###所有.h文件只做函数声明和载入对应.c文件;
###所有.c文件都可以单独使用而不载入.h文件，但是IAR将编译所有函数，导致代码体积很大；