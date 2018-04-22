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
  //gyro.attach();
  
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
  
  //Serial.println(frontSound.getDistance()); // added to test where the issue arises
  //delay(100);
  
  //car.updateMotors();
  //car.setSpeed(-70);
  //car.setSpeed (70);


  /*Serial.println("Left odo speed:");
  Serial.println(odometerLeft.getSpeed());
  delay(100);
  car.setMotorSpeed(0, -60);
  delay(100);
  car.go(-30);*/

  /*double parkspot = isParkingSpotAvailable();

  Serial.println("can park:");
  Serial.println(parkspot);*/
  
  //park();
  //Serial.println(gyro.getAngularDisplacement());

  car.setSpeed(-60);


  //steeringAdjustment();
  //gyro.update();

  /*int offset = gyro.calibrate(100);

  Serial.println("offset:");
  Serial.println(offset);
  
  //delay(100);
  //if(!parkMode){
  //handleInput();*/
  //}
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
        //getParkingSpotSize(odometerLeft);
        Serial.println("parking");
        break;
      default: //if you receive something that you don't know, just stop
        car.setSpeed(0);
        car.setAngle(0);
    }
  }
}

/*void steeringAdjustment(){
    
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
    car.setSpeed(-60);
}*/

// Automatic parking
void park() {
  Serial.println("hello");
  
  parkMode = true;
 
  int backDistance = backSound.getMedianDistance();
  int frontDistance = frontSound.getMedianDistance();
  int rightDistance = rightSound.getMedianDistance();
  int angularStartPoint = gyro.getAngularDisplacement();
  const int backSpd = -50;
  const int frontSpd = 50;
  const int right = 40;
  const int left = -40;

  delay(5000);
  int IR_DIST = IR_DIAG_FRONT.getDistance();
  Serial.println("IR_DIST:");
  Serial.println(IR_DIST);

  /*car.setSpeed(-40);
  car.go(-10);
  car.rotate(-40);
  car.go(15);
  car.rotate(40);
  car.go(-5);*/

  int initialRightDistance = rightSound.getMedianDistance();
    
  Serial.print("backDistance: " );
  Serial.println(backDistance);
  //delay(100);
    
  Serial.print("frontDistance: " );
  Serial.println(frontDistance);
   //delay(150);
   //roll back till we get too close to the back object (40 just test number)
        //Reverse to the right
   /* if (backDistance > 0 && frontDistance == 0){ //position parallel to car in front
  
      if(backDistance > 50){
        car.setAngle(0);
        car.setMotorSpeed(-60, -30); 
        car.setAngle(RIGHT);
      }
        if (backDistance > 30) {
          //car.setMotorSpeed(-60,-60);
     
        }
        if (backDistance < 15) {
          //car.setMotorSpeed(70,50);
          car.rotate(-45);
          car.setSpeed(-60);
        }
        if(frontDistance < 10){
          car.rotate(-10);
        }
    }*/

    int rightDistDiff = initialRightDistance - rightDistance;

    /*car.setSpeed(-50);
    if(rightDistDiff < 2 && rightDistDiff > -2){ //car is parallel
      car.rotate(-30-gyroDplmnt);
    }*/


    /*car.setSpeed(-50);
    if(backDistance == 0 && frontDistance == 0){
      //Serial.println("All 0's!");
      car.rotate(-30);
    }

    if(backDistance < 10) {
      if(frontDistance < 25) {
        car.setSpeed(0);
      }
      car.rotate(30);
    }
    
    if(backDistance > 1 && backDistance < 5){
      car.setSpeed(0);
    }*/
    
 /*   if (backDistance > 21 && backDistance < 40 && frontDistance == 0){
        car.setAngle(0); 
        car.setSpeed(backSpd);
    }
    if (backDistance < 21 && backDistance > 14) {
      car.setAngle(left);
      car.setSpeed(backSpd);
    }
   
    if (backDistance < 13 || (frontDistance > 1 && frontDistance < 10)) {
      car.setAngle(0);
        car.setSpeed(0);
        car.setAngle(0); 
    }
*/    
  
    // go forward till the distance between the front and back object is around the same
//    while(frontDistance >= backDistance){
//      car.rotate(0);
//      car.setSpeed(40);
//    }
//  Serial.println(odometerRight.getDistance());
  
  parkMode = false; //resume control
  
}

//========== Check if parking spot size is large enough ==========//
/*
  This variant of checking parking spots uses trigonometric calculation, instead of moving the car, to measure the distance.
  This assumes that there is a sound sensor placed at the right side of the car, and one placed diagonally at the right-front corner of the car.
*/
double isParkingSpotAvailable() {
  double spotHypotenuse = IR_DIAG_FRONT.getDistance(); // Sensor at the right-front of the car.
  double spotWidth = frontSound.getDistance(); // Sensor on the ight side of the car.
  double spotLength = sqrt(spotHypotenuse * spotHypotenuse - spotWidth * spotWidth);

  delay(1000);
  Serial.println("spotLength: ");
  Serial.println(spotLength);
  
  if (spotWidth >= CAR_WIDTH && spotLength >= CAR_LENGTH) {
    return spotLength;
  }
  return spotLength;
  //return false;
}

void panic(){
    //Cry in agony, not enough parking space
}
