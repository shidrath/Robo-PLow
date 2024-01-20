#ifndef LineFollower_h
#define LineFollower_h

#include "Arduino.h"

class LineFollower
{
  public:
    LineFollower(int pin);
    int read();
    float analog();
  private:
    int _pin, val;
};

#endif