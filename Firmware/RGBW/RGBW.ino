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
  Serial.begin(115200);
  Serial1.begin(9600);  //Bluetooth socket
  //while(!Serial);
  MyLED.begin();
  MyLED.setHue(0.0);
  MyLED.setSaturation(1.0);
  MyLED.setIntensity(0.0);
  MyLED.displayColor();

  for ( int i = 0 ; i < 1000 ; i++)
  {
    MyLED.setIntensity((float)i / 1000.0);
    MyLED.displayColor();
    delay(10);
  }
  FadeTrigger = true;
  Hue = 0;
  Sat = 1;
  Int = 1;
}

void loop()
{
  taskManager();
  if (Serial1.available())  //Simple function to check bluetooth connection
  {
    Serial.print(Serial1.read());
  }
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
      FadeTrigger = 1;
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

