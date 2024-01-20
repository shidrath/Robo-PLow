/*Connect the wheel encoder signal to pin A7 in the Arduino.*/
volatile uint32_t CaptureCountA;
volatile boolean CaptureFlag;
int t_old = 0;
int pout = 7;
int lctrl = 9;
int rctrl = 11;
int power_switch = 0;

int snowDistThresh = 50;
int buidlingDistThresh = 50;

bool turnDir = true;


void setup() {
  Serial.begin(115200);

  pinMode(pout, OUTPUT);
  pinMode(lctrl, OUTPUT);
  pinMode(rctrl, OUTPUT);
  pinMode(4, INPUT);
}
void loop() {
  Serial.println("Test");

  //startClear();
  // forward();
  // delay(1000);
  // park();
  // delay(1000);
  // reverse();
  // delay(1000);
  // park();
  // delay(1000);
  // turnLeft();
  // delay(1000);
  // park();
  // delay(1000);
  // turnRight();
  // delay(1000);
  // park();
  // delay(1000);
  //power_switch = digitalRead(4);
  //printf(power_switch);
}

void TC1_Handler() {
  uint32_t status = TC0->TC_CHANNEL[1].TC_SR;
  //Read status register, Clear status
  if ((millis() - t_old) > 50) {
    if (status & TC_SR_LDRAS) {                  // If ISR is fired by LDRAS then ....
      CaptureCountA = TC0->TC_CHANNEL[1].TC_RA;  //read TC_RA
      CaptureFlag = 1;
      t_old = millis();
    }  //Inform the main loop of an update.
  }
}

void turnLeft() {
  digitalWrite(lctrl, HIGH);
  digitalWrite(rctrl, HIGH);

  analogWrite(pout, 100);
}
void turnRight() {
  digitalWrite(lctrl, LOW);
  digitalWrite(rctrl, LOW);

  analogWrite(pout, 100);
}
void forward() {

  digitalWrite(lctrl, LOW);
  digitalWrite(rctrl, HIGH);

  analogWrite(pout, 100);
}
void reverse() {
  digitalWrite(lctrl, HIGH);
  digitalWrite(rctrl, LOW);

  analogWrite(pout, 100);
}

void park() {
  analogWrite(pout, 0);
}

// void startClear() {
//   if (turnDir == true) {
//     turnRight();
//   } else {
//     turnLeft();
//   }

//   if (pathSnowClear()) {
//     forward();
//     while (!hitEdge()) {
//     }
//     park();
//   }
// }

// bool pathSnowClear() {
//   if (detectSnow() && !detectBuilding()) {
//     return true;
//   } else {
//     return false;
//   }
// }

// bool detectSnow() {
//   snowDist = (READ ULTRA SONIC);

//   if (snowDist < snowDistThresh) {
//     return true;
//   } else {
//     return false;
//   }
// }

// bool detectBuilding() {
//   LBuidlingDist = (READ left ToF);
//   RBuidlingDist = (READ right ToF);

//   if ((RBuidlingDist < buildingDistThresh) || (LBuidlingDist < buildingDistThresh)) {
//     return true;
//   } else {
//     return false;
//   }
// }

// bool hitEdge() {
//   LLine = (READ LEFT LINE FOLLOWER);
//   RLine = (READ RIGHT LINE FOLLOWER);

//   if (LLine == HIGH) {
//     turnDir = true;
//     while (LLine == HIGH) {
//       turnRight();
//     }
//     park();
//     return true;
//   } else if (RLine == HIGH) {
//     turnDir = false;
//     while (RLine == HIGH) {
//       turnLeft();
//     }
//     park();
//     return true;
//   } else {
//     return false;
//   }
// }