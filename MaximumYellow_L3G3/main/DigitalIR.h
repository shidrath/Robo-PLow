#ifndef DigitalIR_h
#define DigitalIR_h

#include "Arduino.h"

class DigitalIR
{
  public:
    DigitalIR(int pin);
    int read();
  private:
    int _pin;
};

#endif