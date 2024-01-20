#ifndef TOFSensor_h
#define TOFSensor_h

#include "Arduino.h"
#include <Wire.h>
#include <VL53L1X.h>

class TOFSensor
{
  public:
    TOFSensor(int a);
    float read(int id);
  private:

      const uint8_t sensorCount = 2;
      const uint8_t xshutPins[2] = { 5,6 };
      VL53L1X sensors[2];
};

#endif