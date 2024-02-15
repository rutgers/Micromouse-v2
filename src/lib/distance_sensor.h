#ifndef distance_sensor
#define distance_sensor 
#define sensorPin1 21
#define sensorPin2 23
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <SPI.h>
#include <Arduino.h>

int16_t left();
int16_t right();

#endif