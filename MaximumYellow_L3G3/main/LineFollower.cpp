#include "Arduino.h"
#include "LineFollower.h"

LineFollower::LineFollower(int pin)
{
  pinMode(pin, INPUT);
  _pin = pin;
}

int LineFollower::read()
{
  if (digitalRead(_pin) == HIGH){
    val = 1;
  } else if (digitalRead(_pin) == LOW){
    val = 0;
  }
 

  return val; 
}
float LineFollower::analog()
{
  return analogRead(_pin); 
}
