#ifndef RGBWLED_H

#define RGBWLED_H
#include "Arduino.h"

#define PIST 1.0471975512
#define DPIST 2.09439510239
#define QPIST 4.18879020479
#define PI 3.14159265359

class RGBWLED
{
  protected:
    char RedPin, GreenPin, BluePin, WhitePin;
    char _Resolution = 16;
    float _PWMFrequency = 150;
  
  private:
    float H_a, H_b, S_a, S_b, I_a, I_b;   //Linear interpolation parameters for fading
    uint32_t Tzero = 0;

  public:
    float R, G, B, W, _hue, _saturation, _intensity;
    uint8_t IsFadeRunning;

    RGBWLED(char RPin, char GPin, char BPin, char WPin, char Resolution, float PWMFrequency);
    void begin();
    
    void setHue(float hue) {_hue = fmod(hue, 360);};
    void setSaturation(float S) {_saturation = S>0?(S<1?S:1):0;};
    void setIntensity(float I) {_intensity = I>0?(I<1?I:1):0;};
    
    float getHue() {return _hue;};
    float getSaturation() {return _saturation;};
    float getIntensity() {return _intensity;};
    
    void displayColor(void);
    void displayRGBWColor();
    uint8_t fade(float H, float S, float I, uint16_t Duration);
};

#endif
