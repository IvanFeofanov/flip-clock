#include "internal_rtc.h"

#include <Arduino.h>

InternalRtc::InternalRtc()
{
  hour_   = 0;
  minute_ = 0;
  msec_   = millis();
}

void InternalRtc::tick()
{
  uint32_t now = millis();
  
  if(now - msec_ >= 60000){
    if(minute_ < 59){
      minute_++;
    }else{
      minute_ = 0;
      if(hour_ < 23){
        hour_++;
      }else{
        hour_ = 0;
      }
    }
    
    msec_ = now;
  }
}

void InternalRtc::setHour(uint8_t hour)
{
  if(hour > 23){
    hour_ = 0;
  }else{
    hour_ = hour;
  }
  
  msec_ = millis();
}

void InternalRtc::setMinute(uint8_t minute)
{
  if(minute > 59){
    minute_ = 0;
  }else{
    minute_ = minute;
  }
  
  msec_ = millis();
}
