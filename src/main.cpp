#include <Arduino.h>
#include "..\lib\IMU.h"

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:

  IMU* imu = new IMU();

  Serial.print(imu->getHeading());
}