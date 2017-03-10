#ifndef RTC_H
#define RTC_H

/*
 * Супер класс часов реального времени
 */

class Rtc
{
  public:
  Rtc() {}
  inline uint8_t hour();
  inline uint8_t minute();
  void setHour(uint8_t hour);
  void setMinute(uint8_t minute);
};

#endif
