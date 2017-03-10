#include "beeper.h"

Beeper::Beeper(uint8_t pin)
{
  pin_ = pin;
  flag_ = false;
  lastTime_ = 0;
  pinMode(pin, OUTPUT);
}

void Beeper::play(bool isEnable)
{
  if(isEnable){
    if(millis() - lastTime_ > 500){
      if(flag_){
        tone(pin_, 220);
        flag_ = false;
      }else{
        noTone(pin_);
        flag_ = true;
      }
      lastTime_ = millis();
    }
  }else{
    noTone(pin_);
  }
}

