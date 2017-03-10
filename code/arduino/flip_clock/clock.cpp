#include "clock.h"

OneButton Clock::modeButton_;
OneButton Clock::superButton_;

InternalRtc Clock::rtc_;
Beeper      Clock::alarmSignal_;

bool Clock::isAlarmClockEnable_;
bool Clock::isWakeUp_;
uint8_t Clock::wakeUpTimeHour_;
uint8_t Clock::wakeUpTimeMinute_;

uint8_t Clock::state_;

void Clock::setup()
{
  //serial
  Serial.begin(9600);
  
  //debug
  pinMode(LED_PIN, OUTPUT);
    
  //buttons
  modeButton_   = OneButton(MODE_BUTTON_PIN, BUTTON_ACTIVE_LOW);
  modeButton_.attachClick(modeCallback);
  superButton_  = OneButton(SUPER_BUTTON_PIN, BUTTON_ACTIVE_LOW);
  superButton_.attachClick(superCallback);
  
  // alarm clock 
  isAlarmClockEnable_ = false; 
  isWakeUp_           = false;
  wakeUpTimeHour_     = 0;
  wakeUpTimeMinute_   = 0;
  pinMode(POT_ALARM_CLOCK_PIN, INPUT); // potentiometer

  // backlight
  pinMode(LIGHT_SENSOR_PIN, INPUT);     // light sensor
  pinMode(BACKLIGHT_HOUR_PIN, OUTPUT);
  pinMode(BACKLIGHT_MINUTE_PIN, OUTPUT);
  pinMode(BACKLIGHT_ALARM_PIN, OUTPUT);
  
  // indicator
  
  // RTC
  rtc_ = InternalRtc();
  
  // alarm signal
  alarmSignal_ = Beeper(9);
  
  state_ = STATE_DEFAULT;
}

void Clock::loop()
{
  // buttons
  modeButton_.tick();
  superButton_.tick();
  
  // alarm clock
  alarmClockTick();
  printWakeUpTime();
  
  // light sensor
  // backlight
  // indicator
  
  // RTC
  rtc_.tick();
  printCurrentTime();
  
  // alarm signal
  alarmSignal_.play(isWakeUp_);
  
  
}

void Clock::alarmClockTick()
{
  // read alarm clock time
  uint16_t value = analogRead(POT_ALARM_CLOCK_PIN);
  wakeUpTimeHour_ = value / (1023 / 24);
  wakeUpTimeMinute_  = (value % (1023 / 24)) * 60 / (1023 / 24);
}

void Clock::backlightProcess()
{
 
}

void Clock::printCurrentTime()
{
  static uint8_t hour   = 0;
  static uint8_t minute = 0;
  
  if(hour != rtc_.hour() || minute != rtc_.minute()){
    hour = rtc_.hour();
    minute = rtc_.minute();
    
    Serial.print("Current time: ");
    Serial.print(rtc_.hour());
    Serial.print(" : ");
    Serial.println(rtc_.minute());
  }
}

void Clock::printWakeUpTime()
{
  static uint8_t hour = 0;
  static uint8_t minute = 0;
  
  if(abs((hour * 60 + minute) - (wakeUpTimeHour_ * 60 + wakeUpTimeMinute_)) > WAKEUP_EPSILON){
    hour   = wakeUpTimeHour_;
    minute = wakeUpTimeMinute_;
    
    Serial.print("WakeUp time: ");
    Serial.print(wakeUpTimeHour_);
    Serial.print(" : ");
    Serial.println(wakeUpTimeMinute_);
  }
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
    rtc_.setHour(rtc_.hour() + 1);
    break;
    
    case STATE_SET_MIN:
    rtc_.setMinute(rtc_.minute() + 1);
    break;
    
    default:
    isWakeUp_ = false;
    break;
  }
}
