#include <Smartcar.h>

Car car;

const int fSpeed = 70; //70% of the full speed forward
const int bSpeed = -70; //70% of the full speed backward
const int lDegrees = -75; //degrees to turn left
const int rDegrees = 75; //degrees to turn right

// initializes Ultrasonic sensor
SR04 ultrasonicSensor;
const int TRIGGER_PIN = 7; //7
const int ECHO_PIN = 6;   //6

//for distances between 12 and 78 centimeters - Infrared 
GP2Y0A21 sideFrontIR;
const int SIDE_FRONT_PIN = A8; //use analog pins only

Odometer encoder;
const int encoderPin = 2;


void setup() {
 
  car.begin();
  Serial.println("chicken");
  Serial.begin(9600);
  Serial3.begin(9600);

  //ultrasonic
  ultrasonicSensor.attach(TRIGGER_PIN, ECHO_PIN);

  //IR
  sideFrontIR.attach(SIDE_FRONT_PIN);

  //odometer
  encoder.attach(encoderPin);
  encoder.begin();
}

void loop() {
//   Serial.print(encoder.getDistance());

//  Serial.println(sideFrontIR.getDistance());
//  Serial.println(ultrasonicSensor.getDistance());
//  delay(100);
handleInput();

}

void handleInput() { //handle serial input if there is any
  if (Serial3.available()) {
    char input = Serial3.read(); //read everything that has been received so far and log down the last entry
    switch (input) {
      case 'l': //rotate counter-clockwise going forward
        car.setSpeed(fSpeed);
        car.setAngle(lDegrees);
        break;
      case 'r': //turn clock-wise
        car.setSpeed(fSpeed);
        car.setAngle(rDegrees);
        break;
      case 'f': //go ahead
        car.setSpeed(fSpeed);
        car.setAngle(0);
        break;
      case 'b': //go back
        car.setSpeed(bSpeed);
        car.setAngle(0);
        break;
      default: //if you receive something that you don't know, just stop
        car.setSpeed(0);
        car.setAngle(0);
    }
  }
}
