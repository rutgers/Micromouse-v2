#include <Arduino.h>
#include <SPI.h>
#include "lib\Motors.h"

Motors* motors;

void setup() {
  // put your setup code here, to run once:
  motors = new Motors();
  motors->enableMotors();
}

void loop() {
  // put your main code here, to run repeatedly:
  motors->setLeftMotorDirection(true);
  motors->setRightMotorDirection(true);

  motors->setLeftMotorSpeed(64);
  motors->setRightMotorSpeed(64);

  
}