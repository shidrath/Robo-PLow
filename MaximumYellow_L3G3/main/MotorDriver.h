#ifndef MotorDriver_h
#define MotorDriver_h

#include "Arduino.h"

class MotorDriver
{
  public:
    MotorDriver(int lctrl, int rctrl, int pout);
    void start();
    void turnLeft();
    void turnRight();
    void forward(int time= 200);
    void reverse(int time= 100);
    void park();
  private:
    bool running = false, left = false, right = false, forwardflag = false; 
    int _lctrl, _rctrl, _pout, _pout2;
    const int dutyCycle = 150;
};

#endif