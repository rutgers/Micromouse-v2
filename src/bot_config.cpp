#include "bot_config.h"

/* The values listed for the IN pins are defaults for when the bot is
moving forward. They can be changed with the setupMotorDriver method. */

// Motor A
Encoder ENCA(11, 12);
uint8_t AIN1_VAL = HIGH;
uint8_t AIN2_VAL = LOW;

// Motor B
Encoder ENCB(9, 10);
uint8_t BIN1_VAL = HIGH;
uint8_t BIN2_VAL = LOW;


/* General Parameters */
// Wheel Measurements
const float WHEEL_CIRC = 80 * PI; // This is in millimeters
const float MOUSE_RADIUS = 39.5; // This is in millimeters
// Gearbox Ratio
const float gear_num = 31 * 33 * 35 * 34;  // Numerator
const float gear_den = 16 * 14 * 13 * 14;  // Denominator
// Wheel-to-Distance Traveled Relations
const float TICKS = 12 * (gear_num / gear_den);  // Ticks per rotation of gearbox shaft
const float MM_PER_TICK = WHEEL_CIRC / TICKS;  // Millimeters traveled by wheel per tick

/* PD Function Variables */
// Rotational and Translation PD setpoints
int xSet = 30;
int wSet = 0;
// Timer (calls PD Func based on input interval; e.g.: PDTimer.begin(PDFunc, 25000))
IntervalTimer PDTimer;
// PD Constants (tune these as you see fit)
const int KpX = 2, KdX = 1, KpW = 5, KdW = 1;
// PD Error Value Holders
long errorX = 0, oldErrorX = 0, errorW = 0, oldErrorW = 0;

// Bluetooth
SoftwareSerial bt(0, 1);

// Time-of-Flight Sensors
VL53L1X F_ToF;  // Front
VL6180X R_ToF;  // Right
VL6180X L_ToF;  // Left

// Accel + Gyro IMU
Adafruit_ICM20649 IMU;
Adafruit_Sensor *accel, *gyro;
sensors_event_t a, g;

// IMU Orientation Algos
//extern Adafruit_NXPSensorFusion filter; // slowest
Adafruit_Mahony imu_filter;  // fastest/smalleset
elapsedMillis imu_filter_timestamp = 0;

/* Device Setup Functions */
void setupMotorDriver(uint8_t ain1_val, uint8_t ain2_val, uint8_t bin1_val, uint8_t bin2_val)
{
    pinMode(STBY, OUTPUT);

    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(PWMA, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
    pinMode(PWMB, OUTPUT);

    AIN1_VAL = ain1_val;
    AIN2_VAL = ain2_val;
    BIN1_VAL = bin1_val;
    BIN2_VAL = bin2_val;

    analogWriteResolution(10);
    analogWriteFrequency(PWMA, 100000);
    analogWriteFrequency(PWMB, 100000);
}

void setupToF()
{
    L_ToF.init();
    L_ToF.setAddress(0x28);
    L_ToF.configureDefault();
    L_ToF.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 4);

    digitalWrite(14, HIGH);
    delay(50);
    R_ToF.init();
    R_ToF.setAddress(0x2A);
    R_ToF.configureDefault();
    R_ToF.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 4);

    digitalWrite(15, HIGH);
    delay(50);
    F_ToF.init();
    F_ToF.setDistanceMode(VL53L1X::Medium);
    F_ToF.setROISize(4, 4);
    F_ToF.setMeasurementTimingBudget(33000);

    L_ToF.startRangeContinuous(10);
    F_ToF.startContinuous(33);
    R_ToF.startRangeContinuous(10);
}

void setupIMU()
{
    if (!IMU.begin_I2C())
    {
        Serial.println("Failed to find ICM20649 chip");
        while (1)
        {
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

void setPWMA(int new_pwm)
{
    if (new_pwm >= 0)
    {
        digitalWrite(AIN1, AIN1_VAL);
        digitalWrite(AIN2, AIN2_VAL);
        analogWrite(PWMA, new_pwm);
    }
    else
    {
        digitalWrite(AIN1, !AIN1_VAL);
        digitalWrite(AIN2, !AIN2_VAL);
        analogWrite(PWMA, -new_pwm);
    }
}

void setPWMB(int new_pwm)
{
    if (new_pwm >= 0)
    {
        digitalWrite(BIN1, BIN1_VAL);
        digitalWrite(BIN2, BIN2_VAL);
        analogWrite(PWMB, new_pwm);
    }
    else
    {
        digitalWrite(BIN1, !BIN1_VAL);
        digitalWrite(BIN2, !BIN2_VAL);
        analogWrite(PWMB, -new_pwm);
    }
}