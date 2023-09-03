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
// #include "lib\trueNorth.h"

Motors* motors;

void setup() {
  Serial.begin(9600);

  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);

  initialize();
  runMaze('c');
  // backTrack();

  digitalWrite(LED_BUILTIN, LOW);

}

void loop() {

  // Serial.println("============");
  // Serial.println(timeofflight_instance->readL());
  // Serial.println(timeofflight_instance->readFL());
  // Serial.println(timeofflight_instance->readF());
  // Serial.println(timeofflight_instance->readFR());
  // Serial.println(timeofflight_instance->readR());

  delay(500);
  
}