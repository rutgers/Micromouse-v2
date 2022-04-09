#ifndef BOT_CONFIG_H
#define BOT_CONFIG_H

#include <Arduino.h>
#include <Encoder.h>
#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20649.h>
#include <Adafruit_Sensor.h>
<<<<<<< Updated upstream:src/bot_config.h
=======
#include <math.h>
#include <queue>
#include <Adafruit_AHRS.h>
#include <iostream>
#include <algorithm>
#include <utility>
#include <tuple>
#include <vector>
#include <cmath>
#include <SoftwareSerial.h>
#include <VL53L1X.h>
#include <VL6180X.h>
>>>>>>> Stashed changes:src/main/bot_config.h

// Motor Driver Standby Pin (Drive to HIGH to make motor driver function)
#define STBY 5

/* The values listed for the IN pins are defaults for when the bot is
moving forward. They can be changed with the setupMotorDriver method. */

// Motor A
extern Encoder ENCA;
#define AIN1 6
#define AIN2 7
#define PWMA 8
extern uint8_t AIN1_VAL;
extern uint8_t AIN2_VAL;

// Motor B
extern Encoder ENCB;
#define BIN1 4
#define BIN2 3
#define PWMB 2
extern uint8_t BIN1_VAL;
extern uint8_t BIN2_VAL;

// General Params
extern const float WHEEL_CIRC; // This is in millimeters
extern const float MOUSE_RADIUS; // This is in millimeters
extern const float gear_num;
extern const float gear_den;
extern const float WHEEL_TICKSgear_num;
extern const float MM_PER_TICK;
extern const float DEG_PER_MM_DIFF;
extern const float xConst;
extern const float wConst;
extern IntervalTimer MotorTimer;
extern elapsedMillis TimeCheck;
extern bool recording;
extern const float KpX, KdX, KpW, KdW;
extern long errorX, oldErrorX, errorW, oldErrorW;

// Bluetooth
extern SoftwareSerial bt;

// Time-of-Flight Sensors
extern VL53L1X F_ToF;
extern VL6180X R_ToF;
extern VL6180X L_ToF;

// Accel + Gyro IMU
extern Adafruit_ICM20649 IMU;
extern Adafruit_Sensor *accel, *gyro, *magn;
// extern sensors_event_t a, g, t, m;

void setupMotorDriver(uint8_t ain1_val = AIN1_VAL, uint8_t ain2_val = AIN2_VAL,
                      uint8_t bin1_val = BIN1_VAL, uint8_t bin2_val = BIN2_VAL);
void setupToF();
void setupIMU();
void setPWMA(int);
void setPWMB(int);

#endif
