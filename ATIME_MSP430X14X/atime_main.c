#include <msp430x14x.h>
#include "atime_msp430core.h"		              //MSP430���Ŀ�

void main(void)
{ 
  
	watchdog_close();				//�رտ��Ź�
        basic_clock_init();				//ϵͳʱ�ӳ�ʼ��
	while(1);
}
