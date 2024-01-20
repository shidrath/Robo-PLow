#include "Arduino.h"
#include "DistanceSensor.h"

// requires analong pin
DistanceSensor::DistanceSensor(int pin)
{
  pinMode(pin, INPUT);
  _pin = pin;
}


// verify if the table is still correct (ie voltages align with distance)
float DistanceSensor::read()
{
  analogReference(AR_DEFAULT);
  distVal = analogRead(_pin);
  distVltg = (distVal/1023.0);
  return distVltg;

  if (distVltg > 1.65){
    distNum = 0;
  } else if (distVltg > 1.12 & distVltg < 1.64){
    distNum = 2;
  } else if (distVltg > 0.76 & distVltg < 1.11){
    distNum = 4;
  } else if (distVltg > 0.63 & distVltg < 0.75 ){
    distNum = 6;
  } else if (distVltg > 0.53 & distVltg < 0.62){
    distNum = 8;
  } else if (distVltg > 0.45  & distVltg < 0.52){
    distNum = 10;
  } else if (distVltg > 0.38 & distVltg < 0.44){
    distNum = 12;
  } else if (distVltg > 0.34 & distVltg < 0.37){
    distNum = 14;
  } else if (distVltg > 0.28 & distVltg < 0.33){
    distNum = 16;
  }

  return distNum; 
}