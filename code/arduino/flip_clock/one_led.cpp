#include "one_led.h"

OneLed::OneLed(uint8_t pin, uint32_t settligTime)
{
  pin_              = pin;
  
  blinkPeriod_      = 0;
  blinkImpulseTime_ = 0;
  blinkStartImpulse_= 0;
  
  lastTime_         = 0;
  
  currentValue_     = 0;
  maxValue_         = 255;
  minValue_         = 0;
  
  pinMode(pin, OUTPUT);
  setEnable(false);
  setSmooth(settligTime);
}

// settling time must be >= 255ms
void OneLed::setSmooth(uint32_t settlingTime)
{
  tbs_ = settlingTime / maxValue_;
}

void OneLed::setBlink(uint32_t period, uint32_t impulseTime)
{
  blinkPeriod_        = period;
  blinkImpulseTime_   = impulseTime;
}

void OneLed::setEnable(bool isEnable)
{ 
  state_ = isEnable ? STATE_ENABLE : STATE_DISABLE;
}

void OneLed::toggle()
{
  if(state_ == STATE_DISABLE){
    state_ = STATE_ENABLE;
  }else{
    state_ = STATE_DISABLE;
  }
}

void OneLed::blink()
{
  state_ = STATE_BLINK;
}

void OneLed::process()
{
  switch(state_)
  {
    case STATE_DISABLE:
      off();
    break;
    
    case STATE_ENABLE:
      on();
    break;
    
    case STATE_BLINK:
    if(millis() - blinkStartImpulse_ < blinkImpulseTime_){
      on();
    }else{
      off();
    }
    
    if(millis() - blinkStartImpulse_ >= blinkPeriod_){ 
      blinkStartImpulse_ = millis();
    }
    break;
  }
}

void OneLed::on()
{
  if(currentValue_ < maxValue_){
    if(tbs_ > 0){      // smoothly
      if(millis() - lastTime_ > tbs_){
        currentValue_ += 1;
        lastTime_      = millis();
      } 
    }else{
      currentValue_ = maxValue_;
    }
    analogWrite(pin_, currentValue_);
  }
}

void OneLed::off()
{
  if(currentValue_ > minValue_){
    if(tbs_ > 0){      // smoothly
      if(millis() - lastTime_ > tbs_){
        currentValue_ -= 1;
        lastTime_      = millis();
      }    
    }else{
      currentValue_ = minValue_;
    }
    analogWrite(pin_, currentValue_);
  }
}
