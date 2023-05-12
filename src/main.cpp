#include <Arduino.h>
#include <SPI.h>
#include "lib\Motors.h"
#include "lib\tof.h"

// #include<Adafruit_NeoPixel.h>
#include "lib\IMU.h"
#include "lib\PIDRotate.h"
#include "lib\PIDStraight.h"

#include "lib\pathfinding\API.h"
#include "lib\pathfinding\Flood.h"

Motors* motors;

// int a = false;

void setup() {
  Serial.begin(9600);

  // delay(5000);
  // API::moveForward();
  // API::turnLeft();
  // API::turnLeft();
  API::turnRight();
  // API::turnRight();
  
  /*
  Serial.print("Start");
  initialize();
  runMaze('c');

  Serial.print("Done");
  backTrack();
  runMaze('c');
  // backTrack();
  // runMaze('c');
  // backTrack();
  */
  
}

void loop() {
  Serial.println(imu_instance->getHeading());
  delay(300);
}