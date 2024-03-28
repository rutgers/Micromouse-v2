#include <Arduino.h>
#include "motors.h"
#define PI 3.1415926535897932384626433832795


Encoder encRight(2,3);
Encoder encLeft(10,11);


void setLeftPWM(int PWM) {
  if(PWM > 0) {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMB, PWM);
  } else {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    
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

void setFowardPWM(int distance){//distance need to be in mm 
  long old_right = encRight.read();
  long old_left = encLeft.read();
  Serial.println(old_right);
  Serial.println(old_left);
  double number =  (40 * 3.1415)/360;//distance per full rotation mm/tick
  Serial.println(number);
  double new_ticks = distance/number;//tick need

  double newer_right =(double)old_right + new_ticks;
  double newer_left = (double)old_left  +new_ticks;

  Serial.println(newer_right);
  Serial.println(newer_left);

  int pwmleft = 39;
  int pwmright = pwmleft+1;
  while(encRight.read() <= newer_right && encLeft.read() <= newer_left){
    
    
    setLeftPWM(pwmleft);
    setRightPWM(pwmright);
  }
  
  printf("Reached destination.");

  setLeftPWM(0);
  setRightPWM(0);//360 is a full rotation = 125.66 
}