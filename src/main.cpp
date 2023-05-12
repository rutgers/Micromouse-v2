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

  delay(1000);
  // API::moveForward();
  Serial.print("Start");
  initialize();
  runMaze('c');

  Serial.print("Done");
  backTrack();
  runMaze('c');
  // backTrack();
  // runMaze('c');
  // backTrack();
  
  // delay(500);
  // API::turnLeft();
  // delay(500);

  // Serial.print("Hello");

  // API::moveForward();
  // delay(500);
  // API::turnRight();
  // delay(500);

  // API::moveForward();
  // delay(500);
  // API::moveForward();
  // delay(500);

  // API::turnRight();
  // delay(500);
  // API::moveForward();
  // delay(500);

  // API::turnRight();
  // delay(500);
  // API::moveForward();
  // delay(500); 
 
}

void loop() {
  // timeofflight_instance->readDistance();
  // Serial.println("");
  // delay(500);
}