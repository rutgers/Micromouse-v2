#include <Arduino.h>
//#include <SoftwareSerial.h>
#include <Encoder.h>
//#include <VL53L1X.h>
//#include <VL6180X.h>
//#include <Adafruit_ICM20X.h>
//#include <Adafruit_ICM20649.h>
//#include <Adafruit_Sensor.h>
#include <math.h>

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

// Bluetooth
//SoftwareSerial bt(0, 1);

// Time-of-Flight Sensors
//VL53L1X F_ToF;
//VL6180X R_ToF;
//VL6180X L_ToF;

// Accel + Gyro IMU
//Adafruit_ICM20649 IMU;

void setup() {
  delay(7500);
    pinMode(PWMA, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
    pinMode(PWMB, OUTPUT);

    pinMode(STBY, OUTPUT);

    digitalWrite(STBY, HIGH);

    Serial.begin(9600);
    
    reset_pid();
}

void loop() {
  //digitalWrite(AIN1, HIGH);
  //digitalWrite(AIN2, LOW);
  //analogWrite(PWMA,255);
 //delay(10000);
  double ticks_per_inch = (360/(1.57*PI));
  double inch_goal = -12*ticks_per_inch;
  double pid_A = pidA(inch_goal,1.8,.00000000,.0);
 
  
  if (pid_A >0) {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
   digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  } else {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
   digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  }
  analogWrite(PWMA,pid_A);
  analogWrite(PWMB,pid_A);
  //analogWrite(PWMB,pidB(10000,1.8,.00000000,.0));
  Serial.println(pid_A);
  
}


double last_errorA = 0.0;
double errorA = 0.0;
double total_errorA = 0.0;

double start_timeA = 0.0;
double last_start_timeA = 0.0;

double end_timeA = 0.0;
double last_end_timeA = 0.0;

double last_errorB = 0.0;
double errorB = 0.0;
double total_errorB = 0.0;

double start_timeB = 0.0;
double last_start_timeB = 0.0;

double end_timeB = 0.0;
double last_end_timeB = 0.0;

double pidA(double target, double kP, double kI, double kD) {

    last_start_timeA = start_timeA;
    last_end_timeA = end_timeA;
    
    start_timeA = micros();
    
    last_errorA = errorA;
    errorA = target - ENCA.read();
    total_errorA += errorA;
    
    double return_value = 255*tanh((kP * errorA + kD * (errorA - last_errorA)/(last_end_timeA-last_start_timeA) + kI * total_errorA)/100);

    Serial.print("error: ");
    Serial.println(errorA);
  /*  Serial.print("p:");
    Serial.println(kP * errorA);
    Serial.print("i:");
    Serial.println(kI * total_errorA);
    Serial.print("d:");
    Serial.println(kD * (errorA - last_errorA)/(last_end_timeA-last_start_timeA));
    Serial.print("return: ");
    Serial.println(return_value);
    Serial.print("tanh return: ");
    Serial.println(255*tanh((1/100) * return_value)); */
    
    end_timeA = micros();
    
    return return_value;
}

void reset_pid() {
  total_errorA = 0.0;
  total_errorB = 0.0;

  ENCA.write(0);
  ENCB.write(0);
}

double pidB(double target, double kP, double kI, double kD) {

    last_start_timeB = start_timeB;
    last_end_timeB = end_timeB;
    
    start_timeB = micros();
      
    last_errorB = errorB;
    errorB = target - ENCB.read();
    total_errorB += errorB;

    double return_value = kP * errorB - kD * (errorB - last_errorB)/(last_end_timeB-last_start_timeB) + kI * total_errorB;

    end_timeB = micros();
    
    return return_value;
}
