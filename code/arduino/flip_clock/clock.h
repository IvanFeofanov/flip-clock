#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>

#include "config.h"
#include "./OneButton.h"
#include "./internal_rtc.h"

class Clock
{
  public:
  static void setup();
  static void loop();
  
  private:
  static void selectCallback();
  static void plusCallback();
  static void cancelCallback();
  
  // buttons
  static OneButton buttonSelect_;
  static OneButton buttonPlus_;
  static OneButton buttonCancel_;
  
  // alarm clock pot
  // light sensor
  // indicator
  // RTC
  static InternalRtc internalRtc_;
  // player
  
  static uint8_t state_;
  enum{
    STATE_DEFAULT,
    STATE_SET_HOUR,
    STATE_SET_MIN
  };
  
  static bool isAlarm_;
};



#endif
