#include "clock.h"

OneButton Clock::modeButton_;
OneButton Clock::superButton_;

InternalRtc Clock::internalRtc_;

uint8_t Clock::state_;
bool Clock::isAlarm_;

void Clock::setup()
{
  //serial
  Serial.begin(9600);
  
  //debug
  pinMode(LED_PIN, OUTPUT);
    
  //buttons
  modeButton_   = OneButton(MODE_BUTTON_PIN, BUTTON_ACTIVE_LOW);
  superButton_  = OneButton(SUPER_BUTTON_PIN, BUTTON_ACTIVE_LOW);
  
  modeButton_.attachClick(modeCallback);
  superButton_.attachClick(superCallback);
  
  // alarm clock potentiometer
  pinMode(POT_ALARM_CLOCK_PIN, INPUT);
  // light sensor
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  
  // indicator
  
  // RTC
  internalRtc_ = InternalRtc();
  
  // player
  
  state_ = STATE_DEFAULT;
  isAlarm_ = false;
}

void Clock::loop()
{
  // buttons
  modeButton_.tick();
  superButton_.tick();
  
  // read alarm clock time
  uint16_t value = analogRead(POT_ALARM_CLOCK_PIN);
  uint8_t hour = value / (1023 / 24);
  uint8_t minute = (value % (1023 / 24)) * 60 / (1023 / 24);
  Serial.print(internalRtc_.hour());
  Serial.print(" : ");
  Serial.println(internalRtc_.minute());
  
  // light sensor
  // backlight
  // indicator
  // RTC
  internalRtc_.tick();
  
  // player
  
}

void Clock::modeCallback()
{
  if(state_ >= STATE_SET_MIN){
    state_ = STATE_DEFAULT;
  }else{
    state_++;
  }
}

void Clock::superCallback()
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
    isAlarm_ = false;  // disable alarm
    break;
  }
}
