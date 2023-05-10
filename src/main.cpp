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
     //Serial.print(encoder_instanceA.read()); //right
     //Serial.println("instance B");
     //Serial.print("\tout:   ");
     //Serial.println(encoder_instanceB.read()); //left
    
    //pidrotate_instance->InputToMotor(90);
    pidstraight_instance->InputToMotor(0,500); //10 cm. 
    pidrotate_instance->InputToMotor(90);
    pidrotate_instance->InputToMotor(-180);
    pidrotate_instance->InputToMotor(180);
    pidrotate_instance->InputToMotor(-180);
    pidrotate_instance->InputToMotor(-90);
    pidstraight_instance->InputToMotor(0,-500); //10 cm. 




  timeofflight_instance->readDistance();
  Serial.print("\n");
  timeofflight_instance->checkAddresses();

  delay(500);

}