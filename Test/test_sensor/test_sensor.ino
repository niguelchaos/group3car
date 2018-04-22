#include <Smartcar.h>


Car car;

SR04 frontSound; 
const int TRIG_PIN_FRONT = 6; 
const int ECHO_PIN_FRONT = 7; 

SR04 rightSound; 
const int TRIG_PIN_RIGHT = 44;
const int ECHO_PIN_RIGHT = 42; 

SR04 ultrasonicSensor;
const int TRIGGER_PIN = 44; //7
const int ECHO_PIN = 42;   //6

int frontDistance = frontSound.getDistance();

void setup() {
  
  car.begin();
  Serial.begin(9600); //start the serial

  
  frontSound.attach(TRIG_PIN_FRONT, ECHO_PIN_FRONT);
  rightSound.attach(TRIG_PIN_RIGHT, ECHO_PIN_RIGHT);
   ultrasonicSensor.attach(TRIGGER_PIN, ECHO_PIN);
  Serial.println("rawr");

}

void loop() {
//  Serial.print("FrontDistance:");
   
  Serial.println(frontSound.getDistance());
   Serial.println(ultrasonicSensor.getDistance());
  delay(100);

}

