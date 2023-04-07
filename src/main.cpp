#include <Arduino.h>
#include <SPI.h>
#include "lib\Motors.h"
#include "lib\PIDRotate.h"

Motors* motors;

void setup() {
  // put your setup code here, to run once:

  //timeofflight = new tof();
  //timeofflight->checkAddresses();  
  // Serial.println(timeofflight->readDistance().left);

  //Serial.println("hello world");


  motors_instance->enableMotors();
}

void loop() {
  // put your main code here, to run repeatedly:

  motors_instance->setMotorsSpeed((int)floor(pidrotate_instance->rotate_to_angle(-90)));

  
}