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
 *  pwm takes integer from -255 to 255
 */
void Motors::setLeftMotorSpeed(int pwm) {
    if(pwm < 0) {
        setLeftMotorDirection(true);
    } else {
        setLeftMotorDirection(false);
    }

    if(abs(pwm) > 255) {
        analogWrite(pin_PWMB, 255);
    } else {
        analogWrite(pin_PWMB, abs(pwm));
    }

}


/**
 *  Sets pwm of right motor
 *  pwm takes integer from -255 to 255
 */
void Motors::setRightMotorSpeed(int pwm) {
    if(pwm < 0) {
        setRightMotorDirection(true);
    } else {
        setRightMotorDirection(false);
    }


    if(pwm > 255) {
        analogWrite(pin_PWMA, 255);
    } else {
        analogWrite(pin_PWMA, abs(pwm));
    }

}


/**
 *  Sets speed of both motors
 *  pwm takes integer from -255 to 255
 */
void Motors::setMotorsSpeed(int pwm) {
    setLeftMotorSpeed(pwm);
    setRightMotorSpeed(pwm);
}

/**
 *  Sets direction of left motor
 *  forward takes boolean: true is backwards, false is forwards
 */
void Motors::setLeftMotorDirection(bool backwards) {
    if (backwards) {
        digitalWrite(pin_BIN1, HIGH);
        digitalWrite(pin_BIN2, LOW);
    } else {
        digitalWrite(pin_BIN1, LOW);
        digitalWrite(pin_BIN2, HIGH);
    }
}


/**
 *  Sets direction of right motor
 *  forward takes boolean: true is backwards, false is forwards
 */
void Motors::setRightMotorDirection(bool backwards) {
    if (backwards) {
        digitalWrite(pin_AIN1, HIGH);
        digitalWrite(pin_AIN2, LOW);
    } else {
        digitalWrite(pin_AIN1, LOW);
        digitalWrite(pin_AIN2, HIGH);
    }   
}


Motors* motors_instance = new Motors();
Encoder encoder_instanceA(2,3);
Encoder encoder_instanceB(10,11);