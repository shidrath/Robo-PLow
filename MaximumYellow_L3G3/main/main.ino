#include <Wire.h>
#include <VL53L1X.h>
#include "Arduino.h"
#include "DigitalIR.h"
#include "LineFollower.h"
#include "DistanceSensor.h"
#include "Ultrasonic.h"
#include "MotorDriver.h"

// note, the imu and the tof sensors do not work when the motors have power and are on. 
// (ie not an issue with the PMW signal or aurduino. likely the current draw by the motors)

// the pins for the line follower
const int 
lmPin = A0,
lPin = A1,
rPin = A2,
rmPin = A3;

// initilising the line followers
LineFollower LineFollowerL(lPin);
LineFollower LineFollowerR(rPin);
LineFollower LineFollowerLM(lmPin);
LineFollower LineFollowerRM(rmPin);

// initilising the distance sensors
DistanceSensor d_sensor1(A6);
DistanceSensor d_sensor2(A7);

// initilising the ultrasonic sensors
Ultrasonic ultrasonic1(3, 2);
Ultrasonic ultrasonic2(8, 9);

// initilising the motors
MotorDriver drive(12, 13, 11);

// const uint8_t sensorCount = 2;
// const uint8_t xshutPins[sensorCount] = { 5, 6 };
// VL53L1X sensors[sensorCount];

// for the serial input
int incomingByte = 0;

// for reading in the serial input
bool testing = false; 

// for the state, random turn counter, and the itteration counter
int state = 0, randomturn = 0, turncount = 0, timeout = 10000;

// debouncing for distance sensor
int right_debounce = 0, left_debounce = 0;
int right_line_debounce = 0, left_line_debounce = 0;

// watch dog timer flags
bool dl_flag = false, dr_flag = false;
bool ll_flag = false, lr_flag = false, llm_flag = false, lrm_flag = false;
bool uf_flag = false, ur_flag = false; 

void setup() {
  while (!Serial) {}
  Serial.begin(115200);
  Wire.begin();
}
void watchdogSetup()
{
   watchdogEnable(timeout); // timeout set to .5 seconds
}

void loop() {
  // in testing mode
  if (testing == true) {
    watchdogDisable(); 
    if (Serial.available() > 0){
       incomingByte = Serial.read(); // read in serial input from computer
      // Serial.println(incomingByte, DEC);
      if(incomingByte == 97){ // a left
        drive.turnLeft();
      }
      if(incomingByte == 100){ // d right
        drive.turnRight();
      }
      if(incomingByte == 119){ // w foward
        drive.forward();
      }
      if(incomingByte == 115){ // s back
        drive.reverse();
      }
      if(incomingByte == 49){ // is 1
        testWheels();
      }
      if(incomingByte == 50){ // is 2
        testUltraSonic();
      }
      if(incomingByte == 53){ // is 5
        testDistance();
      }
      if(incomingByte == 54){ // is 6
        testLineFollower();
      }
      if(incomingByte == 55){ // is 7
        testAll();
      }
      if(incomingByte == 56){ // is 8 
        watchdogEnable(timeout); 
        testing = false; // enter or resume the running mode
      }
    }
  }
  else { 
    if (Serial.available() > 0){
       incomingByte = Serial.read();
      if(incomingByte == 57){ // is 9
        drive.park();                               // stop the car
        testing = true;                             // return to the testing mode
        
      }
    }
    if(!testing){
      stateMachine(); // loop the state machine
    } 
  }
}

// the wheel test 
void testWheels() {
  Serial.println("start testing motors");
  drive.forward();
  delay(3000);
  drive.reverse();
  delay(3000);
  drive.turnLeft();
  delay(3000);
  drive.turnRight();
  delay(3000);
  drive.park();
  Serial.println("end testing motors");
}

// the ultrasonic test
void testUltraSonic(){
  Serial.println("start testing ultrasonic");
  for(int i = 0; i < 100; i++){
    float out1 = ultrasonic1.read();
    float out2 = ultrasonic2.read();
    Serial.print("Ultrasonic  ");
    Serial.print(out1);
    Serial.print("  ");
    Serial.print(out2);
    Serial.println();
    delay(100);
  }
  Serial.println("end testing ultrasonic");
}
// test the distance sensors
void testDistance(){
  Serial.println("start testing distance sensor");
  for(int i = 0; i < 50; i++){
    float d1 = d_sensor1.read();
    float d2 = d_sensor2.read();
    Serial.print("Distance ");
    Serial.print(d1);
    Serial.print(" ");
    Serial.print(d2);
    Serial.println();
    delay(100);
  }
  Serial.println("end testing distance sensor");  
}

// test the line follower 
void testLineFollower(){
  Serial.println("start testing line follower sensor");
  for(int i = 0; i < 50; i++){
    float LLine =  LineFollowerL.analog();
    float RLine =  LineFollowerR.analog();
    float LMLine =  LineFollowerLM.analog();
    float RMLine =  LineFollowerRM.analog();
    Serial.print("RLine: ");
    Serial.print(RLine);
    Serial.print(" LLine: ");
    Serial.print(LLine);
    Serial.print("RMLine: ");
    Serial.print(RMLine);
    Serial.print(" LMLine: ");
    Serial.print(LMLine);
    Serial.println();
    delay(100);
  }
  Serial.println("end testing line follower sensor");  
}

// test all the sensors concurently
void testAll(){
  Serial.println("start testing");  
  for(int i = 0; i < 100; i++){
    Serial.print("Itteration #");
    Serial.print(i);
    Serial.println();
    float d1 = d_sensor1.read();
    float d2 = d_sensor2.read();
    Serial.print("Distance ");
    Serial.print(d1);
    Serial.print(" ");
    Serial.print(d2);
    Serial.println();
    float LLine =  LineFollowerL.analog();
    float RLine =  LineFollowerR.analog();
    Serial.print("RLine: ");
    Serial.print(RLine);
    Serial.print(" LLine: ");
    Serial.print(LLine);
    Serial.println();
    float uout1 = ultrasonic1.read();
    float uout2 = ultrasonic2.read();
    Serial.print("Ultrasonic  ");
    Serial.print(uout1);
    Serial.print("  ");
    Serial.print(uout2);
    
    Serial.println();
    Serial.println();
    // getIMU();
  }
  Serial.println("end testing");
}

// set wd in startup
// set flag on sensor call
// reset flag when sensor call done
// reset watchdog timer when sensor call done
// if timeout then flag still set so do not do recall that sensor 

void stateMachine(){
    float dl, dr, ll, lr, llm, lrm, uf, ur;

    // read distance sensors
    if(!dl_flag){                   // check if the flag is set
      dl_flag = true;               // set the flag
      dl = d_sensor1.read();  // read the sensor, if the wd timer resets then the flag will still be set and this sensor will be skipped
      dl_flag = false;              // unset the flag
      watchdogReset();               // reset the wd timer
    }

    if(!dr_flag){              // check if the right distance sensor flag is set
      dr_flag = true;          // set the flag dr_flag
      dr = d_sensor2.read();   // read the sensor, if the wd timer resets then the flag will still be set and this sensor will be skipped
      dr_flag = false;        // unset the flag
      watchdogReset();       // reset wd timer
    }

    // read line follower
    if(!ll_flag){               // check to see if the left linefollower flag is set
      ll_flag = true;           // set the flag ll_flag
      ll =  LineFollowerL.analog(); // read the analog value of the left line follower
      ll_flag = false;          // unset the left line follower flag
      watchdogReset();        // reset wd timer
    }
    if(!lr_flag){               // check to see if the right linefollower flag is set
      lr_flag = true;            // set the flag 
      lr =  LineFollowerR.analog();  // read the analog value of the right line follower
      lr_flag = false;         // unset the right line follower flag
      watchdogReset();      // reset wd timer
    } 
    if(!llm_flag){               // check to see if the left middle linefollower flag is set
      llm_flag = true;      // set the flag 
      llm =  LineFollowerLM.analog(); // read the analog value of the left middle line follower
      llm_flag = false;    // unset the left middle linefollower flag
      watchdogReset(); // reset the wd timer
    }
    if(!lrm_flag){              // check to see if the left right linefollower flag is set
      lrm_flag = true;        // set the flag
      lrm =  LineFollowerRM.analog(); // read the analog value of the right middle line follower
      lrm_flag = false;       // unset the  right middle line follower
      watchdogReset(); //    reset the wd timer
    }
    
    // read ultrasonic sensors
    if(!uf_flag){           // check to see if the ultrasonic front sensor flag is set
      uf_flag = true;        // set the ultra sonic front flag
      uf = ultrasonic1.read(); 
      uf_flag = false;        // unset flag
      watchdogReset();     // reset wd timer
    }

    if(turncount % 5 == 0 && false){
      Serial.println();
      Serial.println();
      Serial.println();
      Serial.println();
      Serial.println();
      Serial.println();
      Serial.print("Itteration #");
      Serial.print(turncount);
      Serial.println();
      Serial.print("Distance ");
      Serial.print(dl);
      Serial.print(" ");
      Serial.print(dr);
      Serial.println();
      Serial.print("RLine: ");
      Serial.print(lr);
      Serial.print(" LLine: ");
      Serial.print(ll);
      Serial.println();
      Serial.print("Ultrasonic  ");
      Serial.print(uf);
      Serial.print("  ");
      Serial.print(ur);
      Serial.println();
      Serial.print("randomturn: ");
      Serial.println(randomturn);
      Serial.print("state: ");
      Serial.println(state);
      Serial.println();
    }
    // disable time of flight
    int tl = 1000;
    int tr = 1000;
    ur = 3000;

    turncount += 1; // counts the itterations
      if(state == 0){
        if((uf < 1000 && uf != 0) ||            
                (ur < 2000 && ur != 0)){              // if ultrasonic front or rear is detecting a close value and not zero (indicating a timeout)

          if((uf < 1000 && uf != 0)) 
            Serial.println("front ultrasonic flag");  // log front ultrasonic
          if((ur < 1000 && ur != 0)) 
            Serial.println("rear ultrasonic flag");   // log rear ultrasonic
          randomturn = rand() % 100 + 200;            // set up the random turn for between 50*-140*
          state = 2;                                  // change states to turn right
        }
        else if(ll > 950){               // if both of the left line detectors are detecing a black ish surface
          left_line_debounce += 1;
          if(left_line_debounce > 2){
            Serial.println("left line flag");           // log the detection
            randomturn = 100;                           // setup the backup to about 10 cm (not random)                            
            state = 5;                                  // change states to back up and turn right  
          }
        }
        else if(lr > 950){               // if both of the right line detectors are detecing a black ish surface
          right_line_debounce += 1;
          if(right_line_debounce > 2){
              Serial.println("right line flag");
              randomturn = 100;
              state = 4;                                  // back up and turn left
          }
        } 
        else if(dl > 0.30 && dl < 0.88){              // if distance detector has detects something, less than 0.88 for bad values
          left_debounce += 1;                         // increment the continous detetions
          if(left_debounce > 6){                      // if 4 continous detections
              Serial.println("left distance flag");   // log the detection
              randomturn = 100;                       // setup the backup to about 10 cm (not random)
              state = 5;                              // change states to back up and turn right
          }
        } 
        else if(dr > 0.30 && dr < 0.88) {
          right_debounce += 1;
          if(right_debounce > 6){
              Serial.println("right distance flag");  
              randomturn = 100;
              state = 4;                              // back up and turn left
          }        
        }
        if(!(dl > 0.30 && dl < 0.88)) left_debounce = 0;   // reset the debouncer if no detections noticed
        if(!(dr > 0.30 && dr < 0.88)) right_debounce = 0;   
        
        if(!(lr > 950)) right_line_debounce = 0;   
        if(!(ll > 950)) left_line_debounce = 0;   
      }


      if(state == 0){                       // drive fowards
        drive.forward();
      }
      else if(state == 1){                  // turn left
        if(randomturn > 0){                 // while random turn is still counting down
          drive.turnLeft();                 // turn left
          randomturn -= 1;                  // decrement random turn
        }
        else{
          state = 0;                        // else return to forwards
        }
      }
      else if(state == 2){                  // turn right
        if(randomturn > 0){
          drive.turnRight();
          randomturn -= 1;
        }
        else{
          state = 0;
        }
      }
      else if(state == 3){                    // turn around
        if(randomturn > 0){
          drive.turnRight(); 
          randomturn -= 1;
        } 
        else{
          state = 0;
        }
      }
      else if(state == 4){                    // back up then turn left
        if(randomturn > 0){                   // while random turn is still counting down
          drive.reverse();                    // back up
          randomturn -= 1;                    // decrement random turn
        } 
        else{
          randomturn = rand() % 100 + 200;    // set up the random turn for between 50*-140*
          state = 1;                          // start the left turn
        }
      }
      else if(state == 5){  // back up then turn right
         if(randomturn > 0){
          drive.reverse(); 
          randomturn -= 1;
        } 
        else{
          randomturn = rand() % 100 + 200;  
          state = 2;
        }
      }
}

