#include <reg52.h>
#define uchar unsigned char
#define uint  unsigned int
//����Max7219�˿�
sbit Max7219_pinCLK = P1^2;
sbit Max7219_pinCS  = P1^1;
sbit Max7219_pinDIN = P1^0;

void Delay_xms(uint x)
{
 uint i,j;
 for(i=0;i<x;i++)
  for(j=0;j<112;j++);
}
//--------------------------------------------
//���ܣ���MAX7219(U3)д���ֽ�
//��ڲ�����DATA 
//���ڲ�������
//˵����
void Write_Max7219_byte(uchar DATA)         
{
    	uchar i;    
		Max7219_pinCS=0;		
	    for(i=8;i>=1;i--)
          {		  
            Max7219_pinCLK=0;
            Max7219_pinDIN=DATA&0x80;
            DATA=DATA<<1;
            Max7219_pinCLK=1;
           }                                 
}
//-------------------------------------------
//���ܣ���MAX7219д������
//��ڲ�����address��dat
//���ڲ�������
//˵����
void Write_Max7219(uchar address,uchar dat)
{ 
     Max7219_pinCS=0;
	 Write_Max7219_byte(address);           //д���ַ��������ܱ��
     Write_Max7219_byte(dat);               //д�����ݣ����������ʾ���� 
	 Max7219_pinCS=1;                        
}

void Init_MAX7219(void)
{
 Write_Max7219(0x09, 0xff);       //���뷽ʽ��BCD��
 Write_Max7219(0x0a, 0x03);       //����
 Write_Max7219(0x0b, 0x07);       //ɨ����ޣ�4���������ʾ
 Write_Max7219(0x0c, 0x01);       //����ģʽ��0����ͨģʽ��1
 Write_Max7219(0x0f, 0x01);       //��ʾ���ԣ�1�����Խ�����������ʾ��0
}
void main(void)
{
 Delay_xms(50);
 Init_MAX7219();
 Delay_xms(2000);
 Write_Max7219(0x0f, 0x00);       //��ʾ���ԣ�1�����Խ�����������ʾ��0
 Write_Max7219(1,8);
 Write_Max7219(2,7);
 Write_Max7219(3,6);
 Write_Max7219(4,5); 
 Write_Max7219(5,4);
 Write_Max7219(6,3);
 Write_Max7219(7,2);
 Write_Max7219(8,1);
 while(1);
}
