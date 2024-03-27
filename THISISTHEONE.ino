#include "MeMCore.h"
#define H HIGH
#define L LOW
#define ULTRASONIC 12       //assigns ultrasonic sensor to pin 12
#define SPEED_OF_SOUND 340  //units: m/s
#define TIMEOUT 1250
#define NINETY 475
#define FULL 800



MeLineFollower lineFinder(PORT_2);
int status = 0;  //line reader
MeBuzzer buzzer;
MeDCMotor motor1(M1);       //right motor
MeDCMotor motor2(M2);       //left motor, assume left is tombalek
uint8_t motorSpeed1 = 250;  //right
uint8_t motorSpeed2 = 250;  //left

uint8_t LDR = A0;       // assign LDR to pin A0
uint8_t IRsensor = A1;  //assign IRsensor to pin A1
uint8_t LOGIC1 = A2;    //2 -> 4 logic gates; 2A
uint8_t LOGIC2 = A3;    //2 -> 4 logic gates; 2B

char colourStr[3][6] = { "Red", "Green", "Blue" };

//CALIBRATION VALUES FOR COLOURS 
float colourArray[] = { 0, 0, 0 };  // R,G,B. Array to store readings. 
float whiteArray[] = { 912, 957, 940 }; 
float blackArray[] = { 638, 770, 738 }; //callibrate and get avg
float greyDiff[] = { 274, 187, 202 }; 

  float blackColour[] = { 0, 0, 0 };
  //bracket 1 - "high red intensity"
  float whiteColour[] = { 255, 255, 255 };
  float orangeColour[] = { 247, 185, 152 };
  float redColour[] = { 244, 150, 142};
  //245 180 140
  //235, 128, 117
  //bracket 2 = "high blue intensity"
  float cyanColour[] = { 162, 210, 225 };
  //bracket 3 - "low blue and red intensity"
  float purpleColour[] = { 187, 166, 188 };
  float greenColour[] = { 93, 163, 104 };

//MOVEMENT FUNCTIONS//
//Move Forward
void advance() {
  motor1.run(motorSpeed1);
  motor2.run(-motorSpeed2);
}
//Turn Right
void turnRight(long int turnTime) {
  motor1.run(motorSpeed1 - 150);             // right wheel
  motor2.run(-motorSpeed2);  // left wheel
  delay(turnTime);
}
//Turn Left
void turnLeft(long int turnTime) {
  motor1.run(motorSpeed1);  // right wheel
  motor2.run(-motorSpeed2 + 150);            //left wheel
  delay(turnTime);
}
void kekiriPusing(int turnTime) {  // Stationary left
  motor1.run(200);
  motor2.run(200);
  delay(turnTime);
  berhenti();
}
void kekenanPusing(int turnTime) {  // stationary right
  motor1.run(-200);
  motor2.run(-200);
  delay(turnTime);
  berhenti();
}

void keblakanPusing ()
{
      motor1.run(-250); //right go back
      motor2.run(0); //left stops
      delay(650);
      motor1.run(0); //right stop
      motor2.run(-250); //left go front
      delay(650);
}

//Reverse
void gostan(int time) {
  motor1.run(-200);
  motor2.run(200);
  delay(time);
}
//Stop
void berhenti() {
  motor1.run(0);
  motor2.run(0);
}


//code for ultrasonic sensor to work
float ultra_dist() {
  pinMode(ULTRASONIC, OUTPUT);
  digitalWrite(ULTRASONIC, LOW);  //silence
  delayMicroseconds(2);

  digitalWrite(ULTRASONIC, HIGH);
  delayMicroseconds(10);  //shouts for 10microsec

  digitalWrite(ULTRASONIC, LOW);  //silence
  pinMode(ULTRASONIC, INPUT);
  float duration = pulseIn(ULTRASONIC, HIGH, TIMEOUT);  //listens for echo; if after 1000ms, it will time out and return 0 for duration.

  float dist = duration / 2.0 / 1000000 * SPEED_OF_SOUND * 100;  //calculates dist
  //dist -= 11;                                                     // -9 because 9cm away from wall is optimal
  Serial.println(dist);
  if ((dist == 0) || (dist <= 13 && dist >= 9)) {  // dist == -9 means timed out, so dont count; this range of error is okay so returns 0; acceptable
    return 0;
  } else if (dist < 9) {  // too close to ultrasonic sensor; left side
    return 1;
  } else if (dist > 13) {  // i.e dist >= 3; too close to opp side of ultrasonic sensor; right side
    return 2;
  }
}

/*

int avgIRVolt() {
  digitalWrite(LOGIC1, LOW);
  digitalWrite(LOGIC2, LOW);  //by logic gates, turns on IRemitter
  delay(300);
  int avgIRVolt = 0;
  for (int i = 0; i < 5; i += 1) {
    float detectorVoltage = analogRead(IRsensor);
    avgIRVolt += detectorVoltage;
  }
  avgIRVolt /= 5;

  digitalWrite(LOGIC1, HIGH);
  digitalWrite(LOGIC2, LOW);  //by logic gates, turns off IRemitter; turns on green LED

  return avgIRVolt;
}

int IRdetector() {  //turns on detector without emitter
  delay(200);
  int ambientIR = 0;
  for (int i = 0; i < 5; i += 1) {
    float detectorVoltage = analogRead(IRsensor);
    ambientIR += detectorVoltage;
  }
  ambientIR /= 5;
  return ambientIR;
}

int IR_dist() {
  //delay(100);
  int ambientIR = IRdetector();  //reads ambientIR without IR Emmitter
  int IRVolt = avgIRVolt();      // reads IR with IR Emmitter

  int change_in_IR = ambientIR - IRVolt;
  Serial.println(change_in_IR);

  if (change_in_IR > 500) {  //'xx' is placeholder voltage ** change to a value in the hundreds
    return 1;                // too close to wall
  } else {
    return 0;  // not too close to wall
  }
}

*/

void offLights() {  //will turn on Y0 btw
  digitalWrite(LOGIC1, LOW);
  digitalWrite(LOGIC2, LOW);
}


void onLights(int colour) {
  if (colour == 0) {  // i==0, red LED on
    digitalWrite(LOGIC1, HIGH);
    digitalWrite(LOGIC2, HIGH);
  } else if (colour == 1) {  // i==1, green LED on
    digitalWrite(LOGIC1, HIGH);
    digitalWrite(LOGIC2, LOW);
  } else {  // i==2, blue LED on
    digitalWrite(LOGIC1, LOW);
    digitalWrite(LOGIC2, HIGH);
  }
}


void BlastNRead() {
  for (int colour = 0; colour <= 2; colour += 1) {
    onLights(colour);
    delay(100);  //let LDR settle down
    Serial.print(colourStr[colour]);
    Serial.print(" LED on, ");

    int LDRvoltage = 0;
    for (int i = 0; i < 5; i++) {  //taking one sample at a time, total of 5 samples
      LDRvoltage += analogRead(LDR);
      //delay(10);
    }
    LDRvoltage /= 5;
    Serial.print(LDRvoltage);

    int colourIntensity = (LDRvoltage - blackArray[colour]) / greyDiff[colour] * 255;
    Serial.print(" colour intensity: ");
    Serial.println(colourIntensity);
    colourArray[colour] = colourIntensity;

    offLights();
  }
}


bool identifyColour() {
  //bracket 1 - high red intensity detected - either white orange or red
  if (colourArray[0] >= (redColour[0] - 10.0)) {
    if (colourArray[2] >= (whiteColour[2] - 10.0)) {
      Serial.println("WHITE");
      celebrate();  //play music
    } else if ((colourArray[1] >= (orangeColour[1] -10))) {
      Serial.println("ORANGE");
      //180 turn within same grid
       //buzzer.tone(369.99, 291.5);    // F#4
       //buzzer.tone(415.30, 388.667);  // G#4
       //buzzer.tone(466.16, 388.667);  // A#4
       //kekenanPusing(FULL);
       keblakanPusing();
    } else {
      Serial.println("RED");
      //left turn
      kekiriPusing(NINETY);
    } 
    //else {
      //Serial.println("UNKNOWN");
      //buzzer.tone(466.16, 1249);
     // return true;  // semula == true
    //}
  }
  // bracket 2 - high blue intensity detected, can only be light blue
  else if (colourArray[2] >= (cyanColour[2] - 10.0)) {
    Serial.println("BLUE");
    //two successive right-turns in 2 grids
    kekenanPusing(NINETY-20);
    motor1.run(200);
    motor2.run(-200);
    delay(900);
    kekenanPusing(NINETY-20);
  }
  //bracket 3 - low red and blue intensity detected, can be green or purple
  else {
    if (colourArray[2] >= purpleColour[2] - 10.0) {
      Serial.println("PURPLE");
      //two successive left-turn in 2 grids
      kekiriPusing(NINETY);
      motor1.run(200);
      motor2.run(-200);
      delay(950);
      kekiriPusing(NINETY);
    } else if (colourArray[1] >= greenColour[1] - 10.0) {
      Serial.println("GREEN");
      //right turn
      kekenanPusing(NINETY);
    } else {
      Serial.println("UNKNOWN");
      buzzer.tone(466.16, 1249);
      return true;  // Retake values
      
    }
  }
  return false;
}

void celebrate() {
  // Each of the following "function calls" plays a single tone.
  // The numbers in the bracket specify the frequency and the duration (ms)
  buzzer.tone(622.25, 1166);    //D#5
  buzzer.tone(932.33, 583);     //A#5
  buzzer.tone(830.61, 1940.5);  //G#5
  buzzer.noTone();
  buzzer.tone(554.37, 291.5);  // C#5
  buzzer.tone(739.99, 291.5);  // F#5
  buzzer.tone(698.46, 583);    // E#5
  buzzer.tone(554.37, 291.5);  // C#5
  buzzer.tone(466.16, 291.5);  // A#4
  buzzer.tone(415.30, 291.5);  // G#4
  buzzer.tone(466.16, 1249);   // A#4
  buzzer.noTone();
  buzzer.tone(466.16, 666);   // A#4
  buzzer.tone(554.37, 291.5);  // C#5
  buzzer.tone(622.25, 583);    // D#5
  buzzer.tone(369.99, 583);    // F#4

  buzzer.tone(466.16, 666);   // A#4
  buzzer.tone(554.37, 291.5);  // C#5
  buzzer.tone(622.25, 583);    // D#5
  buzzer.tone(369.99, 583);    // F#4
  buzzer.noTone();
  buzzer.tone(369.99, 291.5);    // F#4
  buzzer.tone(415.30, 388.667);  // G#4
  buzzer.tone(466.16, 388.667);  // A#4
  buzzer.tone(415.30, 388.667);  // G#4
  buzzer.tone(466.16, 2000);     // A#4
  buzzer.noTone();
}

void setup() {
  Serial.begin(9600);
  pinMode(LDR, INPUT);
  // pinMode(IRsensor, INPUT);
  pinMode(LOGIC1, OUTPUT);
  pinMode(LOGIC2, OUTPUT);
}




void loop() {
  //Serial.print("looped once");

  int sensorState = lineFinder.readSensors();  // Line reader sensor
  if (sensorState == S1_IN_S2_IN) {
        berhenti();
        //delay(500);
        digitalWrite(LOGIC1, LOW);  //turn off signal
        digitalWrite(LOGIC2, LOW);
        Serial.println("BLASTING NOW...");
        BlastNRead();
        identifyColour();  // this function should identify the colour and then turn appropriately
        Serial.println(""); 
        }
 
  int ultrasonic = ultra_dist();               // 0 is acceptable, 1 is too left, 2 is too right
    //move fwd, stay within gutters
  if (ultrasonic == 0) {
    advance();
    Serial.println("moving forward");
  } 
  else if (ultrasonic == 1) { 
      turnRight(10);
      Serial.println("turning right");
    
    }

  else if (ultrasonic == 2) {
      turnLeft(10);
      Serial.println("turning left");
    }
}