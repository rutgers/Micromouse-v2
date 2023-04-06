#include <Arduino.h>
#include <SPI.h>
#include "lib\Motors.h"
#include "..\lib\tof.cpp"

Motors* motors;
tof* timeofflight;

void setup() {
  // put your setup code here, to run once:
  // motors = new Motors();
  // motors->enableMotors();


  timeofflight = new tof();
  timeofflight->checkAddresses();  
  // Serial.println(timeofflight->readDistance().left);

  Serial.println("hello world");
}

void loop() {
  // put your main code here, to run repeatedly:
  // motors->setLeftMotorDirection(true);
  // motors->setRightMotorDirection(true);

  // motors->setLeftMotorSpeed(64);
  // motors->setRightMotorSpeed(64);

}