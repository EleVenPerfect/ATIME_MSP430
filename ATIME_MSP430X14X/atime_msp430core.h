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
I/O�ӿ������飬�����޸ģ�
*************************************/
#define PxIN(x)         PxINTEMP(x)
#define PxINTEMP(x)     P##x##IN
#define PxOUT(x)        PxOUTTEMP(x)
#define PxOUTTEMP(x)    P##x##OUT
#define PxDIR(x)        PxDIRTEMP(x)
#define PxDIRTEMP(x)    P##x##DIR
#define PxSEL(x)        PxSELTEMP(x)
#define PxSELTEMP(x)    P##x##SEL

#define PxIFG(x)        PxIFGTEMP(x)
#define PxIFGTEMP(x)    P##x##IFG
#define PxIES(x)        PxIESTEMP(x)
#define PxIESTEMP(x)    P##x##IES
#define PxIE(x)         PxIETEMP(x)
#define PxIETEMP(x)     P##x##IE


#define PxyOUTz(x,y,z)        PxyOUTzTEMP(x,y,z)
#define PxyOUTzTEMP(x,y,z)    (z>0)?((P##x##OUT)|=(0x01<<y)) :((P##x##OUT) &=(~(0x01<<y)))
#define PxyDIRz(x,y,z)        PxyDIRzTEMP(x,y,z)
#define PxyDIRzTEMP(x,y,z)    (z>0)?((P##x##DIR)|=(0x01<<y)) :((P##x##DIR) &=(~(0x01<<y)))
#define PxySELz(x,y,z)        PxySELzTEMP(x,y,z)
#define PxySELzTEMP(x,y,z)    (z>0)?((P##x##SEL)|=(0x01<<y)) :((P##x##SEL) &=(~(0x01<<y)))

#define PxyIFGz(x,y,z)        PxyIFGzTEMP(x,y,z)
#define PxyIFGzTEMP(x,y,z)    (z>0)?((P##x##IFG)|=(0x01<<y)) :((P##x##IFG) &=(~(0x01<<y)))
#define PxyIESz(x,y,z)        PxyIESzTEMP(x,y,z)
#define PxyIESzTEMP(x,y,z)    (z>0)?((P##x##IES)|=(0x01<<y)) :((P##x##IES) &=(~(0x01<<y)))
#define PxyIEz(x,y,z)         PxyIEzTEMP(x,y,z)
#define PxyIEzTEMP(x,y,z)     (z>0)?((P##x##IE)|=(0x01<<y)) :((P##x##IE) &=(~(0x01<<y)))

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
        delay_us(200);                 //��ʱ�ȴ�8MHz��������
 
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
