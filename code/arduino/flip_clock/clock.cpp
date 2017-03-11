#include "clock.h"

OneButton Clock::modeButton_;
OneButton Clock::superButton_;

OneLed Clock::hourLed_;
OneLed Clock::minuteLed_;
OneLed Clock::alarmClockLed_;

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
  
  // light sensor
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  
  // backlight
  hourLed_       = OneLed(BACKLIGHT_HOUR_PIN, 255);
  hourLed_.setBlink(1024, 512);
  minuteLed_     = OneLed(BACKLIGHT_MINUTE_PIN, 255);
  minuteLed_.setBlink(1024, 512);
  alarmClockLed_ = OneLed(BACKLIGHT_ALARM_CLOCK_PIN, 255);
  
  // indicator
  
  // RTC
  rtc_ = InternalRtc();
  
  // alarm clock 
  isAlarmClockEnable_ = false;
  isWakeUp_           = false;
  wakeUpTimeHour_     = 0;
  wakeUpTimeMinute_   = 0;
  pinMode(POT_ALARM_CLOCK_PIN, INPUT); // potentiometer
  
  // alarm signal
  alarmSignal_ = Beeper(WAKEUP_SIGNAL_PIN);
  
  state_ = STATE_DEFAULT;
}

void Clock::loop()
{
  // buttons
  modeButton_.tick();
  superButton_.tick();
  
  // light sensor
  bool isLight = getIsLight();
  
  // backlight
  backlightProcess(isLight);

  // indicator
  
  // RTC
  rtc_.tick();
  printCurrentTime();
  
  // alarm clock
  alarmClockTick();
//  printWakeUpTime();
  
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

bool Clock::getIsLight()
{
  static bool isLight = false;
  
  uint16_t value = analogRead(LIGHT_SENSOR_PIN);
  if(value >= LIGHT_SENSOR_DIS_THRES){
    isLight = true;
  }
  if(value <= LIGHT_SENSOR_EN_THRES){
    isLight = false;
  }
  
  return isLight;
}

void Clock::backlightProcess(bool isLight)
{ 
  // hour and minutes
  switch(state_)
  {
    case STATE_DEFAULT:
    hourLed_.setEnable(isLight);
    minuteLed_.setEnable(isLight);
    break;
    
    case STATE_SET_HOUR:
    hourLed_.blink();
    minuteLed_.setEnable(isLight);
    break;
    
    case STATE_SET_MIN:
    hourLed_.setEnable(isLight);
    minuteLed_.blink();
    break;
  }
  
  // alarm clock
  alarmClockLed_.setEnable(isAlarmClockEnable_ && !isLight);
  
  hourLed_.process();
  minuteLed_.process();
  alarmClockLed_.process();
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
