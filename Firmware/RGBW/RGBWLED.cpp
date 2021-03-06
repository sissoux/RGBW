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

