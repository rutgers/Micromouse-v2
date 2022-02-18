#include <Arduino.h>
//#include <SoftwareSerial.h>
#include <Encoder.h>
//#include <VL53L1X.h>
//#include <VL6180X.h>
//#include <Adafruit_ICM20X.h>
//#include <Adafruit_ICM20649.h>
//#include <Adafruit_Sensor.h>
#include <math.h>
#include <queue>


// Motor Driver Standby Pin (Drive to HIGH to make motor driver function)
#define STBY 5

std::queue<char*> q;

double leftTimer = 0;
double rightTimer = 0;

double goal_threshold = 45;
double time_threshold = 30;

struct pid_return {
  double motor_output;
  bool within_goal;
};

struct pid_return pid_retA;
struct pid_return pid_retB;

IntervalTimer MotorTimer;
double timeStep = 10000;
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

    delay(6500);

    q.push("24");
    q.push("-24");
        
    MotorTimer.begin(setMotorsPID, timeStep);

      
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
  
  
  
  //analogWrite(PWMB,pidB(10000,1.8,.00000000,.0));
  //Serial.print("pid a: ");
  //Serial.println(pid_A);
  //Serial.print("pid b: ");
  //Serial.println(pid_B);
  
}






void setMotorsPID() {
  Serial.println("t");
  if(q.empty()) {
    Serial.println("END");
    
    analogWrite(PWMA,0);
    analogWrite(PWMB,0);
    MotorTimer.end();
    return;
  }

  //Serial.println("enter setMotorsPID()");
  
  double ticks_per_inch = (360/(1.57*PI));
  double inch_goal = atof(q.front())*ticks_per_inch;

  //Serial.println("1");

  
  struct pid_return* a = pid(0, -inch_goal,1.8,.00000000,.0);
  struct pid_return* b = pid(1, inch_goal,1.8,.00000000,.0);

  //Serial.println("2");

  //Serial.println(reinterpret_cast<int>(a));
  
  if (a->motor_output > 0) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  } else {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  }

   //Serial.println("3");

  if (b->motor_output >0) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  } else {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
  }

  //Serial.println("4");
  
  analogWrite(PWMA,a->motor_output);
  analogWrite(PWMB,b->motor_output);

  //Serial.println("5");

  if(a->within_goal == 1 && b->within_goal == 1) {
    q.pop();
    reset_pid();
  }

  //Serial.println("exit setMotorsPid()");

  
}







double last_errorA = 0.0;
double errorA = 0.0;
double total_errorA = 0.0;

//double start_time = 0.0;
//double last_start_time = 0.0;
//
//double end_time = 0.0;
//double last_end_time = 0.0;

double last_errorB = 0.0;
double errorB = 0.0;
double total_errorB = 0.0;


struct pid_return* pid(int controller, double target, double kP, double kI, double kD) {
 // CONTROLLER: 1 == LEFT 2 == RIGHT
// 
//    last_start_time = start_time;
//    last_end_time = end_time;
//    
//    start_time = micros();

    bool within_goal = 0;
    double return_value = 0;
    
    if(controller == 0) {
      last_errorA = errorA;
      errorA = -(target - ENCA.read());
      total_errorA += errorA;
    
      return_value = 255*tanh((kP * errorA + kD * (errorA - last_errorA)/(timeStep) + kI * total_errorA)/100);
      //Serial.print("errorA: ");
      //Serial.println(errorA);
  
      if (abs(errorA) < goal_threshold) {
        leftTimer++;
      } else {
        leftTimer = 0;
      }
   
      if(leftTimer > time_threshold) {
        within_goal = 1;
      }

      
    } else {
      last_errorB = errorB;
      errorB = -(target - ENCB.read());
      Serial.println(errorB);
      total_errorB += errorB;
    
      return_value = 255*tanh((kP * errorB + kD * (errorB - last_errorB)/(timeStep) + kI * total_errorB)/100);
      //Serial.print("errorB: ");
      //Serial.println(errorB);


      if (abs(errorB) < goal_threshold) {
        rightTimer++;
      } else {
        rightTimer = 0;
      }
   
      if(rightTimer > time_threshold) {
        within_goal = 1;
      }
    }
    
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
    
    //end_time = micros();



    //Serial.println(reinterpret_cast<int>(pid_ret));

    //Serial.println(pid_ret.motor_output);
    if (controller == 0) {
      pid_retA.motor_output = return_value;
      pid_retA.within_goal = within_goal;
      return &pid_retA;
    } else {
      pid_retB.motor_output = return_value;
      pid_retB.within_goal = within_goal;
      return &pid_retB;
    }
}





void reset_pid() {
  total_errorA = 0.0;
  total_errorB = 0.0;

  ENCA.write(0);
  ENCB.write(0);
}
