/************************************
�⹦�ܣ����SPI��
��Ҫ�����������Ѷ��壺
        #include <msp430x14x.h>
        #include "atime_msp430core.h"
Ӧ�ú�����
�޸���ʷ��
		   	 ���޸��ˡ�   ���޸����ݡ�  ���޸�ʱ�䡯
				��			��			��
����޸�ʱ�䣺2016-01-29
���ߣ� ATIME	��Ȩ����
ʵ������
����������ͣ�
***************************************/


#ifndef _ATIME_MSP430_SPI_SOFTWARE_H_ 
#define _ATIME_MSP430_SPI_SOFTWARE_H_


/************************************
��ȫ�ֱ�����
***************************************/
struct spi_s
{
    unsigned char cs_p;         //�豸ʹ���źţ�port
    unsigned char cs_b;         //�豸ʹ���źţ�bit
    unsigned char clk_p;        //ʱ���źţ�port
    unsigned char clk_b;        //ʱ���źţ�bit
    unsigned char sdi_p;        //���豸���룬port
    unsigned char sdi_b;        //���豸���룬bit
    unsigned char sdo_p;        //���豸�����port
    unsigned char sdo_b;        //���豸�����bit
    unsigned char mode;         //���ù���ģʽ��0~3��
};                               //����SPI�����ź������ݴ���ṹ

/************************************
����ģʽ˵����
ģʽ0��CPHA=0;CPOL=0;
ģʽ1��CPHA=0;CPOL=1;
ģʽ2��CPH1=1;CPOL=0;
ģʽ3��CPHA=1;CPOL=0;
CPHA:ʱ����λ���ã�
     0��ʾÿ���ڵ�һ��ʱ���ز�����
     1��ʾÿ���ڵڶ���ʱ���ز�����
CPOL:ʱ���ź����ã�
     0��ʾ��ʱΪ0��
     1��ʾ��ʱΪ1.
***************************************/

 /**************************************
�������ܣ��˿ڳ�ʼ��
���ݲ�����struct spi_s a���˿���Ϣ
����ֵ����
***************************************/
void spi_s_init(struct spi_s a)
{
    unsigned char *add;
    
    add =ioreg_trans(a.cs_p,2);
    *add |=(0x01<<a.cs_b);
    add =ioreg_trans(a.cs_p,3);
    *add &=~(0x01<<a.cs_b);
    
    add =ioreg_trans(a.clk_p,2);
    *add |=(0x01<<a.clk_b);
    add =ioreg_trans(a.clk_p,3);
    *add &=~(0x01<<a.clk_b);
    
    add =ioreg_trans(a.sdo_p,2);
    *add |=(0x01<<a.sdo_b);
    add =ioreg_trans(a.sdo_p,3);
    *add &=~(0x01<<a.sdo_b);
    
    add =ioreg_trans(a.sdi_p,2);
    *add &=~(0x01<<a.sdi_b);
    add =ioreg_trans(a.sdi_p,3);
    *add &=~(0x01<<a.sdi_b);
}


 /**************************************
�������ܣ��ߵ�λ����
���ݲ�������ת���ֽ�
����ֵ�����
***************************************/
unsigned char transbit( unsigned char x)
{
	unsigned char i;
	i =(x & 0x7e) | ( ( (x&0x80) >> 7 ) | ( (x&0x01) << 7 ) );
	x =i;
	i =(x & 0xbd) | ( ( (x&0x40) >> 5 ) | ( (x&0x02) << 5 ) );
	x =i;
	i =(x & 0xdb) | ( ( (x&0x20) >> 3 ) | ( (x&0x04) << 3 ) );
	x =i;		
	i =(x & 0xe7) | ( ( (x&0x10) >> 1 ) | ( (x&0x08) << 1 ) );
	x =i;  
	return x;		
}


/************************************
�������ܣ�SPIд����0
���ݲ�������
����ֵ����
***************************************/
unsigned char spi_s_write0( struct spi_s a, unsigned char data)
{
    unsigned char i;
    
    
    return (i);
}
/************************************
�������ܣ�SPIд����1
���ݲ�������
����ֵ����
***************************************/
unsigned char spi_s_write1( struct spi_s a, unsigned char data)
{
    unsigned char i;
    

    return (i);
}
/************************************
�������ܣ�SPIд����2
���ݲ�������
����ֵ����
***************************************/
unsigned char spi_s_write2( struct spi_s a, unsigned char data)
{
    unsigned char i;
    

    return (i);
}
/************************************
�������ܣ�SPIд����3
���ݲ�������
����ֵ����
***************************************/
unsigned char spi_s_write3( struct spi_s a, unsigned char data)
{
    unsigned char i;
    

    return (i);
}
/************************************
�������ܣ�SPIд����
���ݲ�������
����ֵ����
***************************************/
unsigned char spi_s_write( struct spi_s a, unsigned char data)
{
    unsigned char i;
    
    switch(a.mode)
    {
        case 0 : i=spi_s_write0( a, data);break;
        case 1 : i=spi_s_write1( a, data);break;
        case 2 : i=spi_s_write2( a, data);break;
        case 3 : i=spi_s_write3( a, data);break;
        default:i=0xff;
    }
    return (i);
}



#endif