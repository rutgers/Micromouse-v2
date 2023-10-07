#include <Arduino.h>
#include "Motors.h"

// #ifdef V3

// Encoder ENCA(2,3);
// Encoder ENCB(10,11);

// #else

Encoder ENCB(2,1);  // these may be reversed
Encoder ENCA(4,3); //3,4

// #endif


void Motors::enableMotors() {

    digitalWrite(pin_STBY, HIGH);

}


void Motors::disableMotors() {

    digitalWrite(pin_STBY, LOW);
}

/**
 *  Sets speed of left motor
 *  speed takes integer from 0 to 255
 */
void Motors::setLeftMotorSpeed(int speed) {
    analogWrite(pin_PWMA, speed);
}


/**
 *  Sets speed of right motor
 *  speed takes integer from 0 to 255
 */
void Motors::setRightMotorSpeed(int speed) {
    analogWrite(pin_PWMB, speed);
}


/**
 *  Sets speed of both motors
 *  speed takes integer from 0 to 255
 */
void Motors::setMotorsSpeed(int speed) {
    setLeftMotorSpeed(speed);
    setRightMotorSpeed(speed);
}

//TODO: might have to reverse directions
//TODO: might have to swap left and right

/**
 *  Sets direction of left motor
 *  forward takes boolean: true is foward, false is backwards
 */
void Motors::setLeftMotorDirection(bool forward) {
    if (forward == true) {
        digitalWrite(pin_AIN1, LOW);
        //Serial.println("AIN1 high and AIN2 low");
        digitalWrite(pin_AIN2, HIGH);
    } else {
        digitalWrite(pin_AIN1, HIGH);
       //Serial.println("AIN1 low and AIN2 high");
        digitalWrite(pin_AIN2, LOW);
    }
}
void Motors::setClockWise(){
    setLeftMotorDirection(false);
    setRightMotorDirection(false);
}
void Motors::setCClockWise(){
    setLeftMotorDirection(true);
    setRightMotorDirection(true);
}
void Motors::setStraight(){
    setLeftMotorDirection(true);
    setRightMotorDirection(true);
}


/**
 *  Sets direction of right motor
 *  forward takes boolean: true is foward, false is backwards
 */
void Motors::setRightMotorDirection(bool forward) {
    if (forward == true) {
        digitalWrite(pin_BIN1, LOW);
        //Serial.println("BIN1 high and BIN2 low");
        digitalWrite(pin_BIN2, HIGH);
    } else {
        digitalWrite(pin_BIN1, HIGH);
        //Serial.println("BIN1 low and BIN2 high");
        digitalWrite(pin_BIN2, LOW);
    }
}
int Motors::getlefttick(){
    return ENCA.read();
}
int Motors::getrighttick(){
    return ENCB.read();
}
void Motors::setlefttick(int set){
    ENCA.write(set);
}
void Motors::setrighttick(int set){
    ENCB.write(set);
}
void Motors::setTick(int set){
    setlefttick(set);
    setrighttick(set);
}


/**
 *  Sets direction of both motors
 *  forward takes boolean: true is foward, false is backwards
 */
void Motors::setMotorsDirection(bool forward) {
    setLeftMotorDirection(forward);
    setRightMotorDirection(!forward);
}
Motors* motors_instance = new Motors();