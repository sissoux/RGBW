#include "Arduino.h"
#include "ComSocket.h"


ComSocket::ComSocket()
{
}

ComSocket::ComSocket(uint32_t BR)
{
  this->BaudRate = BR;
}

void ComSocket::begin()
{
  if (!Serial3Opened)
  {
    Serial3.begin(BaudRate);
    Serial3Opened = true;
  }
}

uint16_t ComSocket::send(byte Cmd, byte DataLenght, byte Data[])
{
  uint8_t Buffer[DataLenght + 5];
  uint8_t Id = (Cmd & 0x0F) << 4 + (DataLenght & 0x0F);
  Buffer[0] = Header1;
  Buffer[1] = Header2;
  Buffer[2] = Id;
  for (int x = 3 ; x <= DataLength + 3 ; x++)
  {
    Buffer[x] = Data[x-3];
  }
  uint16_t CHK = Fletcher16(DataLenght, Data[], Id);
  Buffer[DataLength+3] = (uint8_t)((CHK & 0xF0) >> 8);
  Buffer[DataLength+4] = (uint8_t)((CHK & 0x0F);
  Serial3.write(Buffer,DataLenght + 5);
  return CHK;
}

boolean ComSocket::get(uint8_t* Cmd, uint8_t* DataLenght, uint8_t Data[])
{
  uint32_t Now = millis();
  uint8_t Head1, Head2, ID;
  while (!Serial3.available() && millis() - Now < TimeOut);

  Head1 = read();
  Head2 = read();

  if (!(Head1 == Header1 && Head2 == Header2))
  {
    discardInputBuffer();
    return false;
  }
  else
  {
    ID = read();
    *DataLenght = ID & 0x0F ;
    *Cmd = (ID & 0xF0) >> 4 ;
    if ( MaxDataLenght < *DataLenght)
    {
      discardInputBuffer();
      return false;
    }
    else
    {
      Serial3.readBytes(Data[], DataLenght);
      uint8_t Fletch1 = read();
      uint8_t Fletch2 = read();
      if (Fletcher16(*DataLenght, Data[], ID) == ((uint16_t)Fletch1 << 8 + Fletch2))
      {
        return true;
      }
      else
      {
        discardInputBuffer();
        return false;
      }
    }
  }



}

uint16_t ComSocket::Fletcher16(uint8_t Lenght, uint8_t Data[], uint8_t Id)
{
  uint8_t sum1 = Id;
  uint8_t sum2 = sum1;

  for (int i = 0; i < Lenght; i++ )
  {
    sum1 = (sum1 + Data[i]) % 255;
    sum2 = (sum2 + sum1) % 255;
  }
  return (((uint16_t)sum1) << 8) | sum2;

}

void ComSocket::discardInputBuffer()
{
  while (Serial3.available())
  {
    Serial3.read();
  }
}

uint8_t ComSocket::read()
{
  if (Serial3.available())
  {
    return (uint8_t)Serial3.read();
  }
  return 0;
}


