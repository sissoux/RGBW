#include "RGBWLED.h"
#include "elapsedMillis.h"

//                      R  G   B   W  res frequency
RGBWLED MyLED = RGBWLED(23, 22, 21, 20, 16, 150);

#define SERIAL Serial

#define REFRESH_RATE 20             // ms
elapsedMillis DisplayTimeCounter;    
#define TEMPERATURE_RATE 2000       // For debugging
elapsedMillis TempTimeCounter;
#define APP_CMD_TIMEOUT 10000       // Define delay before continuing standalone mode
elapsedMillis AppCmdTimeoutCounter;

boolean FadeTrigger = false;        // Set this variable will launch a fading to the specifier Hue Sat Int
float Hue, Sat, Int;
uint16_t Delay =10000;

boolean OpenedCom = false;
int ByteCounter = 0;
String Command = String();


void setup()
{
  // put your setup code here, to run once:
  SERIAL.begin(115200);          //Open Bluetooth "Serial Socket"
  
  init();
  
  FadeTrigger = false;
}

void init()
{
  MyLED.begin();                  //Init MyLED Instance
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
}

void loop()
{
  taskManager();                                  //What needs to be done regularly ?
  if (SERIAL.available())                     
  {
    
    Command = getCommand();                       //If data is present in input buffer we get Header
    //delay(10);
    
    if (Command == String("RGBW"))                //If header is RGBW We send back WBGR to aknowledge and set the OpenedCom flag + reset timeout counter
    {
      AppCmdTimeoutCounter = 0;
      OpenedCom = true;
      SERIAL.print("WBGR");
    }
    else if (Command == String("SHSI") && SERIAL.available())   // Static HSI direct color display
    {
      AppCmdTimeoutCounter = 0;
      OpenedCom = 1;
      byte Buffer[6];
      while (ByteCounter < 6)
      {
        Buffer[ByteCounter] = SERIAL.read();
        ByteCounter++;
      }
      MyLED.setHue((float)(Buffer[0] + (Buffer[1] << 8)) / 100.0);
      MyLED.setSaturation((float)(Buffer[2] + (Buffer[3] << 8)) / 1000.0);
      MyLED.setIntensity((float)(Buffer[4] + (Buffer[5] << 8)) / 1000.0);
      MyLED.displayColor();
      ByteCounter = 0;
      Serial2Discard();
    }
    else if (Command == String("SRGB"))                 //Static RGB Direct color display
    {
      AppCmdTimeoutCounter = 0;
      OpenedCom = 1;
      byte Buffer[8];
      while (SERIAL.available() && ByteCounter < 8)
      {
        Buffer[ByteCounter] = (byte)SERIAL.read();
      }
      MyLED.R = (float)(Buffer[0] + Buffer[1] << 8);
      MyLED.G = (float)(Buffer[2] + Buffer[3] << 8);
      MyLED.B = (float)(Buffer[4] + Buffer[5] << 8);
      MyLED.W = (float)(Buffer[6] + Buffer[7] << 8);
      MyLED.displayRGBWColor();
      ByteCounter = 0;
    }
    else if (Command == String("FADE"))                 //Fade display from current color to specified HSI Value
    {
      AppCmdTimeoutCounter = 0;
      OpenedCom = 1;
      uint8_t Buffer[8];
      while (ByteCounter < 8)
      {
        Buffer[ByteCounter] = SERIAL.read();
        ByteCounter++;
      }
      Hue = ((float)(Buffer[0] + (Buffer[1] << 8)) / 100.0);
      Sat = ((float)(Buffer[2] + (Buffer[3] << 8)) / 1000.0);
      Int = ((float)(Buffer[4] + (Buffer[5] << 8)) / 1000.0);
      Delay = (uint16_t)(Buffer[6] + (Buffer[7] << 8));
      ByteCounter = 0;
      FadeTrigger = 1;
      Serial2Discard();
    }
    else if (Command == String("TICK"))             // App shall send reccurent data to avoid "AppTimeout" and back to Standalone
    {
      AppCmdTimeoutCounter = 0;
      OpenedCom = 1;
      Serial2Discard();
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
  while (SERIAL.available())
  {
    SERIAL.read();
  }
}

String getCommand()
{
  String Buffer = String();
  
#ifdef WAIT4INPUT
  delay(3000);
#endif

  while (SERIAL.available() && ByteCounter < 4)       //We capture the Header. Data aquisition will be done during CMD execution
  {
    Buffer = Buffer + (char)SERIAL.read();
    ByteCounter++;
  }
  ByteCounter = 0;
  return Buffer;                                      //Return supposed header
}

void taskManager()
{
  if ( DisplayTimeCounter >= REFRESH_RATE)
  {
    DisplayTimeCounter = 0;
    refreshDisplay();
  }
  if (false)  //TempTimeCounter >= TEMPERATURE_RATE)
  {
    TempTimeCounter = 0;
    SERIAL.print("Temperature: ");
    SERIAL.print(map(analogRead(A0),0,1023,0,3300));
    SERIAL.println(" mV");
  }
  if (AppCmdTimeoutCounter >= APP_CMD_TIMEOUT)
  {
    OpenedCom = 0;
  }
}

void refreshDisplay()
{
  if (MyLED.IsFadeRunning || FadeTrigger)
  {
    FadeTrigger = false;
    MyLED.fade(Hue,Sat,Int,Delay);
  }
  if (!MyLED.IsFadeRunning && !OpenedCom)
  {
    FadeTrigger = true;
    Hue = (float)random(0,36000)/100.0;
    Sat = (float)random(0,1000)/1000.0;
    Int = (float)random(0,1000)/1000.0;
    Delay = random(4000,40000);
  }
}

