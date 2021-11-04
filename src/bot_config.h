#ifndef BOT_CONFIG_H
#define BOT_CONFIG_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Encoder.h>
#include <VL53L1X.h>
#include <VL6180X.h>
#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20649.h>
#include <Adafruit_Sensor.h>

// Motor Driver Standby Pin (Drive to HIGH to make motor driver function)
#define STBY 5

/* The values listed for the IN pins are defaults for when the bot is
moving forward. They can be changed with the setupMotorDriver method. */

// Motor A
Encoder ENCA(12, 11);
#define AIN1 6
#define AIN2 7
#define PWMA 8
uint8_t AIN1_VAL = HIGH;
uint8_t AIN2_VAL = LOW;

// Motor B
Encoder ENCB(10, 9);
#define BIN1 4
#define BIN2 3
#define PWMB 2
uint8_t BIN1_VAL = HIGH;
uint8_t BIN2_VAL = LOW;

// General Params
double WHEEL_CIRC = (80 * PI); // This is in millimeters
double gear_num = 31 * 33 * 35 * 34;
double gear_den = 16 * 14 * 13 * 14;
double WHEEL_TICKS = 12 * (gear_num / gear_den);
double TicksPerMM = WHEEL_TICKS / WHEEL_CIRC;
int xConst = 300 * TicksPerMM;
int wConst = 0;
IntervalTimer MotorTimer;
elapsedMillis TimeCheck;
bool stopRecording = false;
long PWMA_Value, PWMB_Value;
double KpX = 0.1, KdX = 0.05, KpW = 5, KdW = 0.12;
long errorX = 0, oldErrorX = 0, errorW = 0, oldErrorW = 0;
long currA = 0, currB = 0, lastA = 0, lastB = 0;

// Bluetooth
SoftwareSerial bt(0, 1);

// Time-of-Flight Sensors
VL53L1X F_ToF;
VL6180X R_ToF;
VL6180X L_ToF;

// Accel + Gyro IMU
Adafruit_ICM20649 IMU;
Adafruit_Sensor *accel, *gyro;
sensors_event_t a, g;

void setupMotorDriver(uint8_t ain1_val = AIN1_VAL, uint8_t ain2_val = AIN2_VAL,
                      uint8_t bin1_val = BIN1_VAL, uint8_t bin2_val = BIN2_VAL);
void setupToF();
void setupIMU();
void setPWMA(int);
void setPWMB(int);

#endif