#include <Smartcar.h>

Odometer odometerLeft, odometerRight; 
Gyroscope gyro;
Car car;

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
  
 car.updateMotors();

  //Serial.println(gyro.getAngularDisplacement());
  //gyro.update();
  //delay(100);
  if(!parkMode){
  handleInput();
  }
  else{
    park();
  }
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
       car.setSpeed(-40);
        car.setAngle(0);
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

// Automatic parking
void park() {
   
   parkMode = true;
    
  //check if there's enough space for parking
  
  //if(!isParkingSpotAvailable()){
    //panic();
    //parkMode = false;
    //return;
  //}
  
  int backDistance = backSound.getDistance();
  int frontDistance = frontSound.getDistance();
  int rightDistance = rightSound.getDistance();
  int angularStartPoint = gyro.getAngularDisplacement();
  const int backSpd = -40;
  const int frontSpd = 40;
  const int right = 20;
  const int left = -20;
  
  Serial.print("backDistance: " );
  Serial.println(backDistance);  
  Serial.print("frontDistance: " );
  Serial.println(frontDistance);

  
  switch(parkStage){
    
     //Phase 1: Rotate left
     case 0:
      car.rotate(left);
      parkStage = 1;
      car.setSpeed(0);
      break;
      
     //Phase 2:  Reverse till obstacle is too close
     case 1:
      if(backDistance > 5){ car.setSpeed(backSpd); }
      else{ parkStage = 2;}
      break;
      
     //Phase 3: Rotate to face forward
     case 2:
      car.rotate(right);
      parkStage = 3;
      break;
      
     //Phase 4: Roll forward till space between two obstacles is relatively even
     case 3:
      if(frontDistance > backDistance) { car.setSpeed( frontSpd );}
      else{
      car.setSpeed(0);
      parkMode = false;
      parkStage = 0;
      
      }

  }    
    
}


// Workaround for initial wheel friction 
void parkPhaseRoll(int speed){
 
    
  
}


boolean isParkingSpotAvailable(){
  double spotHypotenuse = rightSound.getDistance(); // Sensor at the right-front of the car.
  double spotWidth = frontSound.getDistance(); // Sensor on the ight side of the car.

  double spotLength = sqrt(spotHypotenuse * spotHypotenuse - spotWidth * spotWidth);

  if (spotWidth >= CAR_WIDTH && spotLength >= CAR_LENGTH) {
    return true;
  }
  return false;
}

void panic(){
    //Cry in agony, not enough parking space
    Serial.print("Too T H I C C  to fit in there stop fatshaming me :'<");
}
