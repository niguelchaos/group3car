#include <Smartcar.h>
Car car;

Odometer odometerLeft, odometerRight; 

Gyroscope gyro(3);

GP2Y0A21 IR_DIAG_FRONT;
const int IR_PIN = A0;

SR04 frontSound; 
const int TRIG_PIN_FRONT = 6; 
const int ECHO_PIN_FRONT = 7;
 
SR04 rightSound; 
const int TRIG_PIN_RIGHT = 44;
const int ECHO_PIN_RIGHT = 42; 

SR04 backSound;
const int TRIG_PIN_BACK = 35;
const int ECHO_PIN_BACK = 34;

const int CAR_SIZE = 25;
const double CAR_LENGTH = 25; // Car length
const double CAR_WIDTH = 15.5; // Car Width
const int minParkSpotSize = 50;
boolean parkMode = false;

void setup() {
  // put your setup code here, to run once:
  gyro.attach();
  
  Serial.begin(9600); //start the serial
  //delay(1500);
  
  odometerLeft.attach(2);
  odometerRight.attach(3);
  odometerLeft.begin();
  odometerRight.begin();
  frontSound.attach(TRIG_PIN_FRONT, ECHO_PIN_FRONT);
  rightSound.attach(TRIG_PIN_RIGHT, ECHO_PIN_RIGHT);
  backSound.attach(TRIG_PIN_BACK, ECHO_PIN_BACK);
  IR_DIAG_FRONT.attach(IR_PIN);
  //gyro.begin();
  car.begin(odometerLeft, odometerRight, gyro);
  //car.enableCruiseControl();

  delay(1000);
  
}
void loop() {
  
  // check the distance from one detected obstacle to the right
  // to the next obstacle to the right to determine whether
  // the car fits
  
  
  /*double parkspot = isParkingSpotAvailable();

  Serial.println("can park:");
  Serial.println(parkspot);*/

  //steeringAdjustment();
  //park();
  //Serial.println(gyro.getAngularDisplacement());
  //gyro.update();
  handleInput();
}
void handleInput() { //handle serial input if there is any
  if (Serial.available()) {
    char input = Serial.read(); //read everything that has been received so far and log down the last entry
        switch (input) {
          case 'l': //rotate counter-clockwise going forward
            car.setSpeed(40);
            car.setAngle(-35);
            break;
          case 'r': //turn clock-wise
            car.setSpeed(40);
            car.setAngle(35);
            break;
          case 'f': //go ahead
            car.setSpeed(40);
            car.setAngle(0);
            break;
          case 'b': //go back
            car.setSpeed(-40);
            car.setAngle(0);
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

void steeringAdjustment(){
    
    unsigned int gyroDplmnt = gyro.getAngularDisplacement();

    //Serial.println("offset:");
    //Serial.println(gyro.calibrate());

    int offset = gyro.calibrate();

  
    Serial.println("displacement:");
    Serial.println(gyroDplmnt);
    
    if(gyroDplmnt > 0 && gyroDplmnt <= 180){
      if(gyroDplmnt < 90){
        car.rotate(-gyroDplmnt-offset);
      } else {
        int deviation = gyroDplmnt - 180;
        car.rotate(deviation - offset); 
      }
    } else if(gyroDplmnt > 180 && gyroDplmnt <= 360){
      if(gyroDplmnt <= 270){
        int deviation = gyroDplmnt - 180;
        car.rotate(-deviation - offset);  
      } else {
        int deviation = 360 - gyroDplmnt;
        car.rotate(deviation - offset);  
      }
    }
    //car.setSpeed(-60);
}

// Automatic parking
void park() {
  Serial.println("hello");
  
  //parkMode = true;
 
  int backDistance = backSound.getMedianDistance();
  int frontDistance = frontSound.getMedianDistance();
  int rightDistance = rightSound.getMedianDistance();
  int angularStartPoint = gyro.getAngularDisplacement();
  const int backSpd = -50;
  const int frontSpd = 50;
  const int right = 60;
  const int left = -60;

  car.setSpeed(backSpd);

  if(rightDistance > rightSound.getMedianDistance() || rightSound.getDistance() == 0){
    car.setAngle(left);
  } else if(backSound.getDistance() < 20) {
    car.setAngle(right);
  } /*else{
    car.setSpeed(backSpd);
  }*/
  
}

//========== Get the Size of a Parking Spot ==========//
boolean getParkingSpotSize(Odometer odometer) {
  boolean obstacleDetected = false;

  //parkMode = true;
  car.setSpeed(40);

  double initialSideDistance = rightSound.getMedianDistance();

  /*
    If the initial side distance is too tight then start looking for a spot
    that has a width that is greater than the car width.
  */
  if (initialSideDistance <= CAR_WIDTH || initialSideDistance != 0) {
    while(rightSound.getMedianDistance() <= initialSideDistance) {
      // Do nothing
    }
  }

  /*
    Once the distance to the right is large enough
    we can start looking for the next obstacle that has a distance less then or equal to the car width
    Or, until the distance traveled is greater than the car length
  */
  odometer.begin();
  
  while(!obstacleDetected) {
    double d = rightSound.getMedianDistance();
    double distanceTraveled = odometer.getDistance();
    
    // Stop if an obstacle is detected
    if (d <= CAR_WIDTH && distanceTraveled >= CAR_LENGTH * 2) {
      obstacleDetected = true;
      park();
    } else if(distanceTraveled >= CAR_LENGTH * 2){
      park();
      break;
    } else if(d <= CAR_WIDTH && distanceTraveled < CAR_LENGTH*2){
      //this should be something that notifies the driver. Is this sufficient?
      Serial.println("too small");
      break;
    }
  }

  //----- Cleanup -----//
  car.setSpeed(0);

  //Serial.println("return value:" + odometer.getDistance());
  //return odometer.getMedianDistance();
}
