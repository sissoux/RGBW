#ifndef COMSOCKET_H
#define COMSOCKET_H
#include "Arduino.h"

class ComSocket
{

  public:
  uint32_t BaudRate = 9600;
  uint16_t TimeOut = 100;
  uint8_t Header1 = 0xFF;
  uint8_t Header2 = 0xA4;
  uint8_t MaxDataLenght = 32;
  
  ComSocket();
  ComSocket(uint32_t BR);
  
  void begin();
  uint16_t send(uint8_t Cmd, uint8_t DataLenght, uint8_t Data[]);
  boolean get(uint8_t* Cmd, uint8_t* DataLenght, uint8_t Data[]);
  void discardInputBuffer();
  uint8_t read();

  private: 
  boolean Serial3Opened = false;
  uint16_t Fletcher16(uint8_t Lenght, uint8_t Data[]);
};


#endif
