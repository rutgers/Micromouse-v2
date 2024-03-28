#ifndef distance_sensor
#define distance_sensor 
#define sensorPin1 21
#define sensorPin2 23
#include <Wire.h>
#include <Arduino.h>
#include <VL53L1X.h>


int16_t left();
int16_t right();
void sensorInit();
int16_t front();



#endif