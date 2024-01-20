#include "Arduino.h"
#include "MotorDriver.h"

MotorDriver::MotorDriver(int lctrl, int rctrl, int pout)
{
  pinMode(pout, OUTPUT);
  pinMode(lctrl, OUTPUT);
  pinMode(rctrl, OUTPUT);
  _lctrl = lctrl;
  _rctrl = rctrl;
  _pout = pout;
  _pout2 = 10;
  analogWrite(_pout, 0);
  analogWrite(_pout2, 0);
  running = false; 

}
void MotorDriver::start() {
  if(!running){
    running = true; 
    analogWrite(_pout, dutyCycle);
    analogWrite(_pout2, dutyCycle);
  }
}
void MotorDriver::turnLeft() {
  start();
  // if(!left){
  //   left = true; 
    digitalWrite(_lctrl, HIGH);
    digitalWrite(_rctrl, HIGH);
  // }
  
  // analogWrite(_pout, dutyCycle);
  // delay(time);
  // park();
}
void MotorDriver::turnRight() {
  start();
  // if(!right){
  //   right = true; 
    digitalWrite(_lctrl, LOW);
    digitalWrite(_rctrl, LOW);
  // }

  // analogWrite(_pout, dutyCycle);
  // delay(time);
  // park();
}
void MotorDriver::forward(int time) {
  start();
  // if(!forwardflag){
  //   forwardflag = true;
    digitalWrite(_lctrl, LOW);
    digitalWrite(_rctrl, HIGH);
  // }

  // analogWrite(_pout, dutyCycle);
}
void MotorDriver::reverse(int time) {
  start();
  digitalWrite(_lctrl, HIGH);
  digitalWrite(_rctrl, LOW);
  // analogWrite(_pout, dutyCycle);
  // delay(time);
  // park();
}

void MotorDriver::park() {
  analogWrite(_pout, 0);
  analogWrite(_pout2, 0);
  running = false;
}