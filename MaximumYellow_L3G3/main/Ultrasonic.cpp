#include "Arduino.h"
#include "Ultrasonic.h"

Ultrasonic::Ultrasonic(int trigPin, int echoPin)
{
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  _echoPin = echoPin;
  _trigPin = trigPin;

  // PMC->PMC_PCER1 |= PMC_PCER1_PID36;                 // Enable Clock to PWM module
  // PIOC->PIO_ABSR |= PIO_PC3B_PWMH0;                  // Assign C3 to PWM module (Periph_B)
  // PIOC->PIO_PDR |= PIO_PDR_P3;                       // Release C3 from the PIO module
  // REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(84);  //Set PWM clock 1MHz (Mck/84)
  // PWM->PWM_CH_NUM[0].PWM_CMR |= PWM_CMR_CPRE_CLKA    // Set the clock source as CLKA
  //                               | PWM_CMR_CPOL;      //Set output polarity be high.
  // PWM->PWM_CH_NUM[0].PWM_CPRD = 60000 - 1;           //Set PWM freq 1MHz/(60000) = 66.667Hz
  // PWM->PWM_CH_NUM[0].PWM_CDTY = 10 - 1;              // Set PWM duty cycle
  // PWM->PWM_ENA = PWM_ENA_CHID0;
  
}

int Ultrasonic::read()
{
  
  digitalWrite(_trigPin, HIGH);
  delayMicroseconds(100);
  digitalWrite(_trigPin, LOW);
  duration = pulseIn(_echoPin, HIGH, 5000);
  // dist = 343 * (duration/2);
  return duration;

}