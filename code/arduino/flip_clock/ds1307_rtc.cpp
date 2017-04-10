#include "ds1307_rtc.h"

#include <Wire.h>

Ds1307Rtc::Ds1307Rtc()
{
  lastUpdateTime_ = 0;
  hour_   = 0;
  minute_ = 0;
  
  Wire.begin();
}

void Ds1307Rtc::tick()
{
  if(millis() - lastUpdateTime_ >= 1000){
    readTime();
    lastUpdateTime_ = millis();
  }  
}

void Ds1307Rtc::setHour(uint8_t hour)
{
  resetSeconds();
  
  uint8_t nextHour = (hour_ + 1) % 24;
  
  Registers registers;
  
  registers.fields.decHours = nextHour / 10;
  registers.fields.hours = nextHour - (nextHour / 10) * 10;
  registers.fields.mode = 0;  // 12-hour mode
  
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(REGISTER_HOURS);
  Wire.write(registers.bytes[REGISTER_HOURS]);
  Wire.endTransmission();
  
  readTime();
}

void Ds1307Rtc::setMinute(uint8_t minute)
{
  resetSeconds();
  
  uint8_t nextMinute = (minute_ + 1) % 60;
  
  Registers registers;
   
  registers.fields.decMinutes = nextMinute / 10;
  registers.fields.minutes = nextMinute - (nextMinute / 10) * 10;
  
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(REGISTER_MINUTES);
  Wire.write(registers.bytes[REGISTER_MINUTES]);
  Wire.endTransmission();
  
  readTime();
}

void Ds1307Rtc::readTime()
{
   Registers registers; 

  //чтение данных
  for(uint8_t i = 0; i < 3; i++){
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(i);
    Wire.endTransmission();
    Wire.requestFrom(I2C_ADDRESS, 1);
    if(Wire.available()){
      registers.bytes[i] = Wire.read();
    }else{
      Serial.println("DS1307: slave error");
      return;
    }
  }
  
//  // seconds
//  second_ = data.fields.dec_seconds * 10 + data.fields.seconds;
  
  // minutes
  minute_ = registers.fields.decMinutes * 10 + registers.fields.minutes;
  
  // hours
  if(registers.fields.mode == 0){ //24-hour mode
    hour_ = registers.fields.decHours * 10 + registers.fields.hours;
  }else{  //12-hour mode
    hour_ = (registers.fields.decHours & 0x01) * 10 + registers.fields.hours;
  }
}

void Ds1307Rtc::resetSeconds()
{
  Registers registers;
  
  registers.fields.decSeconds = 0;
  registers.fields.seconds = 0;
  
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(REGISTER_SECONDS);
  Wire.write(registers.bytes[REGISTER_SECONDS]);
  Wire.endTransmission();
}
