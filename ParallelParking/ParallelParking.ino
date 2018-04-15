#include <Smartcar.h>
#include <stdio.h>
#include <time.h>


Odometer odometerLeft, odometerRight; //whoops, this is the speed thingy, not the sound one.
Gyroscope gyro;
Car car;

SR04 frontSound; 
const int TRIG_PIN_FRONT = 6; //don't know, need to check car
const int ECHO_PIN_FRONT = 7; //SAME

SR04 rightSound; 
const int TRIG_PIN_RIGHT = 44;
const int ECHO_PIN_RIGHT = 42; 

GP2Y0A21 rearIR;           // are we using infrared here??
const int rearIR_PIN = 8; //don't know, need to check car


const int CAR_SIZE = 25;
const double CAR_LENGTH = 25; // Car length
const double CAR_WIDTH = 15.5; // Car Width

const int minParkSpotSize = 50;
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
  Serial3.begin(9600); //start the serial
 
}

void loop() {
  // put your main code here, to run repeatedly:
  // check the distance from one detected obstacle to the right
  // to the next obstacle to the right to determine whether
  // the car fits
  car.updateMotors();
  if(!parkMode){
      handleInput();
    }
}

void handleInput() {             //handle serial input if there is any
  if (Serial3.available()) {
    char input = Serial3.read(); //read everything that has been received so far and log down the last entry
    switch (input) {
      
      case 'l':                  //rotate counter-clockwise going forward
        car.setAngle(-75);
        car.setSpeed(62);
        delay(50);
        car.setSpeed(30);
        break;
        
      case 'r': //turn clock-wise
        car.setAngle(75);
        car.setSpeed(62);
        delay(50);
        car.setSpeed(30);
        break;
        
      case 'f': //go ahead
        car.setAngle(0);
        car.setSpeed(62);
        delay(50);
        car.setSpeed(30);
        break;
        
      case 'b': //go back
        car.setAngle(0);
        car.setSpeed(62);
        delay(50);
        car.setSpeed(30);
        break;
        
      case 'p': // park
        park();
        break;
        
      default: //if you receive something that you don't know, just stop
        car.setSpeed(0);
        car.setAngle(0);
    }
  }
}

double detectSpotSize() {
  odometerRight.begin();
  //float parkingSize = 0;
  //parkMode = true;

  // if we have begun detecting a spot and then we once again detect an obstacle, and the spot is greater than
  // the car size, commence parking.
  // P.S I don't think we need the first condition in the OR statement below, but I'm still considering.
  //if(right.getDistance() <= 10 || encoderRight.distance() > CAR_SIZE * 2) {
  if  (rightSound.getDistance() <= 10 && odometerRight.getDistance() > CAR_SIZE * 2 || 
      rightSound.getDistance() == 0 && odometerRight.getDistance() > CAR_SIZE * 2) {
      park();
  }
}



// Automatic parking
void park() {

    parkMode = true;

  //check if there's enough space for parking
  
  //if(getParkingSpotSize(odometerRight) < minParkSpotSize){
    //panic();
    
    //parkMode = false;
    //return;
  //}
    int backDistance = rearIR.getDistance();
    int frontDistance = frontSound.getDistance();
    int rightDistance = rightSound.getDistance();

    double rightOdoDistance = odometerRight.getDistance();
    
    car.setAngle(0);

  //Reverse to the right
    car.rotate(-20);  
    delay (200); //timeout so it's not crazy taxi
 
    car.setSpeed(62); //initial speed so the wheels start moving
    delay(50);
    car.setSpeed(30);
    //roll back till we get too close to the back object (40 just test number)
    while(BackDistance > 40){
        car.go(-5);
    }
    car.setSpeed(0);
    
    delay (400); 
  
  //Roll forward
    car.rotate(20); 
     
    delay (400); 

    car.setSpeed(62); //initial speed so the wheels start moving
    delay(50);
    car.setSpeed(30);

    // go forward till the distance between the front and back object is around the same
    while(frontDistance >= backDistance){
      car.go(5);
    }
  
  car.setSpeed(0); 
  delay (200); 

  Serial.println(rightOdoDistance);

  parkMode = false; //resume control


  
}

//========== Get the Size of a Parking Spot ==========//
double getParkingSpotSize(Odometer odometer) {
  boolean obstacleDetected = false;
  car.setAngle(0);
  car.setSpeed(62);
  delay(50);
  car.setSpeed(25);
  odometer.begin();

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

  Serial.println("return value:" + odometer.getDistance());
  return odometer.getDistance();
}

void panic(){
  //Cry in agony, not enough parking space
  car.setAngle(0);
  while(frontSound.getDistance() == 0 || frontSound.getDistance() > 10 &&
         rightSound.getDistance() == 0 || rightSoundDistance() > 10 ){
           car.setSpeed(62);
           car.rotate(30);
           car.setSpeed(90); //extreme speed
           car.go(10);
           car.setAngle(50);
           car.setSpeed(65);
           car.setMotorSpeed(60, -30);
           car.go(5);
           car.setMotorSpeed(50, 40);
           car.go(40);
           car.setSpeed(0);
      }
}




