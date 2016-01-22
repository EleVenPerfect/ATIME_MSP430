/*************************************
�⹦�ܣ�MSP430���Ĺ���
��Ҫ�󣺳����Ѿ����룺
        #include <msp430x14x.h>
Ӧ�ú�����
        watchdog_close();
        basic_clock_init();
�޸���ʷ��
         ���޸��ˡ�   ���޸����ݡ�  ���޸�ʱ�䡯
	    ��		  ��		 ��
����޸�ʱ�䣺2016-01-22
���ߣ� ATIME	��Ȩ����
ʵ������
      #include <msp430x14x.h>
      #include "atime_msp430core.h"		    //MSP430���Ŀ�
      
      void main(void)
      { 
        
          watchdog_close();					//�رտ��Ź�
          basic_clock_init();				//ϵͳʱ�ӳ�ʼ��
              
          while(1);
      }

����������ͣ�
*************************************/

#ifndef _ATIME_MSP430_CORE_H_ 
#define _ATIME_MSP430_CORE_H_


/*************************************
��ȫ�ֱ�����
*************************************/
#define XIN   ((double)32768)        //�ⲿ��Ƶ����32.768KHZ
#define XT2IN ((double)8000000)      //�ⲿ��Ƶ����8MHZ
#define MSP430_DIVM 1                //MCLK��Ƶ��
#define MSP430_DIVS 1                //SMCLK��Ƶ��

enum msp430_switch { on, off};       //ö���߼�


/*************************************
�������ܣ�׼ȷ��ʱ����
���ݲ�����x
����ֵ����
*************************************/
#define delay_us(x) __delay_cycles((long)(XT2IN*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(XT2IN*(double)x/1000.0)) 


/*************************************
�������ܣ��رտ��Ź�
���ݲ�������
����ֵ����
*************************************/
void watchdog_close(void)
{
     WDTCTL =WDTPW + WDTHOLD;
}


/*************************************
�������ܣ������ʱ(����)
���ݲ�������
����ֵ����
*************************************/
void wait_ms(unsigned int i)
{
     int j=0;
     i =i/MSP430_DIVM;
     for( j=0; j<i; j++)
     {
         delay_us(990);
     }
}


/*************************************
�������ܣ�ϵͳʱ�ӿ���
basic clock ���ã��Ը�Ƶ����ΪMCLK/SMCLKʱ��Դ��������Ƶ��
���ݲ�������
����ֵ����
*************************************/
void basic_clock_init(void)
{
    unsigned char i;  
  
    BCSCTL1 &= ~XT2OFF;                 //��XT2��Ƶ��������
    
    do
    {
        IFG1 &= ~OFIFG;                //�������ʧ�ܱ�־
        for( i =0xFF; i>0; i--);       //��ʱ�ȴ�8MHz��������
 
    }while((IFG1 & OFIFG));            //����ʧЧ��־��Ȼ����
 
    switch(MSP430_DIVM)
    {
        case 1 : i=SELM_2 + SELS + DIVM_0; break;
        case 2 : i=SELM_2 + SELS + DIVM_1; break;
        case 4 : i=SELM_2 + SELS + DIVM_2; break;
        case 8 : i=SELM_2 + SELS + DIVM_3; break;
        default: i=SELM_2 + SELS + DIVM_0; break;
    }
    switch(MSP430_DIVS)
    {
        case 1 : i=i + DIVS_0; break;
        case 2 : i=i + DIVS_1; break;
        case 4 : i=i + DIVS_2; break;
        case 8 : i=i + DIVS_3; break;
        default: i=i + DIVS_0; break;
    }
      
    BCSCTL2 =i;//MCLK��SMCLKѡ���Ƶ����
}



#endif
