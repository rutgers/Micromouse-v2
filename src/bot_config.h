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
#include <Adafruit_Sensor_Calibration.h>
#include <Adafruit_AHRS.h>

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

/* General Parameters */
// Wheel Measurements
extern const float WHEEL_CIRC; // This is in millimeters
extern const float MOUSE_RADIUS; // This is in millimeters
// Gearbox Ratio
extern const float gear_num;  // Numerator
extern const float gear_den;  // Denominator
// Wheel-to-Distance Traveled Relations
extern const float TICKS;  // Ticks per rotation of gearbox shaft
extern const float MM_PER_TICK;  // Millimeters traveled by wheel per tick

/* PD Function Variables */
// Rotational and Translation PD setpoints
extern int xSet;
extern int wSet;
// Timer (calls PD Func based on input interval; e.g.: PDTimer.begin(PDFunc, 25000))
extern IntervalTimer PDTimer;
// PD Constants (tune these as you see fit)
extern const int KpX, KdX, KpW, KdW;
// PD Error Value Holders
extern long errorX, oldErrorX, errorW, oldErrorW;

// Bluetooth
extern SoftwareSerial bt;

// Time-of-Flight Sensors
extern VL53L1X F_ToF;
extern VL6180X R_ToF;
extern VL6180X L_ToF;

// Accel + Gyro IMU
extern Adafruit_ICM20649 IMU;
extern Adafruit_Sensor *accel, *gyro;
extern sensors_event_t a, g;

// IMU Orientation Algos
//extern Adafruit_NXPSensorFusion filter; // slowest
extern Adafruit_Mahony imu_filter;  // fastest/smalleset
#define FILTER_UPDATE_RATE_HZ 100
#define PRINT_EVERY_N_UPDATES 10
extern elapsedMillis imu_filter_timestamp;

/* Device Setup Functions */
void setupMotorDriver(uint8_t ain1_val = AIN1_VAL, uint8_t ain2_val = AIN2_VAL,
                      uint8_t bin1_val = BIN1_VAL, uint8_t bin2_val = BIN2_VAL);
void setupToF();
void setupIMU();

/* Motor Speed Control */
void setPWMA(int);
void setPWMB(int);

#endif