#ifndef imu_h
#define imu_h

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <SPI.h>

void imuSetup();
double getHeading();

#endif