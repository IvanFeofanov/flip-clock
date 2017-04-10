/*
 * Класс часов реального времени на микросхеме DS1307
 */

#ifndef DS1307_RTC_H
#define DS1307_RTC_H

#include <stdint.h>

#include <Arduino.h>

#include "rtc.h"

class Ds1307Rtc : public Rtc
{
public:
  Ds1307Rtc();
  void tick();
  inline uint8_t hour() { return hour_; }
  inline uint8_t minute() { return minute_; }
  void setHour(uint8_t hour);
  void setMinute(uint8_t minute);
  
private:
  void readTime();
  void resetSeconds();
  
private:
  enum{ I2C_ADDRESS = 0b1101000 };
  
  uint32_t lastUpdateTime_;
  uint8_t  hour_;
  uint8_t  minute_;
  
private:
  enum{
    REGISTER_SECONDS,
    REGISTER_MINUTES,
    REGISTER_HOURS
  };
  
  union Registers
  {
    uint8_t bytes[3];
    
    struct{
      //0x00 - seconds
      uint8_t seconds : 4;
      uint8_t decSeconds : 3;
      uint8_t ch: 1;
      
      //0x01 - minutes
      uint8_t minutes : 4;
      uint8_t decMinutes : 3;
      uint8_t : 1;
      
      //0x02 - hours
      uint8_t hours : 4;
      uint8_t decHours : 2;
      uint8_t mode : 1;
      uint8_t : 1;
    }fields; 
  };
};

#endif
