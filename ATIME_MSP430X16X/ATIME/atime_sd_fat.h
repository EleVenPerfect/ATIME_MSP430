
//��δ���ԣ���ֲ��ATIME51��Ƭ���⡣
//��δ���ԣ���ֲ��ATIME51��Ƭ���⡣
//��δ���ԣ���ֲ��ATIME51��Ƭ���⡣


/************************************
�⹦�ܣ�FAT�ļ�ϵͳ
��Ҫ�󣺳����Ѿ����룺
	#include <msp430x14x.h>
        #include "atime_msp430core.h"		//MSP430���Ŀ�
        #include "atime_spi_s.h"
        #include "atime_sdcard.h"
        #include "atime_sd_fat.h"	
Ӧ�ú�����
			unsigned char int_fat(void)
			unsigned char read_file_start512(unsigned long cluster, unsigned char number)
			unsigned char read_file_continue512(unsigned char part)
			unsigned char search_file_name( unsigned long cluster, unsigned char name[12])
�޸���ʷ��
		   	 ���޸��ˡ�       ���޸����ݡ�  			���޸�ʱ�䡯
������ʱ�䣺��δ���ԣ���ֲ��ATIME51��Ƭ���⡣
���ߣ� ATIME	��Ȩ����
ʵ������
����01�����䵥Ԫ����512�ֽڣ�
			#include <reg52.h>
			#include "atime_serial.h"
			#include "atime_spi.h"
			#include "atime_spi_sdcard.h"
			#include "atime_sd_fat.h"
			
			
			void main()
			{
				unsigned char offset;
				unsigned int i;
				serial_int(disable, baud9600, disable);
			
				int_fat();
				read_file_start512(fat_info.fat_clu_dir, 4);
			
				for(offset=FAT_READING; offset!=FAT_READ_OK; )
				{
					offset =read_file_continue512(0);
			
					for(i=0; i<512; i++)
						send_char(fat_temp[i]);
			
				}
			
				while(1);
			}
����02�����䵥Ԫ����1024�ֽڣ�			
			#include <reg52.h>
			#include "atime_serial.h"
			#include "atime_spi.h"
			#include "atime_spi_sdcard.h"
			#include "atime_sd_fat.h"
			
			
			void main()
			{
				unsigned char offset,j;
				unsigned int i;
				serial_int(disable, baud9600, disable);
			
				int_fat();
			
				send_char(fat_info.fat_cluster);
				read_file_start512(fat_info.fat_clu_dir, 4);
				for(i=0; i<512; i++)
					send_char(fat_temp[i]);
				read_file_continue512(1);
				for(i=0; i<512; i++)
					send_char(fat_temp[i]);
				for(offset=FAT_READING; offset!=FAT_READ_OK; )
				{
					for(j=0; j<fat_info.fat_cluster; j++)
					{
						offset =read_file_continue512(j);
				
						for(i=0; i<512; i++)
							send_char(fat_temp[i]);
					}
				}
			
				while(1);
			}

����03��ͨ���ļ������Ҳ����ļ�
			#include <reg52.h>
			#include "atime_serial.h"
			#include "atime_spi.h"
			#include "atime_spi_sdcard.h"
			#include "atime_sd_fat.h"
			
			void main()
			{
				unsigned char number,offset;
				unsigned int i;
				serial_int(disable, baud9600, disable);
				int_fat();
			
				number =search_file_name( fat_info.fat_clu_dir, "CHAR    .TXT");	 //��Ҫ�ļ�ϵͳ��Ŀ¼������CHAR.TXT�ļ�
				send_char(number);
			
				read_file_start512(fat_info.fat_clu_dir, number);
				for(i=0; i<512; i++)
					send_char(fat_temp[i]);
				for(offset=FAT_READING; offset!=FAT_READ_OK; )
				{
					offset =read_file_continue512(0);
				
					for(i=0; i<512; i++)
						send_char(fat_temp[i]);
				}
			
				while(1);
			}

����������ͣ�
	ATIME_DS1302.H(30): error C231: 'P0': redefinition  :  �ظ������˱���  ��������������������#define���д������������ǰ�漸��
	*** WARNING L16: UNCALLED SEGMENT, IGNORED FOR OVERLAY PROCESS	������ĺ�����û�е��ù��ģ�һ����Ժ��Դ˾���
	123.C(7): error C202: 'riht1602': undefined identifier	 ����������ƴд���󣬸���ƴд���󼴿�
	123.C(14): error C214: illegal pointer conversion    ��Ҫ��������ַ���������������ַ�
ע�⣺
	���ļ�ϵͳΪFAT32�ļ�ϵͳ��Ŀǰ��֧�ֶ�ȡ�ļ����ݣ������ṩд���ݹ��ܣ�
	�ļ���ֻ֧������8�ֽڵĶ��ļ������ļ���Ӧ�ô�д��ĸ��ʾ��
	��ȡ�ļ������У�ֻ�����˵�һ��FAT������FAT��1�ͱ�2���ڵĲ���޷��жϣ�
	���⺯�����ṩ�ļ���ȡ������ݴ���
	����ļ����ݵĶ�ȡ��������ȡ�����ļ�����ʵ��С�����ǰ�����������Ŀ����ÿ����ȡ�˵���������ȫ�����ֳ�����
	�⺯���ڲ�ֻ����һ����ȡ���ݵĻ���fat_temp[512]����ˣ��������ȡ���ݵĺ�����Ӧ������ȡ����������ݣ���ֹ��һ���������������ݸ��ǣ�
	�ļ�ϵͳ��ʼ�����ļ�ϵͳ��Ϣfat_info��������ģ�
	��ȡ�ļ����ļ���Ϣ������һ���صĴغŻ�ı��⣬������Ϣ���뵱ǰ��ȡ�ļ�����Ϣ����һ�¡�
	�����ļ����ļ��еĶ�ȡ����Ϊ���ļ����ݵĶ�ȡ�������Ҫ�ж��Ƿ�Ϊ�ļ��У���Ҫ���ж��ļ���С���ļ���־�ȡ�
	�������е�ȫ�ֱ������Ѿ�����ΪXDATA����Щ�ͺŵ�Ƭ�����ܻ��ڴ治�㣬���Գ���ȡ�����ֱ�����XDATA���ԡ�	

	����ĵ�������512M��sd���Ͻ��еģ���ʽ��ʱѡ���ļ�ϵͳFAT32�����䵥Ԫ��С512�ֽڣ�û��ѡ��ԭ�豸��Ĭ��ֵ��
***************************************/


#ifndef _ATIME_SD_FAT_H_ 
#define _ATIME_SD_FAT_H_

#include "atime_sd_fat.c"


/************************************
�������ܣ�FAT�ļ�ϵͳMBR��ʼ��
���ݲ�������
����ֵ����ʼ������
���󷵻�ֵ��NOT_FAT:����FAT�ļ�ϵͳ��û�м�⵽SD��
***************************************/
unsigned long int_mbr_fat(void);


/************************************
�������ܣ�FAT�ļ�ϵͳDBR��ʼ��
���ݲ�����offset����ʼ������
����ֵ����Ŀ¼������
���󷵻�ֵ��NOT_FAT:����FAT�ļ�ϵͳ��û�м�⵽SD��
***************************************/
unsigned long int_dbr_fat(unsigned long offset);


/************************************
�������ܣ�ÿ��������
���ݲ�����offset����ʼ������
����ֵ��ÿ��������
***************************************/
unsigned char cluster_sector(unsigned long offset);


/************************************
�������ܣ�FAT�ļ�ϵͳDBR��ʼ��
���ݲ�����offset����ʼ������
����ֵ��FAT��1��������
***************************************/
unsigned long int_dbr_fat_dir(unsigned long offset);


/************************************
�������ܣ���ȡ��Ŀ¼��ʼ����
���ݲ�����offset����ʼ������
����ֵ����Ŀ¼��ʼ����
***************************************/
unsigned long int_dbr_clu_dir(unsigned long offset);


/************************************
�������ܣ�fat32�ļ�ϵͳ��ʼ��
���ݲ�������
����ֵ����ʼ�����
		FAT_OK  :�ļ�ϵͳ׼������
		NOT_FAT	:����FAT�ļ�ϵͳ��û�в�SD��
���󷵻�ֵ��NOT_FAT:����FAT�ļ�ϵͳ��û�м�⵽SD��
***************************************/
unsigned char int_fat(void);


/************************************
�������ܣ�FAT���в�����һ����λ�ã������µ��ļ������
���ݲ�������
����ֵ����
***************************************/
void find_next_cluster(void);


/************************************
�������ܣ���ת��Ϊ����
���ݲ�����cluster������
����ֵ��������
***************************************/
unsigned long cluster_to_offset(unsigned long cluster);


/************************************
�������ܣ���ȡ�ļ�����Ϣ
���ݲ�����cluster���ļ������ڴ���
		  number���ļ����ţ���0��ʼ��
����ֵ����
***************************************/
void read_filetable(unsigned long cluster, unsigned char number);


/************************************
�������ܣ���ʼ��ȡ�ļ�����
���ݲ�����cluster���ļ������ڴأ�
		  number���ļ������кţ���0��ʼ��������
����ֵ����ȡ�ļ���״̬
		FAT_READ_OK����ȡ���
		FAT_READING:�ļ����ڶ�ȡ��
***************************************/
unsigned char read_file_start512(unsigned long cluster, unsigned char number);


/************************************
�������ܣ�������ȡ�ļ�����
���ݲ�����part���ļ��������ڴصĵڼ�������
����ֵ����ȡ�ļ���״̬
		FAT_READ_OK����ȡ���
		FAT_READING:�ļ����ڶ�ȡ��
***************************************/
unsigned char read_file_continue512(unsigned char part);


/************************************
�������ܣ������ļ������к�
���ݲ�����cluster���������ļ������ڴ�
		  name[11]:�ļ�������չ�����ո���0x20��䡣
		  ��ʽ˵����name[0~7]:�ļ�����name[8]:С���㣬name[9~11]: ��չ�������ֲ��������ÿո���䡣
����ֵ���ļ������кţ����������ڴص��������ļ���������û���ҵ��ļ�
	    һ���ļ���32�ֽڣ�һ������512���ֽڣ������һ���ض�����������16X���ļ���
		����16*fat_info.fat_cluster
***************************************/
unsigned char search_file_name( unsigned long cluster, unsigned char name[12]);



#endif