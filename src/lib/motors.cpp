#include <Arduino.h>
#include "motors.h"

// #define BIN1 7
// #define BIN2 8
// #define PWMB 9
// #define STBY 14


// #define AIN1 6
// #define AIN2 5
// #define PWMA 4

// void setup() {
//   // put your setup code here, to run once:
//   pinMode(BIN1, OUTPUT);  
//   pinMode(BIN2, OUTPUT);
//   pinMode(PWMB, OUTPUT);
//   pinMode(STBY, OUTPUT);

//   pinMode(AIN1, OUTPUT);  
//   pinMode(AIN2, OUTPUT);
//   pinMode(PWMA, OUTPUT);
//   digitalWrite(LED_BUILTIN, HIGH);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   digitalWrite(STBY, HIGH);
//   digitalWrite(BIN1, HIGH);
//   digitalWrite(BIN2, LOW);
//   analogWrite(PWMB, 120);

//   digitalWrite(AIN1, HIGH);
//   digitalWrite(AIN2, LOW);
//   analogWrite(PWMA, 255);
// } 

void setLeftPWM(int PWM) {
  if(PWM > 0) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMB, PWM);
  } else {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMB, -PWM);
  }
}
void setRightPWM(int PWM){

  if(PWM > 0) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    analogWrite(PWMA, PWM);
  } else {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, -PWM);
  }
}
void motorSetup(){
  pinMode(BIN1, OUTPUT);  
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(STBY, OUTPUT);

  pinMode(AIN1, OUTPUT);  
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  digitalWrite(STBY, HIGH);
}