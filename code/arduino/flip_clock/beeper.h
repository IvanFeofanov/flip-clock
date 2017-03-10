#ifndef BEEPER_H
#define BEEPER_H

#include "Arduino.h"

#include "alarm_signal.h"

class Beeper : public AlarmSignal
{
  public:
  Beeper() {}
  Beeper(uint8_t pin);
  void play(bool isEnable);
  
  private:
  uint8_t pin_;
  uint32_t lastTime_;
  bool flag_;
};

#endif
