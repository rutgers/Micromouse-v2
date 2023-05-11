#include <Arduino.h>
#include <SPI.h>
#include "lib\Motors.h"
#include "lib\tof.h"

// #include<Adafruit_NeoPixel.h>
#include "lib\IMU.h"
#include "lib\PIDRotate.h"
#include "lib\PIDStraight.h"

Motors* motors;

void setup() {
  Serial.begin(9600);

}

void loop() {

  //Serial.println("looping");
  // int x;
  // x = pidrotate_instance->InputToMotor(90.0);
  // motors_instance->setLeftMotorSpeed(x);
  // motors_instance->setRightMotorSpeed(x);
     //Serial.println("instance A");
     //Serial.print("\tout:   ");


     //Serial.println("instance B");
     //Serial.print("\tout:   ");


    //pidrotate_instance->InputToMotor(90);
    //pidstraight_instance->InputToMotor(0,500); //10 cm. 
    //pidrotate_instance->InputToMotor(90);
    //pidrotate_instance->InputToMotor(-180);
    //pidrotate_instance->InputToMotor(180);
    //pidrotate_instance->InputToMotor(-180);
    //pidrotate_instance->InputToMotor(-90);
    //pidstraight_instance->InputToMotor(0,-500); //10 cm. 
delay(500);
    pidstraight_instance->InputToMotor(0,11); //go 11 cm. works fine
    //pidrotate_instance->InputToMotor(90); //left 90?




 // timeofflight_instance->readDistance();
//  Serial.print("\n");
 // timeofflight_instance->checkAddresses();


  

}