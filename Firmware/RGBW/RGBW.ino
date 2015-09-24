#include "RGBWLED.h"

//                      R  G   B   W  res frequency
RGBWLED MyLED = RGBWLED(23, 22, 21, 20, 16, 150);

#define REFRESH_RATE 20// ms
uint32_t DisplayTimeCounter = 0;
#define TEMPERATURE_RATE 2000
uint32_t TempTimeCounter = 0;

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

  for ( int i = 0 ; i < 1000 ; i++)
  {
    MyLED.setIntensity((float)i / 1000.0);
    MyLED.displayColor();
    delay(50);
  }
}

void loop()
{
  taskManager();
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
    else if (Command == String("FADE"))
    {
      MyLED.IsFadeRunning = 1;
      SerialDiscard();
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

void taskManager()
{
  uint32_t Now = millis();
  if ( (Now - DisplayTimeCounter) >= REFRESH_RATE)
  {
    DisplayTimeCounter = Now;
    refreshDisplay();
  }
  if ( (Now - TempTimeCounter) >= TEMPERATURE_RATE)
  {
    TempTimeCounter = Now;
  Serial.print("Temperature: ");
  Serial.print(map(analogRead(A0),0,1023,0,3300));
  Serial.println(" mV");
  }
}

void refreshDisplay()
{
  if (MyLED.IsFadeRunning)
  {
   MyLED.fade(60.0,1.0,0.6,3500);
  }

}

