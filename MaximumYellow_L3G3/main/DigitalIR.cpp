#include "Arduino.h"
#include "DigitalIR.h"

DigitalIR::DigitalIR(int pin)
{
  pinMode(pin, INPUT);
  _pin = pin;
}

int DigitalIR::read()
{
  if (digitalRead(_pin) == LOW){
    return 0;
  } else if (digitalRead(_pin) == HIGH){
    return 1;
  }
}