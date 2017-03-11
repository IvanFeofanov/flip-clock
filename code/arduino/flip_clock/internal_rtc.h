#ifndef INTERNAL_RTC_H
#define INTERNAL_RTC_H

#include <stdint.h>

#include "rtc.h"

/*
 * Класс часов реального времени вычисляющий время без использования внешних микросхем,
 * по средствам функции millis()
 */

class InternalRtc : public Rtc
{
public:
  InternalRtc();
  void tick();
  inline uint8_t hour() { return hour_; }
  inline uint8_t minute() { return minute_; }
  void setHour(uint8_t hour);
  void setMinute(uint8_t minute);
  
private:
  uint32_t msec_;
  uint8_t  hour_;
  uint8_t  minute_;
};

#endif
