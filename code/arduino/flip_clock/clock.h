#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>

#include "config.h"
#include "OneButton.h"
#include "one_led.h"
#include "internal_rtc.h"
#include "beeper.h"

class Clock
{
  public:
  static void setup();
  static void loop();
  
  private:
  static void modeCallback();
  static void superCallback();
  static void alarmClockTick();
  static bool getIsLight();
  static void backlightProcess(bool isLight);
  static void printCurrentTime();
  static void printWakeUpTime();
  
  // buttons
  static OneButton modeButton_;
  static OneButton superButton_;
  
  // light sensor
  
  // backlight
  static OneLed hourLed_;
  static OneLed minuteLed_;
  static OneLed alarmClockLed_;
  
  // indicator
  
  // RTC
  static InternalRtc rtc_; 
  
  // alarm signal
  static Beeper alarmSignal_;
  
  // alarm clock
  static bool isAlarmClockEnable_;
  static bool isWakeUp_;
  static uint8_t wakeUpTimeHour_;
  static uint8_t wakeUpTimeMinute_;
  
  static uint8_t state_;
  enum{
    STATE_DEFAULT,
    STATE_SET_HOUR,
    STATE_SET_MIN
  };
};



#endif
