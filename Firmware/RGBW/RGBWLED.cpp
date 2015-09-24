#include "RGBWLED.h"
#include "Arduino.h"
#include "math.h"

RGBWLED::RGBWLED(char RPin, char GPin, char BPin, char WPin, char Resolution, float PWMFrequency)
{
  RedPin = RPin;
  GreenPin = GPin;
  BluePin = BPin;
  WhitePin = WPin;
  _Resolution = Resolution;
  _PWMFrequency = PWMFrequency;
  _intensity = 1;
  _hue = 0;
  _saturation = 1;
}

void RGBWLED::displayColor()
{
  float cos_h, cos_1047_h;
  float H = PI*_hue/180.0;
  
  if (H < DPIST) //2*PI/3
  {
    cos_h = cos(H);
    cos_1047_h = cos(PIST - H);
    R = _saturation * _intensity / 3 * (1 + cos_h / cos_1047_h);
    G = _saturation * _intensity / 3 * (1 + (1 - cos_h / cos_1047_h));
    B = 0;
    W = (1 - _saturation) * _intensity;
  }
  else if (H < QPIST)   // 4*PI/3
  {
    H = H - DPIST;
    cos_h = cos(H);
    cos_1047_h = cos(PIST - H);
    G = _saturation * _intensity / 3 * (1 + cos_h / cos_1047_h);
    B = _saturation * _intensity / 3 * (1 + (1 - cos_h / cos_1047_h));
    R = 0;
    W = (1 - _saturation) * _intensity;
  }
  else
  {
    H = H - QPIST;
    cos_h = cos(H);
    cos_1047_h = cos(PIST - H);
    B = _saturation * _intensity / 3 * (1 + cos_h / cos_1047_h);
    R = _saturation * _intensity / 3 * (1 + (1 - cos_h / cos_1047_h));
    G = 0;
    W = (1 - _saturation) * _intensity;
  }

  analogWrite(RedPin, ((1<<_Resolution)-1)*R);
  analogWrite(GreenPin, ((1<<_Resolution)-1)*G);
  analogWrite(BluePin, ((1<<_Resolution)-1)*B);
  analogWrite(WhitePin, ((1<<_Resolution)-1)*W);
}
void RGBWLED::displayRGBWColor()
{
  analogWrite(RedPin, ((1<<_Resolution)-1)*R);
  analogWrite(GreenPin, ((1<<_Resolution)-1)*G);
  analogWrite(BluePin, ((1<<_Resolution)-1)*B);
  analogWrite(WhitePin, ((1<<_Resolution)-1)*W);
}

void RGBWLED::begin(void) {
  pinMode(RedPin, OUTPUT);
  pinMode(BluePin, OUTPUT);
  pinMode(GreenPin, OUTPUT);
  pinMode(WhitePin, OUTPUT);
  analogWriteFrequency(RedPin, _PWMFrequency);
  analogWriteFrequency(WhitePin, _PWMFrequency);
  analogWriteFrequency(BluePin, _PWMFrequency);
  analogWriteFrequency(WhitePin, _PWMFrequency);
  analogWriteResolution(_Resolution);
}

int RGBWLED::fade(float H, float S, float I, uint16_t Duration)
{
  if (!IsFadeRunning)     //If we are not fading yet, we store current color value to calculate "fading path" 
  {
    IsFadeRunning = 1;
    Tzero = millis();
    
    H_a = (H-_hue)/Duration;
    H_b = _hue;
    S_a = (S - _saturation)/Duration;
    S_b = _saturation;
    I_a = (I - _intensity)/Duration;
    I_b = _intensity;
  }
  uint32_t t = millis();
  if (t-Tzero < Duration)
  {
    _hue = H_a * t + H_b;
    _saturation = S_a * t + S_b;
    _intensity = I_a * t + I_b;
    displayColor();
    return 0;
  }
  else 
  {
    _hue = H;
    _saturation = S;
    _intensity = I;
    displayColor();
    
    IsFadeRunning = false;
    return 1;
  }
}

