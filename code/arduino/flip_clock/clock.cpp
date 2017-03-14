#include "clock.h"

OneButton Clock::modeButton_;
OneButton Clock::plusButton_;

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
  plusButton_  = OneButton(PLUS_BUTTON_PIN, BUTTON_ACTIVE_LOW);
  plusButton_.attachClick(plusCallback);
  
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
  pinMode(POT_ALARM_CLOCK_PIN, INPUT); // potentiometer
  
  isAlarmClockEnable_ = false;
  isWakeUp_           = false;
  wakeUpTimeHour_     = 0;
  wakeUpTimeMinute_   = 0;

  // alarm signal
  alarmSignal_ = Beeper(WAKEUP_SIGNAL_PIN);
  
  state_ = STATE_DEFAULT;
}

void Clock::loop()
{
  // buttons
  modeButton_.tick();
  plusButton_.tick();
  
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
  alarmSignal_.play(isAlarmClockEnable_ && isWakeUp_);  
}

void Clock::alarmClockTick()
{
  
  // read alarm clock time
  uint16_t value = readAnalogMeanValue(POT_ALARM_CLOCK_PIN, 5);
  wakeUpTimeHour_ = value / (1023 / 12);
  wakeUpTimeMinute_  = (value % (1023 / 12)) * 60 / (1023 / 12);
  
//  if((wakeUpTimeHour_ * 60 + wakeUpTimeMinute_) -
//     (rtc_.minute
}

uint16_t Clock::readAnalogMeanValue(uint8_t pin, uint8_t n)
{
  uint32_t value = 0;
  for(int i = 0; i < n; i++){
    value += analogRead(pin);
  }
  value /= n;
  return value;
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
    hourLed_.setEnable(!isLight);
    minuteLed_.setEnable(!isLight);
    break;
    
    case STATE_SET_HOUR:
    hourLed_.blink();
    minuteLed_.setEnable(!isLight);
    break;
    
    case STATE_SET_MIN:
    hourLed_.setEnable(!isLight);
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
  
//  if(abs((hour * 60 + minute) - (wakeUpTimeHour_ * 60 + wakeUpTimeMinute_)) > WAKEUP_EPSILON){
    hour   = wakeUpTimeHour_;
    minute = wakeUpTimeMinute_;
    
    Serial.print("WakeUp time: ");
    Serial.print(wakeUpTimeHour_);
    Serial.print(" : ");
    Serial.println(wakeUpTimeMinute_);
//  }
}

void Clock::modeCallback()
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
    rtc_.setHour(rtc_.hour() + 1);
    break;
    
    case STATE_SET_MIN:
    rtc_.setMinute(rtc_.minute() + 1);
    break;
  }
}
