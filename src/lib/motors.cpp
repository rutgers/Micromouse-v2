#include <Arduino.h>
#include "motors.h"


Encoder encRight(2,3);
Encoder encLeft(10,11);


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