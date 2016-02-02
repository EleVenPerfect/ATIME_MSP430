#include "SD_Card.h"
#include "SPI.h"

extern u8 CardType;

void SD_SendBytes(const u8 *buff,u16 cnt)
{
  SPI_SendBytes(buff,cnt);
}


void SD_ReceiveBytes(u8 *buff,u16 cnt)
{
  SPI_ReceiveBytes(buff,cnt);
}


u8 SD_WaitReady()    //return 0:ok 1:timeout,failed
{
  u8 tmp,i;
  for(i=0;i<250;i++)
  {
    SD_ReceiveBytes(&tmp,1);
    if(tmp==0xff)
      return 0;
  }
  return 1;
}


u8 SD_ReceiveDataBlock(u8 *buff,u16 cnt)   //return 0:ok 1:failed
{
  u8 tmp[2];
  u16 tmr;
  SD_CS_0;
  for(tmr=0;tmr<1000;tmr++)
  {
    SD_ReceiveBytes(tmp,1);
    if(tmp[0]!=0xff)
      break;
    delay_us(10);
  }
  if(tmp[0]!=0xfe)
    return 1;
  SD_ReceiveBytes(buff,cnt);
  SD_ReceiveBytes(tmp,2);
  
  return 0;
}


u8 SD_SendDataBlock(const u8 *buff,u8 token)     //return 0 :ok 1:failed
{
  u8 tmp[2]={0xff,0xff};
  SD_CS_0;
  while(SD_WaitReady());
  SD_SendBytes(tmp,2);
  SD_SendBytes(&token,1);
  if(token!=0xfd)
  {
    SD_SendBytes(buff,512);
    SD_SendBytes(tmp,2);
    SD_ReceiveBytes(tmp,1);
    if((tmp[0]&0x1f)==0x05)
    {
      while(SD_WaitReady());
      SD_CS_1;
      SD_ReceiveBytes(tmp,1);
      return 0;
    }
    else
    {
      while(SD_WaitReady());
      SD_CS_1;
      return tmp[0];
    }
  }
  return 0;
}


u8 SD_SendCommand(u8 cmd,u32 arg)
{
  u8 tmp,crc,buf[6],response,i=0;
  SD_CS_0;
  while(SD_WaitReady());
  buf[0]=0x40|cmd;
  buf[1]=(u8)((arg&0xff000000)>>24);
  buf[2]=(u8)((arg&0x00ff0000)>>16);
  buf[3]=(u8)((arg&0x0000ff00)>>8);
  buf[4]=(u8)((arg&0x000000ff));
  crc=0x01;
  if(cmd==CMD0)
    crc=0x95;
  if(cmd==CMD8)
    crc=0x87;
  buf[5]=crc;
  SD_SendBytes(buf,6);
  do
  {
    SD_ReceiveBytes(&response,1);
    i++;
    if(i==200)
      return 1;
  }while(response&0x80);
  SD_CS_1;
  SD_ReceiveBytes(&tmp,1);
  return response;
  
}


u8 SD_Init()     //return 4:failed 0:mmc 1:SD_V1 2:SD_V2 3:SD_V2HC
{
  u8 response,tmp,i,buf[4],ty;
  u16 tmr;
  SPI_Init(); 
  SD_CS_1;
  for(i=0;i<10;i++)
    SD_ReceiveBytes(&tmp,1);
  while(SD_WaitReady());
  SD_CS_0;
  do
  {
    response=SD_SendCommand(CMD0,0);
    tmr++;
    if(tmr==200)
      return 1;
  }while(response!=1);
  tmr=0;
  if(SD_SendCommand(CMD8,0X1AA)==1)
  {
    SD_CS_0;
    SD_ReceiveBytes(buf,4);
    if(buf[2]==0x01&&buf[3]==0xaa)
    {
      do
      {
        SD_SendCommand(CMD55,0);
        response=SD_SendCommand(CMD41,1UL<<30);
        tmr++;
        if(tmr==200)
          return 4;
      }while(response);
      if(SD_SendCommand(CMD58,0)==0)
      {
        SD_CS_0;
        SD_ReceiveBytes(buf,4);
        if(buf[0]&0x40)
          ty=SD_V2HC;
        else
          ty=SD_V2;
      }
    }
    else
    {
      tmr=0;
      SD_SendCommand(CMD55,0);
      response=SD_SendCommand(CMD41,0);
      if(response<=1)
      {
        do
        {
          SD_SendCommand(CMD55,0);
          response=SD_SendCommand(CMD41,0);
          tmr++;
          if(tmr==200)
            return 4;
        }while(response);
        ty=SD_V1;
      }
      else
      {
        tmr=0;
        do
        {
          response=SD_SendCommand(CMD1,0);
          tmr++;
          if(tmr==200)
            return 4;
        }while(response);
        ty=MMCv3;     
      }        
    }
  }
  tmr=0;
  do
  {
    response=SD_SendCommand(CMD16,512);
    tmr++;
    if(tmr==200)
      return 4;
  }while(response);
  SD_CS_1;
  SD_ReceiveBytes(buf,1);
  return ty;  
}


u8 SD_ReadSectors(u8 *buff,u32 sector,u16 count)
{
  u8 response,i;
  if(!count) return 1;
  if(CardType!=SD_V2HC) 
    sector*=512;
  if(count==1)
  {
    do
    {
      response=SD_SendCommand(CMD17,sector);
      i++;
      if(i==200)
        return 1;
    }while(response);
    SPI_MiddleMode();
    while(SD_WaitReady());
    if(SD_ReceiveDataBlock(buff,512))
      return 1;
    else
      return 0;
  }
  else
  {
    do
    {
      response=SD_SendCommand(CMD18,sector);
      i++;
      if(i==200)
        return 1;
    }while(response);
    SD_CS_0;
    SPI_MiddleMode();
    while(SD_WaitReady());
    do
    {
      response=SD_ReceiveDataBlock(buff,512);
      buff+=512;
    }while(--count&&response==0);
    SD_SendCommand(CMD12,0);
    SD_CS_0;
    SD_ReceiveBytes(&response,1);
    SD_CS_1;
    return 0;
  }
}


u8 SD_WriteSectors(const u8 *buff,u32 sector,u16 count)
{
  u8 response,i;
  if(!count) return 1;
  if(CardType!=SD_V2HC) sector*=512;
  if(count==1)
  {
    do
    {
      response=SD_SendCommand(CMD24,sector);
      i++;
      if(i==200)
        return 1;
    }while(response);
    SD_CS_0;
    i=0;
    SPI_MiddleMode();
    while(SD_WaitReady());
    if(SD_SendDataBlock(buff,0xfe))
      return 1;
    else
      return 0;      
  }
  else
  {
    if(CardType!=MMCv3)
    {
      SD_SendCommand(ACMD23,count);
    }
    do
    {
      response=SD_SendCommand(CMD25,sector);
      i++;
      if(i==200)
        return 1;
    }while(response);
    SPI_MiddleMode();
    while(SD_WaitReady());
    do
    {
      response=SD_SendDataBlock(buff,0xfc);
      buff+=512;       
    }while((--count)&&(response==0));
    if(response!=0||count!=0)
      return 1;
    if(!SD_SendDataBlock(0,0xfd))
      return 0;
    else
      return 1;
  }
  
}































