#include <Arduino.h>
//#include <SoftwareSerial.h>
#include <Encoder.h>
//#include <VL53L1X.h>
//#include <VL6180X.h>
#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20649.h>
#include <Adafruit_Sensor.h>
#include <math.h>
#include <queue>
#include <Adafruit_AHRS.h>

// Motor Driver Standby Pin (Drive to HIGH to make motor driver function)
#define STBY 5

double leftTimer = 0;
double rightTimer = 0;

double rotateTimer = 0;

double goal_threshold = 45;
double time_threshold = 30;

double goal_threshold_deg = 10;

struct pid_return {
  double motor_output;
  bool within_goal;
};

struct pid_return pid_retA;
struct pid_return pid_retB;
struct pid_return pid_retRA;
struct pid_return pid_retRB;

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
Adafruit_ICM20649 IMU;
Adafruit_Sensor *accel, *gyro, *magn;
sensors_event_t g, a, t, m;
Adafruit_Mahony imu_filter;
#define FILTER_UPDATE_RATE_HZ 100
#define PRINT_EVERY_N_UPDATES 10
extern elapsedMillis imu_filter_timestamp;

double zanglegyro = 0;
double zangleaccel = 0;

double rotation_offset = 0;

bool apple = false;

void rotate();
void straight();

void resetRotation() {
  rotation_offset = imu_filter.getYaw();
}


void setup() {
  Serial.begin(9600);

  delay(6500);

  resetRotation();
  reset_pid();

  Serial.println(0);
  IMU.begin_I2C();
  IMU.enableGyrolDLPF(true, ICM20X_GYRO_FREQ_361_4_HZ);
  IMU.setAccelRateDivisor(0);
  IMU.setGyroRateDivisor(0);
  accel = IMU.getAccelerometerSensor();
  gyro = IMU.getGyroSensor();
  magn = IMU.getMagnetometerSensor();


  pinMode(PWMA, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  pinMode(STBY, OUTPUT);

  digitalWrite(STBY, HIGH);
}

void loop() {

  IMU.getEvent(&a, &g, &t, &m);

  imu_filter.update((g.gyro.x-.0053) * SENSORS_RADS_TO_DPS, (g.gyro.y-.016) * SENSORS_RADS_TO_DPS, (g.gyro.z-.0117) * SENSORS_RADS_TO_DPS, a.acceleration.x, a.acceleration.y, a.acceleration.z, 0, 0, 0);

  
  //rotate180();
  if (!apple) {
    reset_pid();
    black_magic(20);
    /*rotate(90);
    straight(6);
    rotate(90);
    straight(6);
    rotate(90);
    straight(6);
    rotate(90);
    straight(6);*/
  }
  apple = true;
  //rotate180();

}

void rotate(int ang) {
  while(!callRotate(ang)) {
     IMU.getEvent(&a, &g, &t, &m);

     imu_filter.update((g.gyro.x-.0053) * SENSORS_RADS_TO_DPS, (g.gyro.y-.016) * SENSORS_RADS_TO_DPS, (g.gyro.z-.0117) * SENSORS_RADS_TO_DPS, a.acceleration.x, a.acceleration.y, a.acceleration.z, 0, 0, 0);

  }
  Serial.println("returned true");
  reset_pid();
}

void straight(int dist) {
  while(!callStraight(dist)) {
    
  }
  reset_pid();
}

void black_magic(int dist) {
  while (!callBlackMagic(dist)) {
    IMU.getEvent(&a, &g, &t, &m);

    imu_filter.update((g.gyro.x-.0053) * SENSORS_RADS_TO_DPS, (g.gyro.y-.016) * SENSORS_RADS_TO_DPS, (g.gyro.z-.0117) * SENSORS_RADS_TO_DPS, a.acceleration.x, a.acceleration.y, a.acceleration.z, 0, 0, 0);

  }
  reset_pid();
}

bool callStraight(double distance) {

  double ticks_per_inch = (360 / (1.57 * PI));

  struct pid_return* a;
  struct pid_return* b;
  
  double goal = distance * ticks_per_inch;

  a = pid(0, -goal, .6, .00000000, .0); //pid(0, -inch_goal,1.8,.00000000,.0);
  b = pid(1, goal, .6, .00000000, .0); //pid(1, inch_goal,1.8,.00000000,.0);

  if (a->motor_output < 0) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  } else {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  }

  if (b->motor_output > 0) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  } else {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
  }
  
  analogWrite(PWMA, abs(a->motor_output));
  analogWrite(PWMB, abs(b->motor_output));

  if (a->within_goal == 1 && b->within_goal == 1) {
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
    reset_pid();
    return true;
  }

  return false;

}

bool callBlackMagic(double dist) {

  struct pid_return* a;
  struct pid_return* b;
  
  double goal = dist;

  a = pid_drive_straight_with_rotate(0, goal, .6, .000, 50000);//a = pid_rotate(0, goal, .6, .001, 50000); //.00000000
  b = pid_drive_straight_with_rotate(1, goal, .6, .000, 50000);//b = pid_rotate(1, goal, .6, .001, 50000);
  b->motor_output *= -1;

  if (a->motor_output < 0) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  } else {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  }

  if (b->motor_output > 0) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  } else {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
  }

  analogWrite(PWMA, abs(a->motor_output));
  analogWrite(PWMB, abs(b->motor_output));

  if (a->within_goal == 1 && b->within_goal == 1) {
    Serial.println("exiting");
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
    reset_pid();
    return true;
  }
  return false;

}

bool callRotate(double ang) {

  struct pid_return* a;
  struct pid_return* b;
  
  double goal = ang;

  a = pid_rotate(0, goal, .6, .000, 50000);//a = pid_rotate(0, goal, .6, .001, 50000); //.00000000
  b = pid_rotate(1, goal, .6, .000, 50000);//b = pid_rotate(1, goal, .6, .001, 50000);
  b->motor_output *= -1;

  if (a->motor_output < 0) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  } else {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  }

  if (b->motor_output > 0) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  } else {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
  }

  analogWrite(PWMA, abs(a->motor_output));
  analogWrite(PWMB, abs(b->motor_output));

  if (a->within_goal == 1 && b->within_goal == 1) {
    Serial.println("exiting");
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
    reset_pid();
    return true;
  }
  return false;

}


double last_errorA = 0.0;
double errorA = 0.0;
double total_errorA = 0.0;

double last_errorB = 0.0;
double errorB = 0.0;
double total_errorB = 0.0;

double last_errorRA = 0.0;
double errorRA = 0.0;
double total_errorRA = 0.0;

double last_errorRB = 0.0;
double errorRB = 0.0;
double total_errorRB = 0.0;


struct pid_return* pid(int controller, double target, double kP, double kI, double kD) {
  // CONTROLLER: 1 == LEFT 2 == RIGHT

  bool within_goal = 0;
  double return_value = 0;

  if (controller == 0) {
    last_errorA = errorA;
    errorA = (target - ENCA.read());
    total_errorA += errorA;

    Serial.print("errorA: ");
    Serial.println(errorA);

    return_value = 255 * tanh((kP * errorA + kD * (errorA - last_errorA) / (timeStep) + kI * total_errorA) / 100);

    if (abs(errorA) < goal_threshold) {
      leftTimer++;
    } else {
      leftTimer = 0;
    }

    if (leftTimer > time_threshold) {
      within_goal = 1;
    }


  } else {
    last_errorB = errorB;
    errorB = -(target - ENCB.read());
    total_errorB += errorB;
    
    Serial.print("errorB: ");
    Serial.println(errorB);

    return_value = 255 * tanh((kP * errorB + kD * (errorB - last_errorB) / (timeStep) + kI * total_errorB) / 100);


    if (abs(errorB) < goal_threshold) {
      rightTimer++;
    } else {
      rightTimer = 0;
    }

    if (rightTimer > time_threshold) {
      within_goal = 1;
    }
  }

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

struct pid_return* pid_rotate(int controller, double target, double kP, double kI, double kD) {
  // CONTROLLER: 1 == LEFT 2 == RIGHT

  if (controller == 0) {
    bool within_goal = 0;
    double return_value = 0;
  
    last_errorRA = errorRA;
    errorRA = remainder((target*0.70 - (imu_filter.getYaw() - rotation_offset)),360.0);
    Serial.print("target: ");
    Serial.println(target);
    Serial.print("rotation offset: ");
    Serial.println(rotation_offset);
    Serial.print("pre mod: ");
    Serial.println(target*0.70 - imu_filter.getYaw() - rotation_offset);
    total_errorRA += errorRA;
  
    return_value = -255 * tanh((kP * errorRA + kD * (errorRA - last_errorRA) / (timeStep) + kI * total_errorRA) / 100);
  
    Serial.print("errorA: ");
    Serial.println(errorRA);
    Serial.print("returnA: ");
    Serial.println(return_value);
    Serial.print("preturnA: ");
    Serial.println(kP * errorRA * -.255);
    Serial.print("dreturnA: ");
    Serial.println(kD * (errorRA - last_errorRA) / (timeStep) * -2.55);
  
    if (abs(errorRA) < goal_threshold_deg) {
      rotateTimer++;
      Serial.println("A in threshold");
    } else {
      rotateTimer = 0;
    }
  
    if (rotateTimer > time_threshold) {
      within_goal = 1;
    }
  
    pid_retRA.motor_output = return_value;
    pid_retRA.within_goal = within_goal;
    return &pid_retRA;
  } else {

    bool within_goal = 0;
    double return_value = 0;
  
    last_errorRB = errorRB;
    errorRB = remainder((target*0.70 - (imu_filter.getYaw() - rotation_offset)),360.0);
    total_errorRB += errorRB;
  
    return_value = -255 * tanh((kP * errorRB + kD * (errorRB - last_errorRB) / (timeStep) + kI * total_errorRB) / 100);
  
    Serial.print("errorB: ");
    Serial.println(errorRB);
    Serial.print("returnB: ");
    Serial.println(return_value);
    Serial.print("preturnB: ");
    Serial.println(kP * errorRB * -.255);
    Serial.print("dreturnB: ");
    Serial.println(kD * (errorRB - last_errorRB) / (timeStep) * -2.55);
    Serial.print("angle: ");
    Serial.println(imu_filter.getYaw());
    Serial.print("corrected angle: ");
    Serial.println(imu_filter.getYaw() - rotation_offset);

  
    if (abs(errorRB) < goal_threshold_deg) {
      rotateTimer++;
      Serial.println("A in threshold");
    } else {
      rotateTimer = 0;
    }
  
    if (rotateTimer > time_threshold) {
      within_goal = 1;
    }
  
    pid_retRB.motor_output = return_value;
    pid_retRB.within_goal = within_goal;
    return &pid_retRB;
  }
}

struct pid_return* pid_drive_straight_with_rotate(int controller, double target, double kP, double kI, double kD) {
  // CONTROLLER: 1 == LEFT 2 == RIGHT
  
  if (controller == 0) {
    bool within_goal = 0;
    double return_value = 0;
  
    last_errorRA = errorRA;
    errorRA = remainder((90*0.70 - (imu_filter.getYaw() - rotation_offset)),360.0);
    Serial.print("target: ");
    Serial.println(target);
    Serial.print("rotation offset: ");
    Serial.println(rotation_offset);
    Serial.print("pre mod: ");
    Serial.println(target*0.70 - imu_filter.getYaw() - rotation_offset);
    total_errorRA += errorRA;
  
    return_value = -255 * tanh((kP * errorRA + kD * (errorRA - last_errorRA) / (timeStep) + kI * total_errorRA) / 100);
  
    Serial.print("errorA: ");
    Serial.println(errorRA);
    Serial.print("returnA: ");
    Serial.println(return_value);
    Serial.print("preturnA: ");
    Serial.println(kP * errorRA * -.255);
    Serial.print("dreturnA: ");
    Serial.println(kD * (errorRA - last_errorRA) / (timeStep) * -2.55);
  
    if (abs(errorRA) < goal_threshold_deg) {
      rotateTimer++;
      Serial.println("A in threshold");
    } else {
      rotateTimer = 0;
    }
  
    if (rotateTimer > time_threshold) {
      within_goal = 1;
    }
  
    pid_retRA.motor_output = return_value;
    pid_retRA.within_goal = within_goal;
    return &pid_retRA;
  } else {

    bool within_goal = 0;
    double return_value = 0;
  
    last_errorRB = errorRB;
    errorRB = remainder((-90*0.70 - imu_filter.getYaw() - rotation_offset),360.0);
    total_errorRB += errorRB;
  
    return_value = -255 * tanh((kP * errorRB + kD * (errorRB - last_errorRB) / (timeStep) + kI * total_errorRB) / 100);
  
    Serial.print("errorB: ");
    Serial.println(errorRB);
    Serial.print("returnB: ");
    Serial.println(return_value);
    Serial.print("preturnB: ");
    Serial.println(kP * errorRB * -.255);
    Serial.print("dreturnB: ");
    Serial.println(kD * (errorRB - last_errorRB) / (timeStep) * -2.55);
    Serial.print("angle: ");
    Serial.println(imu_filter.getYaw());
    Serial.print("corrected angle: ");
    Serial.println(imu_filter.getYaw() - rotation_offset);

  
    if (abs(errorRB) < goal_threshold_deg) {
      rotateTimer++;
      Serial.println("A in threshold");
    } else {
      rotateTimer = 0;
    }
  
    if (rotateTimer > time_threshold) {
      within_goal = 1;
    }
  
    pid_retRB.motor_output = return_value;
    pid_retRB.within_goal = within_goal;
    return &pid_retRB;
  }
}

void reset_pid() {
  total_errorA = 0.0;
  total_errorB = 0.0;

  ENCA.write(0);
  ENCB.write(0);
  total_errorRA = 0.0;
  total_errorRB = 0.0;

  resetRotation();
}
