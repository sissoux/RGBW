#include "RGBWLED.h"

//                      R  G   B   W  res frequency
RGBWLED MyLED = RGBWLED(23, 22, 21, 20, 16, 150);

#define REFRESH_RATE 20// ms
uint32_t DisplayTimeCounter = 0;
#define TEMPERATURE_RATE 2000
uint32_t TempTimeCounter = 0;
boolean FadeTrigger = false;

boolean OpenedCom = false;
int ByteCounter = 0;
String Command = String();

float Hue, Sat, Int;
uint16_t Delay =10000;

void setup()
{
  // put your setup code here, to run once:
  Serial2.begin(115200);
  Serial1.begin(115200);
  //while(!Serial2);
  MyLED.begin();
  MyLED.setHue(0.0);
  MyLED.setSaturation(1.0);
  MyLED.setIntensity(0.0);
  MyLED.displayColor();

  for ( int i = 0 ; i < 1000 ; i++)
  {
    MyLED.setIntensity((float)i / 1000.0);
    MyLED.displayColor();
    //delay(10);
  }
  FadeTrigger = false;
  Hue = 0;
  Sat = 1;
  Int = 1;
}

void loop()
{
  taskManager();
  if (Serial2.available())
  {
    
    Command = getCommand();
    //delay(10);
    
    if (Command == String("RGBW"))
    {
      Serial2.print("WBGR");
      OpenedCom = true;
    }
    else if (Command == String("SHSI") && Serial2.available())
    {
      byte Buffer[6];
      while (ByteCounter < 6)
      {
        Buffer[ByteCounter] = Serial2.read();
        ByteCounter++;
      }
      MyLED.setHue((float)(Buffer[0] + (Buffer[1] << 8)) / 100.0);
      MyLED.setSaturation((float)(Buffer[2] + (Buffer[3] << 8)) / 1000.0);
      MyLED.setIntensity((float)(Buffer[4] + (Buffer[5] << 8)) / 1000.0);
      MyLED.displayColor();
      ByteCounter = 0;
      Serial2Discard();
    }
    else if (Command == String("SRGB"))
    {
      byte Buffer[8];
      while (Serial2.available() && ByteCounter < 8)
      {
        Buffer[ByteCounter] = (byte)Serial2.read();
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
      FadeTrigger = 1;
      Serial2Discard();
    Hue = (float)random(0,36000)/100.0;
    Sat = (float)random(0,1000)/1000.0;
    Int = (float)random(0,1000)/1000.0;
    Delay = random(4000,40000);
    }
    else
    {
      Serial2Discard();
    }
  Command = String("Null");
  }
}




void Serial2Discard()
{
  while (Serial2.available())
  {
    Serial2.read();
  }
}

String getCommand()
{
  String Buffer = String();
  delay(3000);
  while (Serial2.available() && ByteCounter < 4)
  {
    Buffer = Buffer + (char)Serial2.read();
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
  Serial2.print("Temperature: ");
  Serial2.print(map(analogRead(A0),0,1023,0,3300));
  Serial2.println(" mV");
  }
}

void refreshDisplay()
{
  if (MyLED.IsFadeRunning || FadeTrigger)
  {
    FadeTrigger = false;
    MyLED.fade(Hue,Sat,Int,Delay);
  }
  if (!MyLED.IsFadeRunning)
  {
    FadeTrigger = true;
    Hue = (float)random(0,36000)/100.0;
    Sat = (float)random(0,1000)/1000.0;
    Int = (float)random(0,1000)/1000.0;
    Delay = random(4000,40000);
  }
}

