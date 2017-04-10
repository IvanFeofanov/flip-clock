#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>

#include "config.h"
#include "OneButton.h"
#include "one_led.h"
#include "internal_rtc.h"
#include "ds1307_rtc.h"

class Clock
{
  public:
  static void setup();
  static void loop();
  
  private:
  static void modeCallback();
  static void plusCallback();
  static uint16_t readAnalogMeanValue(uint8_t pin, uint8_t n);
  static bool getIsLight();
  static void backlightProcess(bool isLight);
  static void printCurrentTime();
  
  // buttons
  static OneButton modeButton_;
  static OneButton plusButton_;
  
  // light sensor
  
  // backlight
  static OneLed hourLed_;
  static OneLed minuteLed_;
  
  // indicator
  
  // RTC
#ifdef USE_DS1307
  static Ds1307Rtc  rtc_;
#else
  static InternalRtc rtc_; 
#endif

  // states
  static uint8_t state_;
  enum{
    STATE_DEFAULT,
    STATE_SET_HOUR,
    STATE_SET_MIN
  };
};

#endif
