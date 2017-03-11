/*
 * OneLed - класс для управления индикаторами состоящими из одного источника свечения
 */ 

#ifndef ONE_LED_H
#define ONE_LED_H

#include <stdint.h>
#include "Arduino.h"

class OneLed
{
public:
  OneLed() {}
  OneLed(uint8_t pin, uint32_t settligTime);
  
  // settling time must be >= 255ms
  void setSmooth(uint32_t settligTime);
  void setBlink(uint32_t period, uint32_t impulseTime);
  
  void setEnable(bool isEnable);
  void toggle();
  void blink();
  
  void process();
  
private:
  void on();
  void off();
  
private:
  uint8_t   pin_;
  uint32_t  tbs_;
  
  uint32_t  blinkPeriod_;
  uint32_t  blinkImpulseTime_;
  uint32_t  blinkStartImpulse_;
  
  uint32_t  lastTime_;
  
  uint8_t currentValue_;
  uint8_t maxValue_;
  uint8_t minValue_;
  
  uint8_t state_;
  enum{
    STATE_DISABLE,
    STATE_ENABLE,
    STATE_BLINK
  };
};

#endif
