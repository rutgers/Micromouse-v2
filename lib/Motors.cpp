#include <Arduino.h>
#include "Motors.h"


void enableMotors() {

    digitalWrite(pin_STBY, HIGH);

}


void disableMotors() {

    digitalWrite(pin_STBY, LOW);
    
}

/**
 *  Sets speed of left motor
 *  speed takes integer from 0 to 255
 */
void setLeftMotorSpeed(int speed) {
    digitalWrite(pin_PWMA, speed);
}


/**
 *  Sets speed of right motor
 *  speed takes integer from 0 to 255
 */
void setRightMotorSpeed(int speed) {
    digitalWrite(pin_PWMB, speed);
}


/**
 *  Sets speed of both motors
 *  speed takes integer from 0 to 255
 */
void setMotorsSpeed(int speed) {
    setLeftMotorSpeed(speed);
    setRightMotorSpeed(speed);
}

//TODO: might have to reverse directions
//TODO: might have to swap left and right

/**
 *  Sets direction of left motor
 *  forward takes boolean: true is foward, false is backwards
 */
void setLeftMotorDirection(bool forward) {
    if (forward) {
        digitalWrite(pin_AIN1, HIGH);
        digitalWrite(pin_AIN2, LOW);
    }
}


/**
 *  Sets direction of right motor
 *  forward takes boolean: true is foward, false is backwards
 */
void setRightMotorDirection(bool forward) {
    if (forward) {
        digitalWrite(pin_AIN1, LOW);
        digitalWrite(pin_AIN2, HIGH);
    }   
}


/**
 *  Sets direction of both motors
 *  forward takes boolean: true is foward, false is backwards
 */
void setMotorsDirection(bool forward) {
    setLeftMotorDirection(forward);
    setRightMotorDirection(forward);
}