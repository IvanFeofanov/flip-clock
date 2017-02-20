#include "clock.h"

OneButton Clock::buttonSelect_;
OneButton Clock::buttonPlus_;
OneButton Clock::buttonCancel_;

InternalRtc Clock::internalRtc_;

uint8_t Clock::state_;
bool Clock::isAlarm_;

void Clock::setup()
{
  //serial
  Serial.begin(9600);
  pinMode(13, OUTPUT);
    
  //buttons
  buttonSelect_ = OneButton(BUTTON_SELECT_PIN, true);
  buttonPlus_   = OneButton(BUTTON_PLUS_PIN, true);
  buttonCancel_ = OneButton(BUTTON_CANCEL_PIN, true);
  
  buttonSelect_.attachClick(selectCallback);
  buttonPlus_.attachClick(plusCallback);
  buttonCancel_.attachClick(cancelCallback);
  
  // alarm clock pot
  
  // light sensor
  // indicator
  // RTC
  internalRtc_ = InternalRtc();
  // player
  
  state_ = STATE_DEFAULT;
  isAlarm_ = false;
}

void Clock::loop()
{
  //buttons
  buttonSelect_.tick();
  buttonPlus_.tick();
  buttonCancel_.tick();
  
  //print time
  // alarm clock pot
  
  // light sensor
  // indicator
  // RTC
  internalRtc_.tick();
  Serial.print(internalRtc_.hour());
  Serial.print(" : ");
  Serial.println(internalRtc_.minute());
  
  // player
  
}

void Clock::selectCallback()
{
  if(state_ >= STATE_SET_MIN){
    state_ = STATE_DEFAULT;
  }else{
    state_++;
  }
}

void Clock::plusCallback()
{
  switch(state_)
  {
    case STATE_SET_HOUR:
    internalRtc_.setHour(internalRtc_.hour() + 1);
    break;
    
    case STATE_SET_MIN:
    internalRtc_.setMinute(internalRtc_.minute() + 1);
    break;
    
    default:
    break;
  }
}

void Clock::cancelCallback()
{
  //disable alarm
  if(isAlarm_){
    isAlarm_ = false;
  }
}
