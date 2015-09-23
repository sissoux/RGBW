#include "RGBWLED.h"

//                      R  G   B   W  res frequency
RGBWLED MyLED = RGBWLED(23, 22, 21, 20, 16, 150);


boolean OpenedCom = false;
int ByteCounter = 0;
String Command = String();

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  //while(!Serial);
  MyLED.begin();
  MyLED.setHue(0.0);
  MyLED.setSaturation(1.0);
  MyLED.setIntensity(1.0);
  MyLED.displayColor();

  /*for ( int i = 0 ; i < 1000 ; i++)
  {
    MyLED.setIntensity((float)i / 1000.0);
    MyLED.displayColor();
    delay(50);
  }*/
}

void loop()
{
  delay(2000);
  Serial.print("Temperature: ");
  Serial.print(map(analogRead(A0),0,1023,0,3300));
  Serial.println(" mV");
  if (Serial.available())
  {
    
    Command = getCommand();
    //delay(10);
    
    if (Command == String("RGBW"))
    {
      Serial.print("WBGR");
      OpenedCom = true;
    }
    else if (Command == String("SHSI") && Serial.available())
    {
      byte Buffer[6];
      while (ByteCounter < 6)
      {
        Buffer[ByteCounter] = Serial.read();
        ByteCounter++;
      }
      MyLED.setHue((float)(Buffer[0] + (Buffer[1] << 8)) / 100.0);
      MyLED.setSaturation((float)(Buffer[2] + (Buffer[3] << 8)) / 1000.0);
      MyLED.setIntensity((float)(Buffer[4] + (Buffer[5] << 8)) / 1000.0);
      MyLED.displayColor();
      ByteCounter = 0;
      SerialDiscard();
    }
    else if (Command == String("SRGB"))
    {
      byte Buffer[8];
      while (Serial.available() && ByteCounter < 8)
      {
        Buffer[ByteCounter] = (byte)Serial.read();
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
      SerialDiscard();
    }
  Command = String("Null");
  }
}




void SerialDiscard()
{
  while (Serial.available())
  {
    Serial.read();
  }
}

String getCommand()
{
  String Buffer = String();
  while (Serial.available() && ByteCounter < 4)
  {
    Buffer = Buffer + (char)Serial.read();
    ByteCounter++;
  }
  ByteCounter = 0;
  return Buffer;
}

