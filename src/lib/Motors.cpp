#include <Arduino.h>
#include "Motors.h"
#include <Encoder.h>



Encoder ENCA(2,3); 
Encoder ENCB(10,11);




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
    analogWrite(pin_PWMA, speed/20);
}


/**
 *  Sets speed of right motor
 *  speed takes integer from 0 to 255
 */
void Motors::setRightMotorSpeed(int speed) {
    analogWrite(pin_PWMB, speed/15);
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
    if (forward) {
        digitalWrite(pin_AIN1, HIGH);
        digitalWrite(pin_AIN2, LOW);
    } else {
        digitalWrite(pin_AIN1, LOW);
        digitalWrite(pin_AIN2, HIGH);
    }
}


/**
 *  Sets direction of right motor
 *  forward takes boolean: true is foward, false is backwards
 */
void Motors::setRightMotorDirection(bool forward) {
    if (forward) {
        digitalWrite(pin_BIN1, HIGH);
        digitalWrite(pin_BIN2, LOW);
    } else {
        digitalWrite(pin_BIN1, LOW);
        digitalWrite(pin_BIN2, HIGH);
    }   
}


/**
 *  Sets direction of both motors
 *  forward takes boolean: true is foward, false is backwards
 */
void Motors::setMotorsDirection(bool forward) {
    setLeftMotorDirection(forward);
    setRightMotorDirection(forward);
}
Motors* motors_instance = new Motors();
Encoder encoder_instanceA(2,3);
Encoder encoder_instanceB(10,11);