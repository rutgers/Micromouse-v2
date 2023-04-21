#include <Arduino.h>
#include <SPI.h>
#include "lib\Motors.h"
#include "lib\PIDRotate.h"
#include "lib\PIDStraight.h"
#include "lib\PIDMagicStraight.h"

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  //timeofflight = new tof();
  //timeofflight->checkAddresses();  
  // Serial.println(timeofflight->readDistance().left);

  //Serial.println("hello world");

  delay(5000);


  motors_instance->enableMotors();
}

void loop() {
  // put your main code here, to run repeatedly:

  //Serial.println("looping");

  /*Serial.print("encoder A positon: ");
  Serial.println(ENCA.read());
  
  Serial.print("encoder B positon: ");
  Serial.println(ENCB.read());*/

  //motors_instance->setMotorsSpeed((int)floor(pidrotate_instance->rotate_to_angle(180)));
  //motors_instance->setMotorsSpeed((int)floor(pidstraight_instance->drive_to_position(10)));
  pidmagicstraight_instance->drive_straight_with_magic(10);

  
}