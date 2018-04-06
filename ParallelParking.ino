#include <Smartcar.h>

Odometer encoderLeft, encoderRight;
Gyroscope gyro;
Car car;

SR04 front; 
const int TRIG_PIN_FRONT = 00; //don't know, need to check car
const int ECHO_PIN_FRONT = 00; //SAME

SR04 right; 
const int TRIG_PIN_RIGHT = 7;
const int ECHO_PIN_RIGHT = 6; 

GP2Y0A21 rearIR; // are we using infrared here??
const int rearIR_PIN = 00; //don't know, need to check car

float CAR_SIZE = 20;
boolean parkMode = false;


void setup() {
  // put your setup code here, to run once:
  front.attach(TRIG_PIN_FRONT, ECHO_PIN_FRONT);
  right.attach(TRIG_PIN_FRONT, ECHO_PIN_FRONT);
  rearIR.attach(TRIG_PIN_RIGHT, ECHO_PIN_RIGHT);
  gyro.attach();
  encoderLeft.attach(2);
  encoderRight.attach(3);
  encoderLeft.begin();
  encoderRight.begin();
  car.begin(encoderLeft, encoderRight, gyro);
  car.enableCruiseControl();
  gyro.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  // check the distance from one detected obstacle to the right
  // to the next obstacle to the right to determine whether
  // the car fits
  car.updateMotors();
  handleInput();
  if(right.getDistance() == 0){ //because when 0 it means distance is out of range and
                                  // there is empty space 
    // start counting the space available for parking using odometer
    parkMode = true;
    detectSpotSize();
  }
  }
}

void handleInput() { //handle serial input if there is any
  if (Serial.available()) {
    char input = Serial.read(); //read everything that has been received so far and log down the last entry
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
      case 'p': // park
        detectSpotSize();
        break;
      default: //if you receive something that you don't know, just stop
        car.setSpeed(0);
        car.setAngle(0);
    }
  }
}
  

void detectSpotSize() {
  encoderRight.begin();
  //float parkingSize = 0;
  parkMode = true;

  // if we have begun detecting a spot and then we once again detect an obstacle, and the spot is greater than
  // the car size, commence parking.
  // P.S I don't think we need the first condition in the OR statement below, but I'm still considering.
  //if(right.getDistance() <= 10 || encoderRight.distance() > CAR_SIZE * 2) {
  if(right.getDistance() <= 10 && encoderRight.getDistance() > CAR_SIZE * 2 || right.getDistance() == 0 && encoderRight.distance() > CAR_SIZE * 2) {
      park();
    }
  }
}

// Automatic parking
void park() {

}
