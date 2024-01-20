#ifndef DistanceSensor_h
#define DistanceSensor_h

#include "Arduino.h"

class DistanceSensor
{
  public:
    DistanceSensor(int pin);
    float read();
  private:
    int _pin, duration, dist, distNum;
    float distVal, distVltg;
};

#endif