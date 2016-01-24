/************************************
�⹦�ܣ����ں�����
��Ҫ�����������Ѷ��壺
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
Ӧ�ú�����
�޸���ʷ��
		   	 ���޸��ˡ�   ���޸����ݡ�  ���޸�ʱ�䡯
				��			��			��
����޸�ʱ�䣺2014-01-29
���ߣ� ATIME	��Ȩ����
ʵ������
����������ͣ�

***************************************/


#ifndef _ATIME_MSP430_UART_H_ 
#define _ATIME_MSP430_UART_H_


/************************************
��ȫ�ֱ�����
***************************************/
/*����0����*/
#define UART0_ON                        //����0���أ�UART0_ON��UART0_OFF��
#define UART0_RECEIVE           1       //����0����ʹ�ܣ�1EN��0DIS��
#define UART0_SEND              1       //����0����ʹ�ܣ�1EN��0DIS��
#define UART0_MODE              1       //����0�������ã�1UART��0SPI��
#define UART0_BAUD              9600    //���������ã���
#define UART0_PARITY            0       //��żУ��λ���ã�0NONE,1ODD,2EVEN��
#define UART0_CHAR_LENGTH       8       //����λλ�����ã�8,7��
#define UART0_STOP_BIT          1       //ֹͣλ���ã�1,2��

/*����1����*/
#define UART1_OFF                       //����1���أ�UART1_ON��UART1_OFF��
#define UART1_RECEIVE           EN      //����1����ʹ�ܣ�EN��DIS��
#define UART1_SEND              EN      //����1����ʹ�ܣ�EN��DIS��
#define UART1_MODE              UART    //����1�������ã�UART��SPI��
#define UART1_BAUD              9600    //���������ã���
#define UART1_PARITY            NONE    //��żУ��λ���ã�NONE,ODD,EVEN��
#define UART1_CHAR_LENGTH       8       //����λλ�����ã�8,7��
#define UART1_STOP_BIT          1       //ֹͣλ���ã�1,2��



/////////�������Σ�UART0��غ���/////////////
#ifdef UART0_ON

/************************************
�������ܣ�����������
���ݲ�����
      sysclk:ϵͳʱ��Ƶ�ʣ�
      baud  :�����ʣ�
����ֵ����
ע���ο����͵�ַ��http://www.cnblogs.com/Engin/archive/2011/07/06/2098990.html
***************************************/
int abs_abs_abs(int i);
void set_baud_clock0( long sysclk, long baud)
{

    unsigned int kk;
    char i,mSum;
    int txEr0,txEr1;
    
    mSum =0;
    txEr0 =0;
    txEr1 =0;
    
    kk =(unsigned int)sysclk/baud;
    U0BR1 =kk >> 8;         //��8λ
    U0BR0 =kk & 0xff;       //��8λ
    
     for( i=0; i<8; i++)
    {
        txEr0 =(int) 100 * baud * ((i + 1) * kk + mSum) / sysclk - 100 * (i + 1);
        txEr1 =(int) 100 * baud * ((i + 1) * kk + mSum + 1) / sysclk - 100 * (i + 1);
        if(abs_abs_abs(txEr1) < abs_abs_abs(txEr0))
        {
            mSum++;
            U0MCTL |= (1<<i);
        }
    }
}


/************************************
�������ܣ���ʼ������0
���ݲ�������
����ֵ����
***************************************/
void uart0_init(void)
{

    U0CTL |= SWRST;             //��ʼ��1
    
    U0TCTL |= SSEL1;		    //�����ʷ�����ʱ��Ƶ��SMCLK(1MHz)
    
    set_baud_clock0(XT2IN/MSP430_DIVS,UART0_BAUD);//���ô���0�����ʼĴ���

    #if UART0_PARITY==2
      U0CTL |=PENA;
      U0CTL |=PEV;
    #endif
    #if UART0_PARITY==1
      U0CTL |=PENA;
      U0CTL &=(~PEV);
    #endif                //��żУ��λ����

    #if UART0_STOP_BIT==2
      U0CTL |=SPB;
    #endif                //ֹͣλ����
      
    #if UART0_CHAR_LENGTH==8
      U0CTL |=CHAR;
    #endif                //����λ��������

    #if UART0_MODE==0
      U0CTL |=SYNC;
    #endif
    #if UART0_MODE==1
      U0CTL &=~SYNC;
    #endif                //USARTģʽ����
  
  
    P3SEL |= 0x30;              //����I/O�˿�ģʽ
    P3DIR |= 0x10;
	
    U0ME |= UTXE0 + URXE0;      //���ͽ���ʹ��
	
    U0CTL &= (~SWRST);		//�������SWRSTλ
	
    U0IE |= URXIE0 + UTXIE0;	//���ͽ����ж�ʹ��
}


/************************************
�������ܣ�����0��������
���ݲ�����
        ch �����������ݣ�
����ֵ����
***************************************/
void uart0_sendchar(unsigned char ch)
{
    //while(!(IFG1&UTXIFG0));          //���ͼĴ����յ�ʱ��������
    U0TXBUF =ch;
}


/************************************
�������ܣ����ڷ����ַ���
���ݲ�����
        ch �����������ݣ�
����ֵ����
***************************************/
void uart0_send(unsigned char ch[])
{
    unsigned int i =0;
    for( i=0; ch[i]!=0x0; i++)
      uart0_sendchar(ch[i]);
}



#endif


/////////�������Σ�UART2��غ���/////////////
#ifdef UART1_ON



#endif



/////////�������Σ���������/////////////

/************************************
�������ܣ�ȡ����ֵ
���ݲ�����ֵ
����ֵ������ֵ
***************************************/
int abs_abs_abs(int i)
{
    return (i>=0?i:(-i));
}


/************************************
�������ܣ���ʼ������
���ݲ�������
����ֵ����
***************************************/
void uart_init(void)
{
#ifdef UART1_ON
    uart1_init();
#endif
#ifdef UART0_ON
    uart0_init();
#endif
}


/************************************
�������ܣ����ڷ�������
���ݲ�����
        num�����ںţ�0,1����
        ch �����������ݣ�
����ֵ����
***************************************/
void uart_sendchar( unsigned char num, unsigned char ch)
{

}


/************************************
�������ܣ����ڷ����ַ���
���ݲ�����
        num�����ںţ�0,1����
        ch �����������ݣ�
����ֵ����
***************************************/
void uart_send( unsigned char num, unsigned char ch[])
{
    unsigned int i =0;
    for( i=0; ch[i]!=0x0; i++)
      uart_sendchar( num, ch[i]);
}




#endif