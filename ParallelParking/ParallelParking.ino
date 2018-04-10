#include <Smartcar.h>

Odometer odometerLeft, odometerRight; //whoops, this is the speed thingy, not the sound one.
Gyroscope gyro;
Car car;

SR04 frontSound; 
const int TRIG_PIN_FRONT = 00; //don't know, need to check car
const int ECHO_PIN_FRONT = 00; //SAME

SR04 rightSound; 
const int TRIG_PIN_RIGHT = 7;
const int ECHO_PIN_RIGHT = 6; 

GP2Y0A21 rearIR; // are we using infrared here??
const int rearIR_PIN = 00; //don't know, need to check car

const int CAR_SIZE = 20;

const double CAR_LENGTH = 25; // Car length
const double CAR_WIDTH = 15.5; // Car Width

boolean parkMode = false;

void setup() {
  // put your setup code here, to run once:
  front.attach(TRIG_PIN_FRONT, ECHO_PIN_FRONT);
  right.attach(TRIG_PIN_RIGHT, ECHO_PIN_RIGHT);
  rearIR.attach(rearIR_PIN);
  gyro.attach();
  sonarLeft.attach(2);
  sonarRight.attach(3);
  sonarLeft.begin();
  sonarRight.begin();
  car.begin(encoderLeft, encoderRight, gyro);
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
        detectSpotSize();
        break;
      default: //if you receive something that you don't know, just stop
        car.setSpeed(0);
        car.setAngle(0);
    }
  }
}

void detectSpotSize() {
  odometerRight.begin();
  //float parkingSize = 0;
  //parkMode = true;

  // if we have begun detecting a spot and then we once again detect an obstacle, and the spot is greater than
  // the car size, commence parking.
  // P.S I don't think we need the first condition in the OR statement below, but I'm still considering.
  //if(right.getDistance() <= 10 || encoderRight.distance() > CAR_SIZE * 2) {
  if(rightSound.getDistance() <= 10 && odometerRight.getDistance() > CAR_SIZE * 2 || rightSound.getDistance() == 0 && odometerRight.getDistance() > CAR_SIZE * 2) {
      park();
  }
}

// Automatic parking
void park() {
  Serial.println(odometerRight.getDistance());
}

//========== Get the Size of a Parking Spot ==========//
double getParkingSpotSize() {
  boolean obstacleDetected = false;

  parkMode = true;
  car.setSpeed(20);
  odometer.begin();
  rightSound.start();
  car.go();

  double initialSideDistance = rightSound.getDistance();

  /*
    If the initial side distance is too tight then start looking for a spot
    that has a width that is greater than the car width.
  */
  if (initialSideDistance <= CAR_WIDTH) {
    while(rightSound.getDistance() <= initialSideDistance) {
      // Do nothing
    }
  }

  /*
    Once the distance to the right is large enough
    we can start looking for the next obstacle that has a distance less then or equal to the car width
    Or, until the distance traveled is greater than the car length
  */

  while(!obstacleDetected) {
    double d = rightSound.getDistance();

    // Stop if an obstacle is detected
    if (d <= CAR_WIDTH) {
      obstacleDetected = true;
    }

    // Stop if distance traveled is greater than or equal to twice the car size
    if (odometer.getDistance() >= CAR_LENGTH * 2) {
      obstacleDetected = true;
    }

  }

  //----- Cleanup -----//
  car.stop();
  odometer.stop();
  rightSound.stop();
  parkMode = false;

  return odometer.getDistance();
}