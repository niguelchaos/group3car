#include <Smartcar.h>

Odometer odometerLeft, odometerRight; //whoops, this is the speed thingy, not the sound one.
Gyroscope gyro;
Car car;

SR04 frontSound; 
const int TRIG_PIN_FRONT = 6; //don't know, need to check car
const int ECHO_PIN_FRONT = 7; //SAME

SR04 rightSound; 
const int TRIG_PIN_RIGHT = 12;
const int ECHO_PIN_RIGHT = 13; 

GP2Y0A21 rearIR; // are we using infrared here??
const int rearIR_PIN = 00; //don't know, need to check car

const double CAR_LENGTH = 25; // Car length
const double CAR_WIDTH = 15.5; // Car Width

boolean parkMode = false;

void setup() {
  // put your setup code here, to run once:
  frontSound.attach(TRIG_PIN_FRONT, ECHO_PIN_FRONT);
  rightSound.attach(TRIG_PIN_RIGHT, ECHO_PIN_RIGHT);
  rearIR.attach(rearIR_PIN);
  gyro.attach();
  odometerLeft.attach(2);
  odometerRight.attach(3);
  odometerLeft.begin();
  odometerRight.begin();
  car.begin(odometerLeft, odometerRight, gyro);
  //car.enableCruiseControl();
  gyro.begin();
  Serial.begin(9600); //start the serial

}

void loop() {
  // put your main code here, to run repeatedly:
  // check the distance from one detected obstacle to the right
  // to the next obstacle to the right to determine whether
  // the car fits
  car.updateMotors();
  handleInput();
}

void handleInput() { //handle serial input if there is any
  if (Serial.available()) {
    char input = Serial.read(); //read everything that has been received so far and log down the last entry
    switch (input) {
      case 'l': //rotate counter-clockwise going forward
        car.setSpeed(20);
        car.setAngle(-75);
        break;
      case 'r': //turn clock-wise
        car.setSpeed(20);
        car.setAngle(75);
        break;
      case 'f': //go ahead
        car.setSpeed(20);
        car.setAngle(0);
        break;
      case 'b': //go back
        car.setSpeed(-20);
        car.setAngle(0);
        break;
      case 'p': // park
        parkMode = true;
        getParkingSpotSize(odometerRight);
        break;
      default: //if you receive something that you don't know, just stop
        car.setSpeed(0);
        car.setAngle(0);
    }
  }
}

//========== Check if parking spot size is large enough ==========//
/*
  This variant of checking parking spots uses trigonometric calculation, instead of moving the car, to measure the distance.
  This assumes that there is a sound sensor placed at the right side of the car, and one placed diagonally at the right-front corner of the car.
*/
boolean isParkingSpotAvailable() {
  double spotHypotenuse = rightSound.getDistance(); // Sensor at the right-front of the car.
  double spotWidth = frontSound.getDistance(); // Sensor on the ight side of the car.

  double spotLength = sqrt(spotHypotenuse * spotHypotenuse - spotWidth * spotWidth);

  if (spotWidth >= CAR_WIDTH && spotLength >= CAR_LENGTH) {
    return true;
  }
  return false;
}