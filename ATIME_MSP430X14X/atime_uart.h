/************************************
�⹦�ܣ����ں�����
��Ҫ�����������Ѷ��壺
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
Ӧ�ú�����
          uart_init(void)
          uart_sendchar( unsigned char ch, unsigned char num)
          uart_send( unsigned char ch[], unsigned char num)    
�޸���ʷ��
		   	 ���޸��ˡ�   ���޸����ݡ�  ���޸�ʱ�䡯
				��			��			��
����޸�ʱ�䣺2016-01-25
���ߣ� ATIME	��Ȩ����
ʵ������
      #include <msp430x14x.h>
      #include "atime_msp430core.h"		//MSP430���Ŀ�
      #include "atime_uart.h"
      #include "atime_interrupt.c"

      void main(void)
      {

          watchdog_close();			//�رտ��Ź�
          basic_clock_init();			//ϵͳʱ�ӳ�ʼ��
          uart_init();                          //���ڳ�ʼ��
          interrupt_switch(on);                 //�����ж�
          
          while(1)
          {
              uart_send( "ATIME!\n", 0);
              uart_sendchar( '\n', 0);
              wait_ms(500);
          }
      }
      �жϷ�������
      #pragma vector=UART0RX_VECTOR
      __interrupt void UART0_RX_ISR(void)
      {
          unsigned char data=0;
          data =U0RXBUF;                      //���յ������ݴ�����
          uart0_sendchar(data);               //�����յ��������ٷ��ͳ�ȥ
      }


����������ͣ�
Error[e46]: Undefined external "uart1_sendchar" referred in main  ����1����û�д�
�����#define UART1_ON   
***************************************/


#ifndef _ATIME_MSP430_UART_H_ 
#define _ATIME_MSP430_UART_H_


/************************************
��ȫ�ֱ�����
***************************************/
/*����0����*/
#define UART0_ON                        //����0���أ�UART0_ON��UART0_OFF��
#define UART0_BAUD              14400   //���������ã���
#define UART0_RECEIVE           1       //����0�����ж�ʹ�ܣ�1EN��0DIS��
#define UART0_SEND              0       //����0�����ж�ʹ�ܣ�1EN��0DIS��
#define UART0_PARITY            0       //��żУ��λ���ã�0NONE,1ODD,2EVEN��
#define UART0_CHAR_LENGTH       8       //����λλ�����ã�8,7��
#define UART0_STOP_BIT          1       //ֹͣλ���ã�1,2��

/*����1����*/
#define UART1_OFF                       //����1���أ�UART1_ON��UART1_OFF��
#define UART1_BAUD              14400   //���������ã���
#define UART1_RECEIVE           1       //����1�����ж�ʹ�ܣ�1EN��0DIS��
#define UART1_SEND              0       //����1�����ж�ʹ�ܣ�1EN��0DIS��
#define UART1_PARITY            0       //��żУ��λ���ã�0NONE,1ODD,2EVEN��
#define UART1_CHAR_LENGTH       8       //����λλ�����ã�8,7��
#define UART1_STOP_BIT          1       //ֹͣλ���ã�1,2��
/************************************
�������Σ�UART0��غ���
***************************************/
#ifdef UART0_ON

/************************************
�����������飬�����޸�
***************************************/
#define UART0_BAUD_SETTING   (double)((double)(XT2IN/MSP430_DIVS)/((double)UART0_BAUD))           //�����ʼ��㹫ʽ
#define UART0_BAUD_H         (unsigned char)((unsigned long)UART0_BAUD_SETTING>>8)               //U0BR1��λ                                         //��ȡ��λ
#define UART0_BAUD_L         (unsigned char)((unsigned long)UART0_BAUD_SETTING)                  //U0BR0��λ                                         //��λ
#define UART0_BAUD_U0MCTL    (unsigned char)((UART0_BAUD_SETTING-(long)UART0_BAUD_SETTING)*8+0.5) //U0MCTL��λ
//ע��+0.5���壺��������������ת��ʱΪȥ�������֣���0.5����ʹ���������� 
/************************************
�������ܣ���ʼ������0
���ݲ�������
����ֵ����
***************************************/
void uart0_init(void)
{

    unsigned char i=0x01;          //U0CTL����
    
    U0CTL |=SWRST;                  //��ʼ��1
    
    #if UART0_PARITY==2
      i |=PENA;
      i |=PEV;
    #endif
    #if UART0_PARITY==1
      i |=PENA;
      i &=(~PEV);
    #endif                //��żУ��λ����

    #if UART0_STOP_BIT==2
      i |=SPB;
    #endif                //ֹͣλ����
      
    #if UART0_CHAR_LENGTH==8
      i |=CHAR;
    #endif                //����λ��������

    U0CTL |=i;		  //�������SWRSTλ
    
    
    U0TCTL |=SSEL1;		    //�����ʷ�����ʱ��Ƶ��SMCLK(1MHz)
    
    U0BR1 =UART0_BAUD_H;            //ȡ����ʱ��Ƶ��/������Ƶ������
    U0BR0 =UART0_BAUD_L;            //��λ����BR1����λ����BR0
    switch(UART0_BAUD_U0MCTL)      //С������U0MCTL���㷨���Ϸ��궨��
    {
        case 1 :U0MCTL =0x04; break;
        case 2 :U0MCTL =0x22; break;
        case 3 :U0MCTL =0x92; break;
        case 4 :U0MCTL =0xaa; break;
        case 5 :U0MCTL =0x6d; break;
        case 6 :U0MCTL =0xdd; break;
        case 7 :U0MCTL =0xf7; break;
        default:U0MCTL =0x00;
    }
    
    P3SEL |= 0x30;              //����I/O�˿�ģʽP3.4-P3.5
    P3DIR |= 0x10;
	
    U0ME |= UTXE0 + URXE0;      //���ͽ���ʹ��
    
    U0CTL&=~SWRST;              //�������SWRSTλ
    
    #if UART0_RECEIVE==1	
        U0IE |= URXIE0;	        //�����ж�ʹ��
    #endif
        
    #if UART0_SEND==1	
        U0IE |= UTXIE0;	        //�����ж�ʹ��
    #endif    
}


/************************************
�������ܣ�����0��������
���ݲ�����
        ch �����������ݣ�
����ֵ����
***************************************/
void uart0_sendchar(unsigned char ch)
{
    while(!(IFG1&UTXIFG0));          //���ͼĴ����յ�ʱ��������
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
    for( i=0; ch[i]!='\0'; i++)
      uart0_sendchar(ch[i]);
}



#endif


/************************************
�������Σ�UART1��غ���
***************************************/
#ifdef UART1_ON

/************************************
�����������飬�����޸�
***************************************/
#define UART1_BAUD_SETTING   (double)((double)(XT2IN/MSP430_DIVS)/((double)UART1_BAUD))           //�����ʼ��㹫ʽ
#define UART1_BAUD_H         (unsigned char)((unsigned long)UART1_BAUD_SETTING>>8)               //U1BR1��λ                                         //��ȡ��λ
#define UART1_BAUD_L         (unsigned char)((unsigned long)UART1_BAUD_SETTING)                  //U1BR0��λ                                         //��λ
#define UART1_BAUD_U1MCTL    (unsigned char)((UART1_BAUD_SETTING-(long)UART1_BAUD_SETTING)*8+0.5) //U1MCTL��λ
//ע��+0.5���壺��������������ת��ʱΪȥ�������֣���0.5����ʹ���������� 
/************************************
�������ܣ���ʼ������1
���ݲ�������
����ֵ����
***************************************/
void uart1_init(void)
{

    unsigned char i=0x01;          //U1CTL����
    
    U1CTL |=SWRST;                  //��ʼ��1
    
    #if UART1_PARITY==2
      i |=PENA;
      i |=PEV;
    #endif
    #if UART1_PARITY==1
      i |=PENA;
      i &=(~PEV);
    #endif                //��żУ��λ����

    #if UART1_STOP_BIT==2
      i |=SPB;
    #endif                //ֹͣλ����
      
    #if UART1_CHAR_LENGTH==8
      i |=CHAR;
    #endif                //����λ��������

    U1CTL |=i;		  //�������SWRSTλ
    
    
    U1TCTL |=SSEL1;		    //�����ʷ�����ʱ��Ƶ��SMCLK(1MHz)
    
    U1BR1 =UART1_BAUD_H;            //ȡ����ʱ��Ƶ��/������Ƶ������
    U1BR0 =UART1_BAUD_L;            //��λ����BR1����λ����BR1
    switch(UART1_BAUD_U1MCTL)      //С������U1MCTL���㷨���Ϸ��궨��
    {
        case 1 :U1MCTL =0x04; break;
        case 2 :U1MCTL =0x22; break;
        case 3 :U1MCTL =0x92; break;
        case 4 :U1MCTL =0xaa; break;
        case 5 :U1MCTL =0x6d; break;
        case 6 :U1MCTL =0xdd; break;
        case 7 :U1MCTL =0xf7; break;
        default:U1MCTL =0x00;
    }

    P3SEL |= 0xc0;              //����I/O�˿�ģʽP3.6-P3.7
    P3DIR |= 0x40;

    U1ME |= UTXE1 + URXE1;      //���ͽ���ʹ��
    
    U1CTL&=~SWRST;              //�������SWRSTλ
    
    #if UART1_RECEIVE==1	
        U1IE |= URXIE1;	        //�����ж�ʹ��
    #endif
        
    #if UART1_SEND==1	
        U1IE |= UTXIE1;	        //�����ж�ʹ��
    #endif    
}


/************************************
�������ܣ�����1��������
���ݲ�����
        ch �����������ݣ�
����ֵ����
***************************************/
void uart1_sendchar(unsigned char ch)
{
    while(!(IFG2&UTXIFG1));          //���ͼĴ����յ�ʱ��������
    U1TXBUF =ch;
}


/************************************
�������ܣ�����1�����ַ���
���ݲ�����
        ch �����������ݣ�
����ֵ����
***************************************/
void uart1_send(unsigned char ch[])
{
    unsigned int i =0;
    for( i=0; ch[i]!='\0'; i++)
      uart1_sendchar(ch[i]);
}



#endif



/************************************
�������Σ���������
***************************************/

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
����ֵ��
      0xff������
      0������
***************************************/
unsigned char uart_sendchar( unsigned char ch, unsigned char num)
{
    if(num==0)
    {
    #ifdef UART0_ON
        uart0_sendchar(ch);
        return (0);
    #endif
    }
    else 
    if(num==1)
    {
    #ifdef UART1_ON
        uart1_sendchar(ch);
        return (0);
    #endif
    }
    return (0xff);
}


/************************************
�������ܣ����ڷ����ַ���
���ݲ�����
        num�����ںţ�0,1����
        ch �����������ݣ�
����ֵ����
***************************************/
unsigned char uart_send( unsigned char ch[], unsigned char num)
{
    unsigned int i =0;
    unsigned char err=0;
    
    if(num==0)
    {
    #ifdef UART0_ON
        for( i=0; ch[i]!=0x0; i++)
          err +=uart_sendchar( ch[i], num);
        return (err);
    #endif
    }
    else 
    if(num==1)
    {
    #ifdef UART1_ON
        for( i=0; ch[i]!=0x0; i++)
          err +=uart_sendchar( ch[i], num);
        return (err);
    #endif
    }
    return (0xff);
}



#endif