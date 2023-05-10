#include <Arduino.h>
#include <SPI.h>
#include "lib\Motors.h"
#include "lib\tof.h"

// #include<Adafruit_NeoPixel.h>

Motors* motors;
tof* timeofflight;

// Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, 12, NEO_GRB + NEO_KHZ800);

// #define red 0xff0000
// #define green 0x00ff00
// #define blue 0x0000ff



void setup() {
  // put your setup code here, to run once:
  // motors = new Motors();
  // motors->enableMotors();


  timeofflight = new tof();
  timeofflight->checkAddresses();  
  // Serial.println(timeofflight->readDistance().left);

  Serial.println("hello world");
  // strip.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  // motors->setLeftMotorDirection(true);
  // motors->setRightMotorDirection(true);

  // motors->setLeftMotorSpeed(64); 
  // motors->setRightMotorSpeed(64);
  // strip.setPixelColor(5, red);
  // strip.show();

  timeofflight->readDistance();
  Serial.print("\n");
  timeofflight->checkAddresses();

  delay(500);

}