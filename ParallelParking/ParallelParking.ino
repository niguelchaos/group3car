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

int parkStage = 0; //The stage the automatic parking is in

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
  gyro.begin();

  car.begin(odometerLeft, odometerRight, gyro);
  //car.enableCruiseControl();

  delay(1000);

}

void loop() {
  
 car.updateMotors();

  //Serial.println(gyro.getAngularDisplacement());
  gyro.update();
  
  if( parkMode == true){
    Serial.println("entering parkmode");
    Serial.println(parkStage);
    park();
 }
 else{
  handleInput();
 }
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
          
            if(getParkingSpotSize(odometerLeft)){
              car.setSpeed(0);
              delay(50);
              park();  
            }
            
            
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



//========== Get the Size of a Parking Spot ==========//
boolean getParkingSpotSize(Odometer odometer) {


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
  
  while(true) {
    double d = rightSound.getMedianDistance();
    double distanceTraveled = odometer.getDistance();
    
    // Stop if an obstacle is detected
    if (d <= CAR_WIDTH && distanceTraveled >= CAR_LENGTH * 2) {
      return true;

    } else if(distanceTraveled >= CAR_LENGTH * 2){
      return true;
      //break;
    } else if(d <= CAR_WIDTH && distanceTraveled < CAR_LENGTH*2){
      //this should be something that notifies the driver. Is this sufficient?
      Serial.println("too small");
      return false;
      //break;
    }
  }

  //----- Cleanup -----//
  car.setSpeed(0);

  //Serial.println("return value:" + odometer.getDistance());
  //return odometer.getMedianDistance();
  }

 void park() {
  
  int backDistance = backSound.getDistance();
  int frontDistance = frontSound.getDistance();
  int rightDistance = rightSound.getDistance();
  int angularStartPoint = gyro.getAngularDisplacement();

  const int backSpd = -30;
  const int frontSpd = 30;
  const int right = 15;
  const int left = -15;
  
  Serial.print("backDistance: " );
  Serial.println(backDistance);  
  Serial.print("frontDistance: " );
  Serial.println(frontDistance);

  
  switch(parkStage){
    
     //Phase 1: Rotate left
     case 0:
      car.setSpeed(0);
      car.rotate(left);
      parkStage = 1;
      parkMode=true;
      break;
      
     //Phase 2:  Reverse till obstacle is too close
     case 1:
      if(backDistance < 20 && backDistance != 0) { parkStage=2; }
      else{ car.setSpeed(backSpd);}
      break;
      
     //Phase 3: Rotate to face forward
     case 2:
      car.rotate(right);
      car.go(-5);
      parkStage = 3;
      break;
      
     //Phase 4: Roll forward till space between two obstacles is relatively even
     case 3:
      if(frontDistance > backDistance || frontDistance == 0) { car.setSpeed( frontSpd );}
      else{
        Serial.println("Completed Park");
      car.setSpeed(0);
      parkMode = false;
      parkStage = 0;
      
      }

  }    
    
}

void panic(){
    //Cry in agony, not enough parking space
    Serial.print("Too T H I C C  to fit in there stop fatshaming me :'<");

}
