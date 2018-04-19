#include <Smartcar.h>

Odometer odometerLeft, odometerRight; //whoops, this is the speed thingy, not the sound one.
Gyroscope gyro;
Car car;

SR04 frontSound; 
const int TRIG_PIN_FRONT = 6; 
const int ECHO_PIN_FRONT = 7; 

SR04 rightSound; 
const int TRIG_PIN_RIGHT = 44;
const int ECHO_PIN_RIGHT = 42; 

SR04 backSound;
const int TRIG_PIN_BACK = 35;
const int ECHO_PIN_BACK = 34;

GP2Y0A21 rearIR; 
const int rearIR_PIN = 8; 


const int CAR_SIZE = 25;
const double CAR_LENGTH = 25; // Car length
const double CAR_WIDTH = 15.5; // Car Width

const int minParkSpotSize = 50;
boolean parkMode = false;
 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //start the serial
  gyro.attach();  
  frontSound.attach(TRIG_PIN_FRONT, ECHO_PIN_FRONT);
  rightSound.attach(TRIG_PIN_RIGHT, ECHO_PIN_RIGHT);
  backSound.attach(TRIG_PIN_BACK, ECHO_PIN_BACK);

  odometerLeft.attach(2);
  odometerRight.attach(3);
  
  odometerLeft.begin();
  odometerRight.begin();
  gyro.begin();
  car.begin(odometerLeft, odometerRight, gyro);
  //car.enableCruiseControl();
 
 
  
}

void loop() {
  
  // check the distance from one detected obstacle to the right
  // to the next obstacle to the right to determine whether
  // the car fits
  
  park();
  
  //if(!parkMode){
  //handleInput();
  //}
}

void handleInput() { //handle serial input if there is any
  if (Serial.available()) {
    char input = Serial.read(); //read everything that has been received so far and log down the last entry
    switch (input) {
      case 'l': //rotate counter-clockwise going forward
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
        //park();
        getParkingSpotSize(odometerLeft);
        Serial.println("parking");
        break;
      default: //if you receive something that you don't know, just stop
        car.setSpeed(0);
        car.setAngle(0);
    }
  }
}

/*double detectSpotSize() {
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
}*/



// Automatic parking
void park() {
  Serial.println("Entered parkmode");
    parkMode = true;
    
  //check if there's enough space for parking
  
  //if(getParkingSpotSize(odometerRight) < minParkSpotSize){
    //panic();
    
    //parkMode = false;
    //return;
  //}

    int backDistance = backSound.getDistance();
    int frontDistance = frontSound.getDistance();
    int rightDistance = rightSound.getDistance();
    int angularStartPoint = gyro.getAngularDisplacement();

    

    const int backSpd = -60;
    const int frontSpd = 60;
    const int right = 40;
    const int left = -40;

    static const int initialRightDistance = rightSound.getDistance();
    static int turnCount = 0;
    
    Serial.print("Initial rightdistance:");
    Serial.println(initialRightDistance);
    delay(100);
    Serial.print("Right:");
    Serial.println(rightDistance);
    delay(150);
    Serial.print("backDistance: " );
    Serial.println(backDistance);
    delay(100);

    Serial.print("frontDistance: " );
    Serial.println(frontDistance);
    delay(100);

    if(rightDistance == initialRightDistance) {
      car.setSpeed(backSpd);  
    }
    if(rightDistance != initialRightDistance){
       car.setSpeed(0);
       if (rightDistance == 0 && turnCount == 0){
        Serial.println("Turning in");
        Serial.print("Turncount:");
        Serial.println(turnCount);
        car.rotate(-30); 
        turnCount++;
       }
       
    }
    
//  Serial.println(odometerRight.getDistance());
  parkMode = false; //resume control  
}



//========== Get the Size of a Parking Spot ==========//
int getParkingSpotSize(Odometer odometer) {
  boolean obstacleDetected = false;
  car.setAngle(0);
  car.setSpeed(62);
  delay(50);
  car.setSpeed(25);
  odometer.begin();

  int initialSideDistance = rightSound.getDistance();

  Serial.println("intialSide: " + initialSideDistance);
  //Serial.println(odometer.getDistance());
  //Serial.println("hello:" + initialSideDistance);
  /*
    If the initial side distance is too tight then start looking for a spot
    that has a width that is greater than the car width.
  */
  /*if (initialSideDistance <= CAR_WIDTH) {
    while(rightSound.getDistance() <= initialSideDistance) {
      // Do nothing
    }
  }*/

  /*
    Once the distance to the right is large enough
    we can start looking for the next obstacle that has a distance less then or equal to the car width
    Or, until the distance traveled is greater than the car length
  */

  /*while(!obstacleDetected) {
    double d = rightSound.getDistance();

    // Stop if an obstacle is detected
    if (d <= CAR_WIDTH) {
      obstacleDetected = true;
    }

    // Stop if distance traveled is greater than or equal to twice the car size
    if (odometer.getDistance() >= CAR_LENGTH * 2) {
      obstacleDetected = true;
    }

  }*/

  //----- Cleanup -----//
  car.stop();

  Serial.println("return value:" + odometer.getDistance());
  return odometer.getDistance();
}

void panic(){
    //Cry in agony, not enough parking space
}




