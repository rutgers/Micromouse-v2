#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Encoder.h>
#include <VL53L1X.h>
#include <VL6180X.h>
#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20649.h>
#include <Adafruit_Sensor.h>

void setupMotorDriver();
void setupToF();
void setupIMU();
void PDFunc();
void setPWMA(long);
void setPWMB(long);

// Motor Driver Standby Pin (Drive to HIGH to make motor driver function)
#define STBY 5

// Motor A
Encoder ENCA(12, 11);
#define PWMA 8
#define AIN2 7
#define AIN1 6

// Motor B
Encoder ENCB(10, 9);
#define BIN1 4
#define BIN2 3
#define PWMB 2

// General Params
double WHEEL_CIRC = (80*PI);  // This is in millimeters
double gear_num = 31*33*35*34;
double gear_den = 16*14*13*14;
double WHEEL_TICKS = 12*(gear_num/gear_den);
double TicksPerMM = WHEEL_TICKS/WHEEL_CIRC;
int targetSpeed = 300*TicksPerMM;
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

void setup() {
    Serial.begin(115200);
    setupMotorDriver();
    setupToF();
    setupIMU();

    digitalWrite(STBY, HIGH);
    MotorTimer.begin(PDFunc, 1000);
    TimeCheck = 0;
}

void loop() {
    if (!stopRecording) {
        if (TimeCheck > 1000) {
            MotorTimer.end();
            setPWMA(0);
            setPWMB(0);
            stopRecording = true;
        }
    }
}

void PDFunc() {
    oldErrorX = errorX; oldErrorW = errorW;
    currA = ENCA.read(); currB = ENCB.read();
    errorW = 0 + (currA - currB);
    int PWMW = (KpW * errorW) + (KdW * (errorW - oldErrorW));
    setPWMA(70 + PWMW);
    setPWMB(70 - PWMW);
}

void setupMotorDriver() {
    pinMode(PWMA, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
    pinMode(PWMB, OUTPUT);

    analogWriteResolution(10);
    analogWriteFrequency(PWMA, 100000);
    analogWriteFrequency(PWMB, 100000);
    
    pinMode(STBY, OUTPUT);
}

void setupToF() {
    pinMode(20, OUTPUT);
    pinMode(22, OUTPUT);
    digitalWrite(20, LOW);
    digitalWrite(22, LOW);

    Wire.begin();
    Wire.setClock(400000);

    L_ToF.init();
    L_ToF.configureDefault();
    L_ToF.setAddress(0x31);
    L_ToF.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 1);
    L_ToF.stopContinuous();

    digitalWrite(22, HIGH);
    F_ToF.init();
    F_ToF.setDistanceMode(VL53L1X::Long);
    F_ToF.setMeasurementTimingBudget(33000);
    F_ToF.setAddress(0x30);
    F_ToF.stopContinuous();

    digitalWrite(20, HIGH);
    R_ToF.init();
    R_ToF.configureDefault();
    R_ToF.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 1);
    R_ToF.stopContinuous();

    delay(300);

    L_ToF.startRangeContinuous(0);
    F_ToF.startContinuous(33);
    R_ToF.startRangeContinuous(0);
}

void setupIMU() {
    setupToF();
    if (!IMU.begin_I2C()) {
        Serial.println("Failed to find ICM20649 chip");
        while (1) {
            delay(10);
        }
    }
    IMU.enableAccelDLPF(true, ICM20X_ACCEL_FREQ_473_HZ);
    IMU.enableGyrolDLPF(true, ICM20X_GYRO_FREQ_361_4_HZ);
    IMU.setAccelRateDivisor(0);
    IMU.setGyroRateDivisor(0);
    
    accel = IMU.getAccelerometerSensor();
    gyro = IMU.getGyroSensor();
}

void setPWMA(int new_pwm) {
    if (new_pwm >= 0) {
        digitalWrite(AIN2, LOW);
        digitalWrite(AIN1, HIGH);
        analogWrite(PWMA, new_pwm);
    } else {
        digitalWrite(AIN2, HIGH);
        digitalWrite(AIN1, LOW);
        analogWrite(PWMA, -new_pwm);
    }
}

void setPWMB(int new_pwm) {
    if (new_pwm >= 0) {
        digitalWrite(BIN2, LOW);
        digitalWrite(BIN1, HIGH);
        analogWrite(PWMB, new_pwm);
    } else {
        digitalWrite(BIN2, HIGH);
        digitalWrite(BIN1, LOW);
        analogWrite(PWMB, -new_pwm);
    }
}
