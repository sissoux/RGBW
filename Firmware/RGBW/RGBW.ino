#include "RGBWLED.h"

//                      R  G   B   W  res frequency
RGBWLED MyLED = RGBWLED(6, 22, 23, 9, 16, 150);


boolean OpenedCom = false;
int ByteCounter = 0;

void setup()
{
  // put your setup code here, to run once:
  Serial3.begin(115200);
  //while(!Serial3);
  MyLED.begin();
  MyLED.setHue(0.0);
  MyLED.setSaturation(0.0);
  MyLED.setIntensity(1.0);
  MyLED.displayColor();

  for ( int i = 0 ; i < 1000 ; i++)
  {
    MyLED.setIntensity((float)i / 1000.0);
    MyLED.displayColor();
    delay(50);
  }
}

void loop()
{
  if (Serial3.available())
  {
    
    String Command = getCommand();
    
    if (Command == String("RGBW"))
    {
      Serial3.print("WBGR");
      OpenedCom = true;
    }
    else if (Command == String("SHSI"))
    {
      byte Buffer[6];
      while (Serial3.available() && ByteCounter < 6)
      {
        Buffer[ByteCounter] = Serial3.read();
      }
      MyLED.setHue((float)(Buffer[0] + Buffer[1] << 8) / 100.0);
      MyLED.setSaturation((float)(Buffer[2] + Buffer[3] << 8) / 100.0);
      MyLED.setIntensity((float)(Buffer[4] + Buffer[5] << 8) / 100.0);
      MyLED.displayColor();
      ByteCounter = 0;
    }
    else if (Command == String("SRGB"))
    {
      byte Buffer[8];
      while (Serial3.available() && ByteCounter < 8)
      {
        Buffer[ByteCounter] = Serial3.read();
      }
      MyLED.R = (float)(Buffer[0] + Buffer[1] << 8);
      MyLED.G = (float)(Buffer[2] + Buffer[3] << 8);
      MyLED.B = (float)(Buffer[4] + Buffer[5] << 8);
      MyLED.W = (float)(Buffer[6] + Buffer[7] << 8);
      MyLED.displayRGBWColor();
      ByteCounter = 0;
    }
    else
    {
      Serial3Discard();
    }
  }
}




void Serial3Discard()
{
  while (Serial3.available())
  {
    Serial3.read();
  }
}

String getCommand()
{
  String Buffer = String();
  while (Serial3.available() && ByteCounter < 4)
  {
    Buffer = Buffer + (char)Serial3.read();
    ByteCounter++;
  }
  ByteCounter = 0;
  return Buffer;
}

